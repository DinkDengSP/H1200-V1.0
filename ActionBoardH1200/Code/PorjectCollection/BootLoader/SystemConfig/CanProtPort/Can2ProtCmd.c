/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-10-20 08:40:07 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-10-23 14:27:42 +0800
************************************************************************************************/ 
#include "CanProtPort.h"
#include "CanProtBase.h"
#include "DriverConfigSystem.h"
#include "UserMemManage.h"
#include "SrvTaskHeader.h"
#include "ModTaskHeader.h"
#include "AppTaskHeader.h"


#if(CAN2_PROT_CMD_SUPPORT == CONFIG_FUNC_ENABLE)
//定义上传数据转发数组 通道号+转发消息队列指针
static const CAN_CMD_PACK_DISPATCH_UNIT can2CmdPackDispatchUnitArray[] = {
    //辅助信道
    {(uint8_t)CAN_MASTER_CH_ACT_UTIL        , &queueSrvTaskFlashIap },
    {(uint8_t)CAN_MASTER_CH_ACT_IAP         , &queueSrvTaskUtil     },
    {(uint8_t)CAN_MASTER_CH_ACT_PORT_IN     , &queueSrvTaskPortIn   },
    {(uint8_t)CAN_MASTER_CH_ACT_PORT_OUT    , &queueSrvTaskPortOut  },
    {(uint8_t)CAN_MASTER_CH_ACT_PARAM_RW    , &queueSrvTaskParam    },
};
#endif

//CAN_PROT2 指令信息处理,发送过来的数据的内存不能直接用指针,必须深拷贝,因为调用者会清除内存
void Can2ProtCmdMsgSelfProc(CAN_PROT_CMD_PACK* cmdPackPtr)
{
#if(CAN2_PROT_CMD_SUPPORT == CONFIG_FUNC_DISABLE)
    //不支持任何指令
    Can2ProtSendAckInRecv(CAN_PROT_ACK_CMD,cmdPackPtr->srcModuleNo,cmdPackPtr->srcBoardID,cmdPackPtr->singalChannel,
                                                    cmdPackPtr->retryCount,cmdPackPtr->cmdValue,PROT_ACK_REJECT);     
    return;         
#else
    uint16_t indexUtil = 0;
    TX_QUEUE* txQueuePtr = NULL;
    SYS_MSG* sysMsgPtr = NULL;
    CAN_PROT_CMD_PACK* canCmdPackRecvPtr = NULL;
    uint32_t txResult = TX_SUCCESS;
    //查找指令对应的信道
    for(indexUtil = 0; indexUtil < DIM_ARRAY_ELEMENT_COUNT(can2CmdPackDispatchUnitArray);indexUtil++)
    {
        if(cmdPackPtr->singalChannel == can2CmdPackDispatchUnitArray[indexUtil].channel)
        {
            txQueuePtr = can2CmdPackDispatchUnitArray[indexUtil].dispatchQueuePtr;
            break;
        }
    }
    //检查对应信道是否为空,为空则返回不支持该指令
    if(txQueuePtr == NULL)
    {
        //返回ACK不支持
        Can2ProtSendAckInRecv(CAN_PROT_ACK_CMD,cmdPackPtr->srcModuleNo,cmdPackPtr->srcBoardID,cmdPackPtr->singalChannel,
                                                    cmdPackPtr->retryCount,cmdPackPtr->cmdValue,PROT_ACK_REJECT);           
        return;                                    
    }
    //获取转发信道支持,申请内存
    sysMsgPtr = UserMemMalloc(MEM_REGION_SYS_MSG_MAIN,DIM_STRUCT_LENGTH_BYTE(SYS_MSG));
    canCmdPackRecvPtr = UserMemMalloc(MEM_REGION_SYS_MSG_CAN,DIM_STRUCT_LENGTH_BYTE(CAN_PROT_CMD_PACK));
    //检测
    if((sysMsgPtr == NULL)||(canCmdPackRecvPtr == NULL))
    {
        //内存释放
        if(sysMsgPtr != NULL)
        {
            UserMemFree(MEM_REGION_SYS_MSG_MAIN,sysMsgPtr);
        }
        if(canCmdPackRecvPtr != NULL)
        {
            UserMemFree(MEM_REGION_SYS_MSG_CAN,canCmdPackRecvPtr);
        }
        //返回ACK不支持
        Can2ProtSendAckInRecv(CAN_PROT_ACK_CMD,cmdPackPtr->srcModuleNo,cmdPackPtr->srcBoardID,cmdPackPtr->singalChannel,
                                                    cmdPackPtr->retryCount,cmdPackPtr->cmdValue,PROT_ACK_REJECT);          
        return;
    }
    if(cmdPackPtr->paramBufferLength != 0)
    {
        //申请参数缓存
        canCmdPackRecvPtr->paramBufferPtr = UserMemMalloc(MEM_REGION_SYS_MSG_CAN,cmdPackPtr->paramBufferLength);
        if(canCmdPackRecvPtr->paramBufferPtr == NULL)
        {
            UserMemFree(MEM_REGION_SYS_MSG_MAIN,sysMsgPtr);
            UserMemFree(MEM_REGION_SYS_MSG_CAN,canCmdPackRecvPtr);
            //返回ACK不支持
            Can2ProtSendAckInRecv(CAN_PROT_ACK_CMD,cmdPackPtr->srcModuleNo,cmdPackPtr->srcBoardID,cmdPackPtr->singalChannel,
                                                    cmdPackPtr->retryCount,cmdPackPtr->cmdValue,PROT_ACK_REJECT);          
            return;
        }
    }
    else
    {
        canCmdPackRecvPtr->paramBufferPtr = NULL;
    }
    //数据拷贝
    canCmdPackRecvPtr->srcBoardID = cmdPackPtr->srcBoardID;
    canCmdPackRecvPtr->srcModuleNo = cmdPackPtr->srcModuleNo;
    canCmdPackRecvPtr->singalChannel = cmdPackPtr->singalChannel;
    canCmdPackRecvPtr->retryCount = cmdPackPtr->retryCount;
    canCmdPackRecvPtr->cmdValue = cmdPackPtr->cmdValue;
    canCmdPackRecvPtr->paramBufferLength = cmdPackPtr->paramBufferLength;
    if(canCmdPackRecvPtr->paramBufferPtr != NULL)
    {
        UserMemCopy(canCmdPackRecvPtr->paramBufferPtr,cmdPackPtr->paramBufferPtr,canCmdPackRecvPtr->paramBufferLength);
    }
    //数据关联
    sysMsgPtr->msgSrc = SYS_MSG_PACK_SRC_CAN2_CMD;
    sysMsgPtr->msgBodyPtr = canCmdPackRecvPtr;
    //拷贝完成的数据转发
    txResult = tx_queue_send(txQueuePtr,(void*)(&sysMsgPtr),TX_NO_WAIT);
    if(txResult == TX_SUCCESS)
    {
        //转发成功,回传正常的ACK
        Can2ProtSendAckInRecv(CAN_PROT_ACK_CMD,cmdPackPtr->srcModuleNo,cmdPackPtr->srcBoardID,cmdPackPtr->singalChannel,
                                                    cmdPackPtr->retryCount,cmdPackPtr->cmdValue,PROT_ACK_OK);          
        return;
    }
    //失败了,回传异常ACK
    CAN_PROT_LOG("E:> %s,%d,tx_queue_send Failed: %d\r\n",__func__,__LINE__,txResult);
    //回传失败
    Can2ProtSendAckInRecv(CAN_PROT_ACK_CMD,cmdPackPtr->srcModuleNo,cmdPackPtr->srcBoardID,cmdPackPtr->singalChannel,
                                                    cmdPackPtr->retryCount,cmdPackPtr->cmdValue,PROT_ACK_REJECT);     
    //释放内存
    if(canCmdPackRecvPtr->paramBufferPtr != NULL)
    {
        UserMemFree(MEM_REGION_SYS_MSG_CAN,canCmdPackRecvPtr->paramBufferPtr);
    } 
    UserMemFree(MEM_REGION_SYS_MSG_CAN,canCmdPackRecvPtr);  
    UserMemFree(MEM_REGION_SYS_MSG_MAIN,sysMsgPtr);
    return;
#endif
}













