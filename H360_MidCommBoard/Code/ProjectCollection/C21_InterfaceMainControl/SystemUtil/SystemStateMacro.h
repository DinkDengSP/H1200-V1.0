/************************************************************************ 
 ** Author       : 邓小俊
 ** Date         : 2024-01-29 11:51:45
 ** Description  : When I Has Free Time ,I Will Write Description Below.
 ** description  : 
 ** LastEditors  : 邓小俊
 ** LastEditTime : 2024-01-31 14:05:03
 ** Email: xiaojun.deng@aliyun.com
 ** Copyright (c) 2024 by 公司名称, All Rights Reserved. 
 ************************************************************************/
#ifndef __SYSTEM_STATE_MACRO_
#define __SYSTEM_STATE_MACRO_
#include "DriverHeaderBoard.h"

/********************************数据类型宏定义**********************************/
typedef enum CLIENT_CONNECT
{
    CLIENT_CONNECT_RESET = 0X00,//未联机
    CLIENT_CONNECT_SET   = 0X01,//已联机
}CLIENT_CONNECT;

//运行模式
typedef enum MODE_RUNNING
{
    MODE_DEBUG   = 0X00,//老化模式
    MODE_RELEASE = 0X01,//正常测试模式
}MODE_RUNNING;




#endif


