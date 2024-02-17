/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-10-30 10:08:12 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-10-30 11:12:13 +0800
************************************************************************************************/ 
#ifndef __BOARD_IDENTITY_H_
#define __BOARD_IDENTITY_H_
#include "DriverHeaderMCU.h"

//板卡ID识别
void BoardIdentityPortInit(void);

//获取板卡ID,IO实际获取
uint8_t BoardIdentityPortGet(void);

//获取板卡ID,存储变量获取
uint8_t BoardIdentityGet(void);


#endif















