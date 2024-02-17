/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-08-21 10:49:58 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-08-25 11:12:53 +0800
************************************************************************************************/ 
#ifndef __DRIVER_CONFIG_CORE_H_
#define __DRIVER_CONFIG_CORE_H_
#include "DriverConfigBase.h"

/*----------------------------------------------中断向量表管理-----------------------------------------------------*/
//中断向量表的基础地址和偏移,一般是FLASH中
#define VECTOR_TABLE_BASE_ADDR                          FLASH_BASE
#define VECTOR_TABLE_OFFSET_ADDR                        0X00000000
//中断向量表长度            
#define VECTOR_TABLE_LENGTH                             0X400
//中断向量表地址            
#define VECTOR_TABLE_ADDR                               (VECTOR_TABLE_BASE_ADDR+VECTOR_TABLE_OFFSET_ADDR)

/*-----------------------------------------------------------FLASH管理------------------------------------------------*/
//芯片FLASH最大尺寸,1024KB
#define MCU_FLASH_BANK_MAX_SIZE                         0X100000
//FLASH等待超时时间
#define MCU_FLASH_WAIT_OP_TIME                          50000
//STM32 FLASH 基础地址
#define MCU_FLASH_BASE                                  FLASH_BASE
//STM32 BOOT程序地址
#define MCU_FLASH_BOOT                                  MCU_FLASH_BASE
//STM32 APP程序地址
#define MCU_FLASH_APP                                   (MCU_FLASH_BOOT+0X40000)

/*----------------------------------------------设置系统ICACHE DCACHE 预取指是否使能--------------------------------*/
//指令CACHE是否使能
#define CORE_ICACHE_SET                                 ENABLE
//数据CACHE是否使能         
#define CORE_DCACHE_SET                                 ENABLE
//FLASH预取指是否是能           
#define CORE_FLASH_PREFETCH_SET                         ENABLE

/*----------------------------------------------系统中断管理--------------------------------------------------------*/
//设置关闭全局中断是使用PRIMASK还是FAULTMASK
//primask将不会关闭hardfault,faultmask将关闭hardfault,只留下nmifault
#define CORE_LOCK_INT_GLOBAL_PRIMASK                    0
#define CORE_LOCK_INT_GLOBAL_FAULTMASK                  1
//用户选择中断关断模式          
#define CORE_LOCK_INT_GLOBAL_USER                       CORE_LOCK_INT_GLOBAL_PRIMASK

/*------------------------------------------系统延时模式选择------------------------------------------------------*/
//使用DWT定时器作为延时时钟
#define CORE_DELAY_MODE_DWT                             0
//使用外部定时器作为延时时钟            
#define CORE_DELAY_MODE_TIMER6                          1
//使用外部定时器作为延时时钟            
#define CORE_DELAY_MODE_TIMER7                          2
//使用滴答定时器作为延时时钟            
#define CORE_DELAY_MODE_SYSTICK                         3

//用户实际使用的延时方式设定            
#define CORE_DELAY_MODE_USER                            CORE_DELAY_MODE_DWT

/*---------------------------------------------系统内核时钟定义---------------------------------------------*/
//PLLM 对HSE进行分频后输出PLL锁相环
#define CORE_CLOCK_MAIN_PLL_M                           24
//PLLN 锁相环内对PLLM进行倍频
#define CORE_CLOCK_MAIN_PLL_N                           336
//PLLP PLLN倍频后输出到系统时钟选择器的分频
#define CORE_CLOCK_MAIN_PLL_P                           2
//PLLQ PLLN倍频之后用于系统内部的部分外设的时钟,用于USB OTG FS SDIO 随机数发生器的时钟
//如果使用USB FS模式,内核时钟必须是168MHZ
#define CORE_CLOCK_MAIN_PLL_Q                           7
//系统内核时钟
#define CORE_CLOCK_SYSTEM_MAIN_FREQ                     (((HSE_VALUE/CORE_CLOCK_MAIN_PLL_M)*CORE_CLOCK_MAIN_PLL_N)/2)

//高速AHB,与内核频率一致,
#define CORE_CLOCK_BUS_AHB                              RCC_SYSCLK_DIV1
//低速APB1 ,内核168M的情况下,APB1主频42M
#define CORE_CLOCK_BUS_APB1                             RCC_HCLK_DIV4
//高速APB2,内核168M的情况下,APB1主频84M
#define CORE_CLOCK_BUS_APB2                             RCC_HCLK_DIV2
//FLASH等待周期,一个等待周期对应30MHZ主频,0等待周期对应30MHZ,5等待周期对应(6*30)=180MHZ主频
#define CORE_FLASH_WAIT_CYCLE                           FLASH_LATENCY_5
//用户选择RTC的时钟源           
#define CORE_RTC_USE_LSE                                CONFIG_FUNC_DISABLE

#endif






















