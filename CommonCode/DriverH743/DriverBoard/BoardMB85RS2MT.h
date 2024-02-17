/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-08-21 10:19:22 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-08-21 16:31:09 +0800
************************************************************************************************/ 
#ifndef __BOARD_MB85RS2MT_H_
#define __BOARD_MB85RS2MT_H_
#include "BoardMB85RS2MT_Config.h"

//初始化铁电存储器
void BoardMB85RS2MT_PortInit(void);

//铁电存储器自检
ERROR_SUB BoardMB85RS2MT_SelfCheck(void);

//配置铁电存储器默认状态
ERROR_SUB BoardMB85RS2MT_ConfigDefault(void);

//写缓存
ERROR_SUB BoardMB85RS2MT_WriteBuffer(uint32_t address, uint8_t *pdata, uint32_t length);

//读缓存
ERROR_SUB BoardMB85RS2MT_ReadBuffer(uint32_t address, uint8_t *pdata, uint32_t length);

//芯片低级格式化
ERROR_SUB BoardMB85RS2MT_EraseChip(void);


#endif



