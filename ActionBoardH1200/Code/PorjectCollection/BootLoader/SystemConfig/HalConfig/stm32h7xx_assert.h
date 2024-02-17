/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-03-10 14:02:02 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-03-10 14:02:22 +0800
************************************************************************************************/ 
#ifndef __STM32H7XX_ASSERT_H_
#define __STM32H7XX_ASSERT_H_
#include "stdint.h"

#ifdef __cplusplus
 extern "C" {
#endif


//#define USE_FULL_ASSERT

//定义了实际的断言函数的实现
#ifdef USE_FULL_ASSERT
//参数表达式断言检测
#define assert_param(expr) ((expr) ? (void)0U : assert_failed((uint8_t *)__FILE__, __LINE__))
//断言失败调用的输出函数
void assert_failed(uint8_t *file, uint32_t line);

//断言定义的空实现,不适用系统断言功能
#else
#define assert_param(expr) ((void)0U)

#endif 



#ifdef __cplusplus
}
#endif















#endif
