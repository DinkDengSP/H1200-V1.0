/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-10-20 08:40:07 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-10-23 14:21:35 +0800
************************************************************************************************/ 
#include "CanProtPort.h"
#include "CanProtBase.h"
#include "DriverConfigSystem.h"
#include "UserMemManage.h"
#include "SrvTaskHeader.h"
#include "ModTaskHeader.h"
#include "AppTaskHeader.h"
#include "CanMasterHeader.h"

#if(CAN1_PROT_UPLOAD_SUPPORT == CONFIG_FUNC_ENABLE)
//定义上传数据转发数组 上传ID+转发消息队列指针
static const PROT_UPLOAD_PACK_FWD_UNIT can1UploadPackDispatchUnitArray[] = {
    //主控心跳上报
    {(uint32_t)0XFFFFFFFF , &queueSrvTaskUtil   },
};
#endif

//CAN1_PROT 主动上传信息处理
void Can1ProtUploadMsgSelfProc(CAN_PROT_UPLOAD_PACK* uploadPackPtr)
{
    //无论是否支持上报ID,都要给ACK,不要将通讯链路堵塞
    Can1ProtSendAckInRecv(CAN_PROT_ACK_UPLOAD,uploadPackPtr->srcModuleNo,uploadPackPtr->srcBoardID,0,
                            uploadPackPtr->retryCount,uploadPackPtr->uploadID,PROT_ACK_OK);  
#if(CAN1_PROT_UPLOAD_SUPPORT == CONFIG_FUNC_DISABLE)
    //不支持上报ID,直接返回即可           
    return;         
#else
    uint16_t indexUtil = 0;
    TX_QUEUE* txQueuePtr = NULL;
    SYS_MSG* sysMsgPtr = NULL;
    CAN_PROT_UPLOAD_PACK* canUploadPackRecvPtr = NULL;
    uint32_t txResult = TX_SUCCESS;
    //查找指令对应的信道
    for(indexUtil = 0; indexUtil < DIM_ARRAY_ELEMENT_COUNT(can1UploadPackDispatchUnitArray);indexUtil++)
    {
        if(uploadPackPtr->uploadID == can1UploadPackDispatchUnitArray[indexUtil].uploadID)
        {
            txQueuePtr = can1UploadPackDispatchUnitArray[indexUtil].dispatchQueuePtr;
            break;
        }
    }
    //检查对应信道是否为空,为空则返回不支持该指令
    if(txQueuePtr == NULL)
    {
        //不支持的上传ID
        CAN_PROT_LOG("E:> %s,%d,UnSupport UploadID:: 0X%08X\r\n",__func__,__LINE__,uploadPackPtr->uploadID);          
        return;                                    
    }
    //获取转发信道支持,申请内存
    sysMsgPtr = UserMemMalloc(MEM_REGION_SYS_MSG_MAIN,DIM_STRUCT_LENGTH_BYTE(SYS_MSG));
    canUploadPackRecvPtr = UserMemMalloc(MEM_REGION_SYS_MSG_CAN,DIM_STRUCT_LENGTH_BYTE(CAN_PROT_UPLOAD_PACK));
    //检测
    if((sysMsgPtr == NULL)||(canUploadPackRecvPtr == NULL))
    {
        //内存释放
        if(sysMsgPtr != NULL)
        {
            UserMemFree(MEM_REGION_SYS_MSG_MAIN,sysMsgPtr);
        }
        if(canUploadPackRecvPtr != NULL)
        {
            UserMemFree(MEM_REGION_SYS_MSG_CAN,canUploadPackRecvPtr);
        }
        //内存申请失败
        CAN_PROT_LOG("E:> %s,%d,MemMallocFailed UploadID:: 0X%08X\r\n",__func__,__LINE__,uploadPackPtr->uploadID);         
        return;
    }
    if(uploadPackPtr->uploadDataBufferLength != 0)
    {
        //申请参数缓存
        canUploadPackRecvPtr->uploadDataBufferPtr = UserMemMalloc(MEM_REGION_SYS_MSG_CAN,uploadPackPtr->uploadDataBufferLength);
        if(canUploadPackRecvPtr->uploadDataBufferPtr == NULL)
        {
            UserMemFree(MEM_REGION_SYS_MSG_MAIN,sysMsgPtr);
            UserMemFree(MEM_REGION_SYS_MSG_CAN,canUploadPackRecvPtr);
            //内存申请失败
            CAN_PROT_LOG("E:> %s,%d,MemMallocBufferFailed UploadID:: 0X%08X\r\n",__func__,__LINE__,uploadPackPtr->uploadID);         
            return;
        }
    }
    else
    {
        canUploadPackRecvPtr->uploadDataBufferPtr = NULL;
    }
    //数据拷贝
    canUploadPackRecvPtr->srcBoardID = uploadPackPtr->srcBoardID;
    canUploadPackRecvPtr->srcModuleNo = uploadPackPtr->srcModuleNo;
    canUploadPackRecvPtr->singalChannel = uploadPackPtr->singalChannel;
    canUploadPackRecvPtr->retryCount = uploadPackPtr->retryCount;
    canUploadPackRecvPtr->uploadID = uploadPackPtr->uploadID;
    canUploadPackRecvPtr->uploadDataBufferLength = uploadPackPtr->uploadDataBufferLength;
    if(canUploadPackRecvPtr->uploadDataBufferPtr != NULL)
    {
        UserMemCopy(canUploadPackRecvPtr->uploadDataBufferPtr,uploadPackPtr->uploadDataBufferPtr,
                        canUploadPackRecvPtr->uploadDataBufferLength);
    }
    //数据关联
    sysMsgPtr->msgSrc = SYS_MSG_PACK_SRC_CAN1_UPLOAD;
    sysMsgPtr->msgBodyPtr = canUploadPackRecvPtr;
    //拷贝完成的数据转发
    txResult = tx_queue_send(txQueuePtr,(void*)(&sysMsgPtr),TX_NO_WAIT);
    if(txResult == TX_SUCCESS)
    {
        //转发成功          
        return;
    }
    //失败了,回传异常ACK
    CAN_PROT_LOG("E:> %s,%d,tx_queue_send Failed: %d,UpLoadID: 0X%08X\r\n",__func__,__LINE__,txResult,uploadPackPtr->uploadID);
    //释放内存
    if(canUploadPackRecvPtr->uploadDataBufferPtr != NULL)
    {
        UserMemFree(MEM_REGION_SYS_MSG_CAN,canUploadPackRecvPtr->uploadDataBufferPtr);
    } 
    UserMemFree(MEM_REGION_SYS_MSG_CAN,canUploadPackRecvPtr);  
    UserMemFree(MEM_REGION_SYS_MSG_MAIN,sysMsgPtr);
    return;
#endif
}














