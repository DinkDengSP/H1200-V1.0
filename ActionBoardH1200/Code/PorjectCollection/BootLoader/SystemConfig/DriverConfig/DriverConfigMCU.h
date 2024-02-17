/************************************************************************ 
 ** Author       : 邓小俊
 ** Date         : 2024-01-16 11:35:28
 ** Description  : When I Has Free Time ,I Will Write Description Below.
 ** description  : 
 ** LastEditors  : 邓小俊
 ** LastEditTime : 2024-01-16 11:37:35
 ** Email: xiaojun.deng@aliyun.com
 ** Copyright (c) 2024 by 公司名称, All Rights Reserved. 
 ************************************************************************/
#ifndef __DRIVER_CONFIG_MCU_H_
#define __DRIVER_CONFIG_MCU_H_
#include "DriverConfigCore.h"

/*----------------------------------------------RTT打印配置------------------------------------------------------------*/
//打印缓存数据长度
#define SEGGER_RTT_PRINTF_LENGTH                        BUFFER_SIZE_UP
//RTT使用的主通道
#define SEGGER_RTT_MAIN_CHANNEL                         0
//数据传输是锁定还是非锁定,上行模式,MCU发送到PC端
#define SEGGER_RTT_UP_MODE                              SEGGER_RTT_MODE_NO_BLOCK_SKIP
//数据传输是锁定还是非锁定,下行模式,PC发送到MCU端
#define SEGGER_RTT_DOWN_MODE                            SEGGER_RTT_MODE_NO_BLOCK_SKIP

/*-----------------------------------------------------------系统低级打印配置-----------------------------------------*/
//关闭输出
#define MCU_LOG_SHOW_MODE_OFF                           0
//RTT输出               
#define MCU_LOG_SHOW_MODE_RTT                           1
//MCU UART1输出             
#define MCU_LOG_SHOW_MODE_UART1                         2
//MCU UART6输出             
#define MCU_LOG_SHOW_MODE_UART6                         3

//日志模块总开关，注释掉将关闭日志输出
#define MCU_LOG_SHOW_MODE_USER                          MCU_LOG_SHOW_MODE_RTT
//单行显示缓存数据长度
#define LOW_LEVEL_BUF_SHOW_SINGLE_LINE                  20

/*-----------------------------------------------------------MCU内存管理长度-----------------------------------------*/
//AXI_SRAM用作动态内存管理的大小
#define MEM_MANAGE_BYTE_SIZE_AXI_SRAM                   300*1024
//ITCM用作动态内存管理的大小
#define MEM_MANAGE_BYTE_SIZE_ITCM_SRAM                  0*1024
//DTCM用作动态内存管理的大小
#define MEM_MANAGE_BYTE_SIZE_DTCM_SRAM                  0*1024
//D2 SRAM1用作动态内存管理的大小
#define MEM_MANAGE_BYTE_SIZE_D2_SRAM1                   0*1024
//D3 SRAM4用作动态内存管理的大小
#define MEM_MANAGE_BYTE_SIZE_D3_SRAM4                   0*1024

/*-----------------------------------------------------------CRC配置-----------------------------------------*/
//CRC测试用例是否使能
#define MCU_CRC_TEST_DEMO_ENABLE                        CONFIG_FUNC_DISABLE

/*----------------------------------------------------------系统RTC-------------------------------------------------------------*/
//RTC在备份域的时间标识,是否需要写入初始时间            
#define MCU_RTC_INIT_FLAG_BKP                           0XA55A
//初始化默认时间            
#define MCU_RTC_INIT_DEFAULT_YEAR                       20
#define MCU_RTC_INIT_DEFAULT_MOUTH                      7
#define MCU_RTC_INIT_DEFAULT_DAY                        11
#define MCU_RTC_INIT_DEFAULT_WEEKDAY                    6
#define MCU_RTC_INIT_DEFAULT_HOUR                       14
#define MCU_RTC_INIT_DEFAULT_MINUTE                     12
#define MCU_RTC_INIT_DEFAULT_SECOND                     11
//闹钟A的匹配模式           
#define MCU_RTC_ALARM_A_MATCH_MASK                      (RTC_ALARMMASK_HOURS|RTC_ALARMMASK_MINUTES)
//闹钟B的匹配模式           
#define MCU_RTC_ALARM_B_MATCH_MASK                      (RTC_ALARMMASK_DATEWEEKDAY|RTC_ALARMMASK_HOURS|RTC_ALARMMASK_MINUTES)
//是否启用RTC测试用例
#define MCU_RTC_TEST_CASE_ENABLE                        CONFIG_FUNC_DISABLE

/*-----------------------------------------------------------窗口看门狗配置-------------------------------------------------------*/
//窗口看门狗最大0X7F,每次喂狗的值
#define MCU_WWDG_RELOAD_VAL                             0X7F
//窗口值,上窗口值最低不能小于等待0X40,最大0X7F,计数值在0X40和MCU_WWDG_WINDOW_VAL之间
//才能喂狗,否则将会导致中断
#define MCU_WWDG_WINDOW_VAL                             (MCU_WWDG_RELOAD_VAL -1)
//是否启用提前唤醒中断,启用的话将在中断中喂狗,否则需要用户自己喂狗
#define MCU_WWDG_ENABLE_EWI_SET                         CONFIG_FUNC_DISABLE

/*-----------------------------------------------------------MCU内存管理长度-----------------------------------------*/
//AXI_SRAM用作动态内存管理的大小
#define MEM_MANAGE_BYTE_SIZE_AXI_SRAM                   300*1024
//ITCM用作动态内存管理的大小
#define MEM_MANAGE_BYTE_SIZE_ITCM_SRAM                  0*1024
//DTCM用作动态内存管理的大小
#define MEM_MANAGE_BYTE_SIZE_DTCM_SRAM                  0*1024
//D2 SRAM1用作动态内存管理的大小
#define MEM_MANAGE_BYTE_SIZE_D2_SRAM1                   0*1024
//D3 SRAM4用作动态内存管理的大小
#define MEM_MANAGE_BYTE_SIZE_D3_SRAM4                   0*1024

/*------------------------------------------------------------芯片IIC--------------------------------------------------------------*/
//不同的MCU IIC模式
#define MCU_IIC_MODE_SIM                                0X00//模拟IIC
#define MCU_IIC_MODE_POOL                               0X01//查询IIC
#define MCU_IIC_MODE_INT                                0X02//中断IIC
#define MCU_IIC_MODE_DMA                                0X03//DMA IIC
//必须打开IIC内部回调
#if(USE_HAL_I2C_REGISTER_CALLBACKS == 0)
#error "Lib IIC Must Use CallBack FuncPtr !!!"
#endif

/*------------------------------------------------------------IIC1--------------------------------------------------------------*/
//IIC总线端口定义
#define PIN_MCU_IIC1_SCL                                MCU_PIN_B_6
#define PIN_MCU_IIC1_SDA                                MCU_PIN_B_7
//用户选择的IIC模式
#define MCU_IIC1_MODE_USER                              MCU_IIC_MODE_DMA
//是否显示IIC通讯时间讯息   
#define TIME_MEASURE_MCU_IIC1                           CONFIG_FUNC_DISABLE
//失败之后的重试最大次数    
#define RETRY_MAX_MCU_IIC1                              10
//是否显示IIC中断讯息   
#define MCU_IIC1_SHOW_INT_MSG                           CONFIG_FUNC_DISABLE

//IIC分频系数 0X00-0X0F
#define MCU_IIC1_PRESCALER                              0x05
//IIC SCL数据建立时间 SCLDEL 0X00-0X0                   
#define MCU_IIC1_SETUP_TIME                             0X03
//IIC SCL数据保持时间 SDADEL 0X00-0X0                   
#define MCU_IIC1_HOLD_TIME                              0X03
//IIC SCL高电平周期 0X00-0XFF                   
#define MCU_IIC1_SCLH_PERIOD                            0X03
//IIC SCL低电平周期 0X00-0XFF                   
#define MCU_IIC1_SCLL_PERIOD                            0X09
//IIC时序,决定IIC通讯速度,以上使用H7中文手册1817页,时钟48MHZ下,400KHZ的通讯速率                
#define MCU_IIC1_TIMING                                 __LL_I2C_CONVERT_TIMINGS(MCU_IIC1_PRESCALER,MCU_IIC1_SETUP_TIME,MCU_IIC1_HOLD_TIME,MCU_IIC1_SCLH_PERIOD,MCU_IIC1_SCLL_PERIOD)
//IIC硬件操作最大超时时间
#define IIC1_TIME_OUT_MS                                50

#if(MCU_IIC1_MODE_USER == MCU_IIC_MODE_DMA)
//DMA模式下缓冲区长度
#define IIC1_BUFFER_LENGTH_DMA_MODE                     512
//IIC1 TX DMA选择
#define IIC1_DMA_TX_CLK_ENABLE()                        __HAL_RCC_DMA2_CLK_ENABLE()
#define IIC1_DMA_TX_INSTANCE                            DMA2_Stream0
#define IIC1_DMA_TX_PRIORITY                            DMA_PRIORITY_LOW
#define IIC1_DMA_TX_IRQ                                 DMA2_Stream0_IRQn
#define IIC1_DMA_TX_IRQ_HANDLER                         DMA2_Stream0_IRQHandler
//IIC1 RX DMA选择 
#define IIC1_DMA_RX_CLK_ENABLE()                        __HAL_RCC_DMA2_CLK_ENABLE()
#define IIC1_DMA_RX_INSTANCE                            DMA2_Stream1
#define IIC1_DMA_RX_PRIORITY                            DMA_PRIORITY_MEDIUM
#define IIC1_DMA_RX_IRQ                                 DMA2_Stream1_IRQn
#define IIC1_DMA_RX_IRQ_HANDLER                         DMA2_Stream1_IRQHandler
#endif


/*------------------------------------------------------------IIC2--------------------------------------------------------------*/
//IIC总线端口定义
#define PIN_MCU_IIC2_SCL                                MCU_PIN_H_4
#define PIN_MCU_IIC2_SDA                                MCU_PIN_H_5
//用户选择的IIC模式
#define MCU_IIC2_MODE_USER                              MCU_IIC_MODE_DMA
//是否显示IIC通讯时间讯息   
#define TIME_MEASURE_MCU_IIC2                           CONFIG_FUNC_DISABLE
//失败之后的重试最大次数    
#define RETRY_MAX_MCU_IIC2                              10
//是否显示IIC中断讯息   
#define MCU_IIC2_SHOW_INT_MSG                           CONFIG_FUNC_DISABLE

//IIC分频系数 0X00-0X0F
#define MCU_IIC2_PRESCALER                              0x05
//IIC SCL数据建立时间 SCLDEL 0X00-0X0                   
#define MCU_IIC2_SETUP_TIME                             0X03
//IIC SCL数据保持时间 SDADEL 0X00-0X0                   
#define MCU_IIC2_HOLD_TIME                              0X03
//IIC SCL高电平周期 0X00-0XFF                   
#define MCU_IIC2_SCLH_PERIOD                            0X03
//IIC SCL低电平周期 0X00-0XFF                   
#define MCU_IIC2_SCLL_PERIOD                            0X09
//IIC时序,决定IIC通讯速度,以上使用H7中文手册1817页,时钟48MHZ下,400KHZ的通讯速率                
#define MCU_IIC2_TIMING                                 __LL_I2C_CONVERT_TIMINGS(MCU_IIC2_PRESCALER,MCU_IIC2_SETUP_TIME,MCU_IIC2_HOLD_TIME,MCU_IIC2_SCLH_PERIOD,MCU_IIC2_SCLL_PERIOD)
//IIC硬件操作最大超时时间
#define IIC2_TIME_OUT_MS                                50

#if(MCU_IIC2_MODE_USER == MCU_IIC_MODE_DMA)
//DMA模式下缓冲区长度
#define IIC2_BUFFER_LENGTH_DMA_MODE                     512
//IIC2 TX DMA选择
#define IIC2_DMA_TX_CLK_ENABLE()                        __HAL_RCC_DMA2_CLK_ENABLE()
#define IIC2_DMA_TX_INSTANCE                            DMA2_Stream2
#define IIC2_DMA_TX_PRIORITY                            DMA_PRIORITY_LOW
#define IIC2_DMA_TX_IRQ                                 DMA2_Stream2_IRQn
#define IIC2_DMA_TX_IRQ_HANDLER                         DMA2_Stream2_IRQHandler
//IIC2 RX DMA选择 
#define IIC2_DMA_RX_CLK_ENABLE()                        __HAL_RCC_DMA2_CLK_ENABLE()
#define IIC2_DMA_RX_INSTANCE                            DMA2_Stream3
#define IIC2_DMA_RX_PRIORITY                            DMA_PRIORITY_MEDIUM
#define IIC2_DMA_RX_IRQ                                 DMA2_Stream3_IRQn
#define IIC2_DMA_RX_IRQ_HANDLER                         DMA2_Stream3_IRQHandler
#endif


/*------------------------------------------------------------芯片SPI--------------------------------------------------------------*/
//查询模式
#define MCU_SPI_MODE_POOL                               0
//中断模式  
#define MCU_SPI_MODE_INT                                1
//DMA模式   
#define MCU_SPI_MODE_DMA                                2
//SPI通讯,单字节最长时间
#define TIME_OUT_MS_SPI_SINGLE_BYTE                     50
//SPI空数据,用于不想读取的时候
#define SPI_PTR_NULL                                    NULL
//必须打开SPI内部回调
#if(USE_HAL_SPI_REGISTER_CALLBACKS == 0)
#error "Lib SPI Must Use CallBack FuncPtr !!!"
#endif


/*------------------------------------------------------------SPI4--------------------------------------------------------------*/
//SPI通讯引脚定义
#define MCU_SPI4_PIN_SCK                                MCU_PIN_E_2
#define MCU_SPI4_PIN_MISO                               MCU_PIN_E_5
#define MCU_SPI4_PIN_MOSI                               MCU_PIN_E_6
//SPI模式设定
#define MCU_SPI4_MODE_USER                              MCU_SPI_MODE_DMA
//中断模式下,等待中断的时候是否开启操作系统调度,不开启快,开启了慢但是对系统有好处
#define MCU_SPI4_INT_ENABLE_SCHEDULE                    CONFIG_FUNC_ENABLE
//取消传输最大超时时间
#define MCU_SPI4_ABORT_WAIT_TIME_MAX                    1000
//是否执行中断打印
#define MCU_SPI4_ENABLE_INT_SHOW                        CONFIG_FUNC_DISABLE
//DMA收发缓冲区长度
#define LENGTH_BYTES_SPI4_DMA_BUF                       4096

#if(MCU_SPI4_MODE_USER == MCU_SPI_MODE_DMA)
//DMA发送配置映射
#define SPI4_DMA_TX_CLK_ENABLE()                        __HAL_RCC_DMA2_CLK_ENABLE()
#define SPI4_DMA_TX_INSTANCE                            DMA2_Stream4
#define SPI4_DMA_TX_PRIORITY                            DMA_PRIORITY_MEDIUM
#define SPI4_DMA_TX_IRQ                                 DMA2_Stream4_IRQn
#define SPI4_DMA_TX_IRQ_HANDLER                         DMA2_Stream4_IRQHandler
//DMA接收配置映射
#define SPI4_DMA_RX_CLK_ENABLE()                        __HAL_RCC_DMA2_CLK_ENABLE()
#define SPI4_DMA_RX_INSTANCE                            DMA2_Stream5
#define SPI4_DMA_RX_PRIORITY                            DMA_PRIORITY_MEDIUM
#define SPI4_DMA_RX_IRQ                                 DMA2_Stream5_IRQn
#define SPI4_DMA_RX_IRQ_HANDLER                         DMA2_Stream5_IRQHandler
#endif

/*------------------------------------------SPI5------------------------------------*/
//SPI通讯引脚定义
#define MCU_SPI5_PIN_SCK                                MCU_PIN_F_7
#define MCU_SPI5_PIN_MISO                               MCU_PIN_F_8
#define MCU_SPI5_PIN_MOSI                               MCU_PIN_F_9
//SPI模式设定
#define MCU_SPI5_MODE_USER                              MCU_SPI_MODE_DMA
//中断模式下,等待中断的时候是否开启操作系统调度,不开启快,开启了慢但是对系统有好处
#define MCU_SPI5_INT_ENABLE_SCHEDULE                    CONFIG_FUNC_ENABLE
//取消传输最大超时时间
#define MCU_SPI5_ABORT_WAIT_TIME_MAX                    1000
//是否执行中断打印
#define MCU_SPI5_ENABLE_INT_SHOW                        CONFIG_FUNC_DISABLE
//DMA收发缓冲区长度
#define LENGTH_BYTES_SPI5_DMA_BUF                       4096

#if(MCU_SPI5_MODE_USER == MCU_SPI_MODE_DMA)
//DMA发送配置映射
#define SPI5_DMA_TX_CLK_ENABLE()                        __HAL_RCC_DMA2_CLK_ENABLE()
#define SPI5_DMA_TX_INSTANCE                            DMA2_Stream6
#define SPI5_DMA_TX_PRIORITY                            DMA_PRIORITY_MEDIUM
#define SPI5_DMA_TX_IRQ                                 DMA2_Stream6_IRQn
#define SPI5_DMA_TX_IRQ_HANDLER                         DMA2_Stream6_IRQHandler
//DMA接收配置映射
#define SPI5_DMA_RX_CLK_ENABLE()                        __HAL_RCC_DMA2_CLK_ENABLE()
#define SPI5_DMA_RX_INSTANCE                            DMA2_Stream7
#define SPI5_DMA_RX_PRIORITY                            DMA_PRIORITY_MEDIUM
#define SPI5_DMA_RX_IRQ                                 DMA2_Stream7_IRQn
#define SPI5_DMA_RX_IRQ_HANDLER                         DMA2_Stream7_IRQHandler
#endif


#endif








