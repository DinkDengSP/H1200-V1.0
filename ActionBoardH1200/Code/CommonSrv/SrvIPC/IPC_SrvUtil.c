/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-08-25 08:45:09 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-10-26 10:21:15 +0800
************************************************************************************************/ 
#include "IPC_SrvUtil.h"
#include "CommonSrvTaskHeader.h"

//通道携带的数据缓存长度
#define LENGTH_BUFFER_IPC_SRV_UTIL             128
//IPC通道讯息
static IPC_CHANNEL_DATA ipcChannelDataSrvUtil;
//通道名称
static const uint8_t* ipcChannelSrvUtilNameStr = "SrvUtil";


/*****************************************************通道控制************************************************************/
//初始化IPC通道
void IPC_SrvUtilInit(void)
{
    IPC_BaseInitChannelData(&ipcChannelDataSrvUtil,(uint8_t*)ipcChannelSrvUtilNameStr,
                                &queueSrvTaskUtil,LENGTH_BUFFER_IPC_SRV_UTIL);
}

//清空IPC信道
ERROR_SUB IPC_SrvUtilClearChannelByCmd(uint32_t cmdCode)
{
    return IPC_BaseChannelClearChannel(&ipcChannelDataSrvUtil,cmdCode);
}

//清空全部信道
ERROR_SUB IPC_SrvUtilClearChannel(void)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    //首先清除通讯测试指令
    errorSub = IPC_BaseChannelClearChannel(&ipcChannelDataSrvUtil,IPC_CMD_MACRO_COMM_TEST);
    if(errorSub != ERROR_SUB_OK)
    {
        SystemPrintf("%s,Cmd: 0X%08X,ErrorSub: 0X%08X\r\n",__func__,IPC_CMD_MACRO_COMM_TEST,errorSub);
        return errorSub;
    }
    //通道内部全部指令清空
    uint32_t cmdCode = 0X00000000;
    for(cmdCode = IPC_CMD_MACRO_UTIL_ONLINE;cmdCode <= IPC_CMD_MACRO_UTIL_GET_VERSION;cmdCode++)
    {
        errorSub = IPC_BaseChannelClearChannel(&ipcChannelDataSrvUtil,cmdCode);
        if(errorSub != ERROR_SUB_OK)
        {
            SystemPrintf("%s,Cmd: 0X%08X,ErrorSub: 0X%08X\r\n",__func__,cmdCode,errorSub);
            return errorSub;
        }
    }
    return ERROR_SUB_OK;
}


/*****************************************************等待指令返回************************************************************/
//等待指定指令结果返回
void IPC_SrvUtilWaitCmdReturn(uint32_t cmdCode,SYS_RESULT_PACK* resultPackPtr,IPC_RETURN_DATA* returnDataPtr)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    //先赋初值
    if(returnDataPtr != NULL)
    {
        returnDataPtr->returnBufferPtr = NULL;
        returnDataPtr->returnDataLength = 0;
    }
    //等待指令返回
    errorSub = IPC_BaseWaitCmdReturn(&ipcChannelDataSrvUtil,cmdCode,resultPackPtr);
    if(errorSub != ERROR_SUB_OK)
    {
        SystemPrintf("%s,Cmd: 0X%08X,ErrorSub: 0X%08X\r\n",__func__,cmdCode,errorSub);
        //IPC报错,返回错误
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_IPC_PROT_CMD_WAIT_RESULT,ERROR_LEVEL_ERROR,
                                        errorSub);
        return;
    }
    //结果是否存在数据,如果存在数据,需要申请内存将数据返回
    if((ipcChannelDataSrvUtil.channelCache.returnDataBufferLength != 0)&&(returnDataPtr != NULL))
    {
        //申请内存
        returnDataPtr->returnDataLength = ipcChannelDataSrvUtil.channelCache.returnDataBufferLength;
        //不为0拷贝数据
        if(returnDataPtr->returnDataLength != 0)
        {
            returnDataPtr->returnBufferPtr = IPC_PortMemMallocWhileSuccess(returnDataPtr->returnDataLength);
            //数据拷贝
            UserMemCopy(returnDataPtr->returnBufferPtr,ipcChannelDataSrvUtil.channelCache.dataBufferPtr,returnDataPtr->returnDataLength);
        }
    }
    //设置无错误返回
    CoreUtilSetSystemResultPackNoError(resultPackPtr);
    return;
}

//等待上一次发送的指令结果返回
void IPC_SrvUtilWaitLastCmdReturn(SYS_RESULT_PACK* resultPackPtr,IPC_RETURN_DATA* returnDataPtr)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    //先赋初值
    if(returnDataPtr != NULL)
    {
        returnDataPtr->returnBufferPtr = NULL;
        returnDataPtr->returnDataLength = 0;
    }
    //等待指令返回
    errorSub = IPC_BaseWaitLastCmdReturn(&ipcChannelDataSrvUtil,resultPackPtr);
    if(errorSub != ERROR_SUB_OK)
    {
        SystemPrintf("%s,ErrorSub: 0X%08X\r\n",__func__,errorSub);
        //IPC报错,返回错误
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_IPC_PROT_CMD_WAIT_RESULT,ERROR_LEVEL_ERROR,
                                        errorSub);
        return;
    }
    //结果是否存在数据,如果存在数据,需要申请内存将数据返回
    if((ipcChannelDataSrvUtil.channelCache.returnDataBufferLength != 0)&&(returnDataPtr != NULL))
    {
        //申请内存
        returnDataPtr->returnDataLength = ipcChannelDataSrvUtil.channelCache.returnDataBufferLength;
        //不为0拷贝数据
        if(returnDataPtr->returnDataLength != 0)
        {
            returnDataPtr->returnBufferPtr = IPC_PortMemMallocWhileSuccess(returnDataPtr->returnDataLength);
            //数据拷贝
            UserMemCopy(returnDataPtr->returnBufferPtr,ipcChannelDataSrvUtil.channelCache.dataBufferPtr,returnDataPtr->returnDataLength);
        }
    }
    //设置无错误返回
    CoreUtilSetSystemResultPackNoError(resultPackPtr);
    return;
}


/*****************************************************查询指令结果-通用************************************************************/
//查询指定指令的状态
void IPC_SrvUtilQueryCmdState(uint32_t cmdCode,IPC_CMD_STAGE* cmdStagePtr,SYS_RESULT_PACK* resultPackPtr,
                                        IPC_RETURN_DATA* returnDataPtr)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    //先赋初值
    if(returnDataPtr != NULL)
    {
        returnDataPtr->returnBufferPtr = NULL;
        returnDataPtr->returnDataLength = 0;
    }
    errorSub = IPC_BaseQueryCmdState(&ipcChannelDataSrvUtil,cmdCode,cmdStagePtr,resultPackPtr);
    if(errorSub != ERROR_SUB_OK)
    {
        SystemPrintf("%s,Cmd: 0X%08X,ErrorSub: 0X%08X\r\n",__func__,cmdCode,errorSub);
        //IPC报错,返回错误
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_IPC_PROT_QUERY_CMD_STATE,ERROR_LEVEL_ERROR,
                                        errorSub);
        return;
    }
    if(*cmdStagePtr == IPC_CMD_STAGE_OVER)
    {
        if((resultPackPtr->resultCode == PROT_RESULT_SUCCESS)&&(returnDataPtr != NULL))
        {
            //申请内存
            returnDataPtr->returnDataLength = ipcChannelDataSrvUtil.channelCache.returnDataBufferLength;
            //不为0拷贝数
            if(returnDataPtr->returnDataLength != 0)
            {
                returnDataPtr->returnBufferPtr = IPC_PortMemMallocWhileSuccess(returnDataPtr->returnDataLength);
                //数据拷贝
                UserMemCopy(returnDataPtr->returnBufferPtr,ipcChannelDataSrvUtil.channelCache.dataBufferPtr,returnDataPtr->returnDataLength);
            }
        }
        //设置无错误返回
        CoreUtilSetSystemResultPackNoError(resultPackPtr);
        return;
    }
    //设置无错误返回
    CoreUtilSetSystemResultPackNoError(resultPackPtr);
    return;
}

//查询上一次发送的指令的状态
void IPC_SrvUtilQueryLastCmdState(IPC_CMD_STAGE* cmdStagePtr,SYS_RESULT_PACK* resultPackPtr,IPC_RETURN_DATA* returnDataPtr)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    //先赋初值
    if(returnDataPtr != NULL)
    {
        returnDataPtr->returnBufferPtr = NULL;
        returnDataPtr->returnDataLength = 0;
    }
    errorSub = IPC_BaseQueryLastCmdState(&ipcChannelDataSrvUtil,cmdStagePtr,resultPackPtr);
    if(errorSub != ERROR_SUB_OK)
    {
        SystemPrintf("%s,ErrorSub: 0X%08X\r\n",__func__,errorSub);
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_IPC_PROT_QUERY_CMD_STATE,ERROR_LEVEL_ERROR,
                                        errorSub);
        return;
    }
    if(*cmdStagePtr == IPC_CMD_STAGE_OVER)
    {
        if((resultPackPtr->resultCode == PROT_RESULT_SUCCESS)&&(returnDataPtr != NULL))
        {
            //申请内存
            returnDataPtr->returnDataLength = ipcChannelDataSrvUtil.channelCache.returnDataBufferLength;
            //不为0拷贝数据
            if(returnDataPtr->returnDataLength != 0)
            {
                returnDataPtr->returnBufferPtr = IPC_PortMemMallocWhileSuccess(returnDataPtr->returnDataLength);
                //数据拷贝
                UserMemCopy(returnDataPtr->returnBufferPtr,ipcChannelDataSrvUtil.channelCache.dataBufferPtr,returnDataPtr->returnDataLength);
            }
        }
        //设置无错误返回
        CoreUtilSetSystemResultPackNoError(resultPackPtr);
        return;
    }
    //设置无错误返回
    CoreUtilSetSystemResultPackNoError(resultPackPtr);
    return;
}


/*****************************************************等待指令返回-特用************************************************************/
//通讯测试,WAIT
void IPC_SrvUtilCommTestWaitCmdReturn(SYS_RESULT_PACK* resultPackPtr)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    errorSub = IPC_BaseWaitCmdReturn(&ipcChannelDataSrvUtil,IPC_CMD_MACRO_COMM_TEST,resultPackPtr);
    if(errorSub != ERROR_SUB_OK)
    {
        SystemPrintf("%s,Cmd: 0X%08X,ErrorSub: 0X%08X\r\n",__func__,IPC_CMD_MACRO_COMM_TEST,errorSub);
        //IPC报错,返回错误
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_IPC_PROT_CMD_WAIT_RESULT,ERROR_LEVEL_ERROR,
                                        errorSub);
        return;
    }
    return;
}

//联机,WAIT
void IPC_SrvUtilOnLineWaitCmdReturn(SYS_RESULT_PACK* resultPackPtr,BOARD_RUN_MODE* boardModePtr)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    //默认值
    if(boardModePtr != NULL)
    {
        *boardModePtr = BOARD_RUN_MODE_BOOT;
    }
    errorSub = IPC_BaseWaitCmdReturn(&ipcChannelDataSrvUtil,IPC_CMD_MACRO_UTIL_ONLINE,resultPackPtr);
    if(errorSub != ERROR_SUB_OK)
    {
        SystemPrintf("%s,Cmd: 0X%08X,ErrorSub: 0X%08X\r\n",__func__,IPC_CMD_MACRO_UTIL_ONLINE,errorSub);
        //IPC报错,返回错误
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_IPC_PROT_CMD_WAIT_RESULT,ERROR_LEVEL_ERROR,
                                        errorSub);
        return;
    }
    if(resultPackPtr->resultCode == PROT_RESULT_SUCCESS)
    {
        if(boardModePtr != NULL)
        {
            //结果解析
            *boardModePtr = (BOARD_RUN_MODE)(ipcChannelDataSrvUtil.channelCache.dataBufferPtr[0]);
        }
    }
    return;
}

//脱机,WAIT
void IPC_SrvUtilOffLineWaitCmdReturn(SYS_RESULT_PACK* resultPackPtr,BOARD_RUN_MODE* boardModePtr)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    //默认值
    if(boardModePtr != NULL)
    {
        *boardModePtr = BOARD_RUN_MODE_BOOT;
    }
    errorSub = IPC_BaseWaitCmdReturn(&ipcChannelDataSrvUtil,IPC_CMD_MACRO_UTIL_OFFLINE,resultPackPtr);
    if(errorSub != ERROR_SUB_OK)
    {
        SystemPrintf("%s,Cmd: 0X%08X,ErrorSub: 0X%08X\r\n",__func__,IPC_CMD_MACRO_UTIL_OFFLINE,errorSub);
        //IPC报错,返回错误
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_IPC_PROT_CMD_WAIT_RESULT,ERROR_LEVEL_ERROR,
                                        errorSub);
        return;
    }
    if(resultPackPtr->resultCode == PROT_RESULT_SUCCESS)
    {
        if(boardModePtr != NULL)
        {
            //结果解析
            *boardModePtr = (BOARD_RUN_MODE)(ipcChannelDataSrvUtil.channelCache.dataBufferPtr[0]);
        }
    }
    return;
}

//UTC同步,WAIT
void IPC_SrvUtilUtcSyncWaitCmdReturn(SYS_RESULT_PACK* resultPackPtr)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    errorSub = IPC_BaseWaitCmdReturn(&ipcChannelDataSrvUtil,IPC_CMD_MACRO_UTIL_UTC_SYNC,resultPackPtr);
    if(errorSub != ERROR_SUB_OK)
    {
        SystemPrintf("%s,Cmd: 0X%08X,ErrorSub: 0X%08X\r\n",__func__,IPC_CMD_MACRO_UTIL_UTC_SYNC,errorSub);
        //IPC报错,返回错误
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_IPC_PROT_CMD_WAIT_RESULT,ERROR_LEVEL_ERROR,
                                        errorSub);
        return;
    }
    return;
}

//心跳获取,WAIT
void IPC_SrvUtilHeartGetWaitCmdReturn(SYS_RESULT_PACK* resultPackPtr,uint64_t* utcTimeStampPtr,BOARD_RUN_MODE* boardModePtr)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    //默认值
    if(utcTimeStampPtr != NULL)
    {
        *utcTimeStampPtr = 0X0000000000000000;
    }
    if(boardModePtr != NULL)
    {
        *boardModePtr = BOARD_RUN_MODE_BOOT;
    }
    errorSub = IPC_BaseWaitCmdReturn(&ipcChannelDataSrvUtil,IPC_CMD_MACRO_UTIL_HEART_GET,resultPackPtr);
    if(errorSub != ERROR_SUB_OK)
    {
        SystemPrintf("%s,Cmd: 0X%08X,ErrorSub: 0X%08X\r\n",__func__,IPC_CMD_MACRO_UTIL_HEART_GET,errorSub);
        //IPC报错,返回错误
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_IPC_PROT_CMD_WAIT_RESULT,ERROR_LEVEL_ERROR,
                                        errorSub);
        return;
    }
    if(resultPackPtr->resultCode == PROT_RESULT_SUCCESS)
    {
        //结果解析
        if(utcTimeStampPtr != NULL)
        {
            *utcTimeStampPtr = ProtUtilReadUint64Buffer(ipcChannelDataSrvUtil.channelCache.dataBufferPtr);
        }
        if(boardModePtr != NULL)
        {
            *boardModePtr = (BOARD_RUN_MODE)(ipcChannelDataSrvUtil.channelCache.dataBufferPtr[8]);
        }
    }
    return;
}

//急停,WAIT
void IPC_SrvUtilEmStopWaitCmdReturn(SYS_RESULT_PACK* resultPackPtr)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    errorSub = IPC_BaseWaitCmdReturn(&ipcChannelDataSrvUtil,IPC_CMD_MACRO_UTIL_EM_STOP,resultPackPtr);
    if(errorSub != ERROR_SUB_OK)
    {
        SystemPrintf("%s,Cmd: 0X%08X,ErrorSub: 0X%08X\r\n",__func__,IPC_CMD_MACRO_UTIL_EM_STOP,errorSub);
        //IPC报错,返回错误
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_IPC_PROT_CMD_WAIT_RESULT,ERROR_LEVEL_ERROR,
                                        errorSub);
        return;
    }
    return;
}

//获取版本号,WAIT
void IPC_SrvUtilGetVersionWaitCmdReturn(SYS_RESULT_PACK* resultPackPtr,BOARD_SOFT_VERSION* bootVesionPtr,BOARD_SOFT_VERSION* srvVersionPtr,
                                            BOARD_SOFT_VERSION* appVersionPtr)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    //默认值
    if(bootVesionPtr != NULL)
    {
        bootVesionPtr->mainVersion = 0X00;
        bootVesionPtr->subVersion = 0X00;
        bootVesionPtr->tempVersion = 0X00;
        bootVesionPtr->devVersion = 0X00000000;
    }
    if(srvVersionPtr != NULL)
    {
        srvVersionPtr->mainVersion = 0X00;
        srvVersionPtr->subVersion = 0X00;
        srvVersionPtr->tempVersion = 0X00;
        srvVersionPtr->devVersion = 0X00000000;
    }
    if(appVersionPtr != NULL)
    {
        appVersionPtr->mainVersion = 0X00;
        appVersionPtr->subVersion = 0X00;
        appVersionPtr->tempVersion = 0X00;
        appVersionPtr->devVersion = 0X00000000;
    }
    errorSub = IPC_BaseWaitCmdReturn(&ipcChannelDataSrvUtil,IPC_CMD_MACRO_UTIL_GET_VERSION,resultPackPtr);
    if(errorSub != ERROR_SUB_OK)
    {
        SystemPrintf("%s,Cmd: 0X%08X,ErrorSub: 0X%08X\r\n",__func__,IPC_CMD_MACRO_UTIL_GET_VERSION,errorSub);
        //IPC报错,返回错误
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_IPC_PROT_CMD_WAIT_RESULT,ERROR_LEVEL_ERROR,
                                        errorSub);
        return;
    }
    if(resultPackPtr->resultCode == PROT_RESULT_SUCCESS)
    {
        //结果解析
        if(bootVesionPtr != NULL)
        {
            bootVesionPtr->mainVersion = ipcChannelDataSrvUtil.channelCache.dataBufferPtr[0];
            bootVesionPtr->subVersion = ipcChannelDataSrvUtil.channelCache.dataBufferPtr[1];
            bootVesionPtr->tempVersion = ipcChannelDataSrvUtil.channelCache.dataBufferPtr[2];
            bootVesionPtr->devVersion = ProtUtilReadUint32Buffer(ipcChannelDataSrvUtil.channelCache.dataBufferPtr+3);
        }
        if(srvVersionPtr != NULL)
        {
            srvVersionPtr->mainVersion = ipcChannelDataSrvUtil.channelCache.dataBufferPtr[7];
            srvVersionPtr->subVersion = ipcChannelDataSrvUtil.channelCache.dataBufferPtr[8];
            srvVersionPtr->tempVersion = ipcChannelDataSrvUtil.channelCache.dataBufferPtr[9];
            srvVersionPtr->devVersion = ProtUtilReadUint32Buffer(ipcChannelDataSrvUtil.channelCache.dataBufferPtr+10);
        }
        if(appVersionPtr != NULL)
        {
            appVersionPtr->mainVersion = ipcChannelDataSrvUtil.channelCache.dataBufferPtr[14];
            appVersionPtr->subVersion = ipcChannelDataSrvUtil.channelCache.dataBufferPtr[15];
            appVersionPtr->tempVersion = ipcChannelDataSrvUtil.channelCache.dataBufferPtr[16];
            appVersionPtr->devVersion = ProtUtilReadUint32Buffer(ipcChannelDataSrvUtil.channelCache.dataBufferPtr+17);
        }
    }
    return;
}


/*****************************************************查询指令结果-特用************************************************************/
//通讯测试,QUERY
void IPC_SrvUtilQueryCommTestState(IPC_CMD_STAGE* cmdStagePtr,SYS_RESULT_PACK* resultPackPtr)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    errorSub = IPC_BaseQueryCmdState(&ipcChannelDataSrvUtil,IPC_CMD_MACRO_COMM_TEST,cmdStagePtr,resultPackPtr);
    if(errorSub != ERROR_SUB_OK)
    {
        SystemPrintf("%s,Cmd: 0X%08X,ErrorSub: 0X%08X\r\n",__func__,IPC_CMD_MACRO_COMM_TEST,errorSub);
        //IPC报错,返回错误
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_IPC_PROT_QUERY_CMD_STATE,ERROR_LEVEL_ERROR,
                                        errorSub);
        return;
    }
    return;
}

//联机,QUERY
void IPC_SrvUtilQueryOnLineState(IPC_CMD_STAGE* cmdStagePtr,SYS_RESULT_PACK* resultPackPtr,BOARD_RUN_MODE* boardModePtr)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    //默认值
    if(boardModePtr != NULL)
    {
        *boardModePtr = BOARD_RUN_MODE_BOOT;
    }
    errorSub = IPC_BaseQueryCmdState(&ipcChannelDataSrvUtil,IPC_CMD_MACRO_UTIL_ONLINE,cmdStagePtr,resultPackPtr);
    if(errorSub != ERROR_SUB_OK)
    {
        SystemPrintf("%s,Cmd: 0X%08X,ErrorSub: 0X%08X\r\n",__func__,IPC_CMD_MACRO_UTIL_ONLINE,errorSub);
        //IPC报错,返回错误
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_IPC_PROT_QUERY_CMD_STATE,ERROR_LEVEL_ERROR,
                                        errorSub);
        return;
    }
    if((*cmdStagePtr == IPC_CMD_STAGE_OVER)&&(resultPackPtr->resultCode == PROT_RESULT_SUCCESS))
    {
        if(boardModePtr != NULL)
        {
            //结果解析
            *boardModePtr = (BOARD_RUN_MODE)(ipcChannelDataSrvUtil.channelCache.dataBufferPtr[0]);
        }
    }
    return;
}

//脱机,QUERY
void IPC_SrvUtilQueryOffLineState(IPC_CMD_STAGE* cmdStagePtr,SYS_RESULT_PACK* resultPackPtr,BOARD_RUN_MODE* boardModePtr)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    //默认值
    if(boardModePtr != NULL)
    {
        *boardModePtr = BOARD_RUN_MODE_BOOT;
    }
    errorSub = IPC_BaseQueryCmdState(&ipcChannelDataSrvUtil,IPC_CMD_MACRO_UTIL_OFFLINE,cmdStagePtr,resultPackPtr);
    if(errorSub != ERROR_SUB_OK)
    {
        SystemPrintf("%s,Cmd: 0X%08X,ErrorSub: 0X%08X\r\n",__func__,IPC_CMD_MACRO_UTIL_OFFLINE,errorSub);
        //IPC报错,返回错误
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_IPC_PROT_QUERY_CMD_STATE,ERROR_LEVEL_ERROR,
                                        errorSub);
        return;
    }
    if((*cmdStagePtr == IPC_CMD_STAGE_OVER)&&(resultPackPtr->resultCode == PROT_RESULT_SUCCESS))
    {
        if(boardModePtr != NULL)
        {
            //结果解析
            *boardModePtr = (BOARD_RUN_MODE)(ipcChannelDataSrvUtil.channelCache.dataBufferPtr[0]);
        }
    }
    return;
}

//UTC同步,QUERY
void IPC_SrvUtilQueryUtcSyncState(IPC_CMD_STAGE* cmdStagePtr,SYS_RESULT_PACK* resultPackPtr)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    errorSub = IPC_BaseQueryCmdState(&ipcChannelDataSrvUtil,IPC_CMD_MACRO_UTIL_UTC_SYNC,cmdStagePtr,resultPackPtr);
    if(errorSub != ERROR_SUB_OK)
    {
        SystemPrintf("%s,Cmd: 0X%08X,ErrorSub: 0X%08X\r\n",__func__,IPC_CMD_MACRO_UTIL_UTC_SYNC,errorSub);
        //IPC报错,返回错误
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_IPC_PROT_QUERY_CMD_STATE,ERROR_LEVEL_ERROR,
                                        errorSub);
        return;
    }
    return;
}

//心跳获取,QUERY
void IPC_SrvUtilQueryHeartGetState(IPC_CMD_STAGE* cmdStagePtr,SYS_RESULT_PACK* resultPackPtr,uint64_t* utcTimeStampPtr,BOARD_RUN_MODE* boardModePtr)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    //默认值
    if(utcTimeStampPtr != NULL)
    {
        *utcTimeStampPtr = 0X0000000000000000;
    }
    if(boardModePtr != NULL)
    {
        *boardModePtr = BOARD_RUN_MODE_BOOT;
    }
    errorSub = IPC_BaseQueryCmdState(&ipcChannelDataSrvUtil,IPC_CMD_MACRO_UTIL_HEART_GET,cmdStagePtr,resultPackPtr);
    if(errorSub != ERROR_SUB_OK)
    {
        SystemPrintf("%s,Cmd: 0X%08X,ErrorSub: 0X%08X\r\n",__func__,IPC_CMD_MACRO_UTIL_HEART_GET,errorSub);
        //IPC报错,返回错误
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_IPC_PROT_QUERY_CMD_STATE,ERROR_LEVEL_ERROR,
                                        errorSub);
        return;
    }
    if((*cmdStagePtr == IPC_CMD_STAGE_OVER)&&(resultPackPtr->resultCode == PROT_RESULT_SUCCESS))
    {
        //结果解析
        if(utcTimeStampPtr != NULL)
        {
            *utcTimeStampPtr = ProtUtilReadUint64Buffer(ipcChannelDataSrvUtil.channelCache.dataBufferPtr);
        }
        if(boardModePtr != NULL)
        {
            *boardModePtr = (BOARD_RUN_MODE)(ipcChannelDataSrvUtil.channelCache.dataBufferPtr[8]);
        }
    }
    return;
}

//急停,QUERY
void IPC_SrvUtilQueryEmStopState(IPC_CMD_STAGE* cmdStagePtr,SYS_RESULT_PACK* resultPackPtr)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    errorSub = IPC_BaseQueryCmdState(&ipcChannelDataSrvUtil,IPC_CMD_MACRO_UTIL_EM_STOP,cmdStagePtr,resultPackPtr);
    if(errorSub != ERROR_SUB_OK)
    {
        SystemPrintf("%s,Cmd: 0X%08X,ErrorSub: 0X%08X\r\n",__func__,IPC_CMD_MACRO_UTIL_EM_STOP,errorSub);
        //IPC报错,返回错误
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_IPC_PROT_QUERY_CMD_STATE,ERROR_LEVEL_ERROR,
                                        errorSub);
        return;
    }
    return;
}

//获取版本号,QUERY
void IPC_SrvUtilQueryGetVersionState(IPC_CMD_STAGE* cmdStagePtr,SYS_RESULT_PACK* resultPackPtr,BOARD_SOFT_VERSION* bootVesionPtr,
                                        BOARD_SOFT_VERSION* srvVersionPtr,BOARD_SOFT_VERSION* appVersionPtr)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    //默认值
    if(bootVesionPtr != NULL)
    {
        bootVesionPtr->mainVersion = 0X00;
        bootVesionPtr->subVersion = 0X00;
        bootVesionPtr->tempVersion = 0X00;
        bootVesionPtr->devVersion = 0X00000000;
    }
    if(srvVersionPtr != NULL)
    {
        srvVersionPtr->mainVersion = 0X00;
        srvVersionPtr->subVersion = 0X00;
        srvVersionPtr->tempVersion = 0X00;
        srvVersionPtr->devVersion = 0X00000000;
    }
    if(appVersionPtr != NULL)
    {
        appVersionPtr->mainVersion = 0X00;
        appVersionPtr->subVersion = 0X00;
        appVersionPtr->tempVersion = 0X00;
        appVersionPtr->devVersion = 0X00000000;
    }
    errorSub = IPC_BaseQueryCmdState(&ipcChannelDataSrvUtil,IPC_CMD_MACRO_UTIL_GET_VERSION,cmdStagePtr,resultPackPtr);
    if(errorSub != ERROR_SUB_OK)
    {
        SystemPrintf("%s,Cmd: 0X%08X,ErrorSub: 0X%08X\r\n",__func__,IPC_CMD_MACRO_UTIL_GET_VERSION,errorSub);
        //IPC报错,返回错误
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_IPC_PROT_QUERY_CMD_STATE,ERROR_LEVEL_ERROR,
                                        errorSub);
        return;
    }
    if((*cmdStagePtr == IPC_CMD_STAGE_OVER)&&(resultPackPtr->resultCode == PROT_RESULT_SUCCESS))
    {
        //结果解析
        if(bootVesionPtr != NULL)
        {
            bootVesionPtr->mainVersion = ipcChannelDataSrvUtil.channelCache.dataBufferPtr[0];
            bootVesionPtr->subVersion = ipcChannelDataSrvUtil.channelCache.dataBufferPtr[1];
            bootVesionPtr->tempVersion = ipcChannelDataSrvUtil.channelCache.dataBufferPtr[2];
            bootVesionPtr->devVersion = ProtUtilReadUint32Buffer(ipcChannelDataSrvUtil.channelCache.dataBufferPtr+3);
        }
        if(srvVersionPtr != NULL)
        {
            srvVersionPtr->mainVersion = ipcChannelDataSrvUtil.channelCache.dataBufferPtr[7];
            srvVersionPtr->subVersion = ipcChannelDataSrvUtil.channelCache.dataBufferPtr[8];
            srvVersionPtr->tempVersion = ipcChannelDataSrvUtil.channelCache.dataBufferPtr[9];
            srvVersionPtr->devVersion = ProtUtilReadUint32Buffer(ipcChannelDataSrvUtil.channelCache.dataBufferPtr+10);
        }
        if(appVersionPtr != NULL)
        {
            appVersionPtr->mainVersion = ipcChannelDataSrvUtil.channelCache.dataBufferPtr[14];
            appVersionPtr->subVersion = ipcChannelDataSrvUtil.channelCache.dataBufferPtr[15];
            appVersionPtr->tempVersion = ipcChannelDataSrvUtil.channelCache.dataBufferPtr[16];
            appVersionPtr->devVersion = ProtUtilReadUint32Buffer(ipcChannelDataSrvUtil.channelCache.dataBufferPtr+17);
        }
    }
    return;
}


/*****************************************************发送指令等待ACK************************************************************/
static ERROR_SUB IPC_SrvUtilCheckClearLastCmd(uint32_t cmdCode)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    PROT_CH_STA channelState;
    //首先检查是否上个数据未完成或者完成之后未读取
    channelState = IPC_BaseCheckCmdChannelState(&ipcChannelDataSrvUtil,cmdCode);
    if(channelState == PROT_CH_STA_BUSY)
    {
        //清除上一次的指令
        errorSub = IPC_BaseChannelClearChannel(&ipcChannelDataSrvUtil,cmdCode);
        if(errorSub != ERROR_SUB_OK)
        {
            SystemPrintf("%s,Cmd: 0X%08X,ErrorSub: 0X%08X\r\n",__func__,cmdCode,errorSub);
            return errorSub;
        }
    }
    return ERROR_SUB_OK;
}

//通讯测试,ACK
void IPC_SrvUtilCommTestWhileAck(SYS_RESULT_PACK* resultPackPtr)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    errorSub = IPC_SrvUtilCheckClearLastCmd(IPC_CMD_MACRO_COMM_TEST);
    if(errorSub != ERROR_SUB_OK)
    {
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_IPC_PROT_CLEAR_LAST_CMD,ERROR_LEVEL_ERROR,
                                        errorSub);
        return;
    }
    //设置参数
    IPC_CMD_BASE_DATA ipcCmdBaseData;
    //先设置为默认值
    IPC_BaseSetCmdPackDefault(&ipcCmdBaseData);
    //然后写入
    ipcCmdBaseData.commandCode = IPC_CMD_MACRO_COMM_TEST;
    //清除掉上一次的指令了,进行下一次指令发送
    errorSub = IPC_BaseSendCmdWhileAck(&ipcChannelDataSrvUtil,&ipcCmdBaseData);
    if(errorSub != ERROR_SUB_OK)
    {
        SystemPrintf("%s,Cmd: 0X%08X,ErrorSub: 0X%08X\r\n",__func__,ipcCmdBaseData.commandCode,
                        errorSub);
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_IPC_PROT_CMD_WAIT_ACK,ERROR_LEVEL_ERROR,
                                        errorSub);
        return;
    }
    //设置无错误返回
    CoreUtilSetSystemResultPackNoError(resultPackPtr);
    return;
}

//联机,ACK
void IPC_SrvUtilOnLineWhileAck(PROT_CONNECT_FLAG protConnectFlag,SYS_RESULT_PACK* resultPackPtr)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    errorSub = IPC_SrvUtilCheckClearLastCmd(IPC_CMD_MACRO_UTIL_ONLINE);
    if(errorSub != ERROR_SUB_OK)
    {
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_IPC_PROT_CLEAR_LAST_CMD,ERROR_LEVEL_ERROR,
                                        errorSub);
        return;
    }
    //设置参数
    IPC_CMD_BASE_DATA ipcCmdBaseData;
    //先设置为默认值
    IPC_BaseSetCmdPackDefault(&ipcCmdBaseData);
    //然后写入
    ipcCmdBaseData.commandCode = IPC_CMD_MACRO_UTIL_ONLINE;
    ipcCmdBaseData.cmdParam1 = (uint32_t)protConnectFlag;
    //清除掉上一次的指令了,进行下一次指令发送
    errorSub = IPC_BaseSendCmdWhileAck(&ipcChannelDataSrvUtil,&ipcCmdBaseData);
    if(errorSub != ERROR_SUB_OK)
    {
        SystemPrintf("%s,Cmd: 0X%08X,ErrorSub: 0X%08X\r\n",__func__,ipcCmdBaseData.commandCode,
                        errorSub);
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_IPC_PROT_CMD_WAIT_ACK,ERROR_LEVEL_ERROR,
                                        errorSub);
        return;
    }
    //设置无错误返回
    CoreUtilSetSystemResultPackNoError(resultPackPtr);
    return;
}

//脱机,ACK
void IPC_SrvUtilOffLineWhileAck(PROT_DISCONNECT_FLAG disconnectFlag,SYS_RESULT_PACK* resultPackPtr)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    errorSub = IPC_SrvUtilCheckClearLastCmd(IPC_CMD_MACRO_UTIL_OFFLINE);
    if(errorSub != ERROR_SUB_OK)
    {
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_IPC_PROT_CLEAR_LAST_CMD,ERROR_LEVEL_ERROR,
                                        errorSub);
        return;
    }
    //设置参数
    IPC_CMD_BASE_DATA ipcCmdBaseData;
    //先设置为默认值
    IPC_BaseSetCmdPackDefault(&ipcCmdBaseData);
    //然后写入
    ipcCmdBaseData.commandCode = IPC_CMD_MACRO_UTIL_OFFLINE;
    //写入参数
    ipcCmdBaseData.cmdParam1 = (uint32_t)disconnectFlag;
    //清除掉上一次的指令了,进行下一次指令发送
    errorSub = IPC_BaseSendCmdWhileAck(&ipcChannelDataSrvUtil,&ipcCmdBaseData);
    if(errorSub != ERROR_SUB_OK)
    {
        SystemPrintf("%s,Cmd: 0X%08X,ErrorSub: 0X%08X\r\n",__func__,ipcCmdBaseData.commandCode,
                        errorSub);
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_IPC_PROT_CMD_WAIT_ACK,ERROR_LEVEL_ERROR,
                                        errorSub);
        return;
    }
    //设置无错误返回
    CoreUtilSetSystemResultPackNoError(resultPackPtr);
    return;
}

//UTC同步,ACK
void IPC_SrvUtilUtcSyncWhileAck(uint64_t utcTimeStamp,SYS_RESULT_PACK* resultPackPtr)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    errorSub = IPC_SrvUtilCheckClearLastCmd(IPC_CMD_MACRO_UTIL_UTC_SYNC);
    if(errorSub != ERROR_SUB_OK)
    {
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_IPC_PROT_CLEAR_LAST_CMD,ERROR_LEVEL_ERROR,
                                        errorSub);
        return;
    }
    //设置参数
    IPC_CMD_BASE_DATA ipcCmdBaseData;
    //先设置为默认值
    IPC_BaseSetCmdPackDefault(&ipcCmdBaseData);
    //然后写入
    ipcCmdBaseData.commandCode = IPC_CMD_MACRO_UTIL_UTC_SYNC;
    //写入参数
    ipcCmdBaseData.cmdParam1 = (uint32_t)(utcTimeStamp>>32);
    ipcCmdBaseData.cmdParam2 = (uint32_t)(utcTimeStamp);
    //清除掉上一次的指令了,进行下一次指令发送
    errorSub = IPC_BaseSendCmdWhileAck(&ipcChannelDataSrvUtil,&ipcCmdBaseData);
    if(errorSub != ERROR_SUB_OK)
    {
        SystemPrintf("%s,Cmd: 0X%08X,ErrorSub: 0X%08X\r\n",__func__,ipcCmdBaseData.commandCode,
                        errorSub);
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_IPC_PROT_CMD_WAIT_ACK,ERROR_LEVEL_ERROR,
                                        errorSub);
        return;
    }
    //设置无错误返回
    CoreUtilSetSystemResultPackNoError(resultPackPtr);
    return;
}

//心跳获取,ACK
void IPC_SrvUtilHeartGetWhileAck(SYS_RESULT_PACK* resultPackPtr)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    errorSub = IPC_SrvUtilCheckClearLastCmd(IPC_CMD_MACRO_UTIL_HEART_GET);
    if(errorSub != ERROR_SUB_OK)
    {
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_IPC_PROT_CLEAR_LAST_CMD,ERROR_LEVEL_ERROR,
                                        errorSub);
        return;
    }
    //设置参数
    IPC_CMD_BASE_DATA ipcCmdBaseData;
    //先设置为默认值
    IPC_BaseSetCmdPackDefault(&ipcCmdBaseData);
    //然后写入
    ipcCmdBaseData.commandCode = IPC_CMD_MACRO_UTIL_HEART_GET;
    //清除掉上一次的指令了,进行下一次指令发送
    errorSub = IPC_BaseSendCmdWhileAck(&ipcChannelDataSrvUtil,&ipcCmdBaseData);
    if(errorSub != ERROR_SUB_OK)
    {
        SystemPrintf("%s,Cmd: 0X%08X,ErrorSub: 0X%08X\r\n",__func__,ipcCmdBaseData.commandCode,
                        errorSub);
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_IPC_PROT_CMD_WAIT_ACK,ERROR_LEVEL_ERROR,
                                        errorSub);
        return;
    }
    //设置无错误返回
    CoreUtilSetSystemResultPackNoError(resultPackPtr);
    return;
}

//急停,ACK
void IPC_SrvUtilEmStopWhileAck(SYS_RESULT_PACK* resultPackPtr)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    errorSub = IPC_SrvUtilCheckClearLastCmd(IPC_CMD_MACRO_UTIL_EM_STOP);
    if(errorSub != ERROR_SUB_OK)
    {
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_IPC_PROT_CLEAR_LAST_CMD,ERROR_LEVEL_ERROR,
                                        errorSub);
        return;
    }
    //设置参数
    IPC_CMD_BASE_DATA ipcCmdBaseData;
    //先设置为默认值
    IPC_BaseSetCmdPackDefault(&ipcCmdBaseData);
    //然后写入
    ipcCmdBaseData.commandCode = IPC_CMD_MACRO_UTIL_EM_STOP;
    //清除掉上一次的指令了,进行下一次指令发送
    errorSub = IPC_BaseSendCmdWhileAck(&ipcChannelDataSrvUtil,&ipcCmdBaseData);
    if(errorSub != ERROR_SUB_OK)
    {
        SystemPrintf("%s,Cmd: 0X%08X,ErrorSub: 0X%08X\r\n",__func__,ipcCmdBaseData.commandCode,
                        errorSub);
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_IPC_PROT_CMD_WAIT_ACK,ERROR_LEVEL_ERROR,
                                        errorSub);
        return;
    }
    //设置无错误返回
    CoreUtilSetSystemResultPackNoError(resultPackPtr);
    return;
}

//获取版本号,ACK
void IPC_SrvUtilGetVersionWhileAck(SYS_RESULT_PACK* resultPackPtr)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    errorSub = IPC_SrvUtilCheckClearLastCmd(IPC_CMD_MACRO_UTIL_GET_VERSION);
    if(errorSub != ERROR_SUB_OK)
    {
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_IPC_PROT_CLEAR_LAST_CMD,ERROR_LEVEL_ERROR,
                                        errorSub);
        return;
    }
    //设置参数
    IPC_CMD_BASE_DATA ipcCmdBaseData;
    //先设置为默认值
    IPC_BaseSetCmdPackDefault(&ipcCmdBaseData);
    //然后写入
    ipcCmdBaseData.commandCode = IPC_CMD_MACRO_UTIL_GET_VERSION;
    //清除掉上一次的指令了,进行下一次指令发送
    errorSub = IPC_BaseSendCmdWhileAck(&ipcChannelDataSrvUtil,&ipcCmdBaseData);
    if(errorSub != ERROR_SUB_OK)
    {
        SystemPrintf("%s,Cmd: 0X%08X,ErrorSub: 0X%08X\r\n",__func__,ipcCmdBaseData.commandCode,
                        errorSub);
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_IPC_PROT_CMD_WAIT_ACK,ERROR_LEVEL_ERROR,
                                        errorSub);
        return;
    }
    //设置无错误返回
    CoreUtilSetSystemResultPackNoError(resultPackPtr);
    return;
}


/*****************************************************发送指令等待返回************************************************************/
//通讯测试,RETURN
void IPC_SrvUtilCommTestWhileReturn(SYS_RESULT_PACK* resultPackPtr)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    //清除上一个指令
    errorSub = IPC_SrvUtilCheckClearLastCmd(IPC_CMD_MACRO_COMM_TEST);
    if(errorSub != ERROR_SUB_OK)
    {
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_IPC_PROT_CLEAR_LAST_CMD,ERROR_LEVEL_ERROR,
                                        errorSub);
        return;
    }
    //设置参数
    IPC_CMD_BASE_DATA ipcCmdBaseData;
    //先设置为默认值
    IPC_BaseSetCmdPackDefault(&ipcCmdBaseData);
    //然后写入
    ipcCmdBaseData.commandCode = IPC_CMD_MACRO_COMM_TEST;
    //清除掉上一次的指令了,进行下一次指令发送
    errorSub = IPC_BaseSendCmdWhileReturn(&ipcChannelDataSrvUtil,&ipcCmdBaseData,resultPackPtr);
    if(errorSub != ERROR_SUB_OK)
    {
        SystemPrintf("%s,Cmd: 0X%08X,ErrorSub: 0X%08X\r\n",__func__,ipcCmdBaseData.commandCode,
                        errorSub);
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_IPC_PROT_CMD_WAIT_RESULT,ERROR_LEVEL_ERROR,
                                        errorSub);
        return;
    }
    return;
}

//联机,RETURN
void IPC_SrvUtilOnLineWhileReturn(PROT_CONNECT_FLAG protConnectFlag,SYS_RESULT_PACK* resultPackPtr,BOARD_RUN_MODE* boardModePtr)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    //默认值
    if(boardModePtr != NULL)
    {
        *boardModePtr = BOARD_RUN_MODE_BOOT;
    }
    errorSub = IPC_SrvUtilCheckClearLastCmd(IPC_CMD_MACRO_UTIL_ONLINE);
    if(errorSub != ERROR_SUB_OK)
    {
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_IPC_PROT_CLEAR_LAST_CMD,ERROR_LEVEL_ERROR,
                                        errorSub);
        return;
    }
    //设置参数
    IPC_CMD_BASE_DATA ipcCmdBaseData;
    //先设置为默认值
    IPC_BaseSetCmdPackDefault(&ipcCmdBaseData);
    //然后写入
    ipcCmdBaseData.commandCode = IPC_CMD_MACRO_UTIL_ONLINE;
    //清除掉上一次的指令了,进行下一次指令发送
    errorSub = IPC_BaseSendCmdWhileReturn(&ipcChannelDataSrvUtil,&ipcCmdBaseData,resultPackPtr);
    if(errorSub != ERROR_SUB_OK)
    {
        SystemPrintf("%s,Cmd: 0X%08X,ErrorSub: 0X%08X\r\n",__func__,ipcCmdBaseData.commandCode,
                        errorSub);
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_IPC_PROT_CMD_WAIT_RESULT,ERROR_LEVEL_ERROR,
                                        errorSub);
        return;
    }
    if(resultPackPtr->resultCode == PROT_RESULT_SUCCESS)
    {
        //结果解析
        if(boardModePtr != NULL)
        {
            //结果解析
            *boardModePtr = (BOARD_RUN_MODE)(ipcChannelDataSrvUtil.channelCache.dataBufferPtr[0]);
        }
    }
    return;
}

//脱机,RETURN
void IPC_SrvUtilOffLineWhileReturn(PROT_DISCONNECT_FLAG disconnectFlag,SYS_RESULT_PACK* resultPackPtr,BOARD_RUN_MODE* boardModePtr)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    //默认值
    if(boardModePtr != NULL)
    {
        *boardModePtr = BOARD_RUN_MODE_BOOT;
    }
    errorSub = IPC_SrvUtilCheckClearLastCmd(IPC_CMD_MACRO_UTIL_OFFLINE);
    if(errorSub != ERROR_SUB_OK)
    {
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_IPC_PROT_CLEAR_LAST_CMD,ERROR_LEVEL_ERROR,
                                        errorSub);
        return;
    }
    //设置参数
    IPC_CMD_BASE_DATA ipcCmdBaseData;
    //先设置为默认值
    IPC_BaseSetCmdPackDefault(&ipcCmdBaseData);
    //然后写入
    ipcCmdBaseData.commandCode = IPC_CMD_MACRO_UTIL_OFFLINE;
    //参数设置
    ipcCmdBaseData.cmdParam1 = disconnectFlag;
    //清除掉上一次的指令了,进行下一次指令发送
    errorSub = IPC_BaseSendCmdWhileReturn(&ipcChannelDataSrvUtil,&ipcCmdBaseData,resultPackPtr);
    if(errorSub != ERROR_SUB_OK)
    {
        SystemPrintf("%s,Cmd: 0X%08X,ErrorSub: 0X%08X\r\n",__func__,ipcCmdBaseData.commandCode,
                        errorSub);
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_IPC_PROT_CMD_WAIT_RESULT,ERROR_LEVEL_ERROR,
                                        errorSub);
        return;
    }
    if(resultPackPtr->resultCode == PROT_RESULT_SUCCESS)
    {
        if(boardModePtr != NULL)
        {
            //结果解析
            *boardModePtr = (BOARD_RUN_MODE)(ipcChannelDataSrvUtil.channelCache.dataBufferPtr[0]);
        }
    }
    return;
}

//UTC同步,RETURN
void IPC_SrvUtilUtcSyncWhileReturn(uint64_t utcTimeStamp,SYS_RESULT_PACK* resultPackPtr)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    errorSub = IPC_SrvUtilCheckClearLastCmd(IPC_CMD_MACRO_UTIL_UTC_SYNC);
    if(errorSub != ERROR_SUB_OK)
    {
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_IPC_PROT_CLEAR_LAST_CMD,ERROR_LEVEL_ERROR,
                                        errorSub);
        return;
    }
    //设置参数
    IPC_CMD_BASE_DATA ipcCmdBaseData;
    //先设置为默认值
    IPC_BaseSetCmdPackDefault(&ipcCmdBaseData);
    //然后写入
    ipcCmdBaseData.commandCode = IPC_CMD_MACRO_UTIL_UTC_SYNC;
    //写入参数
    ipcCmdBaseData.cmdParam1 = (uint32_t)(utcTimeStamp>>32);
    ipcCmdBaseData.cmdParam2 = (uint32_t)(utcTimeStamp);
    //清除掉上一次的指令了,进行下一次指令发送
    errorSub = IPC_BaseSendCmdWhileReturn(&ipcChannelDataSrvUtil,&ipcCmdBaseData,resultPackPtr);
    if(errorSub != ERROR_SUB_OK)
    {
        SystemPrintf("%s,Cmd: 0X%08X,ErrorSub: 0X%08X\r\n",__func__,ipcCmdBaseData.commandCode,
                        errorSub);
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_IPC_PROT_CMD_WAIT_RESULT,ERROR_LEVEL_ERROR,
                                        errorSub);
        return;
    }
    return;
}

//心跳获取,RETURN
void IPC_SrvUtilHeartGetWhileReturn(SYS_RESULT_PACK* resultPackPtr,uint64_t* utcTimeStampPtr,BOARD_RUN_MODE* boardModePtr)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    //默认值
    if(utcTimeStampPtr != NULL)
    {
        *utcTimeStampPtr = 0X0000000000000000;
    }
    if(boardModePtr != NULL)
    {
        *boardModePtr = BOARD_RUN_MODE_BOOT;
    }
    errorSub = IPC_SrvUtilCheckClearLastCmd(IPC_CMD_MACRO_UTIL_HEART_GET);
    if(errorSub != ERROR_SUB_OK)
    {
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_IPC_PROT_CLEAR_LAST_CMD,ERROR_LEVEL_ERROR,
                                        errorSub);
        return;
    }
    //设置参数
    IPC_CMD_BASE_DATA ipcCmdBaseData;
    //先设置为默认值
    IPC_BaseSetCmdPackDefault(&ipcCmdBaseData);
    //然后写入
    ipcCmdBaseData.commandCode = IPC_CMD_MACRO_UTIL_HEART_GET;
    //清除掉上一次的指令了,进行下一次指令发送
    errorSub = IPC_BaseSendCmdWhileReturn(&ipcChannelDataSrvUtil,&ipcCmdBaseData,resultPackPtr);
    if(errorSub != ERROR_SUB_OK)
    {
        SystemPrintf("%s,Cmd: 0X%08X,ErrorSub: 0X%08X\r\n",__func__,ipcCmdBaseData.commandCode,
                        errorSub);
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_IPC_PROT_CMD_WAIT_RESULT,ERROR_LEVEL_ERROR,
                                        errorSub);
        return;
    }
    if(resultPackPtr->resultCode == PROT_RESULT_SUCCESS)
    {
        //结果解析
        if(utcTimeStampPtr != NULL)
        {
            *utcTimeStampPtr = ProtUtilReadUint64Buffer(ipcChannelDataSrvUtil.channelCache.dataBufferPtr);
        }
        if(boardModePtr != NULL)
        {
            *boardModePtr = (BOARD_RUN_MODE)(ipcChannelDataSrvUtil.channelCache.dataBufferPtr[8]);
        }
    }
    return;
}

//急停,RETURN
void IPC_SrvUtilEmStopWhileReturn(SYS_RESULT_PACK* resultPackPtr)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    errorSub = IPC_SrvUtilCheckClearLastCmd(IPC_CMD_MACRO_UTIL_EM_STOP);
    if(errorSub != ERROR_SUB_OK)
    {
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_IPC_PROT_CLEAR_LAST_CMD,ERROR_LEVEL_ERROR,
                                        errorSub);
        return;
    }
    //设置参数
    IPC_CMD_BASE_DATA ipcCmdBaseData;
    //先设置为默认值
    IPC_BaseSetCmdPackDefault(&ipcCmdBaseData);
    //然后写入
    ipcCmdBaseData.commandCode = IPC_CMD_MACRO_UTIL_EM_STOP;
    //清除掉上一次的指令了,进行下一次指令发送
    errorSub = IPC_BaseSendCmdWhileReturn(&ipcChannelDataSrvUtil,&ipcCmdBaseData,resultPackPtr);
    if(errorSub != ERROR_SUB_OK)
    {
        SystemPrintf("%s,Cmd: 0X%08X,ErrorSub: 0X%08X\r\n",__func__,ipcCmdBaseData.commandCode,
                        errorSub);
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_IPC_PROT_CMD_WAIT_RESULT,ERROR_LEVEL_ERROR,
                                        errorSub);
        return;
    }
    return;
}

//获取版本号,RETURN
void IPC_SrvUtilGetVersionWhileReturn(SYS_RESULT_PACK* resultPackPtr,BOARD_SOFT_VERSION* bootVesionPtr,BOARD_SOFT_VERSION* srvVersionPtr,
                                        BOARD_SOFT_VERSION* appVersionPtr)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    //默认值
    if(bootVesionPtr != NULL)
    {
        bootVesionPtr->mainVersion = 0X00;
        bootVesionPtr->subVersion = 0X00;
        bootVesionPtr->tempVersion = 0X00;
        bootVesionPtr->devVersion = 0X00000000;
    }
    if(srvVersionPtr != NULL)
    {
        srvVersionPtr->mainVersion = 0X00;
        srvVersionPtr->subVersion = 0X00;
        srvVersionPtr->tempVersion = 0X00;
        srvVersionPtr->devVersion = 0X00000000;
    }
    if(appVersionPtr != NULL)
    {
        appVersionPtr->mainVersion = 0X00;
        appVersionPtr->subVersion = 0X00;
        appVersionPtr->tempVersion = 0X00;
        appVersionPtr->devVersion = 0X00000000;
    }
    errorSub = IPC_SrvUtilCheckClearLastCmd(IPC_CMD_MACRO_UTIL_GET_VERSION);
    if(errorSub != ERROR_SUB_OK)
    {
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_IPC_PROT_CLEAR_LAST_CMD,ERROR_LEVEL_ERROR,
                                        errorSub);
        return;
    }
    //设置参数
    IPC_CMD_BASE_DATA ipcCmdBaseData;
    //先设置为默认值
    IPC_BaseSetCmdPackDefault(&ipcCmdBaseData);
    //然后写入
    ipcCmdBaseData.commandCode = IPC_CMD_MACRO_UTIL_GET_VERSION;
    //清除掉上一次的指令了,进行下一次指令发送
    errorSub = IPC_BaseSendCmdWhileReturn(&ipcChannelDataSrvUtil,&ipcCmdBaseData,resultPackPtr);
    if(errorSub != ERROR_SUB_OK)
    {
        SystemPrintf("%s,Cmd: 0X%08X,ErrorSub: 0X%08X\r\n",__func__,ipcCmdBaseData.commandCode,
                        errorSub);
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_IPC_PROT_CMD_WAIT_RESULT,ERROR_LEVEL_ERROR,
                                        errorSub);
        return;
    }
    if(resultPackPtr->resultCode == PROT_RESULT_SUCCESS)
    {
        //结果解析
        if(bootVesionPtr != NULL)
        {
            bootVesionPtr->mainVersion = ipcChannelDataSrvUtil.channelCache.dataBufferPtr[0];
            bootVesionPtr->subVersion = ipcChannelDataSrvUtil.channelCache.dataBufferPtr[1];
            bootVesionPtr->tempVersion = ipcChannelDataSrvUtil.channelCache.dataBufferPtr[2];
            bootVesionPtr->devVersion = ProtUtilReadUint32Buffer(ipcChannelDataSrvUtil.channelCache.dataBufferPtr+3);
        }
        if(srvVersionPtr != NULL)
        {
            srvVersionPtr->mainVersion = ipcChannelDataSrvUtil.channelCache.dataBufferPtr[7];
            srvVersionPtr->subVersion = ipcChannelDataSrvUtil.channelCache.dataBufferPtr[8];
            srvVersionPtr->tempVersion = ipcChannelDataSrvUtil.channelCache.dataBufferPtr[9];
            srvVersionPtr->devVersion = ProtUtilReadUint32Buffer(ipcChannelDataSrvUtil.channelCache.dataBufferPtr+10);
        }
        if(appVersionPtr != NULL)
        {
            appVersionPtr->mainVersion = ipcChannelDataSrvUtil.channelCache.dataBufferPtr[14];
            appVersionPtr->subVersion = ipcChannelDataSrvUtil.channelCache.dataBufferPtr[15];
            appVersionPtr->tempVersion = ipcChannelDataSrvUtil.channelCache.dataBufferPtr[16];
            appVersionPtr->devVersion = ProtUtilReadUint32Buffer(ipcChannelDataSrvUtil.channelCache.dataBufferPtr+17);
        }
    }
    return;
}















