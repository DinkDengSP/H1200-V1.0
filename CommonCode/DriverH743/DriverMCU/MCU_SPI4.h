/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-08-21 10:19:22 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-08-21 13:56:28 +0800
************************************************************************************************/ 
#ifndef __MCU_SPI4_H_
#define __MCU_SPI4_H_
#include "DriverHeaderCore.h"

//SPI4初始化
void MCU_SPI4_Init(MCU_SPI_45_CLK speed,MCU_SPI_CPOL cpol,MCU_SPI_CPHA cpha);

//SPI4 通讯参数重新设置
void MCU_SPI4_ResetParam(MCU_SPI_45_CLK speed,MCU_SPI_CPOL cpol,MCU_SPI_CPHA cpha);

//锁定SPI4
void MCU_SPI4_GetLock(void);

//解锁SPI
void MCU_SPI4_ReleaseLock(void);

//SPI4设定传输速度
void MCU_SPI4_SetSpeed(MCU_SPI_45_CLK speed);

//SPI4读写数据
ERROR_SUB MCU_SPI4_ReadWriteBuffer(uint8_t* writeDataPtr,uint8_t* readDataPtr,uint16_t dataLength);




#endif


