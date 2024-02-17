/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-07-26 12:07:21 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-07-26 13:20:15 +0800
************************************************************************************************/ 
#include "IPC_Port.h"
#include "IPC_Base.h"
#include "DriverConfigSystem.h"
#include "UserMemManage.h"
#include "SystemBase.h"
#include "tx_mutex.h"


//内存申请
void* IPC_PortMemMalloc(uint32_t sizeBytes)
{
    return UserMemMalloc(MEM_REGION_SYS_MSG_IPC,sizeBytes);
}

//内存申请,必须成功
void* IPC_PortMemMallocWhileSuccess(uint32_t sizeBytes)
{
    return UserMemMallocWhileSuccess(MEM_REGION_SYS_MSG_IPC,sizeBytes);
}

//内存释放
void IPC_PortMemFree(void* memPtr)
{   
    UserMemFree(MEM_REGION_SYS_MSG_IPC,memPtr);
}

//发送上报包
ERROR_SUB IPC_PortSendUploadPack(TX_QUEUE* queuePtr,IPC_UPLOAD_DATA* uploadDataPtr)
{
    SYS_MSG* sysMsgPtr = NULL;
    uint32_t txResult = TX_SUCCESS;
    //获取转发信道支持,申请内存
    sysMsgPtr = UserMemMalloc(MEM_REGION_SYS_MSG_MAIN,DIM_STRUCT_LENGTH_BYTE(SYS_MSG));
    if(sysMsgPtr == NULL)
    {
        return ERROR_SUB_BOARD_IPC_UPLOAD_MEM_MALLOC_SYS_MSG;
    }
    //数据关联
    sysMsgPtr->msgSrc = SYS_MSG_PACK_SRC_IPC_UPLOAD;
    sysMsgPtr->msgBodyPtr = uploadDataPtr;
    //拷贝完成的数据转发
    txResult = tx_queue_send(queuePtr,(void*)(&sysMsgPtr),TX_NO_WAIT);
    if(txResult == TX_SUCCESS)
    {       
        return ERROR_SUB_OK;
    }
    IPC_LOG("E>: %s,%d,tx_queue_send Failed: %d\r\n",__func__,__LINE__,txResult);
    UserMemFree(MEM_REGION_SYS_MSG_MAIN,sysMsgPtr);
    return ERROR_SUB_BOARD_IPC_UPLOAD_SYS_QUEUE_SEND;
}

//发送指令包
ERROR_SUB IPC_PortSendCmdPack(IPC_CHANNEL_DATA* channelDataPtr,IPC_CMD_DATA* cmdDataPtr)
{
    SYS_MSG* sysMsgPtr = NULL;
    uint32_t txResult = TX_SUCCESS;
    //获取转发信道支持,申请内存
    sysMsgPtr = UserMemMalloc(MEM_REGION_SYS_MSG_MAIN,DIM_STRUCT_LENGTH_BYTE(SYS_MSG));
    if(sysMsgPtr == NULL)
    {
        return ERROR_SUB_BOARD_IPC_CMD_MEM_MALLOC_SYS_MSG;
    }
    //数据关联
    sysMsgPtr->msgSrc = SYS_MSG_PACK_SRC_IPC_CMD;
    sysMsgPtr->msgBodyPtr = cmdDataPtr;
    //拷贝完成的数据转发
    txResult = tx_queue_send(channelDataPtr->targetQueuePtr,(void*)(&sysMsgPtr),TX_NO_WAIT);
    if(txResult == TX_SUCCESS)
    {       
        return ERROR_SUB_OK;
    }
    IPC_LOG("E>: %s,%d,tx_queue_send Failed: %d\r\n",__func__,__LINE__,txResult);
    UserMemFree(MEM_REGION_SYS_MSG_MAIN,sysMsgPtr);
    return ERROR_SUB_BOARD_IPC_CMD_SYS_QUEUE_SEND;
}





