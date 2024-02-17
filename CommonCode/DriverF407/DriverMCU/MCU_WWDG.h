/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-08-23 09:44:56 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-08-25 15:33:10 +0800
************************************************************************************************/ 
#ifndef __MCU_WWDG_H_
#define __MCU_WWDG_H_
#include "DriverHeaderCore.h"
#include "DriverConfigMCU.h"

//窗口看门狗喂狗回调函数
typedef void (*MCU_WWDG_EarlyFeedCallBack)(void);

//WWDG初始化,详细看函数注释
#if(MCU_WWDG_ENABLE_EWI_SET == CONFIG_FUNC_DISABLE)
void MCU_WWDG_Init(void);
#else
void MCU_WWDG_Init(MCU_WWDG_EarlyFeedCallBack callBackPtr);
#endif

//窗口看门狗喂狗
void MCU_WWDG_Feed(void);



#endif






