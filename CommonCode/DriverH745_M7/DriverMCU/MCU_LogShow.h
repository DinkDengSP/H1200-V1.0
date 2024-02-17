/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-10-16 13:31:54 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-10-16 17:17:52 +0800
************************************************************************************************/ 
#ifndef __MCU_LOG_SHOW_
#define __MCU_LOG_SHOW_
#include "DriverHeaderCore.h"
#include "DriverConfigMCU.h"

#if(MCU_LOG_SHOW_MODE_USER == MCU_LOG_SHOW_MODE_OFF)
//空实现
#define MCU_LogSendBuffer(bufPtr,bufLen)       asm("nop")
#define MCU_LogSendString(stringPtr)           asm("nop")
#define MCU_LogSendStringInt(stringPtr)        asm("nop")
#define MCU_LogPrintf(format, ...)             asm("nop")
#define MCU_LogShowErrorSubCode(subCode)       asm("nop")
#define MCU_LogShowHalState(state,errorCode)   asm("nop")
#define MCU_LogShowGetLock()                   asm("nop")
#define MCU_LogShowReleaseLock()               asm("nop")

#elif(MCU_LOG_SHOW_MODE_USER == MCU_LOG_SHOW_MODE_RTT)
//使用RTT进行底层输出,包含相关文件
#include "MCU_RTT.h"
#define MCU_LogSendBuffer(bufPtr,bufLen)       MCU_RTT_SendBuffer(bufPtr,bufLen)
#define MCU_LogSendString(stringPtr)           MCU_RTT_SendString(stringPtr)
#define MCU_LogSendStringInt(stringPtr)        MCU_RTT_SendStringInt(stringPtr)
#define MCU_LogPrintf(format, ...)             MCU_RTT_Printf(format, ##__VA_ARGS__)
#define MCU_LogShowErrorSubCode(subCode)       MCU_RTT_Printf("Func : %s,Line : %d,ErrorSubCode :0X%08X\r\n",__FUNCTION__,__LINE__,subCode)
#define MCU_LogShowHalState(state,errorCode)   MCU_RTT_Printf("Func : %s,Line : %d,HalResult : 0X%02X,,HalErrorCode: 0X%08X\r\n",__FUNCTION__,__LINE__,state,errorCode)
#define MCU_LogShowGetLock()                   MCU_RTT_GetLock()
#define MCU_LogShowReleaseLock()               MCU_RTT_ReleaseLock()

#elif(MCU_LOG_SHOW_MODE_USER == MCU_LOG_SHOW_MODE_UART1)
//使用RTT进行底层输出,包含相关文件
#include "MCU_UART1.h"
#define MCU_LogSendBuffer(bufPtr,bufLen)       MCU_UART1_SendBuffer(bufPtr,bufLen)
#define MCU_LogSendString(stringPtr)           MCU_UART1_SendString(stringPtr)
#define MCU_LogSendStringInt(stringPtr)        MCU_UART1_SendStringInt(stringPtr)
#define MCU_LogPrintf(format, ...)             MCU_UART1_Printf(format, ##__VA_ARGS__)
#define MCU_LogShowErrorSubCode(subCode)       MCU_UART1_Printf("Func : %s,Line : %d,ErrorSubCode :0X%08X\r\n",__FUNCTION__,__LINE__,subCode)
#define MCU_LogShowHalState(state,errorCode)   MCU_UART1_Printf("Func : %s,Line : %d,HalResult : 0X%02X,,HalErrorCode: 0X%08X\r\n",__FUNCTION__,__LINE__,state,errorCode)
#define MCU_LogShowGetLock()                   MCU_UART1_GetLock()
#define MCU_LogShowReleaseLock()               MCU_UART1_ReleaseLock()

#elif(MCU_LOG_SHOW_MODE_USER == MCU_LOG_SHOW_MODE_UART6)
//使用RTT进行底层输出,包含相关文件
#include "MCU_UART6.h"
#define MCU_LogSendBuffer(bufPtr,bufLen)       MCU_UART6_SendBuffer(bufPtr,bufLen)
#define MCU_LogSendString(stringPtr)           MCU_UART6_SendString(stringPtr)
#define MCU_LogSendStringInt(stringPtr)        MCU_UART6_SendStringInt(stringPtr)
#define MCU_LogPrintf(format, ...)             MCU_UART6_Printf(format, ##__VA_ARGS__)
#define MCU_LogShowErrorSubCode(subCode)       MCU_UART6_Printf("Func : %s,Line : %d,ErrorSubCode :0X%08X\r\n",__FUNCTION__,__LINE__,subCode)
#define MCU_LogShowHalState(state,errorCode)   MCU_UART6_Printf("Func : %s,Line : %d,HalResult : 0X%02X,,HalErrorCode: 0X%08X\r\n",__FUNCTION__,__LINE__,state,errorCode)
#define MCU_LogShowGetLock()                   MCU_UART6_GetLock()
#define MCU_LogShowReleaseLock()               MCU_UART6_ReleaseLock()

#endif


//底层打印数组
void MCU_LogShowArrayBuffer(uint8_t* prefixStr,uint8_t* bufferPtr,uint16_t bufferLength);




#endif














