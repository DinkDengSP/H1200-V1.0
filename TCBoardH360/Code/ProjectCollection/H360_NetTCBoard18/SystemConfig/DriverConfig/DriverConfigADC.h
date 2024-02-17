/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-11-21 09:14:50 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-11-21 12:07:10 +0800
************************************************************************************************/ 
#ifndef __DRIVER_CONFIG_ADC_H_
#define __DRIVER_CONFIG_ADC_H_
#include "DriverConfigMCU.h"

/*----------------------------------------------ADC1配置讯息-----------------------------------------------*/
#define MODE_ADC1_POOL                          0X00
#define MODE_ADC1_DMA                           0X01


//ADC转换模式设定
#define MODE_ADC1_USER                          MODE_ADC1_DMA
//是否使能外部触发ADC转换
#define EXTERN_TRIG_MCU_ADC1_USER               CONFIG_FUNC_DISABLE
//是否使能中断讯息打印
#define ADC1_NORMAL_INT_MSG_SHOW                CONFIG_FUNC_DISABLE
//是否使能中断错误讯息打印
#define ADC1_ERR_INT_MSG_SHOW                   CONFIG_FUNC_DISABLE
//是否打开测试用例
#define ADC1_TEST_DEMO_ENABLE                   CONFIG_FUNC_DISABLE
//超时时间
#define TIME_OUT_MS_ADC1_CONVERT                200
//ADC使用的内存空间大小
#define MEM_REGION_MCU_ADC1                     MEM_SRAM_IN
//ADC转换分频系数,84M/2=42MHz
#define CLK_DIV_MCU_ADC1                        ADC_CLOCK_SYNC_PCLK_DIV8
//ADC数据结果对齐方式
#define RESULT_ALIGN_MODE_MCU_ADC1              ADC_DATAALIGN_RIGHT
//软件触发模式
#define EXTERN_TRIG_MODE_MCU_ADC1               ADC_SOFTWARE_START

/*------------------------------------------------DMA配置---------------------------------------------------*/
#if(MODE_ADC1_USER == MODE_ADC1_DMA)
#define MCU_ADC1_DMA_CLK_ENABLE()                   __HAL_RCC_DMA2_CLK_ENABLE()
#define MCU_ADC1_DMA_INSTANCE                       DMA2_Stream0
#define MCU_ADC1_DMA_CHANNEL                        DMA_CHANNEL_0
#define MCU_ADC1_DMA_PRIORITY                       DMA_PRIORITY_MEDIUM
#define MCU_ADC1_DMA_IRQ                            DMA2_Stream0_IRQn
#define MCU_ADC1_DMA_IRQ_HANDLER                    DMA2_Stream0_IRQHandler

#endif


#endif


