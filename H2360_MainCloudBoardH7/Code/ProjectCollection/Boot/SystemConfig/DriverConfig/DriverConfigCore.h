/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-11-03 21:10:26 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-11-06 10:18:05 +0800
************************************************************************************************/ 
#ifndef __DRIVER_CONFIG_CORE_H_
#define __DRIVER_CONFIG_CORE_H_
#include "DriverConfigBase.h"

/*----------------------------------------------中断向量表管理-----------------------------------------------------*/
//中断向量表的基础地址和偏移,一般是FLASH中,可以将中断向量表拷贝到快速访问内存,加快中断响应速度
#define VECTOR_TABLE_BASE_ADDR                          FLASH_BANK1_BASE
//中断向量偏移
#define VECTOR_TABLE_OFFSET_ADDR                        0X00000
//中断向量表长度            
#define VECTOR_TABLE_LENGTH                             0X400
//中断向量表地址            
#define VECTOR_TABLE_ADDR                               (VECTOR_TABLE_BASE_ADDR+VECTOR_TABLE_OFFSET_ADDR)

/*----------------------------------------------应用程序地址管理管理-----------------------------------------------------*/
//STM32 服务程序BOOT地址
#define MCU_FLASH_SRV_BOOT                              0x08000000
//STM32 服务程序主程序地址
#define MCU_FLASH_SRV_MAIN                              0x08040000
//STM32 BOOT备份地址
#define MCU_FLASH_SRV_BOOT_BACK_UP                      0x08100000
//STM32 服务程序主程序备份地址
#define MCU_FLASH_SRV_MAIN_BACK_UP                      0x08120000

/*----------------------------------------------MPU配置---------------------------------------------------------*/
//是否是能MPU写透功能,H743因为存在ICACHE,所以不需要Flash预取值,ICACHE实现了这个效果
#define MPU_ENABLE_WRITE_THROUGH                        CONFIG_FUNC_DISABLE
//指令CACHE是否使能
#define CORE_ICACHE_SET                                 ENABLE
//数据CACHE是否使能         
#define CORE_DCACHE_SET                                 ENABLE

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
//使用外部定时器6作为延时时钟            
#define CORE_DELAY_MODE_TIMER6                          1
//使用外部定时器7作为延时时钟            
#define CORE_DELAY_MODE_TIMER7                          2
//使用滴答定时器作为延时时钟            
#define CORE_DELAY_MODE_SYSTICK                         3

//用户实际使用的延时方式设定            
#define CORE_DELAY_MODE_USER                            CORE_DELAY_MODE_DWT


/*----------------------------------------------芯片时钟配置---------------------------------------------------------*/
//AHB总线主频
#define AHB_CLOCK_FREQ                                  200000000
//APB1总线主频
#define APB1_CLOCK_FREQ                                 100000000
//APB2总线主频
#define APB2_CLOCK_FREQ                                 100000000
//APB4总线主频
#define APB4_CLOCK_FREQ                                 100000000
//是否打开LSE外部低速时钟
#define CORE_CLK_LSE_SET                                CONFIG_FUNC_ENABLE

//将HSE分频输入锁相环,HSE Frequency(Hz)    = 24000000
#define CORE_MAIN_PLL_M                                 6
//根据不同的目标时钟选择不同锁相环倍频系数
#define CORE_MAIN_PLL_N                                 200
//锁相环输入时钟分频到系统内核
#define CORE_MAIN_PLL_P                                 2  
//锁相环时钟输入到PLLQ,内核的四分之一,供给SDMMC1,SAI1,SAI4A,SAI4B,USB
//SPI123,FDCAN,QSPI,FMC,SPDIF,SAI2_SAIA,SAI2_SAIB
#define CORE_MAIN_PLL_Q                                 8  
//锁相环时钟输入到PLLR,与内核相等,这个时钟只能供给TRACE
#define CORE_MAIN_PLL_R                                 2  

//PLL2时钟设置,PLL2M,HSE Frequency(Hz)= 24000000,设置输出4M
#define CLK_PLL2_M_PARAM                                6
//锁相环倍频系数,倍频200,输出800M
#define CLK_PLL2_N_PARAM                                200
//PLL2P,400M,输出供给CLK_OUT2,LPTIM2,LPTIM234,ADC,SAI4AB,SPI123,LPTIM1,SAI1,SAI2
#define CLK_PLL2_P_PARAM                                2
//PLL2Q,400M.输出供给FDCAN,SPI6,LPUART1,UART234578,SPI45,USART16
#define CLK_PLL2_Q_PARAM                                2
//PLL2R,200M,输出供给SDMMC1,QSPI,FMC,SPDIF
#define CLK_PLL2_R_PARAM                                4

//将HSE分频输入锁相环,HSE Frequency(Hz)= 24000000,设置输出4M
#define CLK_PLL3_M_PARAM                                5
//将输入锁相环的时钟倍频,4*60=240MHZ                   
#define CLK_PLL3_N_PARAM                                60
//PLL3P,120M,输出供给SAI4AB SPI123,SAI1,SAI2,SAI3                    
#define CLK_PLL3_P_PARAM                                2
//PLL3Q,48M,输出供给,USB,SPI6,LPUART1,USART16,UART234578,SPI45                   
#define CLK_PLL3_Q_PARAM                                5  
//PLL3R,48M,输出供给LTDC,IIC1234,LPTIM12345,ADC,SPDIF                   
#define CLK_PLL3_R_PARAM                                5


#endif



















