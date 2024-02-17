/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-09-15 09:57:20 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-09-19 13:26:50 +0800
************************************************************************************************/ 
#ifndef __BOARD_AD7172_H_
#define __BOARD_AD7172_H_
#include "BoardAD7172_Config.h"

//端口初始化
void BoardAD7172_PortInit(void);

//AD7172复位,写入64个1
void BoardAD7172_ChipReset(void);

//ID检测
ERROR_SUB BoardAD7172_ReadID(uint16_t* idPtr);



#endif











