/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-11-22 09:01:17 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-12-18 15:32:12 +0800
************************************************************************************************/ 
#ifndef __MOD_IMPL_HAND_NEW_CUP_CONFIG_H_
#define __MOD_IMPL_HAND_NEW_CUP_CONFIG_H_
#include "ModImplBase.h"

/***********************************************步进电机配置****************************************************/
//本机电机配置
//新杯机械手前后运转电机编号                
#define HAND_NEW_CUP_STEP_MOTOR_BACKFRONT_LOCAL_ID                  STEP_MOTOR_NO5
//新杯机械手左右运转电机编号
#define HAND_NEW_CUP_STEP_MOTOR_LEFTRIGHT_LOCAL_ID                  STEP_MOTOR_NO4
//新杯机械手升降运转电机编号                
#define HAND_NEW_CUP_STEP_MOTOR_UPDOWN_LOCAL_ID                     STEP_MOTOR_NO6


//系统电机配置      
//新杯机械手前后运转电机编号                
#define HAND_NEW_CUP_STEP_MOTOR_BACKFRONT_SYS_ID                    H360_SYS_BOARD5_SM5
//新杯机械手左右运转电机编号        
#define HAND_NEW_CUP_STEP_MOTOR_LEFTRIGHT_SYS_ID                    H360_SYS_BOARD5_SM4
//新杯机械手升降运转电机编号                
#define HAND_NEW_CUP_STEP_MOTOR_UPDOWN_SYS_ID                       H360_SYS_BOARD5_SM6


/**************************************************输入输出配置**********************************************/
//前后电机原点
#define INPUT_HAND_NEW_CUP_BACKFRONT_ORGIN                          H360_SYS_PORT_IN_BOARD5_ARM_SEN5_PE6
#define VALID_LEVEL_INPUT_HAND_NEW_CUP_BACKFRONT_ORGIN              GPIO_PIN_RESET

//左右电机原点      
#define INPUT_HAND_NEW_CUP_LEFTRIGHT_ORGIN                          H360_SYS_PORT_IN_BOARD5_ARM_SEN4_PI8
#define VALID_LEVEL_INPUT_HAND_NEW_CUP_LEFTRIGHT_ORGIN              GPIO_PIN_RESET

//升降电机原点      
#define INPUT_HAND_NEW_CUP_UPDOWN_ORGIN                             H360_SYS_PORT_IN_BOARD5_ARM_SEN6_PE5
#define VALID_LEVEL_INPUT_HAND_NEW_CUP_UPDOWN_ORGIN                 GPIO_PIN_RESET

//新杯检测有无光纤传感器信号
#define INPUT_PLATE_REACTION_NEW_CUP_LIGHT                          H360_SYS_PORT_IN_BOARD4_ARM_SEN8_PE3
//有效电平,也就是有杯电平       
#define VALID_LEVEL_INPUT_PLATE_REACTION_NEW_CUP_LIGHT              GPIO_PIN_SET

//反应杯放杯电磁铁
#define OUTPUT_HAND_NEW_CUP_ELECTRO_MAGNET                          H360_SYS_PORT_OUT_BOARD5_SLAVE_DC1_VAVLE_PC8
//反应杯放杯电磁铁有效电平,也就是合上的电平
#define VALID_LEVEL_OUTPUT_HAND_NEW_CUP_ELECTRO_MAGNET              GPIO_PIN_RESET

//新杯盘行列号最大值配置
#define HOLE_LEFT_RIGHT_MAX                                         10
#define HOLE_BACK_FRONT_MAX                                         14

//读反应盘光电传感器次数    
#define READ_HAND_NEW_CUP_EXIST_SENSOR_REACT                        3
//反应盘光电信号有效阈值    
#define HAND_NEW_CUP_EXIST_SENSOR_VALID_THRESHOLD_REACT             2
//反应盘光电信号稳定延时    
#define TIME_MS_DELAY_READ_HAND_NEW_CUP_EXIST_SENSOR_REACT          1
/****************************************电机运行曲线配置***************************************************/
/*不同的目标位置,新杯机械手前后的运转曲线 */
#define S_CURVE_HAND_NEW_CUP_BACKFRONT_DEBUG                                SM_CURVE_0//调试曲线
#define S_CURVE_HAND_NEW_CUP_BACKFRONT_NEW_TRAY_TO_PLATE_REACTION           SM_CURVE_1//新杯区到反应盘速度曲线
#define S_CURVE_HAND_NEW_CUP_BACKFRONT_NEW_TRAY_TO_GARBAGE1                 SM_CURVE_2//新杯区到垃圾桶1速度曲线
#define S_CURVE_HAND_NEW_CUP_BACKFRONT_NEW_TRAY_TO_GARBAGE2                 SM_CURVE_3//新杯区到垃圾桶2速度曲线
#define S_CURVE_HAND_NEW_CUP_BACKFRONT_GARBAGE1_TO_PLATE_REACTION           SM_CURVE_4//垃圾桶1到反应盘速度曲线
#define S_CURVE_HAND_NEW_CUP_BACKFRONT_GARBAGE1_TO_NEW_TRAY                 SM_CURVE_5//垃圾桶1到新杯区速度曲线
#define S_CURVE_HAND_NEW_CUP_BACKFRONT_GARBAGE1_TO_GARBAGE2                 SM_CURVE_6//垃圾桶1到垃圾桶2速度曲线
#define S_CURVE_HAND_NEW_CUP_BACKFRONT_GARBAGE2_TO_PLATE_REACTION           SM_CURVE_7//垃圾桶2到反应盘速度曲线
#define S_CURVE_HAND_NEW_CUP_BACKFRONT_GARBAGE2_TO_NEW_TRAY                 SM_CURVE_8//垃圾桶2到新杯区速度曲线
#define S_CURVE_HAND_NEW_CUP_BACKFRONT_GARBAGE2_TO_NEW_GARBAGE1             SM_CURVE_9//垃圾桶2到垃圾桶1速度曲线
#define S_CURVE_HAND_NEW_CUP_BACKFRONT_PLATE_REACTION_TO_NEW_TRAY           SM_CURVE_10//反应盘到新杯区速度曲线
#define S_CURVE_HAND_NEW_CUP_BACKFRONT_PLATE_REACTION_TO_GARBAGE1           SM_CURVE_11//反应盘到垃圾桶1速度曲线
#define S_CURVE_HAND_NEW_CUP_BACKFRONT_PLATE_REACTION_TO_GARBAGE2           SM_CURVE_12//反应盘到垃圾桶2速度曲线

/*不同的目标位置,新杯机械手左右的运转曲线 */
#define S_CURVE_HAND_NEW_CUP_LEFTRIGHT_DEBUG                                SM_CURVE_0//调试曲线
#define S_CURVE_HAND_NEW_CUP_LEFTRIGHT_NEW_TRAY_TO_PLATE_REACTION           SM_CURVE_1//新杯区到反应盘速度曲线
#define S_CURVE_HAND_NEW_CUP_LEFTRIGHT_NEW_TRAY_TO_GARBAGE1                 SM_CURVE_2//新杯区到垃圾桶1速度曲线
#define S_CURVE_HAND_NEW_CUP_LEFTRIGHT_NEW_TRAY_TO_GARBAGE2                 SM_CURVE_3//新杯区到垃圾桶2速度曲线
#define S_CURVE_HAND_NEW_CUP_LEFTRIGHT_GARBAGE1_TO_PLATE_REACTION           SM_CURVE_4//垃圾桶1到反应盘速度曲线
#define S_CURVE_HAND_NEW_CUP_LEFTRIGHT_GARBAGE1_TO_NEW_TRAY                 SM_CURVE_5//垃圾桶1到新杯区速度曲线
#define S_CURVE_HAND_NEW_CUP_LEFTRIGHT_GARBAGE1_TO_GARBAGE2                 SM_CURVE_6//垃圾桶1到垃圾桶2速度曲线
#define S_CURVE_HAND_NEW_CUP_LEFTRIGHT_GARBAGE2_TO_PLATE_REACTION           SM_CURVE_7//垃圾桶2到反应盘速度曲线
#define S_CURVE_HAND_NEW_CUP_LEFTRIGHT_GARBAGE2_TO_NEW_TRAY                 SM_CURVE_8//垃圾桶2到新杯区速度曲线
#define S_CURVE_HAND_NEW_CUP_LEFTRIGHT_GARBAGE2_TO_NEW_GARBAGE1             SM_CURVE_9//垃圾桶2到垃圾桶1速度曲线
#define S_CURVE_HAND_NEW_CUP_LEFTRIGHT_PLATE_REACTION_TO_NEW_TRAY           SM_CURVE_10//反应盘到新杯区速度曲线
#define S_CURVE_HAND_NEW_CUP_LEFTRIGHT_PLATE_REACTION_TO_GARBAGE1           SM_CURVE_11//反应盘到垃圾桶1速度曲线
#define S_CURVE_HAND_NEW_CUP_LEFTRIGHT_PLATE_REACTION_TO_GARBAGE2           SM_CURVE_12//反应盘到垃圾桶2速度曲线

/*不同的运行模式,新杯机械手升降的运行曲线*/
#define S_CURVE_HAND_NEW_CUP_UPDOWN_DEBUG                                   SM_CURVE_0//调试曲线
#define S_CURVE_HAND_NEW_CUP_UPDOWN_CATCH_NEW_TRAY_DOWN_FAST                SM_CURVE_1//机械手新杯区取杯,升降快速下降曲线
#define S_CURVE_HAND_NEW_CUP_UPDOWN_CATCH_NEW_TRAY_DOWN_SLOW                SM_CURVE_2//机械手新杯区取杯,升降慢速下降曲线
#define S_CURVE_HAND_NEW_CUP_UPDOWN_CATCH_NEW_TRAY_UP_FAST                  SM_CURVE_3//机械手新杯区取杯,升降快速上升曲线
#define S_CURVE_HAND_NEW_CUP_UPDOWN_CATCH_NEW_TRAY_UP_SLOW                  SM_CURVE_4//机械手新杯区取杯,升降慢速上升曲线
#define S_CURVE_HAND_NEW_CUP_UPDOWN_CATCH_PLATE_REACTION_DOWN_FAST          SM_CURVE_5//机械手反应盘取杯,升降快速下降曲线
#define S_CURVE_HAND_NEW_CUP_UPDOWN_CATCH_PLATE_REACTION_DOWN_SLOW          SM_CURVE_6//机械手反应盘取杯,升降慢速下降曲线
#define S_CURVE_HAND_NEW_CUP_UPDOWN_CATCH_PLATE_REACTION_UP_FAST            SM_CURVE_7//机械手反应盘取杯,升降快速上升曲线
#define S_CURVE_HAND_NEW_CUP_UPDOWN_CATCH_PLATE_REACTION_UP_SLOW            SM_CURVE_8//机械手反应盘取杯,升降慢速上升曲线
#define S_CURVE_HAND_NEW_CUP_UPDOWN_PUT_PLATE_REACTION_DOWN_FAST            SM_CURVE_9//机械手反应盘放杯,升降快速下降曲线
#define S_CURVE_HAND_NEW_CUP_UPDOWN_PUT_PLATE_REACTION_DOWN_SLOW            SM_CURVE_10//机械手反应盘放杯,升降慢速下降曲线
#define S_CURVE_HAND_NEW_CUP_UPDOWN_PUT_PLATE_REACTION_UP_FAST              SM_CURVE_11//机械手反应盘放杯,升降快速上升曲线
#define S_CURVE_HAND_NEW_CUP_UPDOWN_PUT_PLATE_REACTION_UP_SLOW              SM_CURVE_12//机械手反应盘放杯,升降慢速上升曲线
#define S_CURVE_HAND_NEW_CUP_UPDOWN_PUT_GARBAGE_DOWN_FAST                   SM_CURVE_13//机械手垃圾桶放杯,升降快速下降曲线
#define S_CURVE_HAND_NEW_CUP_UPDOWN_PUT_GARBAGE_DOWN_SLOW                   SM_CURVE_14//机械手垃圾桶放杯,升降慢速下降曲线
#define S_CURVE_HAND_NEW_CUP_UPDOWN_PUT_GARBAGE_UP_FAST                     SM_CURVE_15//机械手垃圾桶放杯,升降快速上升曲线
#define S_CURVE_HAND_NEW_CUP_UPDOWN_PUT_GARBAGE_UP_SLOW                     SM_CURVE_16//机械手垃圾桶放杯,升降慢速上升曲线

/********************************************运行中等待时间配置*****************************************************/
#define TIME_MS_HAND_NEW_CUP_CATCH_CUP_NEW_TRAY_STABLE                      100//新杯机械手新杯区抓杯等待稳定时间
#define TIME_MS_HAND_NEW_CUP_CATCH_CUP_PLATE_REACTION_STABLE                70//新杯机械手反应盘抓杯等待稳定时间
#define TIME_MS_HAND_NEW_CUP_PUT_CUP_PLATE_REACTION_STABLE                  70//新杯机械手反应盘放杯等待稳定时间
#define TIME_MS_HAND_NEW_CUP_PUT_CUP_GARBAGE_STABLE                         120//新杯机械手垃圾桶放杯等待稳定时间
#define TIME_MS_PLATE_REACTION_ELECTRO_MAGNET_STABLE                        150//反应盘新杯电磁铁稳定时间
#define TIME_MS_PLATE_REACTION_LIGHT_SENSOR_STABLE                          30//反应盘新杯光电信号稳定时间
#define TIME_MS_HAND_NEW_CUP_UPDOWN_SPEED_SWITCH                            0//新杯机械手升降切换速度的等待时间

/**********************************************速度切换点配置******************************************************/
//下降速度切换点,若定义为0,则下降没有速度切换,使用下降快速速度
#define OFFSET_SPEED_SWITCH_UPDOWN_NEW_TRAY_CATCH_DOWN                      120//新杯区取杯升降,下降的速度切换点
#define OFFSET_SPEED_SWITCH_UPDOWN_PLATE_REACTION_CATCH_DOWN                120//反应盘取杯升降,下降的速度切换点
#define OFFSET_SPEED_SWITCH_UPDOWN_PLATE_REACTION_PUT_DOWN                  120//反应盘放杯升降,下降的速度切换点
#define OFFSET_SPEED_SWITCH_UPDOWN_GARBAGE_PUT_DOWN                         0//垃圾桶放杯升降,下降的速度切换点
//上升速度切换点,若定义为0,则上升没有速度切换,使用上升快速速度
#define OFFSET_SPEED_SWITCH_UPDOWN_NEW_TRAY_CATCH_UP                        120//新杯区取杯升降,上升的速度切换点
#define OFFSET_SPEED_SWITCH_UPDOWN_PLATE_REACTION_CATCH_UP                  0//反应盘取杯升降,上升的速度切换点
#define OFFSET_SPEED_SWITCH_UPDOWN_PLATE_REACTION_PUT_UP                    0//反应盘放杯升降,上升的速度切换点
#define OFFSET_SPEED_SWITCH_UPDOWN_GARBAGE_PUT_UP                           0//垃圾桶放杯升降,上升的速度切换点



#endif













