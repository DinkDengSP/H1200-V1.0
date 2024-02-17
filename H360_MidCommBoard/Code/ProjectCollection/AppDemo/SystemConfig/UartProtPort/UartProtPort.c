/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-10-17 16:13:03 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-10-17 16:46:24 +0800
************************************************************************************************/ 
#include "UartProtPort.h"
#include "UartProtBase.h"
#include "DriverConfigSystem.h"
#include "UserMemManage.h"
#include "SrvTaskHeader.h"


//数据包CRC16计算
uint16_t UartProtUtilCalcCRC16(uint8_t* bufferPtr,uint16_t bufferLength)
{
    return MCU_Calc2ByteCRC(bufferPtr,bufferLength);
}

//SOCKET自身模块号
uint8_t UartProtGetSelfModuleNo(void)
{
    return SELF_MODULE_NO;
}

//SOCKET自身板号
uint8_t UartProtGetSelfBoardID(void)
{
    return SELF_BOARD_ID;
}

//获取SOCKET互斥锁
void UartProtGetMutexLock(void)
{
    MCU_UART1_GetLock();
}

//释放SOCKET互斥锁
void UartProtReleaseMutexLock(void)
{
    MCU_UART1_ReleaseLock();
}

//发送SOCKET一帧
ERROR_SUB UartProtSendFrame(uint16_t msgBodyLen,uint8_t* bodyBuf)
{
    MCU_UART1_SendBuffer(bodyBuf,msgBodyLen);
    return ERROR_SUB_OK;
}

//内存申请
void* UartProtMemMalloc(uint32_t sizeBytes)
{
    return UserMemMalloc(MEM_REGION_SYS_MSG_UART,sizeBytes);
}

//内存申请,必须成功
void* UartProtMemMallocWhileSuccess(uint32_t sizeBytes)
{
    return UserMemMallocWhileSuccess(MEM_REGION_SYS_MSG_UART,sizeBytes);
}

//内存释放
void UartProtMemFree(void* memPtr)
{
    UserMemFree(MEM_REGION_SYS_MSG_UART,memPtr);
}

//SOCKET 日志信息处理,发送过来的数据的内存不能直接用指针,必须深拷贝,因为调用者会清除内存
void UartProtLogMsgSelfProcess(UART_PROT_LOG_PACK_RECV* logPackPtr)
{
    //转发
    if((logPackPtr->logBodyLength != 0)&&(logPackPtr->logBodyBufferPtr != NULL))
    {
        SystemSendBufferOther(logPackPtr->srcModuleNo,logPackPtr->srcBoardID,
                                logPackPtr->logBodyBufferPtr,logPackPtr->logBodyLength);
    }
}

//ACK信息转发
ERROR_SUB UartProtAckMsgDispatch(UART_PROT_ACK_PACK* ackPackPtr)
{
    uint32_t txResult = TX_SUCCESS;
    txResult = tx_queue_send(&queueSocketUartAckSend,(void*)(&ackPackPtr),TX_NO_WAIT);
    if(txResult == TX_SUCCESS)
    {
        //转发成功          
        return ERROR_SUB_OK;
    }
    UART_PROT_LOG("UartProtAckMsgDispatch Failed,txResult: %d\r\n",txResult);
    return ERROR_SUB_BOARD_UART_ACK_DISPATCH_FAILED;
}








