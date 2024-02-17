/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-08-23 09:44:56 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-08-23 15:55:32 +0800
************************************************************************************************/ 
#ifndef __MCU_SPI3_H_
#define __MCU_SPI3_H_
#include "DriverHeaderCore.h"

//SPI3初始化
void MCU_SPI3_Init(MCU_SPI_23_CLK speed,MCU_SPI_CPOL cpol,MCU_SPI_CPHA cpha);

//SPI3 通讯参数重新设置
void MCU_SPI3_ResetParam(MCU_SPI_23_CLK speed,MCU_SPI_CPOL cpol,MCU_SPI_CPHA cpha);

//锁定SPI3
void MCU_SPI3_GetLock(void);

//解锁SPI
void MCU_SPI3_ReleaseLock(void);

//SPI3设定传输速度
void MCU_SPI3_SetSpeed(MCU_SPI_23_CLK speed);

//SPI3读写数据
ERROR_SUB MCU_SPI3_ReadWriteBuffer(uint8_t* writeDataPtr,uint8_t* readDataPtr,uint16_t dataLength);


#endif










