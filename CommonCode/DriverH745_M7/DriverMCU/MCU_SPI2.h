/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-09-15 09:57:20 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-09-15 14:40:14 +0800
************************************************************************************************/ 
#ifndef __MCU_SPI2_H_
#define __MCU_SPI2_H_
#include "DriverHeaderCore.h"

//SPI2初始化
void MCU_SPI2_Init(MCU_SPI_123_CLK speed,MCU_SPI_CPOL cpol,MCU_SPI_CPHA cpha);

//SPI2 通讯参数重新设置
void MCU_SPI2_ResetParam(MCU_SPI_123_CLK speed,MCU_SPI_CPOL cpol,MCU_SPI_CPHA cpha);

//锁定SPI2
void MCU_SPI2_GetLock(void);

//解锁SPI
void MCU_SPI2_ReleaseLock(void);

//SPI2设定传输速度
void MCU_SPI2_SetSpeed(MCU_SPI_123_CLK speed);

//SPI2读写数据
ERROR_SUB MCU_SPI2_ReadWriteBuffer(uint8_t* writeDataPtr,uint8_t* readDataPtr,uint16_t dataLength);




#endif











