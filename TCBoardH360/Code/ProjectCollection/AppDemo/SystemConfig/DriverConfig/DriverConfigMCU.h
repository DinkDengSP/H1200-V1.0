/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-11-13 10:20:55 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-11-21 11:42:07 +0800
************************************************************************************************/ 
#ifndef __DRIVER_CONFIG_MCU_H_
#define __DRIVER_CONFIG_MCU_H_
#include "DriverConfigCan.h"
#include "DriverConfigUART.h"
#include "DriverConfigTimer.h"

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
//MCU UART2输出             
#define MCU_LOG_SHOW_MODE_UART2                         3
//MCU UART3输出             
#define MCU_LOG_SHOW_MODE_UART3                         4
//MCU UART6输出             
#define MCU_LOG_SHOW_MODE_UART6                         5

//日志模块总开关，注释掉将关闭日志输出
#define MCU_LOG_SHOW_MODE_USER                          MCU_LOG_SHOW_MODE_RTT
//单行显示缓存数据长度
#define LOW_LEVEL_BUF_SHOW_SINGLE_LINE                  20

/*----------------------------------------------RTC时钟源选择-------------------------------------------------------*/
//RTC在备份域的时间标识,是否需要写入初始时间            
#define MCU_RTC_INIT_FLAG_BKP                           0XA55B
//初始化默认时间            
#define MCU_RTC_INIT_DEFAULT_YEAR                       20
#define MCU_RTC_INIT_DEFAULT_MOUTH                      7
#define MCU_RTC_INIT_DEFAULT_DAY                        11
#define MCU_RTC_INIT_DEFAULT_WEEKDAY                    6
#define MCU_RTC_INIT_DEFAULT_HOUR                       14
#define MCU_RTC_INIT_DEFAULT_MINUTE                     12
#define MCU_RTC_INIT_DEFAULT_SECOND                     11
//闹钟A的匹配模式           
#define MCU_RTC_ALARM_A_MATCH_MASK                      (RTC_ALARMMASK_HOURS|RTC_ALARMMASK_MINUTES|RTC_ALARMMASK_SECONDS)
//闹钟B的匹配模式           
#define MCU_RTC_ALARM_B_MATCH_MASK                      (RTC_ALARMMASK_DATEWEEKDAY|RTC_ALARMMASK_HOURS|RTC_ALARMMASK_MINUTES)


/*-----------------------------------------------------------MCU内存管理长度-----------------------------------------*/
#define MEM_MANAGE_BYTE_SIZE_SRAM_IN                    60*1024

/*-----------------------------------------------------------窗口看门狗配置-------------------------------------------------------*/
//窗口看门狗最大0X7F,每次喂狗的值
#define MCU_WWDG_RELOAD_VAL                             0X7F
//窗口值,上窗口值最低不能小于等待0X40,最大0X7F,计数值在0X40和MCU_WWDG_WINDOW_VAL之间
//才能喂狗,否则将会导致中断
#define MCU_WWDG_WINDOW_VAL                             (MCU_WWDG_RELOAD_VAL -1)
//是否启用提前唤醒中断
#define MCU_WWDG_ENABLE_EWI_SET                         CONFIG_FUNC_DISABLE

/*------------------------------------------芯片SPI------------------------------------*/
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

/*------------------------------------------SPI1------------------------------------*/
//SPI通讯引脚定义
#define MCU_SPI1_PIN_SCK                                MCU_PIN_A_5
#define MCU_SPI1_PIN_MISO                               MCU_PIN_A_6
#define MCU_SPI1_PIN_MOSI                               MCU_PIN_A_7
//SPI模式设定
#define MCU_SPI1_MODE_USER                              MCU_SPI_MODE_DMA
//中断模式下,等待中断的时候是否开启操作系统调度,不开启快,开启了慢但是对系统有好处
#define MCU_SPI1_INT_ENABLE_SCHEDULE                    CONFIG_FUNC_ENABLE
//取消传输最大超时时间
#define MCU_SPI1_ABORT_WAIT_TIME_MAX                    1000
//是否执行中断打印
#define MCU_SPI1_ENABLE_INT_SHOW                        CONFIG_FUNC_DISABLE
//DMA收发缓冲区长度
#define LENGTH_BYTES_SPI1_DMA_BUF                       4096

#if(MCU_SPI1_MODE_USER == MCU_SPI_MODE_DMA)
//DMA发送配置映射,只能选DMA1_Stream4
#define SPI1_DMA_TX_CLK_ENABLE()                        __HAL_RCC_DMA2_CLK_ENABLE() 
#define SPI1_DMA_TX_INSTANCE                            DMA2_Stream3
#define SPI1_DMA_TX_CHANNEL                             DMA_CHANNEL_3 
#define SPI1_DMA_TX_PRIORITY                            DMA_PRIORITY_HIGH 
#define SPI1_DMA_TX_IRQ                                 DMA2_Stream3_IRQn 
#define SPI1_DMA_TX_IRQ_HANDLER                         DMA2_Stream3_IRQHandler 
//DMA接收配置映射,只能选DMA1_Stream3
#define SPI1_DMA_RX_CLK_ENABLE()                        __HAL_RCC_DMA2_CLK_ENABLE()    
#define SPI1_DMA_RX_INSTANCE                            DMA2_Stream2                   
#define SPI1_DMA_RX_CHANNEL                             DMA_CHANNEL_3                  
#define SPI1_DMA_RX_PRIORITY                            DMA_PRIORITY_MEDIUM            
#define SPI1_DMA_RX_IRQ                                 DMA2_Stream2_IRQn              
#define SPI1_DMA_RX_IRQ_HANDLER                         DMA2_Stream2_IRQHandler        
#endif

/*------------------------------------------SPI2------------------------------------*/
//SPI通讯引脚定义
#define MCU_SPI2_PIN_SCK                                MCU_PIN_B_13
#define MCU_SPI2_PIN_MISO                               MCU_PIN_B_14
#define MCU_SPI2_PIN_MOSI                               MCU_PIN_B_15
//SPI模式设定
#define MCU_SPI2_MODE_USER                              MCU_SPI_MODE_DMA
//中断模式下,等待中断的时候是否开启操作系统调度,不开启快,开启了慢但是对系统有好处
#define MCU_SPI2_INT_ENABLE_SCHEDULE                    CONFIG_FUNC_ENABLE
//取消传输最大超时时间
#define MCU_SPI2_ABORT_WAIT_TIME_MAX                    1000
//是否执行中断打印
#define MCU_SPI2_ENABLE_INT_SHOW                        CONFIG_FUNC_DISABLE
//DMA收发缓冲区长度
#define LENGTH_BYTES_SPI2_DMA_BUF                       4096

#if(MCU_SPI2_MODE_USER == MCU_SPI_MODE_DMA)
//DMA发送配置映射,只能选DMA1_Stream4
#define SPI2_DMA_TX_CLK_ENABLE()                        __HAL_RCC_DMA1_CLK_ENABLE() 
#define SPI2_DMA_TX_INSTANCE                            DMA1_Stream4
#define SPI2_DMA_TX_CHANNEL                             DMA_CHANNEL_0 
#define SPI2_DMA_TX_PRIORITY                            DMA_PRIORITY_HIGH 
#define SPI2_DMA_TX_IRQ                                 DMA1_Stream4_IRQn 
#define SPI2_DMA_TX_IRQ_HANDLER                         DMA1_Stream4_IRQHandler 
//DMA接收配置映射,只能选DMA1_Stream3
#define SPI2_DMA_RX_CLK_ENABLE()                        __HAL_RCC_DMA1_CLK_ENABLE()    
#define SPI2_DMA_RX_INSTANCE                            DMA1_Stream3                   
#define SPI2_DMA_RX_CHANNEL                             DMA_CHANNEL_0                  
#define SPI2_DMA_RX_PRIORITY                            DMA_PRIORITY_MEDIUM            
#define SPI2_DMA_RX_IRQ                                 DMA1_Stream3_IRQn              
#define SPI2_DMA_RX_IRQ_HANDLER                         DMA1_Stream3_IRQHandler        
#endif




#endif















