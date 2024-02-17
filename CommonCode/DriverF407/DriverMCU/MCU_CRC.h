/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-08-23 09:44:56 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-08-23 10:52:24 +0800
************************************************************************************************/ 
#ifndef __MCU_CRC_H_
#define __MCU_CRC_H_
#include "DriverHeaderCore.h"

//CRC初始化
void MCU_CRC_Init(void);

//CRC计算,计算中会用到上一次的CRC值,可以用作多段式的CRC校验
uint32_t MCU_CRC_CalcWithLastResult(uint32_t* bufferPtr,uint32_t dataLength);

//CRC计算,计算中复位上一次的CRC值,单次CRC用这个函数
uint32_t MCU_CRC_CalcClearLastResult(uint32_t* bufferPtr,uint32_t dataLength);

//软件CRC计算
uint32_t MCU_CRC_SoftCalcClearLastResult(uint32_t* bufferPtr,uint32_t dataLength);

//使能CRC测试
void MCU_CRC_TestDemoRunOnce(void);

//16位CRC计算
uint16_t MCU_Calc2ByteCRC(uint8_t* bufferPtr,uint16_t bufferLength);


#endif





