/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-09-15 09:57:20 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-09-15 14:40:27 +0800
************************************************************************************************/ 
#ifndef __MCU_SPI1_H_
#define __MCU_SPI1_H_
#include "DriverHeaderCore.h"

//SPI1初始化
void MCU_SPI1_Init(MCU_SPI_123_CLK speed,MCU_SPI_CPOL cpol,MCU_SPI_CPHA cpha);

//SPI1 通讯参数重新设置
void MCU_SPI1_ResetParam(MCU_SPI_123_CLK speed,MCU_SPI_CPOL cpol,MCU_SPI_CPHA cpha);

//锁定SPI1
void MCU_SPI1_GetLock(void);

//解锁SPI
void MCU_SPI1_ReleaseLock(void);

//SPI1设定传输速度
void MCU_SPI1_SetSpeed(MCU_SPI_123_CLK speed);

//SPI1读写数据
ERROR_SUB MCU_SPI1_ReadWriteBuffer(uint8_t* writeDataPtr,uint8_t* readDataPtr,uint16_t dataLength);




#endif










