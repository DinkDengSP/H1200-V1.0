/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-10-20 08:40:07 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-11-03 12:59:53 +0800
************************************************************************************************/ 
#include "CanProtPort.h"
#include "CanProtBase.h"
#include "DriverConfigSystem.h"
#include "UserMemManage.h"

//数据中转区域
static uint8_t can2ProtDataBufferTemp[CAN2_PROT_DATA_BUF_LEN] = {0};
/*******************************************移植接口*****************************************/
//CAN2_PROT自身模块号
uint8_t Can2ProtPortGetSelfModuleNo(void)
{
    return (uint8_t)SELF_MODULE_NO;
}

//CAN2_PROT自身板号
uint8_t Can2ProtPortGetSelfBoardID(void)
{   
    return (uint8_t)SELF_BOARD_ID;
}

//获取CAN2_PROT互斥锁
void Can2ProtPortGetMutexLock(void)
{
    BoardCAN2_GetLock();
}

//释放CAN2_PROT互斥锁
void Can2ProtPortReleaseMutexLock(void)
{
    BoardCAN2_ReleaseLock();
}

//发送CAN2_PROT一帧
ERROR_SUB Can2ProtPortSendFrame(uint32_t extendID,uint8_t msgBodyLen,uint8_t* bodyBuf)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    uint8_t realDataLength = 0;
    //检查数据长度
    if(msgBodyLen > CAN2_PROT_DATA_BUF_LEN)
    {
        //错误信息
        MCU_LogShowErrorSubCode(ERROR_SUB_BOARD_CAN2_PROT_FRAME_LEN_OVER);
        //数据溢出
        return ERROR_SUB_BOARD_CAN2_PROT_FRAME_LEN_OVER;
    }
    //申请互斥信号量
    Can2ProtPortGetMutexLock();
    //数据拷贝
    if(msgBodyLen <= 8)
    {
        UserMemCopy(can2ProtDataBufferTemp,bodyBuf,msgBodyLen);
        realDataLength = msgBodyLen;
    }
    //发送数据
    errorSub = BoardCAN2_SendMsg(extendID,CAN_ID_TYPE_EXT,realDataLength,can2ProtDataBufferTemp);
    if(errorSub != ERROR_SUB_OK)
    {
        MCU_LogShowErrorSubCode(errorSub);
    }
    //可选延迟
#if(TIME_INVERVAL_MS_CAN2_PROT_SEND_FRAME != 0)
    CoreDelayMs(TIME_INVERVAL_MS_CAN2_PROT_SEND_FRAME);
#endif
    //释放互斥量
    Can2ProtPortReleaseMutexLock();
    //返回结果
    return errorSub;
}

//内存申请
void* Can2ProtPortMemMalloc(uint32_t sizeBytes)
{
    return UserMemMalloc(MEM_REGION_SYS_MSG_CAN,sizeBytes);
}

//内存申请,必须成功
void* Can2ProtPortMemMallocWhileSuccess(uint32_t sizeBytes)
{
    return UserMemMallocWhileSuccess(MEM_REGION_SYS_MSG_CAN,sizeBytes);
}

//内存释放
void Can2ProtPortMemFree(void* memPtr)
{
    UserMemFree(MEM_REGION_SYS_MSG_CAN,memPtr);
}

//ACK信息转发
ERROR_SUB Can2ProtAckMsgDispatch(CAN_PROT_ACK_PACK* ackPackPtr)
{
    return ERROR_SUB_OK;
}














