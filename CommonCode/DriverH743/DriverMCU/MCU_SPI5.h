/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-08-21 10:19:22 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-08-21 13:56:28 +0800
************************************************************************************************/ 
#ifndef __MCU_SPI5_H_
#define __MCU_SPI5_H_
#include "DriverHeaderCore.h"

//SPI5初始化
void MCU_SPI5_Init(MCU_SPI_45_CLK speed,MCU_SPI_CPOL cpol,MCU_SPI_CPHA cpha);

//SPI5 通讯参数重新设置
void MCU_SPI5_ResetParam(MCU_SPI_45_CLK speed,MCU_SPI_CPOL cpol,MCU_SPI_CPHA cpha);

//锁定SPI5
void MCU_SPI5_GetLock(void);

//解锁SPI
void MCU_SPI5_ReleaseLock(void);

//SPI5设定传输速度
void MCU_SPI5_SetSpeed(MCU_SPI_45_CLK speed);

//SPI5读写数据
ERROR_SUB MCU_SPI5_ReadWriteBuffer(uint8_t* writeDataPtr,uint8_t* readDataPtr,uint16_t dataLength);




#endif









