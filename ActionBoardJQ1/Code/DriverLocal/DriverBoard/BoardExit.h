/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-08-21 10:49:58 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-08-25 10:59:43 +0800
************************************************************************************************/ 
#ifndef __BOARD_EXIT_H_
#define __BOARD_EXIT_H_
#include "BoardPortIn.h"

//板上中断全部初始化
void BoardExtiResetAll(void);

//外部中断注册
ERROR_SUB BoardExtiRegister(BOARD_PORT_IN inPin,MCU_EXTI_MODE trigMode,MCU_Exti_CallBackFuncPtr callBack);

//外部中断解除注册
void BoardExtiUnRegister(BOARD_PORT_IN inPin);

#endif













