/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-07-27 10:53:33 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-07-27 10:53:57 +0800
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



