/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-10-17 16:13:03 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-10-24 17:08:04 +0800
************************************************************************************************/ 
#include "UartProtPort.h"
#include "UartProtBase.h"
#include "DriverConfigSystem.h"
#include "UserMemManage.h"
#include "SrvTaskHeader.h"
#include "ModTaskHeader.h"
#include "AppTaskHeader.h"
#include "UartMasterHeader.h"

//指令转发信道
typedef struct UART_PROT_CMD_FWD_UNIT
{
    uint32_t cmdCode;
    TX_QUEUE* dispatchQueuePtr;
}UART_PROT_CMD_FWD_UNIT;


#if(UART_PROT_CMD_SUPPORT == UART_PROT_CMD_SUPPORT)
//定义上传数据转发数组 通道号+转发消息队列指针
static const UART_PROT_CMD_FWD_UNIT uartProtCmdPackFwdUnitArray[] = {
    //动作指令
    //系统辅助
    {(uint32_t)UART_MASTER_CMD_MAIN_UTIL_ONLINE                        , &queueSrvTaskUtil          },
    {(uint32_t)UART_MASTER_CMD_MAIN_UTIL_OFFLINE                       , &queueSrvTaskUtil          },
    {(uint32_t)UART_MASTER_CMD_MAIN_UTIL_UTC_SYNC                      , &queueSrvTaskUtil          },
    {(uint32_t)UART_MASTER_CMD_MAIN_UTIL_HEART_GET                     , &queueSrvTaskUtil          },
    {(uint32_t)UART_MASTER_CMD_MAIN_UTIL_EM_STOP                       , &queueSrvTaskUtil          },
    {(uint32_t)UART_MASTER_CMD_MAIN_UTIL_GET_VERSION                   , &queueSrvTaskUtil          },
    //在线升级
    {(uint32_t)UART_MASTER_CMD_MAIN_IAP_PREPARE                        , &queueSrvTaskFlashIap      },
    {(uint32_t)UART_MASTER_CMD_MAIN_IAP_START_MSG                      , &queueSrvTaskFlashIap      },
    {(uint32_t)UART_MASTER_CMD_MAIN_IAP_DATA_TRANS                     , &queueSrvTaskFlashIap      },
    {(uint32_t)UART_MASTER_CMD_MAIN_IAP_DATA_CHECK                     , &queueSrvTaskFlashIap      },
    {(uint32_t)UART_MASTER_CMD_MAIN_IAP_FORCE_JMP                      , &queueSrvTaskFlashIap      },
    //调试辅助
    {(uint32_t)UART_MASTER_CMD_MAIN_PORT_IN_READ_SINGLE                , &queueSrvTaskUtil          },
    {(uint32_t)UART_MASTER_CMD_MAIN_PORT_IN_READ_RANGE                 , &queueSrvTaskUtil          },
    {(uint32_t)UART_MASTER_CMD_MAIN_PORT_IN_READ_ANY                   , &queueSrvTaskUtil          },
    {(uint32_t)UART_MASTER_CMD_MAIN_PORT_OUT_WRITE_SINGLE              , &queueSrvTaskUtil          },
    {(uint32_t)UART_MASTER_CMD_MAIN_PORT_OUT_WRITE_ANY                 , &queueSrvTaskUtil          },
    {(uint32_t)UART_MASTER_CMD_MAIN_PARAM_RW_READ_SINGLE               , &queueSrvTaskUtil          },
    {(uint32_t)UART_MASTER_CMD_MAIN_PARAM_RW_READ_ANY                  , &queueSrvTaskUtil          },
    {(uint32_t)UART_MASTER_CMD_MAIN_PARAM_RW_WRITE_SINGLE              , &queueSrvTaskUtil          },
    {(uint32_t)UART_MASTER_CMD_MAIN_PARAM_RW_WRITE_ANY                 , &queueSrvTaskUtil          },
};
#endif

//SOCKET 指令信息处理,发送过来的数据的内存不能直接用指针,必须深拷贝,因为调用者会清除内存
void UartProtCmdMsgSelfProcess(UART_PROT_CMD_PACK_RECV* cmdPackPtr)
{
#if(UART_PROT_CMD_SUPPORT == CONFIG_FUNC_DISABLE)
    //不支持任何指令
    UartProtSendAckInRecv(UART_PROT_ACK_CMD,cmdPackPtr->srcModuleNo,cmdPackPtr->srcBoardID,
                            cmdPackPtr->retryCount,cmdPackPtr->cmdCode,PROT_ACK_REJECT);         
    return;         
#else
    uint16_t indexUtil = 0;
    TX_QUEUE* txQueuePtr = NULL;
    SYS_MSG* sysMsgPtr = NULL;
    UART_PROT_CMD_PACK_RECV* uartCmdPackRecvPtr = NULL;
    uint32_t txResult = TX_SUCCESS;
    //查找指令对应的信道
    for(indexUtil = 0; indexUtil < DIM_ARRAY_ELEMENT_COUNT(uartProtCmdPackFwdUnitArray);indexUtil++)
    {
        if(cmdPackPtr->cmdCode == uartProtCmdPackFwdUnitArray[indexUtil].cmdCode)
        {
            txQueuePtr = uartProtCmdPackFwdUnitArray[indexUtil].dispatchQueuePtr;
            break;
        }
    }
    //检查对饮信道是否为空,为空则返回不支持该指令
    if(txQueuePtr == NULL)
    {
        //返回ACK不支持
        UartProtSendAckInRecv(UART_PROT_ACK_CMD,cmdPackPtr->srcModuleNo,cmdPackPtr->srcBoardID,
                            cmdPackPtr->retryCount,cmdPackPtr->cmdCode,PROT_ACK_REJECT);          
        return;                                    
    }
    //获取转发信道支持,申请内存
    sysMsgPtr = UserMemMalloc(MEM_REGION_SYS_MSG_MAIN,DIM_STRUCT_LENGTH_BYTE(SYS_MSG));
    uartCmdPackRecvPtr = UserMemMalloc(MEM_REGION_SYS_MSG_UART,DIM_STRUCT_LENGTH_BYTE(UART_PROT_CMD_PACK_RECV));
    //检测
    if((sysMsgPtr == NULL)||(uartCmdPackRecvPtr == NULL))
    {
        //内存释放
        if(sysMsgPtr != NULL)
        {
            UserMemFree(MEM_REGION_SYS_MSG_MAIN,sysMsgPtr);
        }
        if(uartCmdPackRecvPtr != NULL)
        {
            UserMemFree(MEM_REGION_SYS_MSG_UART,uartCmdPackRecvPtr);
        }
        //返回ACK不支持
        UartProtSendAckInRecv(UART_PROT_ACK_CMD,cmdPackPtr->srcModuleNo,cmdPackPtr->srcBoardID,
                            cmdPackPtr->retryCount,cmdPackPtr->cmdCode,PROT_ACK_REJECT);         
        return;
    }
    if(cmdPackPtr->dataBodyLength != 0)
    {
        //申请参数缓存
        uartCmdPackRecvPtr->dataBodyBufferPtr = UserMemMalloc(MEM_REGION_SYS_MSG_UART,cmdPackPtr->dataBodyLength);
        if(uartCmdPackRecvPtr->dataBodyBufferPtr == NULL)
        {
            UserMemFree(MEM_REGION_SYS_MSG_MAIN,sysMsgPtr);
            UserMemFree(MEM_REGION_SYS_MSG_UART,uartCmdPackRecvPtr);
            //返回ACK不支持
            UartProtSendAckInRecv(UART_PROT_ACK_CMD,cmdPackPtr->srcModuleNo,cmdPackPtr->srcBoardID,
                            cmdPackPtr->retryCount,cmdPackPtr->cmdCode,PROT_ACK_REJECT);          
            return;
        }
    }
    else
    {
        uartCmdPackRecvPtr->dataBodyBufferPtr = NULL;
    }
    //数据拷贝
    uartCmdPackRecvPtr->srcModuleNo = cmdPackPtr->srcModuleNo;
    uartCmdPackRecvPtr->srcBoardID = cmdPackPtr->srcBoardID;
    uartCmdPackRecvPtr->targetModule = cmdPackPtr->targetModule;
    uartCmdPackRecvPtr->targetBoardID = cmdPackPtr->targetBoardID;
    uartCmdPackRecvPtr->cmdCode = cmdPackPtr->cmdCode;
    uartCmdPackRecvPtr->retryCount = cmdPackPtr->retryCount;
    uartCmdPackRecvPtr->dataBodyLength = cmdPackPtr->dataBodyLength;
    if(uartCmdPackRecvPtr->dataBodyBufferPtr != NULL)
    {
        UserMemCopy(uartCmdPackRecvPtr->dataBodyBufferPtr,cmdPackPtr->dataBodyBufferPtr,uartCmdPackRecvPtr->dataBodyLength);
    }
    //数据关联
    sysMsgPtr->msgSrc = SYS_MSG_PACK_SRC_UART_CMD;
    sysMsgPtr->msgBodyPtr = uartCmdPackRecvPtr;
    //拷贝完成的数据转发
    txResult = tx_queue_send(txQueuePtr,(void*)(&sysMsgPtr),TX_NO_WAIT);
    if(txResult == TX_SUCCESS)
    {
        //转发成功,回传正常的ACK
        UartProtSendAckInRecv(UART_PROT_ACK_CMD,cmdPackPtr->srcModuleNo,cmdPackPtr->srcBoardID,
                            cmdPackPtr->retryCount,cmdPackPtr->cmdCode,PROT_ACK_OK);           
        return;
    }
    //失败了,回传异常ACK
    UART_PROT_LOG("E:> %s,%d,tx_queue_send Failed: %d\r\n",__func__,__LINE__,txResult);
    //回传失败
    UartProtSendAckInRecv(UART_PROT_ACK_CMD,cmdPackPtr->srcModuleNo,cmdPackPtr->srcBoardID,
                            cmdPackPtr->retryCount,cmdPackPtr->cmdCode,PROT_ACK_REJECT);    
    //释放内存
    if(uartCmdPackRecvPtr->dataBodyBufferPtr != NULL)
    {
        UserMemFree(MEM_REGION_SYS_MSG_UART,uartCmdPackRecvPtr->dataBodyBufferPtr);
    } 
    UserMemFree(MEM_REGION_SYS_MSG_UART,uartCmdPackRecvPtr);  
    UserMemFree(MEM_REGION_SYS_MSG_MAIN,sysMsgPtr);
    return;
#endif
}






