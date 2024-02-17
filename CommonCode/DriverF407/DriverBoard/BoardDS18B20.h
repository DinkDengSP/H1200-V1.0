/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-11-13 11:21:37 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-11-13 13:25:22 +0800
************************************************************************************************/ 
#ifndef __BOARD_DS18B20_H_
#define __BOARD_DS18B20_H_
#include "BoardDS18B20_Config.h"

//板上DS18B20初始化
void BoardDS18B20PortInit(void);

//检测板上DS18B20是否存在
ERROR_SUB BoardDS18B20CheckExist(void);

//读取DS18B20当前温度
ERROR_SUB BoardDS18B20ReadCurrent(float* curentTemp);



#endif












