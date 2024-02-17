/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-11-24 15:47:08 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-12-06 14:28:17 +0800
************************************************************************************************/ 
#ifndef __MOD_IMPL_MIX_REAGENT_CONFIG_H_
#define __MOD_IMPL_MIX_REAGENT_CONFIG_H_
#include "ModImplBase.h"

//本机ID
//试剂混匀升降电机编号
#define MIX_REAGENT_STEP_MOTOR_UPDOWN_LOCAL_ID         STEP_MOTOR_NO3
//试剂混匀旋转电机编号
#define MIX_REAGENT_STEP_MOTOR_ROTATE_LOCAL_ID         STEP_MOTOR_NO4

//系统ID       
//试剂混匀升降电机编号
#define MIX_REAGENT_STEP_MOTOR_UPDOWN_SYS_ID           H360_SYS_BOARD4_SM3 
//试剂混匀旋转电机编号
#define MIX_REAGENT_STEP_MOTOR_ROTATE_SYS_ID           H360_SYS_BOARD4_SM4

//升降原点配置
#define INPUT_MIX_REAGENT_UPDOWN_ORGIN                 H360_SYS_PORT_IN_BOARD4_ARM_SEN3_PC13
#define VALID_LEVEL_INPUT_MIX_REAGENT_UPDOWN_ORGIN     GPIO_PIN_RESET

//旋转原点配置
#define INPUT_MIX_REAGENT_ROTATE_ORGIN                 H360_SYS_PORT_IN_BOARD4_ARM_SEN4_PI8
#define VALID_LEVEL_INPUT_MIX_REAGENT_ROTATE_ORGIN     GPIO_PIN_RESET

//试剂摇匀需要等待反应杯稳定延时时间
#define MIX_REAHENT_NEED_WAIT_CUP_STABLE_MS             700

//不同的摇匀模式对应的不同的旋转速度曲线
#define S_CURVE_MIX_REAGENT_ROTATE_DEBUG                SM_CURVE_0//试剂摇匀旋转调试曲线
#define S_CURVE_MIX_REAGENT_ROTATE_MODE1                SM_CURVE_1//试剂摇匀模式1旋转曲线
#define S_CURVE_MIX_REAGENT_ROTATE_MODE2                SM_CURVE_2//试剂摇匀模式2旋转曲线
#define S_CURVE_MIX_REAGENT_ROTATE_MODE3                SM_CURVE_3//试剂摇匀模式3旋转曲线
#define S_CURVE_MIX_REAGENT_ROTATE_MODE4                SM_CURVE_4//试剂摇匀模式4旋转曲线
#define S_CURVE_MIX_REAGENT_ROTATE_MODE5                SM_CURVE_5//试剂摇匀模式5旋转曲线
#define S_CURVE_MIX_REAGENT_ROTATE_MODE6                SM_CURVE_6//试剂摇匀模式6旋转曲线
#define S_CURVE_MIX_REAGENT_ROTATE_MODE7                SM_CURVE_7//试剂摇匀模式7旋转曲线
#define S_CURVE_MIX_REAGENT_ROTATE_MODE8                SM_CURVE_8//试剂摇匀模式8旋转曲线
#define S_CURVE_MIX_REAGENT_ROTATE_MODE9                SM_CURVE_9//试剂摇匀模式9旋转曲线
#define S_CURVE_MIX_REAGENT_ROTATE_MODE10               SM_CURVE_10//试剂摇匀模式10旋转曲线


//不同的摇匀模式对应的升降速度
#define S_CURVE_MIX_REAGENT_UPDOWN_DEBUG                SM_CURVE_0//调试曲线
#define S_CURVE_MIX_REAGENT_UPDOWN_UP_MODE1             SM_CURVE_1//试剂摇匀升降模式1升起来的速度
#define S_CURVE_MIX_REAGENT_UPDOWN_DOWN_MODE1           SM_CURVE_2//试剂摇匀升降模式1降下去的速度
#define S_CURVE_MIX_REAGENT_UPDOWN_UP_MODE2             SM_CURVE_3//试剂摇匀升降模式2升起来的速度
#define S_CURVE_MIX_REAGENT_UPDOWN_DOWN_MODE2           SM_CURVE_4//试剂摇匀升降模式2降下去的速度
#define S_CURVE_MIX_REAGENT_UPDOWN_UP_MODE3             SM_CURVE_5//试剂摇匀升降模式3升起来的速度
#define S_CURVE_MIX_REAGENT_UPDOWN_DOWN_MODE3           SM_CURVE_6//试剂摇匀升降模式3降下去的速度
#define S_CURVE_MIX_REAGENT_UPDOWN_UP_MODE4             SM_CURVE_7//试剂摇匀升降模式4升起来的速度
#define S_CURVE_MIX_REAGENT_UPDOWN_DOWN_MODE4           SM_CURVE_8//试剂摇匀升降模式4降下去的速度
#define S_CURVE_MIX_REAGENT_UPDOWN_UP_MODE5             SM_CURVE_9//试剂摇匀升降模式5升起来的速度
#define S_CURVE_MIX_REAGENT_UPDOWN_DOWN_MODE5           SM_CURVE_10//试剂摇匀升降模式5降下去的速度
#define S_CURVE_MIX_REAGENT_UPDOWN_UP_MODE6             SM_CURVE_11//试剂摇匀升降模式6升起来的速度
#define S_CURVE_MIX_REAGENT_UPDOWN_DOWN_MODE6           SM_CURVE_12//试剂摇匀升降模式6降下去的速度
#define S_CURVE_MIX_REAGENT_UPDOWN_UP_MODE7             SM_CURVE_13//试剂摇匀升降模式7升起来的速度
#define S_CURVE_MIX_REAGENT_UPDOWN_DOWN_MODE7           SM_CURVE_14//试剂摇匀升降模式7降下去的速度
#define S_CURVE_MIX_REAGENT_UPDOWN_UP_MODE8             SM_CURVE_15//试剂摇匀升降模式8升起来的速度
#define S_CURVE_MIX_REAGENT_UPDOWN_DOWN_MODE8           SM_CURVE_16//试剂摇匀升降模式8降下去的速度
#define S_CURVE_MIX_REAGENT_UPDOWN_UP_MODE9             SM_CURVE_17//试剂摇匀升降模式9升起来的速度
#define S_CURVE_MIX_REAGENT_UPDOWN_DOWN_MODE9           SM_CURVE_18//试剂摇匀升降模式9降下去的速度
#define S_CURVE_MIX_REAGENT_UPDOWN_UP_MODE10            SM_CURVE_19//试剂摇匀升降模式10升起来的速度
#define S_CURVE_MIX_REAGENT_UPDOWN_DOWN_MODE10          SM_CURVE_20//试剂摇匀升降模式10降下去的速度






#endif

