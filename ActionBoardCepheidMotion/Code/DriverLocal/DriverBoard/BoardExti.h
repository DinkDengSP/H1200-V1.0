/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-10-31 19:05:56 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-10-31 19:07:19 +0800
************************************************************************************************/ 
#ifndef __BOARD_EXTI_H_
#define __BOARD_EXTI_H_
#include "DriverHeaderMCU.h"
#include "BoardPortIn.h"

//板上中断全部初始化
void BoardExtiResetAll(void);

//外部中断注册
ERROR_SUB BoardExtiRegister(BOARD_PORT_IN inPin,MCU_EXTI_MODE trigMode,MCU_Exti_CallBackFuncPtr callBack);

//外部中断解除注册
void BoardExtiUnRegister(BOARD_PORT_IN inPin);


#endif











