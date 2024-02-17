/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-09-15 09:57:20 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-09-15 14:37:22 +0800
************************************************************************************************/ 
#ifndef __MCU_WWDG_H_
#define __MCU_WWDG_H_
#include "DriverHeaderCore.h"
#include "DriverConfigMCU.h"

//WWDG初始化,详细看函数注释
#if(MCU_WWDG_ENABLE_EWI_SET == CONFIG_FUNC_DISABLE)
void MCU_WWDG_Init(void);
#else
//窗口看门狗喂狗回调函数
typedef void (*MCU_WWDG_EarlyFeedCallBack)(void);
void MCU_WWDG_Init(MCU_WWDG_EarlyFeedCallBack callBackPtr);
#endif

//窗口看门狗喂狗
void MCU_WWDG_Feed(void);


#endif














