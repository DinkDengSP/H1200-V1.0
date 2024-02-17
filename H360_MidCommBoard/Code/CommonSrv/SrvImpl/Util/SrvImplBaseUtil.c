/************************************************************************ 
 ** Author       : 邓小俊
 ** Date         : 2024-01-31 16:34:18
 ** Description  : When I Has Free Time ,I Will Write Description Below.
 ** description  : 
 ** LastEditors  : 邓小俊
 ** LastEditTime : 2024-02-01 11:27:17
 ** Email: xiaojun.deng@aliyun.com
 ** Copyright (c) 2024 by 公司名称, All Rights Reserved. 
 ************************************************************************/
#include "SrvImplBaseUtil.h"
#include "CommonSrvTaskHeader.h"
#include "tx_mutex.h"
#include "tx_queue.h"
#include "SocketNetUtilW5500.h"
#include "SrvImplSocketNet5500.h"
#include "SocketNetUtilLAN8700.h"
#include "SrvImplSocketNet8700.h"


//系统日志打印的互斥信号量
TX_MUTEX mutexSrvTaskSysProbe;
//用于printf的缓冲区
uint8_t blockBufferSystProbe[BLOCK_SIZE_SYSTEM_PROBE] = {0};

//获取锁
void SysProbeGetLock(void)
{
    tx_mutex_get(&mutexSrvTaskSysProbe,TX_WAIT_FOREVER);
}
//释放锁
void SysProbeReleaseLock(void)
{
    tx_mutex_put(&mutexSrvTaskSysProbe);
}

//任务全局对象初始化
void SrvImplSysProbeVarInit(void)
{
    //创建互斥信号量
    tx_mutex_create(&mutexSrvTaskSysProbe,"mutexSrvTaskSysProbe",TX_INHERIT);
    //清空日志缓存长度
    UserMemClear((void*)blockBufferSystProbe,BLOCK_SIZE_SYSTEM_PROBE);
}

//探针数据发送函数
void SystemProbePrintf(const char *format, ...)
{
    //转换
    va_list arg;
    int rv;
    if(mutexSrvTaskSysProbe.tx_mutex_id != TX_MUTEX_ID)
    {
        //日志任务还没初始化
        return;
    }
    //打印加锁
    SysProbeGetLock();
    //关闭调度器
    uint32_t oldState = CoreLockSchedule();
    //启动可变参数解析
    va_start(arg, format);
    //将数据转换成带打印缓存
    rv = vsnprintf((char *)blockBufferSystProbe, BLOCK_SIZE_SYSTEM_LOG, format, arg);
    va_end(arg);
    //格式转换完成,打开调度器
    CoreUnlockSchedule(oldState);
    SYS_MSG* sysMsgPtr = NULL;
    SYS_LOG_MSG* sysLogMsgPtr = NULL;
    uint32_t txResult = TX_SUCCESS;
    //将数据发送出去
    if ((rv > 0) && (rv <= BLOCK_SIZE_SYSTEM_LOG))
    {
        if(queueSrvTaskUtil.tx_queue_id != TX_QUEUE_ID)
        {
            //队列还没初始化,直接底层发送
            MCU_LogSendBuffer(blockBufferSystProbe,rv);
            //打印解锁
            SysProbeReleaseLock();
            return;
        }   
        //申请日志对象
        sysMsgPtr = UserMemMalloc(MEM_REGION_SYS_MSG_MAIN,DIM_STRUCT_LENGTH_BYTE(SYS_MSG));
        sysLogMsgPtr = UserMemMalloc(MEM_REGION_SYS_MSG_LOG,DIM_STRUCT_LENGTH_BYTE(SYS_LOG_MSG));
        if((sysMsgPtr == NULL)||(sysLogMsgPtr == NULL))
        {
            if(sysMsgPtr != NULL)
                UserMemFree(MEM_REGION_SYS_MSG_MAIN,sysMsgPtr);
            if(sysLogMsgPtr != NULL)
                UserMemFree(MEM_REGION_SYS_MSG_LOG,sysLogMsgPtr);
            //底层打印
            MCU_LogSendBuffer(blockBufferSystProbe,rv);
            //打印解锁
            SysProbeReleaseLock();
            MCU_LogPrintf("%s Mem Malloc Failed\r\n",__func__);
            return;
        }
        sysLogMsgPtr->logDataLength = rv;
        sysLogMsgPtr->logDataBuffer = UserMemMalloc(MEM_REGION_SYS_MSG_LOG,sysLogMsgPtr->logDataLength);
        if(sysLogMsgPtr->logDataBuffer == NULL)
        {
            MCU_LogSendBuffer(blockBufferSystProbe,sysLogMsgPtr->logDataLength);
            UserMemFree(MEM_REGION_SYS_MSG_MAIN,sysMsgPtr);
            UserMemFree(MEM_REGION_SYS_MSG_LOG,sysLogMsgPtr);
            //打印解锁
            SysProbeReleaseLock();
            MCU_LogPrintf("%s logDataBuffer Malloc Failed\r\n",__func__);
            return;
        }
        //数据拷贝
        UserMemCopy(sysLogMsgPtr->logDataBuffer,(void*)blockBufferSystProbe,sysLogMsgPtr->logDataLength);
        //主消息赋值
        sysMsgPtr->msgSrc = SYS_MSG_PACK_SRC_LOG;
        sysMsgPtr->msgBodyPtr = sysLogMsgPtr;
        txResult = tx_queue_send(&queueSrvTaskUtil,(void*)(&sysMsgPtr),TX_NO_WAIT);
        if(txResult != TX_SUCCESS)
        {
            //底层打印
            MCU_LogSendBuffer(blockBufferSystProbe,sysLogMsgPtr->logDataLength);
            //内存释放
            UserMemFree(MEM_REGION_SYS_MSG_LOG,sysLogMsgPtr->logDataBuffer);
            UserMemFree(MEM_REGION_SYS_MSG_LOG,sysLogMsgPtr);
            UserMemFree(MEM_REGION_SYS_MSG_MAIN,sysMsgPtr);
            //打印解锁
            SysProbeReleaseLock();
            MCU_LogPrintf("%s,queueSrvTaskUtil Send Failed,ErrorCode: %d\r\n",__func__,txResult);
            return;
        }
    }
    else
    {
        MCU_LogPrintf("%s,Format Size Too Big,Max Length: %d\r\n",__func__,BLOCK_SIZE_SYSTEM_LOG);
    }
    //打印解锁
    SysProbeReleaseLock();
}

//探针数据发送字符串
void SystemProbeSendString(uint8_t* strPtr)
{
    SYS_MSG* sysMsgPtr = NULL;
    SYS_LOG_MSG* sysLogMsgPtr = NULL;
    uint32_t txResult = TX_SUCCESS;
    if(strPtr == NULL)
    {
        MCU_LogSendString("SystemSendString Ptr NULL\r\n");
        return;
    }
    uint16_t strLen = strlen((char const*)strPtr);
    if(mutexSrvTaskSysProbe.tx_mutex_id != TX_MUTEX_ID)
    {
        //日志任务还没初始化,直接底层发送
        MCU_LogSendBuffer(strPtr,strLen);
        return;
    }
    if(queueSrvTaskUtil.tx_queue_id != TX_QUEUE_ID)
    {
        //队列还没初始化,直接底层发送
        MCU_LogSendBuffer(strPtr,strLen);
        return;
    }  
    //将数据发送出去
    if ((strLen > 0) && (strLen <= BLOCK_SIZE_SYSTEM_LOG))
    {
        sysMsgPtr = UserMemMalloc(MEM_REGION_SYS_MSG_MAIN,DIM_STRUCT_LENGTH_BYTE(SYS_MSG));
        sysLogMsgPtr = UserMemMalloc(MEM_REGION_SYS_MSG_LOG,DIM_STRUCT_LENGTH_BYTE(SYS_LOG_MSG));
        if((sysMsgPtr == NULL)||(sysLogMsgPtr == NULL))
        {
            if(sysMsgPtr != NULL)
                UserMemFree(MEM_REGION_SYS_MSG_MAIN,sysMsgPtr);
            if(sysLogMsgPtr != NULL)
                UserMemFree(MEM_REGION_SYS_MSG_LOG,sysLogMsgPtr);
            //底层打印
            MCU_LogSendBuffer(strPtr,strLen);
            MCU_LogPrintf("%s Mem Malloc Failed\r\n",__func__);
            return;
        }
        sysLogMsgPtr->logDataBuffer = UserMemMalloc(MEM_REGION_SYS_MSG_LOG,strLen);
        if(sysLogMsgPtr->logDataBuffer == NULL)
        {
            MCU_LogSendBuffer(strPtr,strLen);
            UserMemFree(MEM_REGION_SYS_MSG_MAIN,sysMsgPtr);
            UserMemFree(MEM_REGION_SYS_MSG_LOG,sysLogMsgPtr);
            MCU_LogPrintf("%s logDataBuffer Malloc Failed\r\n",__func__);
            return;
        }
        sysLogMsgPtr->logDataLength = strLen;
        //数据拷贝
        UserMemCopy(sysLogMsgPtr->logDataBuffer,(void*)strPtr,strLen);
        //主消息赋值
        sysMsgPtr->msgSrc = SYS_MSG_PACK_SRC_LOG;
        sysMsgPtr->msgBodyPtr = sysLogMsgPtr;
        txResult = tx_queue_send(&queueSrvTaskUtil,(void*)(&sysMsgPtr),TX_NO_WAIT);
        if(txResult != TX_SUCCESS)
        {
            MCU_LogSendBuffer(strPtr,strLen);
            //内存释放
            UserMemFree(MEM_REGION_SYS_MSG_LOG,sysLogMsgPtr->logDataBuffer);
            UserMemFree(MEM_REGION_SYS_MSG_LOG,sysLogMsgPtr);
            UserMemFree(MEM_REGION_SYS_MSG_MAIN,sysMsgPtr);
            MCU_LogPrintf("%s,queueCommonSrvTaskSysLog Send Failed,ErrorCode: %d\r\n",__func__,txResult);
            return;
        }
    }
    else
    {
        MCU_LogPrintf("%s,Size Too Big,Max Length: %d\r\n",__func__,BLOCK_SIZE_SYSTEM_LOG);
    }
}

//探针数据发送缓冲区
void SystemProbeSendBuffer(uint8_t* bufferPtr,uint16_t bufferLength)
{
    SYS_MSG* sysMsgPtr = NULL;
    SYS_LOG_MSG* sysLogMsgPtr = NULL;
    uint32_t txResult = TX_SUCCESS;
    if(bufferPtr == NULL)
    {
        MCU_LogSendString("SystemSendBuffer Ptr NULL\r\n");
        return;
    }
    if(mutexSrvTaskSysProbe.tx_mutex_id != TX_MUTEX_ID)
    {
        //日志任务还没初始化,直接底层发送
        MCU_LogSendBuffer(bufferPtr,bufferLength);
        return;
    }
    if(queueSrvTaskUtil.tx_queue_id != TX_QUEUE_ID)
    {
        //队列还没初始化,直接底层发送
        MCU_LogSendBuffer(bufferPtr,bufferLength);
        return;
    }   
    //将数据发送出去
    if ((bufferLength > 0) && (bufferLength <= BLOCK_SIZE_SYSTEM_LOG))
    {
        sysMsgPtr = UserMemMalloc(MEM_REGION_SYS_MSG_MAIN,DIM_STRUCT_LENGTH_BYTE(SYS_MSG));
        sysLogMsgPtr = UserMemMalloc(MEM_REGION_SYS_MSG_LOG,DIM_STRUCT_LENGTH_BYTE(SYS_LOG_MSG));
        if((sysMsgPtr == NULL)||(sysLogMsgPtr == NULL))
        {
            if(sysMsgPtr != NULL)
                UserMemFree(MEM_REGION_SYS_MSG_MAIN,sysMsgPtr);
            if(sysLogMsgPtr != NULL)
                UserMemFree(MEM_REGION_SYS_MSG_LOG,sysLogMsgPtr);
            //底层打印
            MCU_LogSendBuffer(bufferPtr,bufferLength);
            MCU_LogPrintf("%s Mem Malloc Failed\r\n",__func__);
            return;
        }
        sysLogMsgPtr->logDataBuffer = UserMemMalloc(MEM_REGION_SYS_MSG_LOG,bufferLength);
        if(sysLogMsgPtr->logDataBuffer == NULL)
        {
            MCU_LogSendBuffer(bufferPtr,bufferLength);
            UserMemFree(MEM_REGION_SYS_MSG_MAIN,sysMsgPtr);
            UserMemFree(MEM_REGION_SYS_MSG_LOG,sysLogMsgPtr);
            MCU_LogPrintf("%s logDataBuffer Malloc Failed\r\n",__func__);
            return;
        }
        sysLogMsgPtr->logDataLength = bufferLength;
        //数据拷贝
        UserMemCopy(sysLogMsgPtr->logDataBuffer,(void*)bufferPtr,bufferLength);
        //主消息赋值
        sysMsgPtr->msgSrc = SYS_MSG_PACK_SRC_LOG;
        sysMsgPtr->msgBodyPtr = sysLogMsgPtr;
        txResult = tx_queue_send(&queueSrvTaskUtil,(void*)(&sysMsgPtr),TX_NO_WAIT);
        if(txResult != TX_SUCCESS)
        {
            //底层打印
            MCU_LogSendBuffer(bufferPtr,bufferLength);
            //内存释放
            UserMemFree(MEM_REGION_SYS_MSG_LOG,sysLogMsgPtr->logDataBuffer);
            UserMemFree(MEM_REGION_SYS_MSG_LOG,sysLogMsgPtr);
            UserMemFree(MEM_REGION_SYS_MSG_MAIN,sysMsgPtr);
            //报错
            MCU_LogPrintf("%s,queueSrvTaskUtil Send Failed,ErrorCode: %d\r\n",__func__,txResult);
            return;
        }
    }
    else
    {
        MCU_LogPrintf("%s,Size Too Big,Max Length: %d\r\n",__func__,BLOCK_SIZE_SYSTEM_LOG);
    }
}

//探针数据发送数组显示
void SysProbeShowArrayBuffer(uint8_t* prefixStr,uint8_t* bufferPtr,uint16_t bufferLength)
{
    uint16_t indexUtilCount = 0;
    uint16_t resultStringLen = 0;
    //申请内存
    uint8_t* bufferStringPtr = blockBufferSystProbe;
    //打印加锁
    SysProbeGetLock();
    //首先打印前缀
    SystemSendString(prefixStr);
    //循环转换并打印
    do
    {
        //数据转换
        if(bufferLength - indexUtilCount >= SYS_BUF_SHOW_SINGLE_LINE)
        {
            CoreConvertByteBufToHexStr(bufferPtr + indexUtilCount,SYS_BUF_SHOW_SINGLE_LINE,bufferStringPtr,
                                        LENGTH_BYTES_BUF_TO_HEX_STR(SYS_BUF_SHOW_SINGLE_LINE),&resultStringLen);
            indexUtilCount += SYS_BUF_SHOW_SINGLE_LINE;
        }
        else
        {
            CoreConvertByteBufToHexStr(bufferPtr + indexUtilCount,(bufferLength - indexUtilCount),bufferStringPtr,
                                        LENGTH_BYTES_BUF_TO_HEX_STR((bufferLength - indexUtilCount)),&resultStringLen);
            indexUtilCount = bufferLength;
        }
        //数据打印
        SystemSendString(bufferStringPtr);
    }while(indexUtilCount < bufferLength);
    //打印解锁
    SysProbeReleaseLock();
    return;
}

//探针数据发送
__weak void SrvImplBaseUtilProbeSendBuffer(uint8_t* sendBufferPtr,uint16_t bufferLength)
{
    uint32_t sendResult = 0;
    //检查Socket是否初始化
    if(SUCCESS == SocketNetUtilLAN8700CheckBaseInitOver())
    {
        //探针数据输出
        sendResult = SrvImplSocketNetLAN8700ProbeSendBuf(sendBufferPtr,bufferLength);
        if((sendResult != NX_SUCCESS)&&(sendResult != NX_NOT_CONNECTED))
        {
            //发送失败且不是因为未连接
            MCU_LogPrintf("SrvImplSocketNetLAN8700ProbeSendBuf Failed,Reason: 0X%02X\r\n",sendResult);
        }
    }
    //检查Socket是否初始化
    if(SUCCESS == SocketNetUtilW5500CheckBaseInitOver())
    {
        //探针数据输出
        sendResult = SrvImplSocketNetW5500ProbeSendBuf(sendBufferPtr,bufferLength);
        if((sendResult != NX_SUCCESS)&&(sendResult != NX_NOT_CONNECTED))
        {
            //发送失败且不是因为未连接
            MCU_LogPrintf("SrvImplSocketNetW5500ProbeSendBuf Failed,Reason: 0X%02X\r\n",sendResult);
        }
    }
}


//联机
__weak void SrvImplBaseUtilOnline(PROT_CONNECT_FLAG protConnectFlag,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen)
{
    //返回内存长度设定
    *dataBufLen = 2;
    *dataBufPtrPtr = UserMemMallocWhileSuccess(MEM_REGION_SRV_TASK,(*dataBufLen));
    uint8_t* resultBufferPtr = *dataBufPtrPtr;
    resultBufferPtr[0] = (uint8_t)protConnectFlag;
    resultBufferPtr[1] = (uint8_t)BOARD_RUN_MODE_APP;
    //构建完成
    CoreUtilSetSystemResultPackNoError(resultPackPtr);
    //设置客户端联机
    SYSTEM_STATE_HEART* sysCommHeartStatePtr = SystemStateGetCommHeart();
    sysCommHeartStatePtr->clientConnectState = CLIENT_CONNECT_SET;
}

//脱机
__weak void SrvImplBaseUtilOffLine(PROT_DISCONNECT_FLAG disconnectFlag,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen)
{
    //返回内存长度设定
    *dataBufPtrPtr = UserMemMallocWhileSuccess(MEM_REGION_SRV_TASK,1);
    *dataBufLen = 1;
    uint8_t* resultBufferPtr = *dataBufPtrPtr;
    resultBufferPtr[0] = (uint8_t)BOARD_RUN_MODE_APP;
    //构建完成
    CoreUtilSetSystemResultPackNoError(resultPackPtr);
    //设置客户端脱机
    SYSTEM_STATE_HEART* sysCommHeartStatePtr = SystemStateGetCommHeart();
    sysCommHeartStatePtr->clientConnectState = CLIENT_CONNECT_RESET;
}

//UTC同步
__weak void SrvImplBaseUtilUtcSync(uint64_t utcTimeStamp,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    RTC_TIMER* rtcTimePtr = NULL;
    errorSub = MCU_RTC_SyncTimeByUTC((uint32_t)(utcTimeStamp));
    if(errorSub != ERROR_SUB_OK)
    {
        //报错
        SystemPrintf("%s,MCU_RTC_SyncTimeByUTC ErrorSub: 0X%08X\r\n",__func__,errorSub);
        //返回错误
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_UTIL_UTC_SYNC_RTC,ERROR_LEVEL_WARNING,errorSub);
        return;
    }
    //获取打印当前时间
    errorSub = MCU_RTC_GetTimeRecordSync(&rtcTimePtr);
    if(errorSub != ERROR_SUB_OK)
    {
        //报错
        SystemPrintf("%s,MCU_RTC_GetTimeRecordSync ErrorSub: 0X%08X\r\n",__func__,errorSub);
        //返回错误
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_UTIL_UTC_SYNC_RTC,ERROR_LEVEL_WARNING,errorSub);
        return;
    }
    SystemPrintf("CurrentWeek: %d,CurrentTime %04d-%02d-%02d,%02d:%02d:%02d\r\n",rtcTimePtr->weekDay,
                    rtcTimePtr->year,rtcTimePtr->month,rtcTimePtr->day,rtcTimePtr->hour,rtcTimePtr->minute,
                    rtcTimePtr->second);
    //构建完成无错误
    CoreUtilSetSystemResultPackNoError(resultPackPtr);
}

//心跳获取
__weak void SrvImplBaseUtilHeartGet(SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    uint32_t utcTimeStamp = 0;
    *dataBufPtrPtr = UserMemMallocWhileSuccess(MEM_REGION_SRV_TASK,9);
    *dataBufLen = 9;
    uint8_t* resultBufferPtr = *dataBufPtrPtr;
    //获取时间
    errorSub = MCU_RTC_GetTimeCurrentUTC(&utcTimeStamp);
    if(errorSub != ERROR_SUB_OK)
    {
        //报错
        SystemPrintf("%s,MCU_RTC_GetTimeCurrentUTC ErrorSub: 0X%08X\r\n",__func__,errorSub);
        //返回错误
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_UTIL_GET_HEART_RTC,ERROR_LEVEL_WARNING,errorSub);
        return;
    }
    //写入时间
    ProtUtilWriteUint64ToBuffer((uint64_t)(utcTimeStamp*1000),resultBufferPtr);
    //写入模式
    resultBufferPtr[8] = (uint8_t)BOARD_RUN_MODE_APP;
    //构建完成无错误
    CoreUtilSetSystemResultPackNoError(resultPackPtr);
}

//急停
__weak void SrvImplBaseUtilEmStop(SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen)
{
    //构建完成无错误
    CoreUtilSetSystemResultPackNoError(resultPackPtr);
}

//获取版本号
__weak void SrvImplBaseUtilGetVersion(SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen)
{
    BOARD_SOFT_VERSION versionCode;
    ERROR_SUB errorSub = ERROR_SUB_OK;
    *dataBufPtrPtr = UserMemMallocWhileSuccess(MEM_REGION_SRV_TASK,21);
    *dataBufLen = 21;
    uint8_t* resultBufferPtr = *dataBufPtrPtr;
    //BOOT版本号
    errorSub = BoardParamReadBootVersion(&versionCode);
    if(errorSub != ERROR_SUB_OK)
    {
        //报错
        SystemPrintf("%s,BoardParamReadBootVersion ErrorSub: 0X%08X\r\n",__func__,errorSub);
        //返回错误
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_UTIL_READ_VERSION_BOOT,ERROR_LEVEL_WARNING,errorSub);
        return;
    }
    //打印版本号
    SystemPrintf("BootVersion: %d.%d.%d-%d\r\n",versionCode.mainVersion,versionCode.subVersion,versionCode.tempVersion,
                    versionCode.devVersion);
    //写入BOOT版本号
    resultBufferPtr[0] = (uint8_t)versionCode.mainVersion;
    resultBufferPtr[1] = (uint8_t)versionCode.subVersion;
    resultBufferPtr[2] = (uint8_t)versionCode.tempVersion;
    ProtUtilWriteUint32ToBuffer(versionCode.devVersion,resultBufferPtr+3);
    //SRV版本号
    errorSub = BoardParamReadServiceVersion(&versionCode);
    if(errorSub != ERROR_SUB_OK)
    {
        //报错
        SystemPrintf("%s,BoardParamReadServiceVersion ErrorSub: 0X%08X\r\n",__func__,errorSub);
        //返回错误
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_UTIL_READ_VERSION_SRV,ERROR_LEVEL_WARNING,errorSub);
        return;
    }
    //打印版本号
    SystemPrintf("SrvVersion: %d.%d.%d-%d\r\n",versionCode.mainVersion,versionCode.subVersion,versionCode.tempVersion,
                    versionCode.devVersion);
    resultBufferPtr[7] = (uint8_t)versionCode.mainVersion;
    resultBufferPtr[8] = (uint8_t)versionCode.subVersion;
    resultBufferPtr[9] = (uint8_t)versionCode.tempVersion;
    ProtUtilWriteUint32ToBuffer(versionCode.devVersion,resultBufferPtr+10);
    //APP版本号
    errorSub = BoardParamReadAppVersion(&versionCode);
    if(errorSub != ERROR_SUB_OK)
    {
        //报错
        SystemPrintf("%s,BoardParamReadAppVersion ErrorSub: 0X%08X\r\n",__func__,errorSub);
        //返回错误
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_UTIL_READ_VERSION_APP,ERROR_LEVEL_WARNING,errorSub);
        return;
    }
    //打印版本号
    SystemPrintf("AppVersion: %d.%d.%d-%d\r\n",versionCode.mainVersion,versionCode.subVersion,versionCode.tempVersion,
                    versionCode.devVersion);
    resultBufferPtr[14] = (uint8_t)versionCode.mainVersion;
    resultBufferPtr[15] = (uint8_t)versionCode.subVersion;
    resultBufferPtr[16] = (uint8_t)versionCode.tempVersion;
    ProtUtilWriteUint32ToBuffer(versionCode.devVersion,resultBufferPtr+17);
    //构建完成无错误
    CoreUtilSetSystemResultPackNoError(resultPackPtr);
}

















