/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-11-13 11:39:55 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-11-13 11:47:54 +0800
************************************************************************************************/ 
#ifndef __MCU_SPI1_H_
#define __MCU_SPI1_H_
#include "DriverHeaderCore.h"

//SPI1初始化
void MCU_SPI1_Init(MCU_SPI_1456_CLK speed,MCU_SPI_CPOL cpol,MCU_SPI_CPHA cpha);

//SPI1 通讯参数重新设置
void MCU_SPI1_ResetParam(MCU_SPI_1456_CLK speed,MCU_SPI_CPOL cpol,MCU_SPI_CPHA cpha);

//锁定SPI1
void MCU_SPI1_GetLock(void);

//解锁SPI
void MCU_SPI1_ReleaseLock(void);

//SPI1设定传输速度
void MCU_SPI1_SetSpeed(MCU_SPI_1456_CLK speed);

//SPI1读写数据
ERROR_SUB MCU_SPI1_ReadWriteBuffer(uint8_t* writeDataPtr,uint8_t* readDataPtr,uint16_t dataLength);


#endif











