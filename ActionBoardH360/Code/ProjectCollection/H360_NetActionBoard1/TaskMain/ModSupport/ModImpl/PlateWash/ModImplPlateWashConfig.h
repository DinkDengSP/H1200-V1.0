/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-11-24 15:09:16 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-12-05 12:14:52 +0800
************************************************************************************************/ 
#ifndef __MOD_IMPL_PLATE_WASH_CONFIG_H_
#define __MOD_IMPL_PLATE_WASH_CONFIG_H_
#include "ModImplBase.h"

//本机ID
//清洗盘旋转电机编号
#define PLATE_WASH_STEP_MOTOR_ROTATE_LOCAL_ID           STEP_MOTOR_NO11
//系统ID       
//清洗盘旋转电机编号
#define PLATE_WASH_STEP_MOTOR_ROTATE_SYS_ID             H360_SYS_BOARD1_SM11 

//清洗盘旋转一格步数
#define PLATE_WASH_ONE_HOLE_STEPS                       640

//清洗盘复位之后的杯位序号
#define PLATE_WASH_INIT_CUP_INDEX                       1
//清洗盘最大杯位序号
#define PLATE_WASH_MAX_CUP_INDEX                        24

/*不同的移动杯位数量,清洗盘的运行曲线 */
#define S_CURVE_PLATE_WASH_ROTATE_DEBUG                 SM_CURVE_0//清洗盘旋转调试曲线
#define S_CURVE_PLATE_WASH_ROTATE_MOVE_1_CUP            SM_CURVE_1//清洗盘运转1个杯位的速度曲线
#define S_CURVE_PLATE_WASH_ROTATE_MOVE_4_CUP            SM_CURVE_2//清洗盘运转4个杯位的速度曲线
#define S_CURVE_PLATE_WASH_ROTATE_MOVE_8_CUP            SM_CURVE_3//清洗盘运转8个杯位速度曲线
#define S_CURVE_PLATE_WASH_ROTATE_MOVE_12_CUP           SM_CURVE_4//清洗盘运转12个杯位速度曲线
#define S_CURVE_PLATE_WASH_ROTATE_MOVE_16_CUP           SM_CURVE_5//清洗盘运转16个杯位速度曲线
#define S_CURVE_PLATE_WASH_ROTATE_MOVE_20_CUP           SM_CURVE_6//清洗盘运转20个杯位速度曲线
#define S_CURVE_PLATE_WASH_ROTATE_MOVE_24_CUP           SM_CURVE_7//清洗盘运转24个杯位速度曲线

//清洗盘光电等待稳定时间
#define TIME_MS_PLATE_WASH_WAIT_LIGHT_STABLE             30

//旋转电机原点
#define INPUT_PLATE_WASH_ROTATE_ORGIN                    H360_SYS_PORT_IN_BOARD1_ARM_SEN11_PI6
#define VALID_LEVEL_INPUT_PLATE_WASH_ROTATE_ORGIN        GPIO_PIN_RESET

//新杯检测有无光纤传感器信号
#define INPUT_PLATE_WASH_NEW_CUP_LIGHT                   H360_SYS_PORT_IN_BOARD1_ARM_SEN13_PI4
//有效电平,也就是有杯电平
#define VALID_LEVEL_INPUT_PLATE_WASH_NEW_CUP_LIGHT       GPIO_PIN_SET


#endif



