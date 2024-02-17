/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-09-15 09:57:20 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-09-15 10:49:58 +0800
************************************************************************************************/ 
#ifndef __MCU_IIC1_H_
#define __MCU_IIC1_H_
#include "DriverHeaderCore.h"

//加锁
void MCU_IIC1_GetLock(void);

//释放锁
void MCU_IIC1_ReleaseLock(void);

//IIC端口初始化
void MCU_IIC1_PortInit(void);

//检查设备是否准备好
HAL_StatusTypeDef MCU_IIC1_CheckDeviceReady(uint8_t deviceAddr);

//数据写入
HAL_StatusTypeDef MCU_IIC1_WriteBytesWithAddr(uint8_t deviceAddr,IIC_SUB_ADDR_SIZE subAddrSize,uint16_t memAddr,
                                                    uint8_t* dataBuffer,uint8_t dataLength,uint8_t singleByteTimeOut);

//数据读取
HAL_StatusTypeDef MCU_IIC1_ReadBytesWithAddr(uint8_t deviceAddr,IIC_SUB_ADDR_SIZE subAddrSize,uint16_t memAddr,
                                                    uint8_t* dataBuffer,uint8_t dataLength,uint8_t singleByteTimeOut);

//数据写入
HAL_StatusTypeDef MCU_IIC1_WriteBytes(uint8_t deviceAddr,uint8_t* dataBuffer,uint8_t dataLength,
                                            uint8_t singleByteTimeOut);

//数据读取
HAL_StatusTypeDef MCU_IIC1_ReadBytes(uint8_t deviceAddr,uint8_t* dataBuffer,uint8_t dataLength,
                                            uint8_t singleByteTimeOut);


#endif










