/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-10-19 21:18:23 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-10-19 21:18:24 +0800
************************************************************************************************/ 
#include "BoardCAN.h"
#include "DriverConfigBoard.h"

//获取CAN互斥锁
void BoardCAN1_GetLock(void)
{
    MCU_CAN1_GetLock();
}

//释放CAN1互斥锁
void BoardCAN1_ReleaseLock(void)
{
    MCU_CAN1_ReleaseLock();
}

//初始化
void BoardCAN1_Init(uint32_t extFilterVal,uint32_t extFilterMask,MCU_CAN_RecvPackProcFunc recvCallBackPtr)
{
    //扩展输出端口
    MCU_PinInit(PIN_BOARD_CAN1_AUX_OUT,MCU_PIN_MODE_OUTPUT_PP,MCU_PIN_PULL_PULLUP,MCU_PIN_SPEED_FREQ_MEDIUM,0);
    MCU_PinWriteSingle(PIN_BOARD_CAN1_AUX_OUT,INIT_LEVEL_BOARD_CAN1_AUX_OUT);
#if(MCU_CAN1_MODE_USER != MCU_CAN_MODE_FD_BRS)
    //CAN初始化
    MCU_CAN1_Init(extFilterVal,extFilterMask,BOARD_CAN1_ARBITRATION_BAUD_USER,recvCallBackPtr);
#else
    //CAN初始化
    MCU_CAN1_Init(extFilterVal,extFilterMask,BOARD_CAN1_ARBITRATION_BAUD_USER,BOARD_CAN1_DATA_BAUD_USER,recvCallBackPtr);
#endif
}

//将接收到并处理完成的数据放回驱动空闲队列
void BoardCAN1_RecvDatRelease(CAN_RECV_PACK* recvDataPtr)
{
    MCU_CAN1_RecvDatRelease(recvDataPtr);
}

//设置AUX输出
void BoardCAN1_SetAuxOutState(PIN_STATE setState)
{
    if(setState == PIN_STATE_VALID)
    {
        MCU_PinWriteSingle(PIN_BOARD_CAN1_AUX_OUT,VALID_LEVEL_BOARD_CAN1_AUX_OUT);
    }
    else
    {
        MCU_PinWriteSingle(PIN_BOARD_CAN1_AUX_OUT,PIN_INVALID_LEVEL(VALID_LEVEL_BOARD_CAN1_AUX_OUT));
    }
}

//CAN总线发送数据
ERROR_SUB BoardCAN1_SendMsg(uint32_t id,CAN_ID_TYPE idType,uint8_t dataLen,uint8_t* dataBuf)
{
    return MCU_CAN1_SendMsg(id,idType,dataLen,dataBuf);
}

//获取CAN1的各项信息
MCU_CAN_INFO* BoardCAN1_GetInfo(void)
{
    return MCU_CAN1_GetInfo();
}




//获取CAN互斥锁
void BoardCAN2_GetLock(void)
{
    MCU_CAN2_GetLock();
}

//释放CAN2互斥锁
void BoardCAN2_ReleaseLock(void)
{
    MCU_CAN2_ReleaseLock();
}

//初始化
void BoardCAN2_Init(uint32_t extFilterVal,uint32_t extFilterMask,MCU_CAN_RecvPackProcFunc recvCallBackPtr)
{
    //扩展输出端口
    MCU_PinInit(PIN_BOARD_CAN2_AUX_OUT,MCU_PIN_MODE_OUTPUT_PP,MCU_PIN_PULL_PULLUP,MCU_PIN_SPEED_FREQ_MEDIUM,0);
    MCU_PinWriteSingle(PIN_BOARD_CAN2_AUX_OUT,INIT_LEVEL_BOARD_CAN2_AUX_OUT);
#if(MCU_CAN2_MODE_USER != MCU_CAN_MODE_FD_BRS)
    //CAN初始化
    MCU_CAN2_Init(extFilterVal,extFilterMask,BOARD_CAN2_ARBITRATION_BAUD_USER,recvCallBackPtr);
#else
    //CAN初始化
    MCU_CAN2_Init(extFilterVal,extFilterMask,BOARD_CAN2_ARBITRATION_BAUD_USER,BOARD_CAN2_DATA_BAUD_USER,recvCallBackPtr);
#endif
}

//将接收到并处理完成的数据放回驱动空闲队列
void BoardCAN2_RecvDatRelease(CAN_RECV_PACK* recvDataPtr)
{
    MCU_CAN2_RecvDatRelease(recvDataPtr);
}

//设置AUX输出
void BoardCAN2_SetAuxOutState(PIN_STATE setState)
{
    if(setState == PIN_STATE_VALID)
    {
        MCU_PinWriteSingle(PIN_BOARD_CAN2_AUX_OUT,VALID_LEVEL_BOARD_CAN2_AUX_OUT);
    }
    else
    {
        MCU_PinWriteSingle(PIN_BOARD_CAN2_AUX_OUT,PIN_INVALID_LEVEL(VALID_LEVEL_BOARD_CAN2_AUX_OUT));
    }
}

//CAN总线发送数据
ERROR_SUB BoardCAN2_SendMsg(uint32_t id,CAN_ID_TYPE idType,uint8_t dataLen,uint8_t* dataBuf)
{
    return MCU_CAN2_SendMsg(id,idType,dataLen,dataBuf);
}

//获取CAN2的各项信息
MCU_CAN_INFO* BoardCAN2_GetInfo(void)
{
    return MCU_CAN2_GetInfo();
}

