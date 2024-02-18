/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-08-23 09:44:56 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-08-28 17:31:29 +0800
************************************************************************************************/ 
#ifndef __MCU_FLASH_H_
#define __MCU_FLASH_H_
#include "DriverHeaderCore.h"

//Flash的比较结果
typedef enum MCU_FLASH_CMP_RESULT
{
    MCU_FLASH_CMP_EQU       = 0,//Flash内容和待写入的数据相等，不需要擦除和写操作
    MCU_FLASH_CMP_WRITE     = 1,//Flash不需要擦除，直接写
    MCU_FLASH_CMP_ERASE     = 2,//Flash需要先擦除,再写
    MCU_FLASH_CMP_PARAM_ERR = 3,//函数参数错误
}MCU_FLASH_CMP_RESULT;

//读出字
uint32_t MCU_FlashReadWord(uint32_t addr);	

//从指定地址开始读出指定长度的数据
void MCU_FlashReadBuffer(uint32_t readAddr,uint32_t *bufferPtr,uint32_t wordNumber);

//从指定地址开始读出指定长度的数据
void MCU_FlashReadBufferBytes(uint32_t readAddr,uint8_t *bufferPtr,uint32_t byteCount);

//比较Flash空间和buffer空间的数据是否对应的上,决定该空间是否需要擦除
MCU_FLASH_CMP_RESULT MCU_FlashCompare(uint32_t flashAddr, uint8_t* bufferPtr, uint32_t size); 	

//擦除指定地址的扇区，外部需要关闭中断
ERROR_SUB MCU_FlashEraseSectorByAddr(uint32_t flashAddr);

//擦除使用的FLASH空间，查看是否擦成功
ERROR_SUB MCU_FlashEraseRangeWithCheck(uint32_t startAddr,uint32_t wordNumber);

//从指定地址开始写入指定长度的数据  	  
ERROR_SUB MCU_FlashWrite(uint32_t startAddr,uint32_t *bufferPtr,uint32_t wordNumber);

//MCU Flash Check
ERROR_SUB MCU_FlashCheck(void);

#endif







