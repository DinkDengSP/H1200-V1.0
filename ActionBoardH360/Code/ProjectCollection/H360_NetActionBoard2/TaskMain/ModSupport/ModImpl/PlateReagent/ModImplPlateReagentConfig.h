/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-11-22 09:01:17 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-12-11 14:55:52 +0800
************************************************************************************************/ 
#ifndef __MOD_IMPL_PLATE_REAGENT_CONFIG_H_
#define __MOD_IMPL_PLATE_REAGENT_CONFIG_H_
#include "ModImplBase.h"

//本机ID
//试剂盘旋转电机编号
#define PLATE_REAGENT_STEP_MOTOR_ROTATE_LOCAL_ID            STEP_MOTOR_NO9

//系统ID
//试剂盘旋转电机编号
#define PLATE_REAGENT_STEP_MOTOR_ROTATE_SYS_ID              H360_SYS_BOARD2_SM9

//试剂盘旋转一格的偏移
#define PLATE_REAGENT_ONE_HOLE_OFFSET_STEPS                 800

//旋转电机原点
#define INPUT_PLATE_REAGENT_ROTATE_ORGIN                    H360_SYS_PORT_IN_BOARD2_ARM_SEN9_PE2
#define VALID_LEVEL_INPUT_PLATE_REAGENT_ROTATE_ORGIN        GPIO_PIN_RESET

//试剂盘复位之后的杯位序号
#define PLATE_REAGENT_INIT_CUP_INDEX                        1
//试剂盘复位之后的偏移状态
#define PLATE_REAGENT_INIT_OFFSET                           PLATE_REAGENT_OFFSET_NONE
//试剂盘最大杯位序号
#define PLATE_REAGENT_MAX_CUP_INDEX                         30

/* 试剂盘移动扫码偏移步数 */
#define PLATE_REAGENT_MOVE_SCAN_OFFSET                      300
//试剂盘复位之后的扫码偏移状态
#define PLATE_REAGENT_INIT_SCAN_OFFSET                      PLATE_REAGENT_SCAN_OFFSET_NONE

/*不同的移动杯位数量,试剂盘的运行曲线 */
#define S_CURVE_PLATE_REAGENT_ROTATE_DEBUG                  SM_CURVE_0//试剂盘旋转调试曲线
#define S_CURVE_PLATE_REAGENT_ROTATE_MOVE_1_3_CUP           SM_CURVE_1//试剂盘运转1-3个杯位的速度曲线
#define S_CURVE_PLATE_REAGENT_ROTATE_MOVE_4_6_CUP           SM_CURVE_2//试剂盘运转4-6个杯位的速度曲线
#define S_CURVE_PLATE_REAGENT_ROTATE_MOVE_7_9_CUP           SM_CURVE_3//试剂盘运转7-9个杯位速度曲线
#define S_CURVE_PLATE_REAGENT_ROTATE_MOVE_10_12_CUP         SM_CURVE_4//试剂盘运转10-12个杯位速度曲线
#define S_CURVE_PLATE_REAGENT_ROTATE_MOVE_13_15_CUP         SM_CURVE_5//试剂盘运转13-15个杯位速度曲线
#define S_CURVE_PLATE_REAGENT_ROTATE_MOVE_FOR_BARSCAN       SM_CURVE_12//试剂盘运转扫码速度曲线


#endif












