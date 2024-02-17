/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-10-24 10:44:38 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-10-24 11:43:32 +0800
************************************************************************************************/ 
#include "CanMasterFlashIap.h"
#include "DriverConfigSystem.h"
#include SYS_LOG_INCLUDE_FILE

//通道配置数组
//参数分别是 指令  是否需要结果  超时时间  最小参数长度 附带参数长度
//附带参数长度为不定长的时候,指示参数长度为0,由上级调用者手动申请内存
static const CAN_PROT_CMD_CONFIG canMasterFlashIapCmdConfigArray[] = {
    {CAN_MASTER_CMD_IAP_PREPARE   ,   ENABLE,     60000,      0,      1       },//在线升级准备
    {CAN_MASTER_CMD_IAP_START_MSG ,   ENABLE,     60000,      0,      12      },//在线升级启动信息
    {CAN_MASTER_CMD_IAP_DATA_TRANS,   ENABLE,     60000,      0,      0       },//在线升级数据传输
    {CAN_MASTER_CMD_IAP_DATA_CHECK,   ENABLE,     60000,      0,      4       },//在线升级数据校验
    {CAN_MASTER_CMD_IAP_FORCE_JMP ,   ENABLE,     60000,      0,      1       },//在线升级强制跳转
};

/*************************************发送指令等待ACK************************************************/
//升级准备
void CanMasterFlashIapPrepareWhileAck(uint8_t targetModule,uint8_t targetBoardID,BOARD_IAP_MODE iapMode,
                                                SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_ACT_IAP;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_IAP_PREPARE;
    CAN_PROT_CMD_SEND* canCmdSendPtr = NULL;
    CanProtClearLastCmd(targetModule,targetBoardID,singalChannel,channelCmdCode,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        CAN_PROT_LOG("%s,%d,ErrorSub: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    CAN_PROT_CMD_CONFIG* canCmdConfigPtr = CanProtGetCmdConfig(channelCmdCode,(CAN_PROT_CMD_CONFIG*)canMasterFlashIapCmdConfigArray,
                                                                DIM_ARRAY_ELEMENT_COUNT(canMasterFlashIapCmdConfigArray));
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
    canCmdSendPtr->cmdBufferPtr[0] = (uint8_t)iapMode;
    //指令发送
    CanProtSendCmdWaitAck(canCmdSendPtr,resultPackPtr);
    //销毁发送指令
    CanProtDestroyCmdSend(&canCmdSendPtr);
    return;
}

//升级开始
void CanMasterFlashIapStartWhileAck(uint8_t targetModule,uint8_t targetBoardID,uint32_t byteTotal,uint32_t packTotal,
                                                uint32_t singlePackLength,SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_ACT_IAP;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_IAP_START_MSG;
    CAN_PROT_CMD_SEND* canCmdSendPtr = NULL;
    CanProtClearLastCmd(targetModule,targetBoardID,singalChannel,channelCmdCode,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        CAN_PROT_LOG("%s,%d,ErrorSub: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    CAN_PROT_CMD_CONFIG* canCmdConfigPtr = CanProtGetCmdConfig(channelCmdCode,(CAN_PROT_CMD_CONFIG*)canMasterFlashIapCmdConfigArray,
                                                                DIM_ARRAY_ELEMENT_COUNT(canMasterFlashIapCmdConfigArray));
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
    ProtUtilWriteUint32ToBuffer(byteTotal,canCmdSendPtr->cmdBufferPtr);
    ProtUtilWriteUint32ToBuffer(packTotal,canCmdSendPtr->cmdBufferPtr+4);
    ProtUtilWriteUint32ToBuffer(singlePackLength,canCmdSendPtr->cmdBufferPtr+8);
    //指令发送
    CanProtSendCmdWaitAck(canCmdSendPtr,resultPackPtr);
    //销毁发送指令
    CanProtDestroyCmdSend(&canCmdSendPtr);
    return;
}

//升级数据
void CanMasterFlashIapDataWhileAck(uint8_t targetModule,uint8_t targetBoardID,uint16_t packIndex,uint8_t* dataBufPtr,
                                            uint16_t dataLength,SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_ACT_IAP;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_IAP_DATA_TRANS;
    CAN_PROT_CMD_SEND* canCmdSendPtr = NULL;
    CanProtClearLastCmd(targetModule,targetBoardID,singalChannel,channelCmdCode,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        CAN_PROT_LOG("%s,%d,ErrorSub: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    CAN_PROT_CMD_CONFIG* canCmdConfigPtr = CanProtGetCmdConfig(channelCmdCode,(CAN_PROT_CMD_CONFIG*)canMasterFlashIapCmdConfigArray,
                                                                DIM_ARRAY_ELEMENT_COUNT(canMasterFlashIapCmdConfigArray));
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
    canCmdSendPtr->cmdBufferLength = 2+dataLength;
    canCmdSendPtr->cmdBufferPtr = CanProtMemMalloc(targetModule,targetBoardID,canCmdSendPtr->cmdBufferLength);
    UserMemClear(canCmdSendPtr->cmdBufferPtr,canCmdSendPtr->cmdBufferLength);
    //参数赋值
    ProtUtilWriteUint32ToBuffer(packIndex,canCmdSendPtr->cmdBufferPtr);
    UserMemCopy(canCmdSendPtr->cmdBufferPtr+2,dataBufPtr,dataLength);
    //指令发送
    CanProtSendCmdWaitAck(canCmdSendPtr,resultPackPtr);
    //销毁发送指令
    CanProtDestroyCmdSend(&canCmdSendPtr);
    return;
}

//升级校验
void CanMasterFlashIapCheckWhileAck(uint8_t targetModule,uint8_t targetBoardID,uint32_t crcCode,
                                                SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_ACT_IAP;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_IAP_DATA_CHECK;
    CAN_PROT_CMD_SEND* canCmdSendPtr = NULL;
    CanProtClearLastCmd(targetModule,targetBoardID,singalChannel,channelCmdCode,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        CAN_PROT_LOG("%s,%d,ErrorSub: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    CAN_PROT_CMD_CONFIG* canCmdConfigPtr = CanProtGetCmdConfig(channelCmdCode,(CAN_PROT_CMD_CONFIG*)canMasterFlashIapCmdConfigArray,
                                                                DIM_ARRAY_ELEMENT_COUNT(canMasterFlashIapCmdConfigArray));
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
    ProtUtilWriteUint32ToBuffer(crcCode,canCmdSendPtr->cmdBufferPtr);
    //指令发送
    CanProtSendCmdWaitAck(canCmdSendPtr,resultPackPtr);
    //销毁发送指令
    CanProtDestroyCmdSend(&canCmdSendPtr);
    return;
}

//升级强制跳转
void CanMasterFlashIapJmpWhileAck(uint8_t targetModule,uint8_t targetBoardID,BOARD_FORCE_JMP jmpTarget,
                                            SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_ACT_IAP;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_IAP_FORCE_JMP;
    CAN_PROT_CMD_SEND* canCmdSendPtr = NULL;
    CanProtClearLastCmd(targetModule,targetBoardID,singalChannel,channelCmdCode,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        CAN_PROT_LOG("%s,%d,ErrorSub: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    CAN_PROT_CMD_CONFIG* canCmdConfigPtr = CanProtGetCmdConfig(channelCmdCode,(CAN_PROT_CMD_CONFIG*)canMasterFlashIapCmdConfigArray,
                                                                DIM_ARRAY_ELEMENT_COUNT(canMasterFlashIapCmdConfigArray));
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
    canCmdSendPtr->cmdBufferPtr[0] = (uint8_t)jmpTarget;
    //指令发送
    CanProtSendCmdWaitAck(canCmdSendPtr,resultPackPtr);
    //销毁发送指令
    CanProtDestroyCmdSend(&canCmdSendPtr);
    return;
}


/*************************************等待指定指令完成返回************************************************/
//升级准备
void CanMasterFlashIapPrepareWaitResult(uint8_t targetModule,uint8_t targetBoardID,SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_ACT_IAP;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_IAP_PREPARE;
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
    //释放内存
    CanProtFreeRecvDataBuf(targetModule,targetBoardID,returnDataBufferPtr,returnDataBufferLength);
    return;
}

//升级开始
void CanMasterFlashIapStartWaitResult(uint8_t targetModule,uint8_t targetBoardID,SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_ACT_IAP;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_IAP_START_MSG;
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
    //释放内存
    CanProtFreeRecvDataBuf(targetModule,targetBoardID,returnDataBufferPtr,returnDataBufferLength);
    return;
}

//升级数据
void CanMasterFlashIapDataWaitResult(uint8_t targetModule,uint8_t targetBoardID,SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_ACT_IAP;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_IAP_DATA_TRANS;
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
    //释放内存
    CanProtFreeRecvDataBuf(targetModule,targetBoardID,returnDataBufferPtr,returnDataBufferLength);
    return;
}

//升级校验
void CanMasterFlashIapCheckWaitResult(uint8_t targetModule,uint8_t targetBoardID,SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_ACT_IAP;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_IAP_DATA_CHECK;
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
    //释放内存
    CanProtFreeRecvDataBuf(targetModule,targetBoardID,returnDataBufferPtr,returnDataBufferLength);
    return;
}

//升级强制跳转
void CanMasterFlashIapJmpWaitResult(uint8_t targetModule,uint8_t targetBoardID,SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_ACT_IAP;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_IAP_FORCE_JMP;
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
    //释放内存
    CanProtFreeRecvDataBuf(targetModule,targetBoardID,returnDataBufferPtr,returnDataBufferLength);
    return;
}


/*************************************发送指令等待完成返回************************************************/
//升级准备
void CanMasterFlashIapPrepareWhileReturn(uint8_t targetModule,uint8_t targetBoardID,BOARD_IAP_MODE iapMode,
                                                SYS_RESULT_PACK* resultPackPtr)
{
    //发送指令等待ACK
    CanMasterFlashIapPrepareWhileAck(targetModule,targetBoardID,iapMode,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        CAN_PROT_LOG("%s,%d,CanMasterFlashIapPrepareWhileAck ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    //等待结果
    CanMasterFlashIapPrepareWaitResult(targetModule,targetBoardID,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        CAN_PROT_LOG("%s,%d,CanMasterFlashIapPrepareWaitResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
    }
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//升级开始
void CanMasterFlashIapStartWhileReturn(uint8_t targetModule,uint8_t targetBoardID,uint32_t byteTotal,uint32_t packTotal,
                                                uint32_t singlePackLength,SYS_RESULT_PACK* resultPackPtr)
{
    //发送指令等待ACK
    CanMasterFlashIapStartWhileAck(targetModule,targetBoardID,byteTotal,packTotal,singlePackLength,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        CAN_PROT_LOG("%s,%d,CanMasterFlashIapStartWhileAck ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    //等待结果
    CanMasterFlashIapStartWaitResult(targetModule,targetBoardID,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        CAN_PROT_LOG("%s,%d,CanMasterFlashIapStartWaitResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
    }
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//升级数据
void CanMasterFlashIapDataWhileReturn(uint8_t targetModule,uint8_t targetBoardID,uint16_t packIndex,uint8_t* dataBufPtr,
                                            uint16_t dataLength,SYS_RESULT_PACK* resultPackPtr)
{
    //发送指令等待ACK
    CanMasterFlashIapDataWhileAck(targetModule,targetBoardID,packIndex,dataBufPtr,dataLength,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        CAN_PROT_LOG("%s,%d,CanMasterFlashIapDataWhileAck ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    //等待结果
    CanMasterFlashIapDataWaitResult(targetModule,targetBoardID,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        CAN_PROT_LOG("%s,%d,CanMasterFlashIapDataWaitResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
    }
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//升级校验
void CanMasterFlashIapCheckWhileReturn(uint8_t targetModule,uint8_t targetBoardID,uint32_t crcCode,
                                            SYS_RESULT_PACK* resultPackPtr)
{
    //发送指令等待ACK
    CanMasterFlashIapCheckWhileAck(targetModule,targetBoardID,crcCode,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        CAN_PROT_LOG("%s,%d,CanMasterFlashIapCheckWhileAck ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    //等待结果
    CanMasterFlashIapCheckWaitResult(targetModule,targetBoardID,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        CAN_PROT_LOG("%s,%d,CanMasterFlashIapCheckWaitResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
    }
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//升级强制跳转
void CanMasterFlashIapJmpWhileReturn(uint8_t targetModule,uint8_t targetBoardID,BOARD_FORCE_JMP jmpTarget,
                                            SYS_RESULT_PACK* resultPackPtr)
{
    //发送指令等待ACK
    CanMasterFlashIapJmpWhileAck(targetModule,targetBoardID,jmpTarget,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        CAN_PROT_LOG("%s,%d,CanMasterFlashIapJmpWhileAck ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    //等待结果
    CanMasterFlashIapJmpWaitResult(targetModule,targetBoardID,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        CAN_PROT_LOG("%s,%d,CanMasterFlashIapJmpWaitResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
    }
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}












