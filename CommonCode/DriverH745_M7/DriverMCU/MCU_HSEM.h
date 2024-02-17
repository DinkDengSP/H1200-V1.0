/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-10-16 17:20:56 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-10-16 17:21:17 +0800
************************************************************************************************/ 
#ifndef __MCU_HSEM_H_
#define __MCU_HSEM_H_
#include "DriverHeaderCore.h"

//硬件信号量通道
typedef enum MCU_HSEM_SEM_CH
{
    MCU_HSEM_SEM_CH_0   = __HAL_HSEM_SEMID_TO_MASK(0),
    MCU_HSEM_SEM_CH_1   = __HAL_HSEM_SEMID_TO_MASK(1),
    MCU_HSEM_SEM_CH_2   = __HAL_HSEM_SEMID_TO_MASK(2),
    MCU_HSEM_SEM_CH_3   = __HAL_HSEM_SEMID_TO_MASK(3),
    MCU_HSEM_SEM_CH_4   = __HAL_HSEM_SEMID_TO_MASK(4),
    MCU_HSEM_SEM_CH_5   = __HAL_HSEM_SEMID_TO_MASK(5),
    MCU_HSEM_SEM_CH_6   = __HAL_HSEM_SEMID_TO_MASK(6),
    MCU_HSEM_SEM_CH_7   = __HAL_HSEM_SEMID_TO_MASK(7),
    MCU_HSEM_SEM_CH_8   = __HAL_HSEM_SEMID_TO_MASK(8),
    MCU_HSEM_SEM_CH_9   = __HAL_HSEM_SEMID_TO_MASK(9),
    MCU_HSEM_SEM_CH_10  = __HAL_HSEM_SEMID_TO_MASK(10),
    MCU_HSEM_SEM_CH_11  = __HAL_HSEM_SEMID_TO_MASK(11),
    MCU_HSEM_SEM_CH_12  = __HAL_HSEM_SEMID_TO_MASK(12),
    MCU_HSEM_SEM_CH_13  = __HAL_HSEM_SEMID_TO_MASK(13),
    MCU_HSEM_SEM_CH_14  = __HAL_HSEM_SEMID_TO_MASK(14),
    MCU_HSEM_SEM_CH_15  = __HAL_HSEM_SEMID_TO_MASK(15),
    MCU_HSEM_SEM_CH_16  = __HAL_HSEM_SEMID_TO_MASK(16),
    MCU_HSEM_SEM_CH_17  = __HAL_HSEM_SEMID_TO_MASK(17),
    MCU_HSEM_SEM_CH_18  = __HAL_HSEM_SEMID_TO_MASK(18),
    MCU_HSEM_SEM_CH_19  = __HAL_HSEM_SEMID_TO_MASK(19),
    MCU_HSEM_SEM_CH_20  = __HAL_HSEM_SEMID_TO_MASK(20),
    MCU_HSEM_SEM_CH_21  = __HAL_HSEM_SEMID_TO_MASK(21),
    MCU_HSEM_SEM_CH_22  = __HAL_HSEM_SEMID_TO_MASK(22),
    MCU_HSEM_SEM_CH_23  = __HAL_HSEM_SEMID_TO_MASK(23),
    MCU_HSEM_SEM_CH_24  = __HAL_HSEM_SEMID_TO_MASK(24),
    MCU_HSEM_SEM_CH_25  = __HAL_HSEM_SEMID_TO_MASK(25),
    MCU_HSEM_SEM_CH_26  = __HAL_HSEM_SEMID_TO_MASK(26),
    MCU_HSEM_SEM_CH_27  = __HAL_HSEM_SEMID_TO_MASK(27),
    MCU_HSEM_SEM_CH_28  = __HAL_HSEM_SEMID_TO_MASK(28),
    MCU_HSEM_SEM_CH_29  = __HAL_HSEM_SEMID_TO_MASK(29),
    MCU_HSEM_SEM_CH_30  = __HAL_HSEM_SEMID_TO_MASK(30),
    MCU_HSEM_SEM_CH_31  = __HAL_HSEM_SEMID_TO_MASK(31),
}MCU_HSEM_SEM_CH;

//硬件信号量ID
typedef enum MCU_HSEM_SEM_ID
{
    MCU_HSEM_SEM_ID_0   = 0,
    MCU_HSEM_SEM_ID_1   = 1,
    MCU_HSEM_SEM_ID_2   = 2,
    MCU_HSEM_SEM_ID_3   = 3,
    MCU_HSEM_SEM_ID_4   = 4,
    MCU_HSEM_SEM_ID_5   = 5,
    MCU_HSEM_SEM_ID_6   = 6,
    MCU_HSEM_SEM_ID_7   = 7,
    MCU_HSEM_SEM_ID_8   = 8,
    MCU_HSEM_SEM_ID_9   = 9,
    MCU_HSEM_SEM_ID_10  = 10,
    MCU_HSEM_SEM_ID_11  = 11,
    MCU_HSEM_SEM_ID_12  = 12,
    MCU_HSEM_SEM_ID_13  = 13,
    MCU_HSEM_SEM_ID_14  = 14,
    MCU_HSEM_SEM_ID_15  = 15,
    MCU_HSEM_SEM_ID_16  = 16,
    MCU_HSEM_SEM_ID_17  = 17,
    MCU_HSEM_SEM_ID_18  = 18,
    MCU_HSEM_SEM_ID_19  = 19,
    MCU_HSEM_SEM_ID_20  = 20,
    MCU_HSEM_SEM_ID_21  = 21,
    MCU_HSEM_SEM_ID_22  = 22,
    MCU_HSEM_SEM_ID_23  = 23,
    MCU_HSEM_SEM_ID_24  = 24,
    MCU_HSEM_SEM_ID_25  = 25,
    MCU_HSEM_SEM_ID_26  = 26,
    MCU_HSEM_SEM_ID_27  = 27,
    MCU_HSEM_SEM_ID_28  = 28,
    MCU_HSEM_SEM_ID_29  = 29,
    MCU_HSEM_SEM_ID_30  = 30,
    MCU_HSEM_SEM_ID_31  = 31,
}MCU_HSEM_SEM_ID;

//信号量上锁解锁结果
typedef enum MCU_HSEM_RESULT
{
    MCU_HSEM_RESULT_OK     = 0X00,//上锁成功
    MCU_HSEM_RESULT_FAIL   = 0X01,//上锁失败
}MCU_HSEM_RESULT;

//信号量当前状态
typedef enum MCU_HSEM_STATE
{
    MCU_HSEM_STATE_UNLOCK   = 0,//解锁状态
    MCU_HSEM_STATE_LOCK     = 1,//锁定状态
}MCU_HSEM_STATE;

//通道释放回调函数
typedef void (*MCU_HSEM_SemChannelFreeCallBack)(void);

//硬件信号量复位
void MCU_HSEM_DeInit(void);

//初始化硬件信号量
void MCU_HSEM_Init(void);

//信号量上锁,可以设置上锁线程ID
MCU_HSEM_RESULT MCU_HSEM_LockDetail(MCU_HSEM_SEM_ID semID,uint8_t processID);

//信号量上锁,线程ID默认设置为0
MCU_HSEM_RESULT MCU_HSEM_LockSample(MCU_HSEM_SEM_ID semID);

//信号量解锁,可以设置解锁线程ID
MCU_HSEM_RESULT MCU_HSEM_UnLockDetail(MCU_HSEM_SEM_ID semID,uint8_t processID);

//信号量解锁,线程ID默认设置为0
MCU_HSEM_RESULT MCU_HSEM_UnLockSample(MCU_HSEM_SEM_ID semID);

//清空全部的信号量,当前核心占用的
void MCU_HSEM_UnLockAllSelfCore(void);

//查询信号量当前状态
MCU_HSEM_STATE MCU_HSEM_QueryState(MCU_HSEM_SEM_ID semID);

//注册释放事件
void MCU_HSEM_RegisterEventCallBack(MCU_HSEM_SEM_ID semID,MCU_HSEM_SemChannelFreeCallBack callBackFunc);

//解除注册释放事件
void MCU_HSEM_UnRegisterEventCallBack(MCU_HSEM_SEM_ID semID);

#endif








