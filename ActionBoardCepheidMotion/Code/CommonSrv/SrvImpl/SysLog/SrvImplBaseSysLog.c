/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-10-31 08:43:34 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-10-31 13:18:33 +0800
************************************************************************************************/ 
#include "SrvImplSysLog.h"
#include "SrvImplBaseSysLog.h"
#include "CommonSrvTaskHeader.h"
#include "tx_mutex.h"
#include "tx_queue.h"

//系统日志打印的互斥信号量
TX_MUTEX mutexSrvTaskSysLog;
//用于printf的缓冲区
uint8_t blockBufferSystemLog[BLOCK_SIZE_SYSTEM_LOG] = {0};
//获取锁
void SysLogGetLock(void)
{
    tx_mutex_get(&mutexSrvTaskSysLog,TX_WAIT_FOREVER);
}
//释放锁
void SysLogReleaseLock(void)
{
    tx_mutex_put(&mutexSrvTaskSysLog);
}

//初始化日志端口
void SrvImplBaseSysLogVarInit(void)
{
    //创建互斥信号量
    tx_mutex_create(&mutexSrvTaskSysLog,"mutexSrvTaskSysLog",TX_INHERIT);
    //清空日志缓存长度
    UserMemClear((void*)blockBufferSystemLog,BLOCK_SIZE_SYSTEM_LOG);
}


//系统日志发送函数
void SystemPrintf(const char *format, ...)
{
    //转换
    va_list arg;
    int rv;
    if(mutexSrvTaskSysLog.tx_mutex_id != TX_MUTEX_ID)
    {
        //日志任务还没初始化
        return;
    }
    //打印加锁
    SysLogGetLock();
    //启动可变参数解析
    va_start(arg, format);
    //将数据转换成带打印缓存
    rv = vsnprintf((char *)blockBufferSystemLog, BLOCK_SIZE_SYSTEM_LOG, format, arg);
    va_end(arg);
    SYS_MSG* sysMsgPtr = NULL;
    SYS_LOG_MSG* sysLogMsgPtr = NULL;
    uint32_t txResult = TX_SUCCESS;
    //将数据发送出去
    if ((rv > 0) && (rv <= BLOCK_SIZE_SYSTEM_LOG))
    {
        if(queueSrvTaskSysLog.tx_queue_id != TX_QUEUE_ID)
        {
            //队列还没初始化,直接底层发送
            MCU_LogSendBuffer(blockBufferSystemLog,rv);
            //打印解锁
            SysLogReleaseLock();
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
            MCU_LogSendBuffer(blockBufferSystemLog,rv);
            //打印解锁
            SysLogReleaseLock();
            MCU_LogPrintf("%s Mem Malloc Failed\r\n",__func__);
            return;
        }
        sysLogMsgPtr->logDataBuffer = UserMemMalloc(MEM_REGION_SYS_MSG_LOG,rv);
        if(sysLogMsgPtr->logDataBuffer == NULL)
        {
            MCU_LogSendBuffer(blockBufferSystemLog,rv);
            UserMemFree(MEM_REGION_SYS_MSG_MAIN,sysMsgPtr);
            UserMemFree(MEM_REGION_SYS_MSG_LOG,sysLogMsgPtr);
            //打印解锁
            SysLogReleaseLock();
            MCU_LogPrintf("%s logDataBuffer Malloc Failed\r\n",__func__);
            return;
        }
        sysLogMsgPtr->logDataLength = rv;
        //数据拷贝
        UserMemCopy(sysLogMsgPtr->logDataBuffer,(void*)blockBufferSystemLog,rv);
        //主消息赋值
        sysMsgPtr->msgSrc = SYS_MSG_PACK_SRC_LOG;
        sysMsgPtr->msgBodyPtr = sysLogMsgPtr;
        txResult = tx_queue_send(&queueSrvTaskSysLog,(void*)(&sysMsgPtr),TX_NO_WAIT);
        if(txResult != TX_SUCCESS)
        {
            //底层打印
            MCU_LogSendBuffer(blockBufferSystemLog,rv);
            //内存释放
            UserMemFree(MEM_REGION_SYS_MSG_LOG,sysLogMsgPtr->logDataBuffer);
            UserMemFree(MEM_REGION_SYS_MSG_LOG,sysLogMsgPtr);
            UserMemFree(MEM_REGION_SYS_MSG_MAIN,sysMsgPtr);
            //打印解锁
            SysLogReleaseLock();
            MCU_LogPrintf("%s,queueSrvTaskSysLog Send Failed,ErrorCode: %d\r\n",__func__,txResult);
            return;
        }
    }
    else
    {
        MCU_LogPrintf("%s,Format Size Too Big,Max Length: %d\r\n",__func__,BLOCK_SIZE_SYSTEM_LOG);
    }
    //打印解锁
    SysLogReleaseLock();
}


//系统日志发送缓冲区
void SystemSendBuffer(uint8_t* bufferPtr,uint16_t bufferLength)
{
    SYS_MSG* sysMsgPtr = NULL;
    SYS_LOG_MSG* sysLogMsgPtr = NULL;
    uint32_t txResult = TX_SUCCESS;
    if(bufferPtr == NULL)
    {
        MCU_LogSendString("SystemSendBuffer Ptr NULL\r\n");
        return;
    }
    if(mutexSrvTaskSysLog.tx_mutex_id != TX_MUTEX_ID)
    {
        //日志任务还没初始化,直接底层发送
        MCU_LogSendBuffer(bufferPtr,bufferLength);
        return;
    }
    if(queueSrvTaskSysLog.tx_queue_id != TX_QUEUE_ID)
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
        txResult = tx_queue_send(&queueSrvTaskSysLog,(void*)(&sysMsgPtr),TX_NO_WAIT);
        if(txResult != TX_SUCCESS)
        {
            //底层打印
            MCU_LogSendBuffer(bufferPtr,bufferLength);
            //内存释放
            UserMemFree(MEM_REGION_SYS_MSG_LOG,sysLogMsgPtr->logDataBuffer);
            UserMemFree(MEM_REGION_SYS_MSG_LOG,sysLogMsgPtr);
            UserMemFree(MEM_REGION_SYS_MSG_MAIN,sysMsgPtr);
            //报错
            MCU_LogPrintf("%s,queueSrvTaskSysLog Send Failed,ErrorCode: %d\r\n",__func__,txResult);
            return;
        }
    }
    else
    {
        MCU_LogPrintf("%s,Size Too Big,Max Length: %d\r\n",__func__,BLOCK_SIZE_SYSTEM_LOG);
    }
}


//系统日志发送字符串
void SystemSendString(uint8_t* strPtr)
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
    if(mutexSrvTaskSysLog.tx_mutex_id != TX_MUTEX_ID)
    {
        //日志任务还没初始化,直接底层发送
        MCU_LogSendBuffer(strPtr,strLen);
        return;
    }
    if(queueSrvTaskSysLog.tx_queue_id != TX_QUEUE_ID)
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
        txResult = tx_queue_send(&queueSrvTaskSysLog,(void*)(&sysMsgPtr),TX_NO_WAIT);
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

//系统发送数组显示
void SysLogShowArrayBuffer(uint8_t* prefixStr,uint8_t* bufferPtr,uint16_t bufferLength)
{
    uint16_t indexUtilCount = 0;
    uint16_t resultStringLen = 0;
    //申请内存
    uint8_t* bufferStringPtr = blockBufferSystemLog;
    //打印加锁
    SysLogGetLock();
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
    SysLogReleaseLock();
    return;
}





















