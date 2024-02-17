/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-10-24 10:44:38 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-11-23 11:30:49 +0800
************************************************************************************************/ 
#include "CanMasterPortIn.h"
#include "DriverConfigSystem.h"
#include SYS_LOG_INCLUDE_FILE

//通道配置数组
//参数分别是 指令  是否需要结果  超时时间  最小参数长度 附带参数长度
//附带参数长度为不定长的时候,指示参数长度为0,由上级调用者手动申请内存
static const CAN_PROT_CMD_CONFIG canMasterPortInCmdConfigArray[] = {
    {CAN_MASTER_CMD_PORT_IN_READ_SINGLE  ,        ENABLE,     3000 ,      1,      1       },//输入单个读取
    {CAN_MASTER_CMD_PORT_IN_READ_RANGE   ,        ENABLE,     10000,      1,      2       },//输入区间读取
    {CAN_MASTER_CMD_PORT_IN_READ_ANY     ,        ENABLE,     10000,      1,      0       },//输入批量读取
};

/*************************************发送指令等待ACK************************************************/
//输入单个读取
void CanMasterPortInReadSingleWhileAck(uint8_t targetModule,uint8_t targetBoardID,uint8_t pinIndex,SYS_RESULT_PACK* resultPackPtr)
{
    
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_ACT_PORT_IN;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_PORT_IN_READ_SINGLE;
    CAN_PROT_CMD_SEND* canCmdSendPtr = NULL;
    CanProtClearLastCmd(targetModule,targetBoardID,singalChannel,channelCmdCode,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        CAN_PROT_LOG("%s,%d,ErrorSub: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    CAN_PROT_CMD_CONFIG* canCmdConfigPtr = CanProtGetCmdConfig(channelCmdCode,(CAN_PROT_CMD_CONFIG*)canMasterPortInCmdConfigArray,
                                                                DIM_ARRAY_ELEMENT_COUNT(canMasterPortInCmdConfigArray));
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
    //指令发送
    CanProtSendCmdWaitAck(canCmdSendPtr,resultPackPtr);
    //销毁发送指令
    CanProtDestroyCmdSend(&canCmdSendPtr);
    return;
}

//输入区间读取
void CanMasterPortInReadRangeWhileAck(uint8_t targetModule,uint8_t targetBoardID,uint8_t startPinIndex,
                                                uint8_t pinCount,SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_ACT_PORT_IN;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_PORT_IN_READ_RANGE;
    CAN_PROT_CMD_SEND* canCmdSendPtr = NULL;
    CanProtClearLastCmd(targetModule,targetBoardID,singalChannel,channelCmdCode,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        CAN_PROT_LOG("%s,%d,ErrorSub: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    CAN_PROT_CMD_CONFIG* canCmdConfigPtr = CanProtGetCmdConfig(channelCmdCode,(CAN_PROT_CMD_CONFIG*)canMasterPortInCmdConfigArray,
                                                                DIM_ARRAY_ELEMENT_COUNT(canMasterPortInCmdConfigArray));
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
    canCmdSendPtr->cmdBufferPtr[0] = (uint8_t)startPinIndex;
    canCmdSendPtr->cmdBufferPtr[1] = (uint8_t)pinCount;
    //指令发送
    CanProtSendCmdWaitAck(canCmdSendPtr,resultPackPtr);
    //销毁发送指令
    CanProtDestroyCmdSend(&canCmdSendPtr);
    return;
}

//输入批量读取
void CanMasterPortInReadAnyWhileAck(uint8_t targetModule,uint8_t targetBoardID,uint8_t* pinIndexArray,
                                                uint8_t pinCount,SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_ACT_PORT_IN;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_PORT_IN_READ_ANY;
    CAN_PROT_CMD_SEND* canCmdSendPtr = NULL;
    CanProtClearLastCmd(targetModule,targetBoardID,singalChannel,channelCmdCode,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        CAN_PROT_LOG("%s,%d,ErrorSub: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    CAN_PROT_CMD_CONFIG* canCmdConfigPtr = CanProtGetCmdConfig(channelCmdCode,(CAN_PROT_CMD_CONFIG*)canMasterPortInCmdConfigArray,
                                                                DIM_ARRAY_ELEMENT_COUNT(canMasterPortInCmdConfigArray));
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
    canCmdSendPtr->cmdBufferLength = pinCount;
    canCmdSendPtr->cmdBufferPtr = CanProtMemMalloc(targetModule,targetBoardID,canCmdSendPtr->cmdBufferLength);
    UserMemClear(canCmdSendPtr->cmdBufferPtr,canCmdSendPtr->cmdBufferLength);
    //参数赋值
    uint8_t indexUtil = 0;
    for(indexUtil = 0; indexUtil < pinCount; indexUtil++)
    {
        canCmdSendPtr->cmdBufferPtr[indexUtil] = pinIndexArray[indexUtil];
    }
    //指令发送
    CanProtSendCmdWaitAck(canCmdSendPtr,resultPackPtr);
    //销毁发送指令
    CanProtDestroyCmdSend(&canCmdSendPtr);
    return;
}


/*************************************等待指定指令完成返回************************************************/
//输入单个读取
void CanMasterPortInWaitReadSingleResult(uint8_t targetModule,uint8_t targetBoardID,SYS_RESULT_PACK* resultPackPtr,
                                                GPIO_PinState* pinStatePtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_ACT_PORT_IN;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_PORT_IN_READ_SINGLE;
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
    if(pinStatePtr != NULL)
    {
        //结果解析
        *pinStatePtr = GPIO_PIN_RESET;
        if((returnDataBufferLength != 0)&&(returnDataBufferPtr != NULL))
        {
            *pinStatePtr = (GPIO_PinState)(returnDataBufferPtr[0]);
        }
    }
    //内存释放
    CanProtFreeRecvDataBuf(targetModule,targetBoardID,returnDataBufferPtr,returnDataBufferLength);
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//输入区间读取
void CanMasterPortInWaitReadRangeResult(uint8_t targetModule,uint8_t targetBoardID,SYS_RESULT_PACK* resultPackPtr,
                                                GPIO_PinState* pinStateArrayPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_ACT_PORT_IN;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_PORT_IN_READ_RANGE;
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
        return;
    }
    if(pinStateArrayPtr != NULL)
    {
        //结果解析
        if((returnDataBufferLength != 0)&&(returnDataBufferPtr != NULL))
        {
            uint8_t indexUtil = 0;
            for(indexUtil = 0; indexUtil < returnDataBufferLength; indexUtil++)
            {
                pinStateArrayPtr[indexUtil] = (GPIO_PinState)(returnDataBufferPtr[indexUtil]);
            }
        }
    }
    //内存释放
    CanProtFreeRecvDataBuf(targetModule,targetBoardID,returnDataBufferPtr,returnDataBufferLength);
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//输入批量读取
void CanMasterPortInWaitReadAnyResult(uint8_t targetModule,uint8_t targetBoardID,SYS_RESULT_PACK* resultPackPtr,
                                                GPIO_PinState* pinStateArrayPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_ACT_PORT_IN;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_PORT_IN_READ_ANY;
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
        return;
    }
    if(pinStateArrayPtr != NULL)
    {
        //结果解析
        if((returnDataBufferLength != 0)&&(returnDataBufferPtr != NULL))
        {
            uint8_t indexUtil = 0;
            for(indexUtil = 0; indexUtil < returnDataBufferLength; indexUtil++)
            {
                pinStateArrayPtr[indexUtil] = (GPIO_PinState)(returnDataBufferPtr[indexUtil]);
            }
        }
    }
    //内存释放
    CanProtFreeRecvDataBuf(targetModule,targetBoardID,returnDataBufferPtr,returnDataBufferLength);
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}


/*************************************发送指令等待完成返回************************************************/
//输入单个读取
void CanMasterPortInReadSingleWhileReturn(uint8_t targetModule,uint8_t targetBoardID,uint8_t pinIndex,
                                                    SYS_RESULT_PACK* resultPackPtr,GPIO_PinState* pinStatePtr)
{
    //发送指令等待ACK
    CanMasterPortInReadSingleWhileAck(targetModule,targetBoardID,pinIndex,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        CAN_PROT_LOG("%s,%d,CanMasterPortInReadSingleWhileAck ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    //等待结果
    CanMasterPortInWaitReadSingleResult(targetModule,targetBoardID,resultPackPtr,pinStatePtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        CAN_PROT_LOG("%s,%d,CanMasterPortInWaitReadSingleResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
    }
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//输入区间读取
void CanMasterPortInReadRangeWhileReturn(uint8_t targetModule,uint8_t targetBoardID,uint8_t startPinIndex,uint8_t pinCount,
                                                    SYS_RESULT_PACK* resultPackPtr,GPIO_PinState* pinStateArrayPtr)
{
    //发送指令等待ACK
    CanMasterPortInReadRangeWhileAck(targetModule,targetBoardID,startPinIndex,pinCount,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        CAN_PROT_LOG("%s,%d,CanMasterPortInReadRangeWhileAck ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    //等待结果
    CanMasterPortInWaitReadRangeResult(targetModule,targetBoardID,resultPackPtr,pinStateArrayPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        CAN_PROT_LOG("%s,%d,CanMasterPortInWaitReadRangeResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
    }
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//输入批量读取
void CanMasterPortInReadAnyWhileReturn(uint8_t targetModule,uint8_t targetBoardID,uint8_t* pinIndexArray,uint8_t pinCount,
                                                    SYS_RESULT_PACK* resultPackPtr,GPIO_PinState* pinStateArrayPtr)
{
    //发送指令等待ACK
    CanMasterPortInReadAnyWhileAck(targetModule,targetBoardID,pinIndexArray,pinCount,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        CAN_PROT_LOG("%s,%d,CanMasterPortInReadAnyWhileAck ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    //等待结果
    CanMasterPortInWaitReadAnyResult(targetModule,targetBoardID,resultPackPtr,pinStateArrayPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        CAN_PROT_LOG("%s,%d,CanMasterPortInWaitReadAnyResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
    }
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}




















