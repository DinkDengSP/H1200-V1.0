/************************************************************************ 
 ** Author       : 邓小俊
 ** Date         : 2024-01-16 11:35:28
 ** Description  : When I Has Free Time ,I Will Write Description Below.
 ** description  : 
 ** LastEditors  : 邓小俊
 ** LastEditTime : 2024-01-16 13:15:06
 ** Email: xiaojun.deng@aliyun.com
 ** Copyright (c) 2024 by 公司名称, All Rights Reserved. 
 ************************************************************************/
#ifndef __BOARD_IDENTITY_H_
#define __BOARD_IDENTITY_H_
#include "DriverHeaderMCU.h"

//板卡ID为多少的时候强制保持在BOOT空间
#define BOARD_ID_FORCE_BOOT             0

//板卡ID识别
void BoardIdentityPortInit(void);

//获取板卡ID,IO实际获取
uint8_t BoardIdentityPortGet(void);

//获取板卡ID,存储变量获取
uint8_t BoardIdentityGet(void);


#endif

