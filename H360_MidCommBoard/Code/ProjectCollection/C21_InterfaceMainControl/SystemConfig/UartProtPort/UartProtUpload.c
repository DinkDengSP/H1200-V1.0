/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-10-17 16:13:03 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-10-19 12:06:30 +0800
************************************************************************************************/ 
#include "UartProtPort.h"
#include "UartProtBase.h"
#include "DriverConfigSystem.h"
#include "UserMemManage.h"
#include "SrvTaskHeader.h"
#include "ModTaskHeader.h"
#include "AppTaskHeader.h"
#include "UartMasterHeader.h"

#if(UART_PROT_UPLOAD_SUPPORT == CONFIG_FUNC_ENABLE)
//定义上传数据转发数组 上传ID+转发消息队列指针
static const PROT_UPLOAD_PACK_FWD_UNIT uartUploadPackFwdUnitArray[] = {
    //主控心跳上报
    {(uint32_t)UART_MASTER_REPORT_HEART_BEAT , &queueSrvTaskUtil},
};
#endif

//SOCKET 主动上传信息处理,发送过来的数据的内存不能直接用指针,必须深拷贝,因为调用者会清除内存
void UartProtUploadMsgSelfProcess(UART_PROT_UPLOAD_PACK_RECV* uploadPackPtr)
{
    //无论是否支持上报ID,都要给ACK,不要将通讯链路堵塞
    UartProtSendAckInRecv(UART_PROT_ACK_UPLOAD,uploadPackPtr->srcModuleNo,uploadPackPtr->srcBoardID,
                            uploadPackPtr->retryCount,uploadPackPtr->uploadID,PROT_ACK_OK);  
#if(UART_PROT_UPLOAD_SUPPORT == CONFIG_FUNC_DISABLE)
    //不支持上报ID,直接返回即可           
    return;         
#else
    uint16_t indexUtil = 0;
    TX_QUEUE* txQueuePtr = NULL;
    SYS_MSG* sysMsgPtr = NULL;
    UART_PROT_UPLOAD_PACK_RECV* uartUploadPackRecvPtr = NULL;
    uint32_t txResult = TX_SUCCESS;
    //查找指令对应的信道
    for(indexUtil = 0; indexUtil < DIM_ARRAY_ELEMENT_COUNT(uartUploadPackFwdUnitArray);indexUtil++)
    {
        if(uploadPackPtr->uploadID == uartUploadPackFwdUnitArray[indexUtil].uploadID)
        {
            txQueuePtr = uartUploadPackFwdUnitArray[indexUtil].dispatchQueuePtr;
            break;
        }
    }
    //检查对饮信道是否为空,为空则返回不支持该指令
    if(txQueuePtr == NULL)
    {
        //不支持的上传ID
        UART_PROT_LOG("E:> %s,%d,UnSupport UploadID:: 0X%08X\r\n",__func__,__LINE__,uploadPackPtr->uploadID);          
        return;                                    
    }
    //获取转发信道支持,申请内存
    sysMsgPtr = UserMemMalloc(MEM_REGION_SYS_MSG_MAIN,DIM_STRUCT_LENGTH_BYTE(SYS_MSG));
    uartUploadPackRecvPtr = UserMemMalloc(MEM_REGION_SYS_MSG_UART,DIM_STRUCT_LENGTH_BYTE(UART_PROT_UPLOAD_PACK_RECV));
    //检测
    if((sysMsgPtr == NULL)||(uartUploadPackRecvPtr == NULL))
    {
        //内存释放
        if(sysMsgPtr != NULL)
        {
            UserMemFree(MEM_REGION_SYS_MSG_MAIN,sysMsgPtr);
        }
        if(uartUploadPackRecvPtr != NULL)
        {
            UserMemFree(MEM_REGION_SYS_MSG_UART,uartUploadPackRecvPtr);
        }
        //内存申请失败
        UART_PROT_LOG("E:> %s,%d,MemMallocFailed UploadID:: 0X%08X\r\n",__func__,__LINE__,uploadPackPtr->uploadID);         
        return;
    }
    if(uploadPackPtr->paramBufferLength != 0)
    {
        //申请参数缓存
        uartUploadPackRecvPtr->paramBufferPtr = UserMemMalloc(MEM_REGION_SYS_MSG_UART,uploadPackPtr->paramBufferLength);
        if(uartUploadPackRecvPtr->paramBufferPtr == NULL)
        {
            UserMemFree(MEM_REGION_SYS_MSG_MAIN,sysMsgPtr);
            UserMemFree(MEM_REGION_SYS_MSG_UART,uartUploadPackRecvPtr);
            //内存申请失败
            UART_PROT_LOG("E:> %s,%d,MemMallocBufferFailed UploadID:: 0X%08X\r\n",__func__,__LINE__,uploadPackPtr->uploadID);         
            return;
        }
    }
    else
    {
        uartUploadPackRecvPtr->paramBufferPtr = NULL;
    }
    //数据拷贝
    uartUploadPackRecvPtr->srcModuleNo = uploadPackPtr->srcModuleNo;
    uartUploadPackRecvPtr->srcBoardID = uploadPackPtr->srcBoardID;
    uartUploadPackRecvPtr->targetModule = uploadPackPtr->targetModule;
    uartUploadPackRecvPtr->targetBoardID = uploadPackPtr->targetBoardID;
    uartUploadPackRecvPtr->uploadID = uploadPackPtr->uploadID;
    uartUploadPackRecvPtr->retryCount = uploadPackPtr->retryCount;
    uartUploadPackRecvPtr->paramBufferPtr = uploadPackPtr->paramBufferPtr;
    uartUploadPackRecvPtr->paramBufferLength = uploadPackPtr->paramBufferLength;
    if(uartUploadPackRecvPtr->paramBufferPtr != NULL)
    {
        UserMemCopy(uartUploadPackRecvPtr->paramBufferPtr,uploadPackPtr->paramBufferPtr,
                        uartUploadPackRecvPtr->paramBufferLength);
    }
    //数据关联
    sysMsgPtr->msgSrc = SYS_MSG_PACK_SRC_UART_UPLOAD;
    sysMsgPtr->msgBodyPtr = uartUploadPackRecvPtr;
    //拷贝完成的数据转发
    txResult = tx_queue_send(txQueuePtr,(void*)(&sysMsgPtr),TX_NO_WAIT);
    if(txResult == TX_SUCCESS)
    {
        //转发成功          
        return;
    }
    //失败了,回传异常ACK
    UART_PROT_LOG("E:> %s,%d,tx_queue_send Failed: %d,UpLoadID: 0X%08X\r\n",__func__,__LINE__,txResult,uploadPackPtr->uploadID);
    //释放内存
    if(uartUploadPackRecvPtr->paramBufferPtr != NULL)
    {
        UserMemFree(MEM_REGION_SYS_MSG_UART,uartUploadPackRecvPtr->paramBufferPtr);
    } 
    UserMemFree(MEM_REGION_SYS_MSG_UART,uartUploadPackRecvPtr);  
    UserMemFree(MEM_REGION_SYS_MSG_MAIN,sysMsgPtr);
    return;
#endif
}











