/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-11-24 15:09:16 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-12-04 19:14:54 +0800
************************************************************************************************/ 
#ifndef __MOD_IMPL_LIQUID_BUFFER_CONFIG_H_
#define __MOD_IMPL_LIQUID_BUFFER_CONFIG_H_
#include "ModImplBase.h"

//缓冲液注液电机编号
//本机ID
#define BUFFER_INJECT_STEP_MOTOR_PUMP_LOCAL_ID              STEP_MOTOR_NO10
//系统ID        
#define BUFFER_INJECT_STEP_MOTOR_PUMP_SYS_ID                H360_SYS_BOARD1_SM10

//注射泵原点
#define INPUT_BUFFER_INJECT_PUMP_ORGIN                      H360_SYS_PORT_IN_BOARD1_ARM_SEN10_PI7
#define VALID_LEVEL_INPUT_BUFFER_INJECT_PUMP_ORGIN          GPIO_PIN_SET


/*不同的模式下,缓冲液注液的运行曲线 */
#define S_CURVE_BUFFER_INJECT_PUMP_DEBUG                    SM_CURVE_0//缓冲液注液调试曲线
#define S_CURVE_BUFFER_INJECT_PUMP_INJECT                   SM_CURVE_1//缓冲液注液注液曲线
#define S_CURVE_BUFFER_INJECT_PUMP_PRIME                    SM_CURVE_2//缓冲液注液灌注曲线
#define S_CURVE_BUFFER_INJECT_PUMP_SUCK                     SM_CURVE_3//缓冲液注液吸液曲线
#define S_CURVE_BUFFER_INJECT_PUMP_SUCK_BACK                SM_CURVE_4//缓冲液注液回吐曲线

//灌注一次的步数
#define STEPS_FULL_CIRCLE_BUFFER_INJECT_PUMP_PRIME          16000
//注液一次的步数        
#define STEPS_FULL_CIRCLE_BUFFER_INJECT_PUMP                3200
//缓冲液每Ul对应步数        
#define STEPS_LIQUID_A_PUMP_PER_UL                          16

//缓冲液吸液补偿,多吸量     
#define ABSORB_LIQUID_A_COMPENSATE_UL                       10

//等待注液稳定时间      
#define TIME_MS_WAIT_INJECT_STABLE                          200
//吸液等待稳定时间      
#define TIME_MS_WAIT_ABSORB_STABLE                          100
//吸液回吐稳定时间      
#define TIME_MS_WAIT_ABSORB_BACK_STABLE                     50
//吸液切换到注液稳定时间        
#define TIME_MS_ABSORB_SWITCH_TO_INJECT                     50

#endif



