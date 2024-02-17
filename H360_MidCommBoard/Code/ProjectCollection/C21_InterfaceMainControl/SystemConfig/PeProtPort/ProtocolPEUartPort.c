/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-09-04 13:24:55 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-09-05 08:49:28 +0800
************************************************************************************************/ 
#include "ProtocolPEUartPort.h"
#include "UserMemManage.h"
#include "DriverHeaderBoard.h"
#include "DriverConfigSystem.h"

//获取SOCKET互斥锁
void ProtocolPEUartSocketGetMutexLock(void)
{
    MCU_UART1_GetLock();
}

//释放SOCKET互斥锁
void ProtocolPEUartSocketReleaseMutexLock(void)
{
    MCU_UART1_ReleaseLock();
}

//发送SOCKET一帧
ERROR_SUB ProtocolPEUartSocketSendFrame(uint16_t msgBodyLen,uint8_t* bodyBuf)
{
    MCU_UART1_SendBuffer(bodyBuf,msgBodyLen);
    return ERROR_SUB_OK;
}

//内存申请
void* ProtocolPEUartSocketMemMalloc(uint32_t sizeBytes)
{
    return UserMemMalloc(MEM_REGION_SYS_MSG_UART,sizeBytes);
}

//内存申请,必须成功
void* ProtocolPEUartSocketMemMallocWhileSuccess(uint32_t sizeBytes)
{
    return UserMemMallocWhileSuccess(MEM_REGION_SYS_MSG_UART,sizeBytes);
}

//内存释放
void ProtocolPEUartSocketMemFree(void* memPtr)
{
    UserMemFree(MEM_REGION_SYS_MSG_UART,memPtr);
}


