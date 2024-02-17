/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-08-28 08:57:19 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-08-28 08:57:56 +0800
************************************************************************************************/ 
#ifndef __BOARD_DC_MOTOR_H_
#define __BOARD_DC_MOTOR_H_
#include "DriverHeaderMCU.h"

//直流电机控制
typedef enum BOARD_DC_MOTOR
{
    BOARD_DC_MOTOR_1     = 0X00,
    BOARD_DC_MOTOR_2     = 0X01,
    BOARD_DC_MOTOR_3     = 0X02,
}BOARD_DC_MOTOR;

//直流电机状态
typedef enum BOARD_DC_MOTOR_STATE
{
    BOARD_DC_MOTOR_STATE_OFF    = 0X00,//直流电机关闭
    BOARD_DC_MOTOR_STATE_CW     = 0X01,//直流电机顺时针运转
    BOARD_DC_MOTOR_STATE_CCW    = 0X02,//直流电机逆时针运转
}BOARD_DC_MOTOR_STATE;


//一键全部关闭
void BoardDCMotorAllOff(void);

//报警设置状态
void BoardDCMotorWrite(BOARD_DC_MOTOR dcMotor,BOARD_DC_MOTOR_STATE setState);


#endif










