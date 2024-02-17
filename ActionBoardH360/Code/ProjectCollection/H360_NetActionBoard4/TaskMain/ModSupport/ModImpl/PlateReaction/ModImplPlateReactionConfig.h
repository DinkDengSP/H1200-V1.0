/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-11-24 15:47:08 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-12-13 14:17:36 +0800
************************************************************************************************/ 
#ifndef __MOD_IMPL_PLATE_REACTION_CONFIG_H_
#define __MOD_IMPL_PLATE_REACTION_CONFIG_H_
#include "ModImplBase.h"

//本机ID
//反应盘旋转电机编号
#define PLATE_REACTION_STEP_MOTOR_ROTATE_LOCAL_ID           STEP_MOTOR_NO9

//系统ID
#define PLATE_REACTION_STEP_MOTOR_ROTATE_SYS_ID             H360_SYS_BOARD4_SM9

//反应盘转一格的偏移步数
#define PLATE_REACTION_ONE_HOLE_OFFSET_STEPS                480

//反应盘复位之后的杯位序号
#define PLATE_REACTION_INIT_CUP_INDEX                       31
//反应盘最大杯位序号
#define PLATE_REACTION_MAX_CUP_INDEX                        120

/*不同的移动杯位数量,反应盘的运行曲线 */
#define S_CURVE_PLATE_REACTION_ROTATE_DEBUG                 SM_CURVE_0//反应盘旋转调试曲线
#define S_CURVE_PLATE_REACTION_ROTATE_MOVE_9_CUP            SM_CURVE_1//反应盘运转9个杯位的速度曲线
#define S_CURVE_PLATE_REACTION_ROTATE_MOVE_13_CUP           SM_CURVE_2//反应盘运转13个杯位的速度曲线
#define S_CURVE_PLATE_REACTION_ROTATE_MOVE_40_CUP           SM_CURVE_3//反应盘运转40个杯位速度曲线
#define S_CURVE_PLATE_REACTION_ROTATE_MOVE_59_CUP           SM_CURVE_4//反应盘运转59个杯位速度曲线
#define S_CURVE_PLATE_REACTION_ROTATE_MOVE_120_CUP          SM_CURVE_5//反应盘运转120个杯位速度曲线
#define S_CURVE_PLATE_REACTION_ROTATE_MOVE_1_CUP            SM_CURVE_6//反应盘运转1个杯位速度曲线

//反应盘光电等待稳定时间
#define TIME_MS_PLATE_REACTION_WAIT_LIGHT_STABLE             30

//旋转电机原点
#define INPUT_PLATE_REACTION_ROTATE_ORGIN                    H360_SYS_PORT_IN_BOARD4_ARM_SEN9_PE2
#define VALID_LEVEL_INPUT_PLATE_REACTION_ROTATE_ORGIN        GPIO_PIN_RESET

//反应盘码盘
#define INPUT_PLATE_REACTION_ENCODER                         H360_SYS_PORT_IN_BOARD4_ARM_SEN14_PB9
#define VALID_LEVEL_INPUT_PLATE_REACTION_ENCODER             GPIO_PIN_RESET

//新杯检测有无光纤传感器信号
#define INPUT_PLATE_REACTION_NEW_CUP_LIGHT                   H360_SYS_PORT_IN_BOARD4_ARM_SEN8_PE3
//有效电平,也就是有杯电平
#define VALID_LEVEL_INPUT_PLATE_REACTION_NEW_CUP_LIGHT       GPIO_PIN_SET

//旧杯检测有无光纤传感器信号
#define INPUT_PLATE_REACTION_OLD_CUP_LIGHT                   H360_SYS_PORT_IN_BOARD3_ARM_SEN13_PI4
//有效电平,也就是有杯电平
#define VALID_LEVEL_INPUT_PLATE_REACTION_OLD_CUP_LIGHT       GPIO_PIN_SET





#endif

