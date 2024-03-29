/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-10-24 10:44:38 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-10-24 13:16:49 +0800
************************************************************************************************/ 
#include "CanMasterParam.h"
#include "DriverConfigSystem.h"
#include SYS_LOG_INCLUDE_FILE

//通道配置数组
//参数分别是 指令  是否需要结果  超时时间  最小参数长度 附带参数长度
//附带参数长度为不定长的时候,指示参数长度为0,由上级调用者手动申请内存
static const CAN_PROT_CMD_CONFIG canMasterParamCmdConfigArray[] = {
    {CAN_MASTER_CMD_PARAM_RW_READ_SINGLE     ,        ENABLE,     60000,      4,      2       },//参数单个读取
    {CAN_MASTER_CMD_PARAM_RW_READ_ANY        ,        ENABLE,     60000,      4,      0       },//参数批量读取
    {CAN_MASTER_CMD_PARAM_RW_WRITE_SINGLE    ,        ENABLE,     60000,      0,      6       },//参数单个写入
    {CAN_MASTER_CMD_PARAM_RW_WRITE_ANY       ,        ENABLE,     60000,      0,      0       },//参数批量写入
};

/*************************************发送指令等待ACK************************************************/
//单个参数读取
void CanMasterParamReadSingleWhileAck(uint8_t targetModule,uint8_t targetBoardID,uint8_t mainIndex,uint8_t subIndex,
                                            SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_ACT_PARAM_RW;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_PARAM_RW_READ_SINGLE;
    CAN_PROT_CMD_SEND* canCmdSendPtr = NULL;
    CanProtClearLastCmd(targetModule,targetBoardID,singalChannel,channelCmdCode,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        CAN_PROT_LOG("%s,%d,ErrorSub: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    CAN_PROT_CMD_CONFIG* canCmdConfigPtr = CanProtGetCmdConfig(channelCmdCode,(CAN_PROT_CMD_CONFIG*)canMasterParamCmdConfigArray,
                                                                DIM_ARRAY_ELEMENT_COUNT(canMasterParamCmdConfigArray));
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
    canCmdSendPtr->cmdBufferPtr[0] = (uint8_t)mainIndex;
    canCmdSendPtr->cmdBufferPtr[1] = (uint8_t)subIndex;
    //指令发送
    CanProtSendCmdWaitAck(canCmdSendPtr,resultPackPtr);
    //销毁发送指令
    CanProtDestroyCmdSend(&canCmdSendPtr);
    return;
}

//批量参数读取
void CanMasterParamReadPairsWhileAck(uint8_t targetModule,uint8_t targetBoardID,uint8_t* mainIndexArrayPtr,uint8_t* subIndexArrayPtr,
                                            uint8_t pairs,SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_ACT_PARAM_RW;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_PARAM_RW_READ_ANY;
    CAN_PROT_CMD_SEND* canCmdSendPtr = NULL;
    CanProtClearLastCmd(targetModule,targetBoardID,singalChannel,channelCmdCode,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        CAN_PROT_LOG("%s,%d,ErrorSub: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    CAN_PROT_CMD_CONFIG* canCmdConfigPtr = CanProtGetCmdConfig(channelCmdCode,(CAN_PROT_CMD_CONFIG*)canMasterParamCmdConfigArray,
                                                                DIM_ARRAY_ELEMENT_COUNT(canMasterParamCmdConfigArray));
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
    canCmdSendPtr->cmdBufferLength = 2*pairs;
    canCmdSendPtr->cmdBufferPtr = CanProtMemMalloc(targetModule,targetBoardID,canCmdSendPtr->cmdBufferLength);
    UserMemClear(canCmdSendPtr->cmdBufferPtr,canCmdSendPtr->cmdBufferLength);
    //参数赋值
    uint8_t indexUtil = 0;
    for(indexUtil = 0; indexUtil < pairs; indexUtil++)
    {
        canCmdSendPtr->cmdBufferPtr[indexUtil*2] = mainIndexArrayPtr[indexUtil];
        canCmdSendPtr->cmdBufferPtr[1 + (indexUtil*2)] = subIndexArrayPtr[indexUtil];
    }
    //指令发送
    CanProtSendCmdWaitAck(canCmdSendPtr,resultPackPtr);
    //销毁发送指令
    CanProtDestroyCmdSend(&canCmdSendPtr);
    return;
}

//单个参数写入
void CanMasterParamWriteSingleWhileAck(uint8_t targetModule,uint8_t targetBoardID,uint8_t mainIndex,uint8_t subIndex,int32_t paramWrite,
                                            SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_ACT_PARAM_RW;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_PARAM_RW_WRITE_SINGLE;
    CAN_PROT_CMD_SEND* canCmdSendPtr = NULL;
    CanProtClearLastCmd(targetModule,targetBoardID,singalChannel,channelCmdCode,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        CAN_PROT_LOG("%s,%d,ErrorSub: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    CAN_PROT_CMD_CONFIG* canCmdConfigPtr = CanProtGetCmdConfig(channelCmdCode,(CAN_PROT_CMD_CONFIG*)canMasterParamCmdConfigArray,
                                                                DIM_ARRAY_ELEMENT_COUNT(canMasterParamCmdConfigArray));
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
    canCmdSendPtr->cmdBufferPtr[0] = (uint8_t)mainIndex;
    canCmdSendPtr->cmdBufferPtr[1] = (uint8_t)subIndex;
    ProtUtilWriteInt32ToBuffer(paramWrite,canCmdSendPtr->cmdBufferPtr+2);
    //指令发送
    CanProtSendCmdWaitAck(canCmdSendPtr,resultPackPtr);
    //销毁发送指令
    CanProtDestroyCmdSend(&canCmdSendPtr);
    return;
}

//批量参数写入
void CanMasterParamWritePairsWhileAck(uint8_t targetModule,uint8_t targetBoardID,uint8_t* mainIndexArrayPtr,uint8_t* subIndexArrayPtr,
                                            int32_t* paramWriteArrayPtr,uint8_t pairs,SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_ACT_PARAM_RW;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_PARAM_RW_WRITE_ANY;
    CAN_PROT_CMD_SEND* canCmdSendPtr = NULL;
    CanProtClearLastCmd(targetModule,targetBoardID,singalChannel,channelCmdCode,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        CAN_PROT_LOG("%s,%d,ErrorSub: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    CAN_PROT_CMD_CONFIG* canCmdConfigPtr = CanProtGetCmdConfig(channelCmdCode,(CAN_PROT_CMD_CONFIG*)canMasterParamCmdConfigArray,
                                                                DIM_ARRAY_ELEMENT_COUNT(canMasterParamCmdConfigArray));
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
    canCmdSendPtr->cmdBufferLength = 6*pairs;
    canCmdSendPtr->cmdBufferPtr = CanProtMemMalloc(targetModule,targetBoardID,canCmdSendPtr->cmdBufferLength);
    UserMemClear(canCmdSendPtr->cmdBufferPtr,canCmdSendPtr->cmdBufferLength);
    //参数赋值
    uint8_t indexUtil = 0;
    for(indexUtil = 0; indexUtil < pairs; indexUtil++)
    {
        canCmdSendPtr->cmdBufferPtr[indexUtil*6] = mainIndexArrayPtr[indexUtil];
        canCmdSendPtr->cmdBufferPtr[1 + (indexUtil*6)] = subIndexArrayPtr[indexUtil];
        ProtUtilWriteInt32ToBuffer(paramWriteArrayPtr[indexUtil],canCmdSendPtr->cmdBufferPtr+ 2 + (indexUtil*6));
    }
    //指令发送
    CanProtSendCmdWaitAck(canCmdSendPtr,resultPackPtr);
    //销毁发送指令
    CanProtDestroyCmdSend(&canCmdSendPtr);
    return;
}



/*************************************等待指定指令完成返回************************************************/
//单个参数读取
void CanMasterParamWaitReadSingleResult(uint8_t targetModule,uint8_t targetBoardID,SYS_RESULT_PACK* resultPackPtr,int32_t* readParam)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_ACT_PARAM_RW;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_PARAM_RW_READ_SINGLE;
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
    if(readParam != NULL)
    {
        //结果解析
        *readParam = 0;
        if((returnDataBufferLength != 0)&&(returnDataBufferPtr != NULL))
        {
            *readParam = ProtUtilReadInt32Buffer(returnDataBufferPtr);
        }
    }
    //内存释放
    CanProtFreeRecvDataBuf(targetModule,targetBoardID,returnDataBufferPtr,returnDataBufferLength);
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//批量参数读取
void CanMasterParamWaitReadPairsResult(uint8_t targetModule,uint8_t targetBoardID,SYS_RESULT_PACK* resultPackPtr,int32_t* readParamArrayPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_ACT_PARAM_RW;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_PARAM_RW_READ_ANY;
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
    if(readParamArrayPtr != NULL)
    {
        //结果解析
        if((returnDataBufferLength != 0)&&(returnDataBufferPtr != NULL))
        {
            uint8_t indexUtil = 0;
            for(indexUtil = 0; indexUtil < (returnDataBufferLength/4); indexUtil++)
            {
                readParamArrayPtr[indexUtil] = ProtUtilReadInt32Buffer(returnDataBufferPtr+(indexUtil*4));
            }
        }
    }
    //内存释放
    CanProtFreeRecvDataBuf(targetModule,targetBoardID,returnDataBufferPtr,returnDataBufferLength);
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//单个参数写入
void CanMasterParamWaitWriteSingleResult(uint8_t targetModule,uint8_t targetBoardID,SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_ACT_PARAM_RW;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_PARAM_RW_WRITE_SINGLE;
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
    //协议没报错,对结果数据进行处理
    asm("nop");
    //内存释放
    CanProtFreeRecvDataBuf(targetModule,targetBoardID,returnDataBufferPtr,returnDataBufferLength);
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//批量参数写入
void CanMasterParamWaitWritePairsResult(uint8_t targetModule,uint8_t targetBoardID,SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_ACT_PARAM_RW;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_PARAM_RW_WRITE_ANY;
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
    //协议没报错,对结果数据进行处理
    asm("nop");
    //内存释放
    CanProtFreeRecvDataBuf(targetModule,targetBoardID,returnDataBufferPtr,returnDataBufferLength);
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}



/*************************************发送指令等待完成返回************************************************/
//单个参数读取
void CanMasterParamReadSingleWhileReturn(uint8_t targetModule,uint8_t targetBoardID,uint8_t mainIndex,uint8_t subIndex,
                                                SYS_RESULT_PACK* resultPackPtr,int32_t* readParam)
{
    //发送指令等待ACK
    CanMasterParamReadSingleWhileAck(targetModule,targetBoardID,mainIndex,subIndex,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        CAN_PROT_LOG("%s,%d,CanMasterParamReadSingleWhileAck ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    //等待结果
    CanMasterParamWaitReadSingleResult(targetModule,targetBoardID,resultPackPtr,readParam);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        CAN_PROT_LOG("%s,%d,CanMasterParamWaitReadSingleResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
    }
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//批量参数读取
void CanMasterParamReadPairsWhileReturn(uint8_t targetModule,uint8_t targetBoardID,uint8_t* mainIndexArrayPtr,uint8_t* subIndexArrayPtr,
                                                uint8_t pairs,SYS_RESULT_PACK* resultPackPtr,int32_t* readParamArrayPtr)
{
    //发送指令等待ACK
    CanMasterParamReadPairsWhileAck(targetModule,targetBoardID,mainIndexArrayPtr,subIndexArrayPtr,pairs,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        CAN_PROT_LOG("%s,%d,CanMasterParamReadPairsWhileAck ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    //等待结果
    CanMasterParamWaitReadPairsResult(targetModule,targetBoardID,resultPackPtr,readParamArrayPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        CAN_PROT_LOG("%s,%d,CanMasterParamWaitReadPairsResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
    }
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//单个参数写入
void CanMasterParamWriteSingleWhileReturn(uint8_t targetModule,uint8_t targetBoardID,uint8_t mainIndex,uint8_t subIndex,int32_t paramWrite,
                                                SYS_RESULT_PACK* resultPackPtr)
{
    //发送指令等待ACK
    CanMasterParamWriteSingleWhileAck(targetModule,targetBoardID,mainIndex,subIndex,paramWrite,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        CAN_PROT_LOG("%s,%d,CanMasterParamWriteSingleWhileAck ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    //等待结果
    CanMasterParamWaitWriteSingleResult(targetModule,targetBoardID,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        CAN_PROT_LOG("%s,%d,CanMasterParamWaitWriteSingleResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
    }
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//批量参数写入
void CanMasterParamWritePairsWhileReturn(uint8_t targetModule,uint8_t targetBoardID,uint8_t* mainIndexArrayPtr,uint8_t* subIndexArrayPtr,
                                                int32_t* paramWriteArrayPtr,uint8_t pairs,SYS_RESULT_PACK* resultPackPtr)
{
    //发送指令等待ACK
    CanMasterParamWritePairsWhileAck(targetModule,targetBoardID,mainIndexArrayPtr,subIndexArrayPtr,paramWriteArrayPtr,pairs,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        CAN_PROT_LOG("%s,%d,CanMasterParamWritePairsWhileAck ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    //等待结果
    CanMasterParamWaitWritePairsResult(targetModule,targetBoardID,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        CAN_PROT_LOG("%s,%d,CanMasterParamWaitWritePairsResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
    }
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}


























