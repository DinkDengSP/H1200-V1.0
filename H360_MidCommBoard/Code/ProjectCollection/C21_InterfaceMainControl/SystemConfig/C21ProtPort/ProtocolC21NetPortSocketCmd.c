/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-09-06 09:24:44 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-09-06 13:37:03 +0800
************************************************************************************************/ 
#include "ProtocolC21NetPort.h"
#include "ProtocolC21NetBase.h"
#include "DriverConfigSystem.h"
#include "UserMemManage.h"
#include "C21NetOumengInterHeader.h"
#include "SrvTaskHeader.h"
#include "ModTaskHeader.h"
#include "AppTaskHeader.h"

static const C21_NET_CMD_DISPATCH_UNIT c21NetCmdPackDispatchUnitArray[] = {
    {(uint16_t)MACRO_C21_NET_CMD_C21_APPLY_RANK_IN                  ,   &queueAppTaskMsgProc},
    {(uint16_t)MACRO_C21_NET_CMD_ZETA_REPORT_RANK_IN_RESULT         ,   &queueAppTaskMsgProc},
    {(uint16_t)MACRO_C21_NET_CMD_C21_NOTOFY_RANK_OUT                ,   &queueAppTaskMsgProc},
    {(uint16_t)MACRO_C21_NET_CMD_ZETA_REPORT_RANK_OUT_PROC_RESULT   ,   &queueAppTaskMsgProc},
    {(uint16_t)MACRO_C21_NET_CMD_C21_REPORT_DEVICE_STATE_CHANGE     ,   &queueAppTaskMsgProc},
    {(uint16_t)MACRO_C21_NET_CMD_ZETA_REPORT_DEVICE_STATE_CHANGE    ,   &queueAppTaskMsgProc},
    {(uint16_t)MACRO_C21_NET_CMD_ZETA_REPORT_HEART                  ,   &queueAppTaskMsgProc},
    {(uint16_t)MACRO_C21_NET_CMD_ZETA_POS_ADJUST_START              ,   &queueAppTaskMsgProc},
    {(uint16_t)MACRO_C21_NET_CMD_ZETA_POS_ADJUST_NEXT               ,   &queueAppTaskMsgProc},
    {(uint16_t)MACRO_C21_NET_CMD_ZETA_POS_ADJUST_OVER               ,   &queueAppTaskMsgProc},
    {(uint16_t)MACRO_C21_NET_CMD_ZETA_TROUBLE_SHOOTING              ,   &queueAppTaskMsgProc},
    {(uint16_t)MACRO_C21_NET_CMD_ZETA_REPORT_ERROR                  ,   &queueAppTaskMsgProc},
};

//SOCKET 指令信息处理,发送过来的数据的内存不能直接用指针,必须深拷贝,因为调用者会清除内存
void ProtocolC21NetSocketPackRecvProcessCmd(C21_NET_PACKAGE* uartSocketPackagePtr)
{
    //不管要不要,先回ACK
    ProtocolC21AckMsgDispatch(C21_ACK_RESULT_OK);
    //开始转发
    uint16_t indexUtil = 0;
    TX_QUEUE* txQueuePtr = NULL;
    SYS_MSG* sysMsgPtr = NULL;
    C21_NET_PACKAGE* c21NetCmdPackRecvPtr = NULL;
    uint32_t txResult = TX_SUCCESS;
    //查找指令对应的信道
    for(indexUtil = 0; indexUtil < DIM_ARRAY_ELEMENT_COUNT(c21NetCmdPackDispatchUnitArray);indexUtil++)
    {
        if(uartSocketPackagePtr->cmdCode == c21NetCmdPackDispatchUnitArray[indexUtil].cmdValue)
        {
            txQueuePtr = c21NetCmdPackDispatchUnitArray[indexUtil].dispatchQueuePtr;
            break;
        }
    }
    //检查对应信道是否为空,为空则返回不支持该指令
    if(txQueuePtr == NULL)
    {
        PROTOCOL_C21_NET_SOCKET_LOG("E:> %s,%d,Cmd : 0X%04X Not Support\r\n",__func__,__LINE__,
                                        uartSocketPackagePtr->cmdCode);          
        return;                                    
    }
    //获取转发信道支持,申请内存
    sysMsgPtr = UserMemMalloc(MEM_REGION_SYS_MSG_MAIN,DIM_STRUCT_LENGTH_BYTE(SYS_MSG));
    c21NetCmdPackRecvPtr = UserMemMalloc(MEM_REGION_SYS_MSG_NET,DIM_STRUCT_LENGTH_BYTE(C21_NET_PACKAGE));
    //检测
    if((sysMsgPtr == NULL)||(c21NetCmdPackRecvPtr == NULL))
    {
        //内存释放
        if(sysMsgPtr != NULL)
        {
            UserMemFree(MEM_REGION_SYS_MSG_MAIN,sysMsgPtr);
        }
        if(c21NetCmdPackRecvPtr != NULL)
        {
            UserMemFree(MEM_REGION_SYS_MSG_NET,c21NetCmdPackRecvPtr);
        }
        PROTOCOL_C21_NET_SOCKET_LOG("E:> %s,%d,Cmd: 0X%04X,UserMemMalloc Failed\r\n",__func__,__LINE__,
                                        uartSocketPackagePtr->cmdCode);      
        return;
    }
    if(uartSocketPackagePtr->dataBodyLength != 0)
    {
        //申请参数缓存
        c21NetCmdPackRecvPtr->dataBodyBufferPtr = UserMemMalloc(MEM_REGION_SYS_MSG_NET,uartSocketPackagePtr->dataBodyLength);
        if(c21NetCmdPackRecvPtr->dataBodyBufferPtr == NULL)
        {
            UserMemFree(MEM_REGION_SYS_MSG_MAIN,sysMsgPtr);
            UserMemFree(MEM_REGION_SYS_MSG_NET,c21NetCmdPackRecvPtr);
            PROTOCOL_C21_NET_SOCKET_LOG("E:> %s,%d,Cmd: 0X%04X,UserMemMalloc Failed\r\n",__func__,__LINE__,
                                        uartSocketPackagePtr->cmdCode);      
            return;
        }
    }
    else
    {
        c21NetCmdPackRecvPtr->dataBodyBufferPtr = NULL;
    }
    //数据拷贝
    c21NetCmdPackRecvPtr->cmdCode = uartSocketPackagePtr->cmdCode;
    c21NetCmdPackRecvPtr->dataBodyLength = uartSocketPackagePtr->dataBodyLength;
    if(c21NetCmdPackRecvPtr->dataBodyBufferPtr != NULL)
    {
        UserMemCopy(c21NetCmdPackRecvPtr->dataBodyBufferPtr,uartSocketPackagePtr->dataBodyBufferPtr,
                        c21NetCmdPackRecvPtr->dataBodyLength);
    }
    //数据关联
    sysMsgPtr->msgSrc = SYS_MSG_PACK_SRC_NET5500_CMD;
    sysMsgPtr->msgBodyPtr = c21NetCmdPackRecvPtr;
    //拷贝完成的数据转发
    txResult = tx_queue_send(txQueuePtr,(void*)(&sysMsgPtr),TX_NO_WAIT);
    if(txResult == TX_SUCCESS)
    {
        //转发成功        
        return;
    }
    //失败了,回传异常ACK
    PROTOCOL_C21_NET_SOCKET_LOG("E:> %s,%d,tx_queue_send Failed: %d\r\n",__func__,__LINE__,txResult);  
    //释放内存
    if(c21NetCmdPackRecvPtr->dataBodyBufferPtr != NULL)
    {
        UserMemFree(MEM_REGION_SYS_MSG_NET,c21NetCmdPackRecvPtr->dataBodyBufferPtr);
    } 
    UserMemFree(MEM_REGION_SYS_MSG_NET,c21NetCmdPackRecvPtr);  
    UserMemFree(MEM_REGION_SYS_MSG_MAIN,sysMsgPtr);
    return;
}














