/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-10-24 10:44:38 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-10-24 14:03:44 +0800
************************************************************************************************/ 
#include "CanMasterPortOut.h"
#include "DriverConfigSystem.h"
#include SYS_LOG_INCLUDE_FILE

//通道配置数组
//参数分别是 指令  是否需要结果  超时时间  最小参数长度 附带参数长度
//附带参数长度为不定长的时候,指示参数长度为0,由上级调用者手动申请内存
static const CAN_PROT_CMD_CONFIG canMasterPortOutCmdConfigArray[] = {
    {CAN_MASTER_CMD_PORT_OUT_WRITE_SINGLE     ,        ENABLE,     60000,      0,      2       },//输出单个写入
    {CAN_MASTER_CMD_PORT_OUT_WRITE_ANY        ,        ENABLE,     60000,      0,      0       },//输出批量写入
};

/*************************************发送指令等待ACK************************************************/
//输出单个写入
void CanMasterPortOutWriteSingleWhileAck(uint8_t targetModule,uint8_t targetBoardID,uint8_t pinIndex,GPIO_PinState pinState,
                                            SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_ACT_PORT_OUT;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_PORT_OUT_WRITE_SINGLE;
    CAN_PROT_CMD_SEND* canCmdSendPtr = NULL;
    CanProtClearLastCmd(targetModule,targetBoardID,singalChannel,channelCmdCode,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        CAN_PROT_LOG("%s,%d,ErrorSub: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    CAN_PROT_CMD_CONFIG* canCmdConfigPtr = CanProtGetCmdConfig(channelCmdCode,(CAN_PROT_CMD_CONFIG*)canMasterPortOutCmdConfigArray,
                                                                DIM_ARRAY_ELEMENT_COUNT(canMasterPortOutCmdConfigArray));
    if(canCmdConfigPtr == NULL)
    {
        CAN_PROT_LOG("%s,%d,canCmdConfigPtr NULL\r\n",__func__,__LINE__);
        //设置错误码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_CAN_PROT_GET_CMD_CONFIG,ERROR_LEVEL_ERROR,
                                        ERROR_SUB_BOARD_CAN_PROT_CMD_CONFIG_NOT_EXIST);
        return;
    }
    //创建指令头
    canCmdSendPtr = CanProtCreateCmdSend(targetModule,targetBoardID,singalChannel,channelCmdCode,canCmdConfigPtr);
    //参数赋值
    canCmdSendPtr->cmdBufferPtr[0] = (uint8_t)pinIndex;
    canCmdSendPtr->cmdBufferPtr[1] = (uint8_t)pinState;
    //指令发送
    CanProtSendCmdWaitAck(canCmdSendPtr,resultPackPtr);
    //销毁发送指令
    CanProtDestroyCmdSend(&canCmdSendPtr);
    return;
}

//输出批量写入
void CanMasterPortOutWriteAnyWhileAck(uint8_t targetModule,uint8_t targetBoardID,uint8_t* pinIndexArrayPtr,GPIO_PinState* pinStateArrayPtr,
                                                    uint8_t pinCount,SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_ACT_PORT_OUT;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_PORT_OUT_WRITE_ANY;
    CAN_PROT_CMD_SEND* canCmdSendPtr = NULL;
    CanProtClearLastCmd(targetModule,targetBoardID,singalChannel,channelCmdCode,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        CAN_PROT_LOG("%s,%d,ErrorSub: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    CAN_PROT_CMD_CONFIG* canCmdConfigPtr = CanProtGetCmdConfig(channelCmdCode,(CAN_PROT_CMD_CONFIG*)canMasterPortOutCmdConfigArray,
                                                                DIM_ARRAY_ELEMENT_COUNT(canMasterPortOutCmdConfigArray));
    if(canCmdConfigPtr == NULL)
    {
        CAN_PROT_LOG("%s,%d,canCmdConfigPtr NULL\r\n",__func__,__LINE__);
        //设置错误码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_CAN_PROT_GET_CMD_CONFIG,ERROR_LEVEL_ERROR,
                                        ERROR_SUB_BOARD_CAN_PROT_CMD_CONFIG_NOT_EXIST);
        return;
    }
    //创建指令头
    canCmdSendPtr = CanProtCreateCmdSend(targetModule,targetBoardID,singalChannel,channelCmdCode,canCmdConfigPtr);
    //申请内存
    canCmdSendPtr->cmdBufferLength = pinCount*2;
    canCmdSendPtr->cmdBufferPtr = CanProtMemMalloc(targetModule,targetBoardID,canCmdSendPtr->cmdBufferLength);
    UserMemClear(canCmdSendPtr->cmdBufferPtr,canCmdSendPtr->cmdBufferLength);
    //参数赋值
    uint8_t indexUtil = 0;
    for(indexUtil = 0; indexUtil < pinCount; indexUtil++)
    {
        canCmdSendPtr->cmdBufferPtr[indexUtil*2] = pinIndexArrayPtr[indexUtil];
        canCmdSendPtr->cmdBufferPtr[1 + (indexUtil*2)] = (uint8_t)(pinStateArrayPtr[indexUtil]);
    }
    //指令发送
    CanProtSendCmdWaitAck(canCmdSendPtr,resultPackPtr);
    //销毁发送指令
    CanProtDestroyCmdSend(&canCmdSendPtr);
    return;
}


/*************************************等待指定指令完成返回************************************************/
//输出单个写入
void CanMasterPortOutWaitWriteSingleResult(uint8_t targetModule,uint8_t targetBoardID,SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_ACT_PORT_OUT;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_PORT_OUT_WRITE_SINGLE;
    //保存结果
    uint8_t* returnDataBufferPtr = NULL;
    uint16_t returnDataBufferLength = 0;
    //等待指令返回
    CanProtCmdWaitResult(targetModule,targetBoardID,singalChannel,channelCmdCode,resultPackPtr,
                            &returnDataBufferPtr,&returnDataBufferLength);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        CAN_PROT_LOG("%s,%d,CanProtCmdWaitResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        //释放内存
        CanProtFreeRecvDataBuf(targetModule,targetBoardID,returnDataBufferPtr,returnDataBufferLength);
        return;
    }
    //内存释放
    CanProtFreeRecvDataBuf(targetModule,targetBoardID,returnDataBufferPtr,returnDataBufferLength);
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//输出批量写入
void CanMasterPortOutWaitWriteAnyResult(uint8_t targetModule,uint8_t targetBoardID,SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_ACT_PORT_OUT;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_PORT_OUT_WRITE_ANY;
    //保存结果
    uint8_t* returnDataBufferPtr = NULL;
    uint16_t returnDataBufferLength = 0;
    //等待指令返回
    CanProtCmdWaitResult(targetModule,targetBoardID,singalChannel,channelCmdCode,resultPackPtr,
                            &returnDataBufferPtr,&returnDataBufferLength);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        CAN_PROT_LOG("%s,%d,CanProtCmdWaitResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        //释放内存
        CanProtFreeRecvDataBuf(targetModule,targetBoardID,returnDataBufferPtr,returnDataBufferLength);
        return;
    }
    //内存释放
    CanProtFreeRecvDataBuf(targetModule,targetBoardID,returnDataBufferPtr,returnDataBufferLength);
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}


/*************************************发送指令等待完成返回************************************************/
//输出单个写入
void CanMasterPortOutWriteSingleWhileReturn(uint8_t targetModule,uint8_t targetBoardID,uint8_t pinIndex,GPIO_PinState pinState,
                                                    SYS_RESULT_PACK* resultPackPtr)
{
    //发送指令等待ACK
    CanMasterPortOutWriteSingleWhileAck(targetModule,targetBoardID,pinIndex,pinState,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        CAN_PROT_LOG("%s,%d,CanMasterPortOutWriteSingleWhileAck ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    //等待结果
    CanMasterPortOutWaitWriteSingleResult(targetModule,targetBoardID,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        CAN_PROT_LOG("%s,%d,CanMasterPortOutWaitWriteSingleResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
    }
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//输出批量写入
void CanMasterPortOutWriteAnyWhileReturn(uint8_t targetModule,uint8_t targetBoardID,uint8_t* pinIndexArrayPtr,GPIO_PinState* pinStateArrayPtr,
                                                    uint8_t pinCount,SYS_RESULT_PACK* resultPackPtr)
{
    //发送指令等待ACK
    CanMasterPortOutWriteAnyWhileAck(targetModule,targetBoardID,pinIndexArrayPtr,pinStateArrayPtr,pinCount,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        CAN_PROT_LOG("%s,%d,CanMasterPortOutWriteAnyWhileAck ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    //等待结果
    CanMasterPortOutWaitWriteAnyResult(targetModule,targetBoardID,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        CAN_PROT_LOG("%s,%d,CanMasterPortOutWaitWriteAnyResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
    }
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}


















