/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-10-30 10:08:12 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-10-31 18:17:05 +0800
************************************************************************************************/ 
#ifndef __BOARD_CPLD_H_
#define __BOARD_CPLD_H_
#include "BoardCPLD_Config.h"

//CPLD复位
void BoardCPLD_ChipReset(void);

//CPLD写入单个IO
void BoardCPLD_WriteOutputPin(BOARD_CPLD_OUT_PIN outPin,GPIO_PinState bitValue);

//CPLD读取单个IO
GPIO_PinState BoardCPLD_ReadInputPin(BOARD_CPLD_IN_PIN inPin);

//CPLD初始化,带复位
ERROR_SUB BoardCPLD_Init(BOARD_CPLD_MotorIntProc callBack);

//CPLD 立即急停,在触发急停时候发送,同时,在脉冲发送完成之后的中断中也要发送
void BoardCPLD_StepMotorEmergencyStop(BOARD_CPLD_MOTOR index);

//CPLD立即进入下一步,减速时候需要用
void BoardCPLD_WriteStepMotorRunNextStep(BOARD_CPLD_MOTOR index);

//CPLD写入一次电机数据
void BoardCPLD_WriteStepMotorSingle(BOARD_CPLD_MOTOR index,BOARD_CPLD_MOTOR_EN enable,BOARD_CPLD_MOTOR_DIR dir,
                                        BOARD_CPLD_MOTOR_STOP stopFlag,float rpm,uint8_t div,uint8_t stepCount);



#endif














