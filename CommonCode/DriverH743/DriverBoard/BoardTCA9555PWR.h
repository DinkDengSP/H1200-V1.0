/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-09-15 09:57:20 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-09-18 16:28:32 +0800
************************************************************************************************/ 
#ifndef __BOARD_TCA9555PWR_H_
#define __BOARD_TCA9555PWR_H_
#include "BoardTCA9555PWR_Config.h"

//初始化
void BoardTCA9555PWR_Init(TX_MUTEX* mutexPtr,uint8_t* mutexName);

//查询设备是否空闲,通过检查器件应答的方式，判断内部写操作是否完成, 一般小于 10ms
//CLK频率为200KHz时,查询次数为30次左右
ERROR_SUB BoardTCA9555PWR_WaitIdle(uint8_t deviceAddr,TX_MUTEX* mutexPtr,uint16_t retryCount);

//设置全部极性
ERROR_SUB BoardTCA9555PWR_SetAllPolarity(uint8_t deviceAddr,TX_MUTEX* mutexPtr,TCA9555_PORT_POLARITY_SET* polaritySetPtr);

//设置单个极性
ERROR_SUB BoardTCA9555PWR_SetSinglePolarity(uint8_t deviceAddr,TX_MUTEX* mutexPtr,TCA9555_PIN pin,TCA9555_PORT_POLARITY polaritySet);

//设置全部方向
ERROR_SUB BoardTCA9555PWR_SetAllDir(uint8_t deviceAddr,TX_MUTEX* mutexPtr,TCA9555_PORT_DIR_SET* dirSetPtr);

//设置单个方向
ERROR_SUB BoardTCA9555PWR_SetSingleDir(uint8_t deviceAddr,TX_MUTEX* mutexPtr,TCA9555_PIN pin,TCA9555_PORT_DIR dirSet);

//写入一组输出
ERROR_SUB BoardTCA9555PWR_WriteAllPortOut(uint8_t deviceAddr,TX_MUTEX* mutexPtr,TCA9555_PORT_VAL_SET* valSetPtr);

//写入单个输出
ERROR_SUB BoardTCA9555PWR_WriteSinglePortOut(uint8_t deviceAddr,TX_MUTEX* mutexPtr,TCA9555_PIN pin,GPIO_PinState valSet);

//读取当前全部输出
ERROR_SUB BoardTCA9555PWR_ReadAllPortOutCurrent(uint8_t deviceAddr,TX_MUTEX* mutexPtr,TCA9555_PORT_VAL_SET* valSetPtr);

//读取当前单个输出
ERROR_SUB BoardTCA9555PWR_ReadSinglePortOutCurrent(uint8_t deviceAddr,TX_MUTEX* mutexPtr,TCA9555_PIN pin,GPIO_PinState* valSet);

//反转指定输出
ERROR_SUB BoardTCA9555PWR_ToogleSinglePortOut(uint8_t deviceAddr,TX_MUTEX* mutexPtr,TCA9555_PIN pin);

//读取一组输出
ERROR_SUB BoardTCA9555PWR_ReadAllPortIn(uint8_t deviceAddr,TX_MUTEX* mutexPtr,TCA9555_PORT_VAL_SET* valSetPtr);

//写入单个输出
ERROR_SUB BoardTCA9555PWR_ReadSinglePortIn(uint8_t deviceAddr,TX_MUTEX* mutexPtr,TCA9555_PIN pin,GPIO_PinState* valPtr);


#endif












