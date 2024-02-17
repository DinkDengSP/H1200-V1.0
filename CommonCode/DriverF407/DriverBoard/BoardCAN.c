/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-08-23 09:44:56 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-10-25 14:08:32 +0800
************************************************************************************************/ 
#include "BoardCAN.h"

#ifdef BOARD_CAN1_VDD_SENSE_FUNC
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
#if(BOARD_CAN1_VDD_SENSE_FUNC == CONFIG_FUNC_ENABLE)
    //电压检测端口
    MCU_PinInit(PIN_BOARD_CAN1_VDD_SENSE,MCU_PIN_MODE_INPUT,PULL_MODE_BOARD_CAN1_VDD_SENSE,SPEED_BOARD_CAN1_VDD_SENSE,0);
#endif
    //CAN初始化
    MCU_CAN1_Init(extFilterVal,extFilterMask,recvCallBackPtr);
}

//将接收到并处理完成的数据放回驱动空闲队列
void BoardCAN1_RecvDatRelease(CAN_RECV_PACK* recvDataPtr)
{
    MCU_CAN1_RecvDatRelease(recvDataPtr);
}

//检查PHY状态
ErrorStatus BoardCAN1_CheckPhyState(void)
{
#if(BOARD_CAN1_VDD_SENSE_FUNC == CONFIG_FUNC_ENABLE)
    GPIO_PinState pinState = VALID_LEVEL_BOARD_CAN1_VDD_SENSE;
    //读取PHY端口信号
    pinState = MCU_PinReadSingle(PIN_BOARD_CAN1_VDD_SENSE);
    if(pinState != VALID_LEVEL_BOARD_CAN1_VDD_SENSE)
    {
        return ERROR;
    }
    return SUCCESS;
#else
    return SUCCESS;
#endif
}

//CAN总线发送数据
ERROR_SUB BoardCAN1_SendMsg(uint32_t id,CAN_ID_TYPE idType,uint8_t dataLen,uint8_t* dataBuf)
{
#if(BOARD_CAN1_VDD_SENSE_FUNC == CONFIG_FUNC_ENABLE)
    GPIO_PinState pinState = VALID_LEVEL_BOARD_CAN1_VDD_SENSE;
    //读取PHY端口信号
    pinState = MCU_PinReadSingle(PIN_BOARD_CAN1_VDD_SENSE);
    if(pinState != VALID_LEVEL_BOARD_CAN1_VDD_SENSE)
    {
        return ERROR_SUB_MCU_CAN1_PHY_POWER_DOWN;
    }
#endif
    return MCU_CAN1_SendMsg(id,idType,dataLen,dataBuf);
}

//获取CAN1的各项信息
MCU_CAN_INFO* BoardCAN1_GetInfo(void)
{
    return MCU_CAN1_GetInfo();
}
#endif


#ifdef BOARD_CAN2_VDD_SENSE_FUNC
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
#if(BOARD_CAN2_VDD_SENSE_FUNC == CONFIG_FUNC_ENABLE)
    //电压检测端口
    MCU_PinInit(PIN_BOARD_CAN2_VDD_SENSE,MCU_PIN_MODE_INPUT,PULL_MODE_BOARD_CAN2_VDD_SENSE,SPEED_BOARD_CAN2_VDD_SENSE,0);
#endif
    //CAN初始化
    MCU_CAN2_Init(extFilterVal,extFilterMask,recvCallBackPtr);
}

//将接收到并处理完成的数据放回驱动空闲队列
void BoardCAN2_RecvDatRelease(CAN_RECV_PACK* recvDataPtr)
{
    MCU_CAN2_RecvDatRelease(recvDataPtr);
}

//检查PHY状态
ErrorStatus BoardCAN2_CheckPhyState(void)
{
#if(BOARD_CAN2_VDD_SENSE_FUNC == CONFIG_FUNC_ENABLE)
    GPIO_PinState pinState = VALID_LEVEL_BOARD_CAN2_VDD_SENSE;
    //读取PHY端口信号
    pinState = MCU_PinReadSingle(PIN_BOARD_CAN2_VDD_SENSE);
    if(pinState != VALID_LEVEL_BOARD_CAN2_VDD_SENSE)
    {
        return ERROR;
    }
    return SUCCESS;
#else
    return SUCCESS;
#endif
}

//CAN总线发送数据
ERROR_SUB BoardCAN2_SendMsg(uint32_t id,CAN_ID_TYPE idType,uint8_t dataLen,uint8_t* dataBuf)
{
#if(BOARD_CAN2_VDD_SENSE_FUNC == CONFIG_FUNC_ENABLE)
    GPIO_PinState pinState = VALID_LEVEL_BOARD_CAN2_VDD_SENSE;
    //读取PHY端口信号
    pinState = MCU_PinReadSingle(PIN_BOARD_CAN2_VDD_SENSE);
    if(pinState != VALID_LEVEL_BOARD_CAN2_VDD_SENSE)
    {
        return ERROR_SUB_MCU_CAN2_PHY_POWER_DOWN;
    }
#endif
    return MCU_CAN2_SendMsg(id,idType,dataLen,dataBuf);
}

//获取CAN2的各项信息
MCU_CAN_INFO* BoardCAN2_GetInfo(void)
{
    return MCU_CAN2_GetInfo();
}

#endif









