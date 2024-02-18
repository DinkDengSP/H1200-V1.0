/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-10-20 18:20:58 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-11-29 17:13:27 +0800
************************************************************************************************/ 
#include "CanMasterUtil.h"
#include "DriverConfigSystem.h"
#include SYS_LOG_INCLUDE_FILE

//通道配置数组
//参数分别是 指令  是否需要结果  超时时间  返回最小参数长度 附带参数长度
//附带参数长度为不定长的时候,指示参数长度为0,由上级调用者手动申请内存
static const CAN_PROT_CMD_CONFIG canMasterUtilCmdConfigArray[] = {
    {CAN_MASTER_CMD_UTIL_ONLINE          ,        ENABLE,     3000,      2,      1       },//联机
    {CAN_MASTER_CMD_UTIL_OFFLINE         ,        ENABLE,     3000,      1,      1       },//脱机
    {CAN_MASTER_CMD_UTIL_UTC_SYNC        ,        ENABLE,     3000,      0,      8       },//UTC时间同步
    {CAN_MASTER_CMD_UTIL_HEART_GET       ,        ENABLE,     3000,      9,      0       },//心跳获取
    {CAN_MASTER_CMD_UTIL_EM_STOP         ,        ENABLE,     3000,      0,      0       },//急停
    {CAN_MASTER_CMD_UTIL_GET_VERSION     ,        ENABLE,     3000,      21,     0       },//版本号获取
    {CAN_MASTER_CMD_UTIL_SET_RUNNING_MODE,        ENABLE,     3000,      0,      1       },//设置运行模式
};

/*************************************发送指令等待ACK************************************************/
//联机
void CanMasterUtilOnlineWhileAck(uint8_t targetModule,uint8_t targetBoardID,PROT_CONNECT_FLAG protConnectFlag,
                                        SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_ACT_UTIL;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_UTIL_ONLINE;
    CAN_PROT_CMD_SEND* canCmdSendPtr = NULL;
    CanProtClearLastCmd(targetModule,targetBoardID,singalChannel,channelCmdCode,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        CAN_PROT_LOG("%s,%d,ErrorSub: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    CAN_PROT_CMD_CONFIG* canCmdConfigPtr = CanProtGetCmdConfig(channelCmdCode,(CAN_PROT_CMD_CONFIG*)canMasterUtilCmdConfigArray,
                                                                DIM_ARRAY_ELEMENT_COUNT(canMasterUtilCmdConfigArray));
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
    canCmdSendPtr->cmdBufferPtr[0] = (uint8_t)protConnectFlag;
    //指令发送
    CanProtSendCmdWaitAck(canCmdSendPtr,resultPackPtr);
    //销毁发送指令
    CanProtDestroyCmdSend(&canCmdSendPtr);
    return;
}

//脱机
void CanMasterUtilOffLineWhileAck(uint8_t targetModule,uint8_t targetBoardID,PROT_DISCONNECT_FLAG disconnectFlag,
                                        SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_ACT_UTIL;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_UTIL_OFFLINE;
    CAN_PROT_CMD_SEND* canCmdSendPtr = NULL;
    CanProtClearLastCmd(targetModule,targetBoardID,singalChannel,channelCmdCode,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        CAN_PROT_LOG("%s,%d,ErrorSub: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    CAN_PROT_CMD_CONFIG* canCmdConfigPtr = CanProtGetCmdConfig(channelCmdCode,(CAN_PROT_CMD_CONFIG*)canMasterUtilCmdConfigArray,
                                                                DIM_ARRAY_ELEMENT_COUNT(canMasterUtilCmdConfigArray));
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
    canCmdSendPtr->cmdBufferPtr[0] = (uint8_t)disconnectFlag;
    //指令发送
    CanProtSendCmdWaitAck(canCmdSendPtr,resultPackPtr);
    //销毁发送指令
    CanProtDestroyCmdSend(&canCmdSendPtr);
    return;
}

//UTC时间同步
void CanMasterUtilUtcSyncWhileAck(uint8_t targetModule,uint8_t targetBoardID,uint64_t utcStamp,
                                        SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_ACT_UTIL;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_UTIL_UTC_SYNC;
    CAN_PROT_CMD_SEND* canCmdSendPtr = NULL;
    CanProtClearLastCmd(targetModule,targetBoardID,singalChannel,channelCmdCode,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        CAN_PROT_LOG("%s,%d,ErrorSub: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    CAN_PROT_CMD_CONFIG* canCmdConfigPtr = CanProtGetCmdConfig(channelCmdCode,(CAN_PROT_CMD_CONFIG*)canMasterUtilCmdConfigArray,
                                                                DIM_ARRAY_ELEMENT_COUNT(canMasterUtilCmdConfigArray));
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
    ProtUtilWriteUint64ToBuffer(utcStamp,canCmdSendPtr->cmdBufferPtr);
    //指令发送
    CanProtSendCmdWaitAck(canCmdSendPtr,resultPackPtr);
    //销毁发送指令
    CanProtDestroyCmdSend(&canCmdSendPtr);
    return;
}

//心跳获取
void CanMasterUtilGetHeartWhileAck(uint8_t targetModule,uint8_t targetBoardID,SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_ACT_UTIL;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_UTIL_HEART_GET;
    CAN_PROT_CMD_SEND* canCmdSendPtr = NULL;
    CanProtClearLastCmd(targetModule,targetBoardID,singalChannel,channelCmdCode,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        CAN_PROT_LOG("%s,%d,ErrorSub: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    CAN_PROT_CMD_CONFIG* canCmdConfigPtr = CanProtGetCmdConfig(channelCmdCode,(CAN_PROT_CMD_CONFIG*)canMasterUtilCmdConfigArray,
                                                                DIM_ARRAY_ELEMENT_COUNT(canMasterUtilCmdConfigArray));
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
    //指令发送
    CanProtSendCmdWaitAck(canCmdSendPtr,resultPackPtr);
    //销毁发送指令
    CanProtDestroyCmdSend(&canCmdSendPtr);
    return;
}

//急停
void CanMasterUtilEmStopWhileAck(uint8_t targetModule,uint8_t targetBoardID,SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_ACT_UTIL;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_UTIL_EM_STOP;
    CAN_PROT_CMD_SEND* canCmdSendPtr = NULL;
    CanProtClearLastCmd(targetModule,targetBoardID,singalChannel,channelCmdCode,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        CAN_PROT_LOG("%s,%d,ErrorSub: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    CAN_PROT_CMD_CONFIG* canCmdConfigPtr = CanProtGetCmdConfig(channelCmdCode,(CAN_PROT_CMD_CONFIG*)canMasterUtilCmdConfigArray,
                                                                DIM_ARRAY_ELEMENT_COUNT(canMasterUtilCmdConfigArray));
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
    //指令发送
    CanProtSendCmdWaitAck(canCmdSendPtr,resultPackPtr);
    //销毁发送指令
    CanProtDestroyCmdSend(&canCmdSendPtr);
    return;
}

//版本号获取
void CanMasterUtilGetVersionWhileAck(uint8_t targetModule,uint8_t targetBoardID,SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_ACT_UTIL;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_UTIL_GET_VERSION;
    CAN_PROT_CMD_SEND* canCmdSendPtr = NULL;
    CanProtClearLastCmd(targetModule,targetBoardID,singalChannel,channelCmdCode,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        CAN_PROT_LOG("%s,%d,ErrorSub: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    CAN_PROT_CMD_CONFIG* canCmdConfigPtr = CanProtGetCmdConfig(channelCmdCode,(CAN_PROT_CMD_CONFIG*)canMasterUtilCmdConfigArray,
                                                                DIM_ARRAY_ELEMENT_COUNT(canMasterUtilCmdConfigArray));
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
    //指令发送
    CanProtSendCmdWaitAck(canCmdSendPtr,resultPackPtr);
    //销毁发送指令
    CanProtDestroyCmdSend(&canCmdSendPtr);
    return;
}

//设置系统运行模式
void CanMasterUtilSetRunningModeWhileAck(uint8_t targetModule,uint8_t targetBoardID,SYS_RUNNING_MODE runningMode,SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_ACT_UTIL;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_UTIL_SET_RUNNING_MODE;
    CAN_PROT_CMD_SEND* canCmdSendPtr = NULL;
    CanProtClearLastCmd(targetModule,targetBoardID,singalChannel,channelCmdCode,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        CAN_PROT_LOG("%s,%d,ErrorSub: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    CAN_PROT_CMD_CONFIG* canCmdConfigPtr = CanProtGetCmdConfig(channelCmdCode,(CAN_PROT_CMD_CONFIG*)canMasterUtilCmdConfigArray,
                                                                DIM_ARRAY_ELEMENT_COUNT(canMasterUtilCmdConfigArray));
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
    canCmdSendPtr->cmdBufferPtr[0] = (uint8_t)runningMode;
    //指令发送
    CanProtSendCmdWaitAck(canCmdSendPtr,resultPackPtr);
    //销毁发送指令
    CanProtDestroyCmdSend(&canCmdSendPtr);
    return;
}
/*************************************等待指定指令完成返回************************************************/
//联机
void CanMasterUtilWaitOnlineResult(uint8_t targetModule,uint8_t targetBoardID,SYS_RESULT_PACK* resultPackPtr,
                                            BOARD_RUN_MODE* boardRunModePtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_ACT_UTIL;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_UTIL_ONLINE;
    //保存结果
    uint8_t* returnDataBufferPtr = NULL;
    uint16_t returnDataBufferLength = 0;
    //等待指令返回
    CanProtCmdWaitResult(targetModule,targetBoardID,singalChannel,channelCmdCode,resultPackPtr,
                            &returnDataBufferPtr,&returnDataBufferLength);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        CAN_PROT_LOG("%s,%d,ProtocolCanWaitResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        //释放内存
        CanProtFreeRecvDataBuf(targetModule,targetBoardID,returnDataBufferPtr,returnDataBufferLength);
        return;
    }
    if(boardRunModePtr != NULL)
    {
        //结果解析
        *boardRunModePtr = BOARD_RUN_MODE_APP;
        if((returnDataBufferLength != 0)&&(returnDataBufferPtr != NULL))
        {
            *boardRunModePtr = (BOARD_RUN_MODE)(returnDataBufferPtr[1]);
        }
    }
    //内存释放
    CanProtFreeRecvDataBuf(targetModule,targetBoardID,returnDataBufferPtr,returnDataBufferLength);
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//脱机
void CanMasterUtilWaitOffLineResult(uint8_t targetModule,uint8_t targetBoardID,SYS_RESULT_PACK* resultPackPtr,
                                            BOARD_RUN_MODE* boardRunModePtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_ACT_UTIL;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_UTIL_OFFLINE;
    //保存结果
    uint8_t* returnDataBufferPtr = NULL;
    uint16_t returnDataBufferLength = 0;
    //等待指令返回
    CanProtCmdWaitResult(targetModule,targetBoardID,singalChannel,channelCmdCode,resultPackPtr,
                            &returnDataBufferPtr,&returnDataBufferLength);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        CAN_PROT_LOG("%s,%d,ProtocolCanWaitResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        //释放内存
        CanProtFreeRecvDataBuf(targetModule,targetBoardID,returnDataBufferPtr,returnDataBufferLength);
        return;
    }
    if(boardRunModePtr != NULL)
    {
        //结果解析
        *boardRunModePtr = BOARD_RUN_MODE_APP;
        if((returnDataBufferLength != 0)&&(returnDataBufferPtr != NULL))
        {
            *boardRunModePtr = (BOARD_RUN_MODE)(returnDataBufferPtr[0]);
        }
    }
    //内存释放
    CanProtFreeRecvDataBuf(targetModule,targetBoardID,returnDataBufferPtr,returnDataBufferLength);
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//UTC时间同步
void CanMasterUtilWaitUtcSyncResult(uint8_t targetModule,uint8_t targetBoardID,SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_ACT_UTIL;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_UTIL_UTC_SYNC;
    //保存结果
    uint8_t* returnDataBufferPtr = NULL;
    uint16_t returnDataBufferLength = 0;
    //等待指令返回
    CanProtCmdWaitResult(targetModule,targetBoardID,singalChannel,channelCmdCode,resultPackPtr,
                            &returnDataBufferPtr,&returnDataBufferLength);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        CAN_PROT_LOG("%s,%d,ProtocolCanWaitResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        //释放内存
        CanProtFreeRecvDataBuf(targetModule,targetBoardID,returnDataBufferPtr,returnDataBufferLength);
        return;
    }
    //内存释放
    CanProtFreeRecvDataBuf(targetModule,targetBoardID,returnDataBufferPtr,returnDataBufferLength);
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//心跳获取
void CanMasterUtilWaitGetHeartResult(uint8_t targetModule,uint8_t targetBoardID,SYS_RESULT_PACK* resultPackPtr,
                                            uint64_t* utcStampPtr,BOARD_RUN_MODE* boardRunModePtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_ACT_UTIL;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_UTIL_HEART_GET;
    //保存结果
    uint8_t* returnDataBufferPtr = NULL;
    uint16_t returnDataBufferLength = 0;
    //等待指令返回
    CanProtCmdWaitResult(targetModule,targetBoardID,singalChannel,channelCmdCode,resultPackPtr,
                            &returnDataBufferPtr,&returnDataBufferLength);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        CAN_PROT_LOG("%s,%d,ProtocolCanWaitResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        //释放内存
        CanProtFreeRecvDataBuf(targetModule,targetBoardID,returnDataBufferPtr,returnDataBufferLength);
        return;
    }
    //结果解析
    if(utcStampPtr != NULL)
    {
        *utcStampPtr = 0;
        if((returnDataBufferLength != 0)&&(returnDataBufferPtr != NULL))
        {
            *utcStampPtr = ProtUtilReadUint64Buffer(returnDataBufferPtr);
        }
    }
    if(boardRunModePtr != NULL)
    {
        *boardRunModePtr = BOARD_RUN_MODE_APP;
        if((returnDataBufferLength != 0)&&(returnDataBufferPtr != NULL))
        {
            *boardRunModePtr = (BOARD_RUN_MODE)(returnDataBufferPtr[8]);
        }
    }
    //内存释放
    CanProtFreeRecvDataBuf(targetModule,targetBoardID,returnDataBufferPtr,returnDataBufferLength);
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//急停
void CanMasterUtilWaitEmStopResult(uint8_t targetModule,uint8_t targetBoardID,SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_ACT_UTIL;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_UTIL_EM_STOP;
    //保存结果
    uint8_t* returnDataBufferPtr = NULL;
    uint16_t returnDataBufferLength = 0;
    //等待指令返回
    CanProtCmdWaitResult(targetModule,targetBoardID,singalChannel,channelCmdCode,resultPackPtr,
                            &returnDataBufferPtr,&returnDataBufferLength);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        CAN_PROT_LOG("%s,%d,ProtocolCanWaitResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        //释放内存
        CanProtFreeRecvDataBuf(targetModule,targetBoardID,returnDataBufferPtr,returnDataBufferLength);
        return;
    }
    //内存释放
    CanProtFreeRecvDataBuf(targetModule,targetBoardID,returnDataBufferPtr,returnDataBufferLength);
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//版本号获取
void CanMasterUtilWaitGetVersionResult(uint8_t targetModule,uint8_t targetBoardID,SYS_RESULT_PACK* resultPackPtr,
                                                BOARD_SOFT_VERSION* bootVersionPtr,BOARD_SOFT_VERSION* srvVersionPtr,
                                                BOARD_SOFT_VERSION* appVersionPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_ACT_UTIL;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_UTIL_GET_VERSION;
    //保存结果
    uint8_t* returnDataBufferPtr = NULL;
    uint16_t returnDataBufferLength = 0;
    //等待指令返回
    CanProtCmdWaitResult(targetModule,targetBoardID,singalChannel,channelCmdCode,resultPackPtr,
                            &returnDataBufferPtr,&returnDataBufferLength);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        CAN_PROT_LOG("%s,%d,ProtocolCanWaitResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        //释放内存
        CanProtFreeRecvDataBuf(targetModule,targetBoardID,returnDataBufferPtr,returnDataBufferLength);
        return;
    }
    //结果解析
    if(bootVersionPtr != NULL)
    {
        bootVersionPtr->mainVersion = 0;
        bootVersionPtr->subVersion = 0;
        bootVersionPtr->tempVersion = 0;
        bootVersionPtr->devVersion = 0;
        if((returnDataBufferLength != 0)&&(returnDataBufferPtr != NULL))
        {
            bootVersionPtr->mainVersion = returnDataBufferPtr[0];
            bootVersionPtr->subVersion = returnDataBufferPtr[1];
            bootVersionPtr->tempVersion = returnDataBufferPtr[2];
            bootVersionPtr->devVersion = ProtUtilReadUint32Buffer(returnDataBufferPtr+3);
        }
    }
    if(srvVersionPtr != NULL)
    {
        srvVersionPtr->mainVersion = 0;
        srvVersionPtr->subVersion = 0;
        srvVersionPtr->tempVersion = 0;
        srvVersionPtr->devVersion = 0;
        if((returnDataBufferLength != 0)&&(returnDataBufferPtr != NULL))
        {
            srvVersionPtr->mainVersion = returnDataBufferPtr[7];
            srvVersionPtr->subVersion = returnDataBufferPtr[8];
            srvVersionPtr->tempVersion = returnDataBufferPtr[9];
            srvVersionPtr->devVersion = ProtUtilReadUint32Buffer(returnDataBufferPtr+10);
        }
    }
    if(appVersionPtr != NULL)
    {
        appVersionPtr->mainVersion = 0;
        appVersionPtr->subVersion = 0;
        appVersionPtr->tempVersion = 0;
        appVersionPtr->devVersion = 0;
        if((returnDataBufferLength != 0)&&(returnDataBufferPtr != NULL))
        {
            appVersionPtr->mainVersion = returnDataBufferPtr[14];
            appVersionPtr->subVersion = returnDataBufferPtr[15];
            appVersionPtr->tempVersion = returnDataBufferPtr[16];
            appVersionPtr->devVersion = ProtUtilReadUint32Buffer(returnDataBufferPtr+17);
        }
    }
    //内存释放
    CanProtFreeRecvDataBuf(targetModule,targetBoardID,returnDataBufferPtr,returnDataBufferLength);
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//设置运行模式
void CanMasterUtilWaitSetRunningModeResult(uint8_t targetModule,uint8_t targetBoardID,SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_ACT_UTIL;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_UTIL_SET_RUNNING_MODE;
    //保存结果
    uint8_t* returnDataBufferPtr = NULL;
    uint16_t returnDataBufferLength = 0;
    //等待指令返回
    CanProtCmdWaitResult(targetModule,targetBoardID,singalChannel,channelCmdCode,resultPackPtr,
                            &returnDataBufferPtr,&returnDataBufferLength);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        CAN_PROT_LOG("%s,%d,ProtocolCanWaitResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
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
//联机
void CanMasterUtilOnlineWhileReturn(uint8_t targetModule,uint8_t targetBoardID,SYS_RESULT_PACK* resultPackPtr,
                                                PROT_CONNECT_FLAG protConnectFlag,BOARD_RUN_MODE* boardRunModePtr)
{
    //发送指令等待ACK
    CanMasterUtilOnlineWhileAck(targetModule,targetBoardID,protConnectFlag,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        CAN_PROT_LOG("%s,%d,CanMasterUtilOnlineWhileAck ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    //等待结果
    CanMasterUtilWaitOnlineResult(targetModule,targetBoardID,resultPackPtr,boardRunModePtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        CAN_PROT_LOG("%s,%d,CanMasterUtilWaitOnlineResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
    }
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//脱机
void CanMasterUtilOffLineWhileReturn(uint8_t targetModule,uint8_t targetBoardID,PROT_DISCONNECT_FLAG disconnectFlag,
                                                    SYS_RESULT_PACK* resultPackPtr,BOARD_RUN_MODE* boardRunModePtr)
{
    //发送指令等待ACK
    CanMasterUtilOffLineWhileAck(targetModule,targetBoardID,disconnectFlag,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        CAN_PROT_LOG("%s,%d,CanMasterUtilOffLineWhileAck ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    //等待结果
    CanMasterUtilWaitOffLineResult(targetModule,targetBoardID,resultPackPtr,boardRunModePtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        CAN_PROT_LOG("%s,%d,CanMasterUtilWaitOffLineResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
    }
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//UTC时间同步
void CanMasterUtilUtcSyncWhileReturn(uint8_t targetModule,uint8_t targetBoardID,uint64_t utcStamp,
                                                    SYS_RESULT_PACK* resultPackPtr)
{
    //发送指令等待ACK
    CanMasterUtilUtcSyncWhileAck(targetModule,targetBoardID,utcStamp,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        CAN_PROT_LOG("%s,%d,CanMasterUtilUtcSyncWhileAck ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    //等待结果
    CanMasterUtilWaitUtcSyncResult(targetModule,targetBoardID,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        CAN_PROT_LOG("%s,%d,CanMasterUtilWaitUtcSyncResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
    }
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}
                                                
//心跳获取
void CanMasterUtilGetHeartWhileReturn(uint8_t targetModule,uint8_t targetBoardID,SYS_RESULT_PACK* resultPackPtr,
                                                uint64_t* utcStampPtr,BOARD_RUN_MODE* boardRunModePtr)
{
    //发送指令等待ACK
    CanMasterUtilGetHeartWhileAck(targetModule,targetBoardID,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        CAN_PROT_LOG("%s,%d,CanMasterUtilGetHeartWhileAck ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    //等待结果
    CanMasterUtilWaitGetHeartResult(targetModule,targetBoardID,resultPackPtr,utcStampPtr,boardRunModePtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        CAN_PROT_LOG("%s,%d,CanMasterUtilWaitGetHeartResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
    }
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//急停
void CanMasterUtilEmStopWhileReturn(uint8_t targetModule,uint8_t targetBoardID,SYS_RESULT_PACK* resultPackPtr)
{
    //发送指令等待ACK
    CanMasterUtilEmStopWhileAck(targetModule,targetBoardID,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        CAN_PROT_LOG("%s,%d,CanMasterUtilEmStopWhileAck ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    //等待结果
    CanMasterUtilWaitEmStopResult(targetModule,targetBoardID,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        CAN_PROT_LOG("%s,%d,CanMasterUtilWaitEmStopResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
    }
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//版本号获取
void CanMasterUtilGetVersionWhileReturn(uint8_t targetModule,uint8_t targetBoardID,SYS_RESULT_PACK* resultPackPtr,
                                                BOARD_SOFT_VERSION* bootVersionPtr,BOARD_SOFT_VERSION* srvVersionPtr,
                                                BOARD_SOFT_VERSION* appVersionPtr)
{
    //发送指令等待ACK
    CanMasterUtilGetVersionWhileAck(targetModule,targetBoardID,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        CAN_PROT_LOG("%s,%d,CanMasterUtilGetVersionWhileAck ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    //等待结果
    CanMasterUtilWaitGetVersionResult(targetModule,targetBoardID,resultPackPtr,bootVersionPtr,srvVersionPtr,appVersionPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        CAN_PROT_LOG("%s,%d,CanMasterUtilWaitGetVersionResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
    }
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//设置运行模式
void CanMasterUtilSetRunningModeWhileReturn(uint8_t targetModule,uint8_t targetBoardID,SYS_RUNNING_MODE runningMode,SYS_RESULT_PACK* resultPackPtr)
{
    //发送指令等待ACK
    CanMasterUtilSetRunningModeWhileAck(targetModule,targetBoardID,runningMode,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        CAN_PROT_LOG("%s,%d,CanMasterUtilSetRunningModeWhileAck ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    //等待结果
    CanMasterUtilWaitSetRunningModeResult(targetModule,targetBoardID,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        CAN_PROT_LOG("%s,%d,CanMasterUtilWaitSetRunningModeResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
    }
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}




























