/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-08-21 10:19:22 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-08-21 13:56:28 +0800
************************************************************************************************/ 
#ifndef __MCU_IIC2_H_
#define __MCU_IIC2_H_
#include "DriverHeaderCore.h"

//加锁
void MCU_IIC2_GetLock(void);

//释放锁
void MCU_IIC2_ReleaseLock(void);

//IIC端口初始化
void MCU_IIC2_PortInit(void);

//检查设备是否准备好
HAL_StatusTypeDef MCU_IIC2_CheckDeviceReady(uint8_t deviceAddr);

//数据写入
HAL_StatusTypeDef MCU_IIC2_WriteBytesWithAddr(uint8_t deviceAddr,IIC_SUB_ADDR_SIZE subAddrSize,uint16_t memAddr,
                                                    uint8_t* dataBuffer,uint8_t dataLength,uint8_t singleByteTimeOut);

//数据读取
HAL_StatusTypeDef MCU_IIC2_ReadBytesWithAddr(uint8_t deviceAddr,IIC_SUB_ADDR_SIZE subAddrSize,uint16_t memAddr,
                                                    uint8_t* dataBuffer,uint8_t dataLength,uint8_t singleByteTimeOut);

//数据写入
HAL_StatusTypeDef MCU_IIC2_WriteBytes(uint8_t deviceAddr,uint8_t* dataBuffer,uint8_t dataLength,
                                            uint8_t singleByteTimeOut);

//数据读取
HAL_StatusTypeDef MCU_IIC2_ReadBytes(uint8_t deviceAddr,uint8_t* dataBuffer,uint8_t dataLength,
                                            uint8_t singleByteTimeOut);


#endif









