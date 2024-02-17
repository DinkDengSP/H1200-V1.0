/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-11-24 15:09:16 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-12-05 09:17:23 +0800
************************************************************************************************/ 
#ifndef __MOD_IMPL_MIX_WASH_CONFIG_H_
#define __MOD_IMPL_MIX_WASH_CONFIG_H_
#include "ModImplBase.h"

//本机ID
//清洗混匀升降电机编号
#define MIX_WASH_STEP_MOTOR_UPDOWN_LOCAL_ID         STEP_MOTOR_NO7
//清洗混匀旋转电机编号
#define MIX_WASH_STEP_MOTOR_ROTATE_LOCAL_ID         STEP_MOTOR_NO8

//系统ID       
//清洗混匀升降电机编号
#define MIX_WASH_STEP_MOTOR_UPDOWN_SYS_ID           H360_SYS_BOARD1_SM7 
//清洗混匀旋转电机编号
#define MIX_WASH_STEP_MOTOR_ROTATE_SYS_ID           H360_SYS_BOARD1_SM8

//升降原点配置
#define INPUT_MIX_WASH_UPDOWN_ORGIN                 H360_SYS_PORT_IN_BOARD1_ARM_SEN7_PE4
#define VALID_LEVEL_INPUT_MIX_WASH_UPDOWN_ORGIN     GPIO_PIN_RESET

//旋转原点配置
#define INPUT_MIX_WASH_ROTATE_ORGIN                 H360_SYS_PORT_IN_BOARD1_ARM_SEN8_PE3
#define VALID_LEVEL_INPUT_MIX_WASH_ROTATE_ORGIN     GPIO_PIN_RESET

//不同的摇匀模式对应的不同的旋转速度曲线
#define S_CURVE_MIX_WASH_ROTATE_DEBUG               SM_CURVE_0//清洗摇匀旋转调试曲线
#define S_CURVE_MIX_WASH_ROTATE_MODE1               SM_CURVE_1//清洗摇匀模式1旋转曲线
#define S_CURVE_MIX_WASH_ROTATE_MODE2               SM_CURVE_2//清洗摇匀模式2旋转曲线
#define S_CURVE_MIX_WASH_ROTATE_MODE3               SM_CURVE_3//清洗摇匀模式3旋转曲线
#define S_CURVE_MIX_WASH_ROTATE_MODE4               SM_CURVE_4//清洗摇匀模式4旋转曲线
#define S_CURVE_MIX_WASH_ROTATE_MODE5               SM_CURVE_5//清洗摇匀模式5旋转曲线
#define S_CURVE_MIX_WASH_ROTATE_MODE6               SM_CURVE_6//清洗摇匀模式6旋转曲线
#define S_CURVE_MIX_WASH_ROTATE_MODE7               SM_CURVE_7//清洗摇匀模式7旋转曲线
#define S_CURVE_MIX_WASH_ROTATE_MODE8               SM_CURVE_8//清洗摇匀模式8旋转曲线
#define S_CURVE_MIX_WASH_ROTATE_MODE9               SM_CURVE_9//清洗摇匀模式9旋转曲线
#define S_CURVE_MIX_WASH_ROTATE_MODE10              SM_CURVE_10//清洗摇匀模式10旋转曲线

//不同的摇匀模式对应的升降速度
#define S_CURVE_MIX_WASH_UPDOWN_DEBUG               SM_CURVE_0//调试曲线
#define S_CURVE_MIX_WASH_UPDOWN_UP_MODE1            SM_CURVE_1//清洗摇匀升降模式1升起来的速度
#define S_CURVE_MIX_WASH_UPDOWN_DOWN_MODE1          SM_CURVE_2//清洗摇匀升降模式1降下去的速度
#define S_CURVE_MIX_WASH_UPDOWN_UP_MODE2            SM_CURVE_3//清洗摇匀升降模式2升起来的速度
#define S_CURVE_MIX_WASH_UPDOWN_DOWN_MODE2          SM_CURVE_4//清洗摇匀升降模式2降下去的速度
#define S_CURVE_MIX_WASH_UPDOWN_UP_MODE3            SM_CURVE_5//清洗摇匀升降模式3升起来的速度
#define S_CURVE_MIX_WASH_UPDOWN_DOWN_MODE3          SM_CURVE_6//清洗摇匀升降模式3降下去的速度
#define S_CURVE_MIX_WASH_UPDOWN_UP_MODE4            SM_CURVE_7//清洗摇匀升降模式4升起来的速度
#define S_CURVE_MIX_WASH_UPDOWN_DOWN_MODE4          SM_CURVE_8//清洗摇匀升降模式4降下去的速度
#define S_CURVE_MIX_WASH_UPDOWN_UP_MODE5            SM_CURVE_9//清洗摇匀升降模式5升起来的速度
#define S_CURVE_MIX_WASH_UPDOWN_DOWN_MODE5          SM_CURVE_10//清洗摇匀升降模式5降下去的速度
#define S_CURVE_MIX_WASH_UPDOWN_UP_MODE6            SM_CURVE_11//清洗摇匀升降模式6升起来的速度
#define S_CURVE_MIX_WASH_UPDOWN_DOWN_MODE6          SM_CURVE_12//清洗摇匀升降模式6降下去的速度
#define S_CURVE_MIX_WASH_UPDOWN_UP_MODE7            SM_CURVE_13//清洗摇匀升降模式7升起来的速度
#define S_CURVE_MIX_WASH_UPDOWN_DOWN_MODE7          SM_CURVE_14//清洗摇匀升降模式7降下去的速度
#define S_CURVE_MIX_WASH_UPDOWN_UP_MODE8            SM_CURVE_15//清洗摇匀升降模式8升起来的速度
#define S_CURVE_MIX_WASH_UPDOWN_DOWN_MODE8          SM_CURVE_16//清洗摇匀升降模式8降下去的速度
#define S_CURVE_MIX_WASH_UPDOWN_UP_MODE9            SM_CURVE_17//清洗摇匀升降模式9升起来的速度
#define S_CURVE_MIX_WASH_UPDOWN_DOWN_MODE9          SM_CURVE_18//清洗摇匀升降模式9降下去的速度
#define S_CURVE_MIX_WASH_UPDOWN_UP_MODE10           SM_CURVE_19//清洗摇匀升降模式10升起来的速度
#define S_CURVE_MIX_WASH_UPDOWN_DOWN_MODE10         SM_CURVE_20//清洗摇匀升降模式10降下去的速度


#endif



