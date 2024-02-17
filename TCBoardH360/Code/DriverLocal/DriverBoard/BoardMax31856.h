/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-11-13 11:21:37 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-11-13 16:00:27 +0800
************************************************************************************************/ 
#ifndef __BOARD_MAX_31856_H_
#define __BOARD_MAX_31856_H_
#include "BoardMax31856_Config.h"


//多通道MAX31856初始化
void BoardMax31856PortInit(void);

//设置板上指定MAX31856的初始化配置
ERROR_SUB BoardMax31856SetConfigDefault(CH_MAX31856 ch);

//获取DRDY状态
PIN_STATE BoardMax31856_GetDataRedayState(CH_MAX31856 ch);

//获取FAULT状态
PIN_STATE BoardMax31856_GetDeviceFaultState(CH_MAX31856 ch);

//清除指定芯片的错误
ERROR_SUB BoardMax31856_ClearChipError(CH_MAX31856 ch);

//设置指定通道MAX31856的冷端温度,也就是环境温度,从DS18B20中获取
ERROR_SUB BoardMax31856SetCoolTemp(CH_MAX31856 ch,float environmentTemp);

//读取当前冷端温度与测量温度,返回当前错误代码数据,如果为0,代表无错误
ERROR_SUB BoardMax31856ReadCurentTemp(CH_MAX31856 ch,float* sensorTemperature,uint8_t* chipFaultPtr);

//读取错误状态
ERROR_SUB BoardMax31856FaultState(CH_MAX31856 ch,uint8_t* chipFaultPtr);



#endif

