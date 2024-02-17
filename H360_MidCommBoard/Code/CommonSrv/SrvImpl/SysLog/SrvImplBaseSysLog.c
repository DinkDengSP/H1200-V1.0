/************************************************************************ 
 ** Author       : 邓小俊
 ** Date         : 2024-01-31 16:34:18
 ** Description  : When I Has Free Time ,I Will Write Description Below.
 ** description  : 
 ** LastEditors  : 邓小俊
 ** LastEditTime : 2024-02-02 11:49:56
 ** Email: xiaojun.deng@aliyun.com
 ** Copyright (c) 2024 by 公司名称, All Rights Reserved. 
 ************************************************************************/
#include "SrvImplBaseSysLog.h"
#include "CommonSrvTaskHeader.h"
#include "tx_mutex.h"
#include "tx_queue.h"
#if(SYSTEM_LOG_OUT_NET_8700_SOCKET == CONFIG_FUNC_ENABLE)
#include "SrvImplSocketNet8700.h"
#include "SocketNetUtilLAN8700.h"
#endif
#if(SYSTEM_LOG_OUT_NET_5500_SOCKET == CONFIG_FUNC_ENABLE)
#include "SrvImplSocketNet5500.h"
#include "SocketNetUtilW5500.h"
#endif

//系统日志打印的互斥信号量
TX_MUTEX mutexSrvTaskSysLog;
//用于printf的缓冲区
uint8_t blockBufferSystemLog[BLOCK_SIZE_SYSTEM_LOG] = {0};
//本机输出固定前缀
static const char logPrefixStrM7[] = "[DMC_H743_Srv]:";


//自动创建文件名的最大长度
#define MAX_LEN_LOG_MODULE_BOARD_DESP_CREATE           64
//自动生成文件名
static char logModuleBoardNameCreate[MAX_LEN_LOG_MODULE_BOARD_DESP_CREATE];
static const char logModuleBoardNameDefault[] = "[Unknow ModuleBoard]:";

//根据模块号找文件名
typedef struct LOG_MODULE_BOARD_DESP
{
    uint8_t moduleNo;
    uint8_t boardID;
    uint8_t* despStrPtr;
}LOG_MODULE_BOARD_DESP;
//ID映射数组
static const LOG_MODULE_BOARD_DESP logModuleBoardDespArray[] = {
    {SYSTEM_MODULE_NO_PC      ,SYSTEM_BOARD_ID_PC,          "[PC]:"     },
    {SYSTEM_MODULE_NO_ACT     ,SYSTEM_BOARD_ID_ACT_1,       "[DACT 1]:" },
    {SYSTEM_MODULE_NO_ACT     ,SYSTEM_BOARD_ID_ACT_2,       "[DACT 2]:" },
    {SYSTEM_MODULE_NO_ACT     ,SYSTEM_BOARD_ID_ACT_3,       "[DACT 3]:" },
    {SYSTEM_MODULE_NO_ACT     ,SYSTEM_BOARD_ID_ACT_4,       "[DACT 4]:" },
    {SYSTEM_MODULE_NO_ACT     ,SYSTEM_BOARD_ID_ACT_5,       "[DACT 5]:" },
    {SYSTEM_MODULE_NO_ACT     ,SYSTEM_BOARD_ID_ACT_6,       "[DACT 6]:" },
    {SYSTEM_MODULE_NO_ACT     ,SYSTEM_BOARD_ID_ACT_7,       "[DACT 7]:" },
    {SYSTEM_MODULE_NO_ACT     ,SYSTEM_BOARD_ID_ACT_8,       "[DACT 8]:" },
};


//没有前缀的情况下自动生成
static uint8_t* SysLogGetNameByModuleBoardPrintf(const char * format, ... )
{
    int rv = 0;
    UserMemSet((void*)logModuleBoardNameCreate,0,MAX_LEN_LOG_MODULE_BOARD_DESP_CREATE);
    va_list args;
	va_start (args, format);
	rv = vsnprintf ((char*)logModuleBoardNameCreate,MAX_LEN_LOG_MODULE_BOARD_DESP_CREATE-1,format, args);
	va_end (args);	
    if(rv < (MAX_LEN_LOG_MODULE_BOARD_DESP_CREATE - 1))
    {
        return (uint8_t*)logModuleBoardNameCreate;
    }
    else
    {
        return (uint8_t*)logModuleBoardNameDefault;
    }
}


//靠模块号从列表中获取前缀
static uint8_t* SysLogGetNameByModuleBoard(uint8_t moduleNo,uint8_t boardID)
{
    uint32_t indexUtil = 0;
    for(indexUtil = 0; indexUtil < DIM_ARRAY_ELEMENT_COUNT(logModuleBoardDespArray); indexUtil++)
    {
        if((moduleNo == logModuleBoardDespArray[indexUtil].moduleNo)&&(boardID == logModuleBoardDespArray[indexUtil].boardID))
        {
            return logModuleBoardDespArray[indexUtil].despStrPtr;
        }
    }
    return SysLogGetNameByModuleBoardPrintf("[Module-%d_Board-%d]:",moduleNo,boardID);
}

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

//任务全局对象初始化
void SrvImplSysLogVarInit(void)
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
    //关闭调度器
    uint32_t oldState = CoreLockSchedule();
    //启动可变参数解析
    va_start(arg, format);
    //拷贝固定前缀
    UserMemCopy(blockBufferSystemLog,(uint8_t*)logPrefixStrM7,DIM_STR_ELEMENT_COUNT(logPrefixStrM7));
    //将数据转换成带打印缓存
    rv = vsnprintf((char *)blockBufferSystemLog + DIM_STR_ELEMENT_COUNT(logPrefixStrM7), 
                    BLOCK_SIZE_SYSTEM_LOG - DIM_STR_ELEMENT_COUNT(logPrefixStrM7), format, arg);
    va_end(arg);
    //格式转换完成,打开调度器
    CoreUnlockSchedule(oldState);
    SYS_MSG* sysMsgPtr = NULL;
    SYS_LOG_MSG* sysLogMsgPtr = NULL;
    uint32_t txResult = TX_SUCCESS;
    //将数据发送出去
    if ((rv > 0) && (rv <= (BLOCK_SIZE_SYSTEM_LOG - DIM_STR_ELEMENT_COUNT(logPrefixStrM7))))
    {
        if(queueSrvTaskSysLog.tx_queue_id != TX_QUEUE_ID)
        {
            //队列还没初始化,直接底层发送
            MCU_LogSendBuffer(blockBufferSystemLog,rv + DIM_STR_ELEMENT_COUNT(logPrefixStrM7));
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
            MCU_LogSendBuffer(blockBufferSystemLog,rv + DIM_STR_ELEMENT_COUNT(logPrefixStrM7));
            //打印解锁
            SysLogReleaseLock();
            MCU_LogPrintf("%s Mem Malloc Failed\r\n",__func__);
            return;
        }
        sysLogMsgPtr->logDataLength = rv + DIM_STR_ELEMENT_COUNT(logPrefixStrM7);
        sysLogMsgPtr->logDataBuffer = UserMemMalloc(MEM_REGION_SYS_MSG_LOG,sysLogMsgPtr->logDataLength);
        if(sysLogMsgPtr->logDataBuffer == NULL)
        {
            MCU_LogSendBuffer(blockBufferSystemLog,sysLogMsgPtr->logDataLength);
            UserMemFree(MEM_REGION_SYS_MSG_MAIN,sysMsgPtr);
            UserMemFree(MEM_REGION_SYS_MSG_LOG,sysLogMsgPtr);
            //打印解锁
            SysLogReleaseLock();
            MCU_LogPrintf("%s logDataBuffer Malloc Failed\r\n",__func__);
            return;
        }
        //数据拷贝
        UserMemCopy(sysLogMsgPtr->logDataBuffer,(void*)blockBufferSystemLog,sysLogMsgPtr->logDataLength);
        //主消息赋值
        sysMsgPtr->msgSrc = SYS_MSG_PACK_SRC_LOG;
        sysMsgPtr->msgBodyPtr = sysLogMsgPtr;
        txResult = tx_queue_send(&queueSrvTaskSysLog,(void*)(&sysMsgPtr),TX_NO_WAIT);
        if(txResult != TX_SUCCESS)
        {
            //底层打印
            MCU_LogSendBuffer(blockBufferSystemLog,sysLogMsgPtr->logDataLength);
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
    if ((bufferLength > 0) && (bufferLength <= (BLOCK_SIZE_SYSTEM_LOG - DIM_STR_ELEMENT_COUNT(logPrefixStrM7))))
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
        sysLogMsgPtr->logDataBuffer = UserMemMalloc(MEM_REGION_SYS_MSG_LOG,bufferLength + DIM_STR_ELEMENT_COUNT(logPrefixStrM7));
        if(sysLogMsgPtr->logDataBuffer == NULL)
        {
            MCU_LogSendBuffer(bufferPtr,bufferLength);
            UserMemFree(MEM_REGION_SYS_MSG_MAIN,sysMsgPtr);
            UserMemFree(MEM_REGION_SYS_MSG_LOG,sysLogMsgPtr);
            MCU_LogPrintf("%s logDataBuffer Malloc Failed\r\n",__func__);
            return;
        }
        sysLogMsgPtr->logDataLength = bufferLength + DIM_STR_ELEMENT_COUNT(logPrefixStrM7);
        //数据拷贝
        UserMemCopy(sysLogMsgPtr->logDataBuffer,(void*)logPrefixStrM7,DIM_STR_ELEMENT_COUNT(logPrefixStrM7));
        UserMemCopy(sysLogMsgPtr->logDataBuffer + DIM_STR_ELEMENT_COUNT(logPrefixStrM7),(void*)bufferPtr,bufferLength);
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
    if ((strLen > 0) && (strLen <= (BLOCK_SIZE_SYSTEM_LOG - DIM_STR_ELEMENT_COUNT(logPrefixStrM7))))
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
        sysLogMsgPtr->logDataBuffer = UserMemMalloc(MEM_REGION_SYS_MSG_LOG,strLen + DIM_STR_ELEMENT_COUNT(logPrefixStrM7));
        if(sysLogMsgPtr->logDataBuffer == NULL)
        {
            MCU_LogSendBuffer(strPtr,strLen);
            UserMemFree(MEM_REGION_SYS_MSG_MAIN,sysMsgPtr);
            UserMemFree(MEM_REGION_SYS_MSG_LOG,sysLogMsgPtr);
            MCU_LogPrintf("%s logDataBuffer Malloc Failed\r\n",__func__);
            return;
        }
        sysLogMsgPtr->logDataLength = strLen + DIM_STR_ELEMENT_COUNT(logPrefixStrM7);
        //数据拷贝
        UserMemCopy(sysLogMsgPtr->logDataBuffer,(void*)logPrefixStrM7,DIM_STR_ELEMENT_COUNT(logPrefixStrM7));
        UserMemCopy(sysLogMsgPtr->logDataBuffer + DIM_STR_ELEMENT_COUNT(logPrefixStrM7),(void*)strPtr,strLen);
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


//系统日志发送缓冲区
void SystemSendBufferOther(uint8_t moduleNo,uint8_t boardID,uint8_t* bufferPtr,uint16_t bufferLength)
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
    if ((bufferLength > 0) && (bufferLength <= (BLOCK_SIZE_SYSTEM_LOG - MAX_LEN_LOG_MODULE_BOARD_DESP_CREATE)))
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
        uint8_t* logPerifxStr = NULL;
        uint16_t logPerifxStrLen = 0;
        //打印加锁
        SysLogGetLock();
        //关闭调度器
        uint32_t oldState = CoreLockSchedule();
        //获取前缀
        logPerifxStr = SysLogGetNameByModuleBoard(moduleNo,boardID);
        //获取长度
        logPerifxStrLen = strlen((char const*)logPerifxStr);
        //格式转换完成,打开调度器
        CoreUnlockSchedule(oldState);
        //申请内存
        sysLogMsgPtr->logDataBuffer = UserMemMalloc(MEM_REGION_SYS_MSG_LOG,bufferLength + logPerifxStrLen);
        if(sysLogMsgPtr->logDataBuffer == NULL)
        {
            //打印解锁
            SysLogReleaseLock();
            //底层打印
            MCU_LogSendBuffer(bufferPtr,bufferLength);
            UserMemFree(MEM_REGION_SYS_MSG_MAIN,sysMsgPtr);
            UserMemFree(MEM_REGION_SYS_MSG_LOG,sysLogMsgPtr);
            MCU_LogPrintf("%s logDataBuffer Malloc Failed\r\n",__func__);
            return;
        }
        sysLogMsgPtr->logDataLength = bufferLength + logPerifxStrLen;
        //数据拷贝
        UserMemCopy(sysLogMsgPtr->logDataBuffer,(void*)logPerifxStr,logPerifxStrLen);
        UserMemCopy(sysLogMsgPtr->logDataBuffer + logPerifxStrLen,(void*)bufferPtr,bufferLength);
        //打印解锁
        SysLogReleaseLock();
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
        MCU_LogPrintf("%s,Size Too Big,Max Length: %d\r\n",__func__,(BLOCK_SIZE_SYSTEM_LOG - MAX_LEN_LOG_MODULE_BOARD_DESP_CREATE));
    }
}


//调用基础输出
__weak void SrvImplBaseSysLogSendBuffer(uint8_t* dataBufferPtr,uint16_t bufferLength)
{
    //RTT日志输出
#if(SYSTEM_LOG_OUT_RTT_ENABLE == CONFIG_FUNC_ENABLE)
    MCU_RTT_SendBuffer(dataBufferPtr,bufferLength);
#endif
    //UART日志输出
#if(SYSTEM_LOG_OUT_UART_ENABLE == CONFIG_FUNC_ENABLE)
    BoardRS1Uart6_SendBuffer(dataBufferPtr,bufferLength);
#endif
    //NET 8700输出
#if(SYSTEM_LOG_OUT_NET_8700_SOCKET == CONFIG_FUNC_ENABLE)
    //8700网口输出,先检测网口是否连接
    if((SUCCESS == SocketNetUtilLAN8700CheckBaseInitOver())&&(NX_TCP_ESTABLISHED == SrvImplSocketNetLAN8700LogGetState()))
    {
        //网口数据发送
        SrvImplSocketNetLAN8700LogSendBuf(dataBufferPtr,bufferLength);
    }
#endif
    //NET 5500输出
#if(SYSTEM_LOG_OUT_NET_5500_SOCKET == CONFIG_FUNC_ENABLE)
    //5500网口输出
    if((SUCCESS == SocketNetUtilW5500CheckBaseInitOver())&&(NX_TCP_ESTABLISHED == SrvImplSocketNetW5500LogGetState()))
    {
        //网口数据发送
        SrvImplSocketNetW5500LogSendBuf(dataBufferPtr,bufferLength);
    }
#endif
}










