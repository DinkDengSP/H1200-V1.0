/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-08-23 09:44:56 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-08-23 10:52:24 +0800
************************************************************************************************/ 
#include "DriverConfigMCU.h"
#include "DriverHeaderCore.h"

/*--------------------------------------CAN波特率配置数组-----------------------------------*/
//CAN总线在不同的速率下的配置
//CAN 时钟频率为 APB 1 = 42 MHz
//brp :波特率分频器.范围:1~1024; tq = brp/PCANCLK = brp/APB1,
//tsjw:重新同步跳跃时间单元.默认为1,取值范围CAN_SJW_1TQ-CAN_SJW_4TQ,计算默认取1
//tbs1:时间段1的时间单元.取值CAN_BS1_1TQ-CAN_BS1_16TQ
//tbs2:时间段2的时间单元,取值CAN_BS2_1TQ-CAN_BS2_8TQ
//波特率 = PLLQ/((tbs1+tbs2+1)*brp);
//根据CIA官方建议,(SJW+BS1)/(SJW+BS1+BS2)
// 速度 > 800K以上时候,采样点 75%  
// 速度 > 500K,采样点 80%
// 速度 <= 500K,采样点 87.5%
/*tqWithSJW*//*tqWithBS1*//*tqWithBS2*//*brpDiv*/
//位时间最大25个时间片,最小4个时间片,不能为了匹配采样点随便搞
const MCU_CAN_BPS_SETTING MCU_CAN_BaudArray[] = {
    {CAN_SJW_4TQ  , CAN_BS1_15TQ  , CAN_BS2_5TQ  ,   2    },//1000K 76.1%
    {CAN_SJW_4TQ  , CAN_BS1_16TQ  , CAN_BS2_4TQ  ,   4    },//500K 80.9%
    {CAN_SJW_4TQ  , CAN_BS1_16TQ  , CAN_BS2_4TQ  ,   8    },//250K 80.9%
    {CAN_SJW_4TQ  , CAN_BS1_16TQ  , CAN_BS2_4TQ  ,   16   },//125K 80.9%
    {CAN_SJW_4TQ  , CAN_BS1_16TQ  , CAN_BS2_4TQ  ,   40   },//50K 80.9%
};








