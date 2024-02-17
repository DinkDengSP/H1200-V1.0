/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-11-03 21:10:26 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-11-06 10:18:05 +0800
************************************************************************************************/ 
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

/*-----------------------------------------------------------CRC配置-----------------------------------------*/
//CRC测试用例是否使能
#define MCU_CRC_TEST_DEMO_ENABLE                        CONFIG_FUNC_ENABLE

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
#define SPI5_DMA_TX_CLK_ENABLE()                        __HAL_RCC_DMA1_CLK_ENABLE()
#define SPI5_DMA_TX_INSTANCE                            DMA1_Stream6
#define SPI5_DMA_TX_PRIORITY                            DMA_PRIORITY_MEDIUM
#define SPI5_DMA_TX_IRQ                                 DMA1_Stream6_IRQn
#define SPI5_DMA_TX_IRQ_HANDLER                         DMA1_Stream6_IRQHandler
//DMA接收配置映射
#define SPI5_DMA_RX_CLK_ENABLE()                        __HAL_RCC_DMA1_CLK_ENABLE()
#define SPI5_DMA_RX_INSTANCE                            DMA1_Stream7
#define SPI5_DMA_RX_PRIORITY                            DMA_PRIORITY_MEDIUM
#define SPI5_DMA_RX_IRQ                                 DMA1_Stream7_IRQn
#define SPI5_DMA_RX_IRQ_HANDLER                         DMA1_Stream7_IRQHandler
#endif

/*------------------------------------------------------------------芯片内置MAC配置-----------------------------------------*/
//MCU动态内存匹配对象       
#define MCU_ETH_MEM_REGION                              MEM_AXI_SRAM
//ETH单包数据发送超时时间       
#define TIME_OUT_MS_ETH_SEND                            100
//RMII端口定义
#define MCU_ETH_MII_CRS                                 MCU_PIN_A_0
#define MCU_ETH_MII_RX_CLK                              MCU_PIN_A_1
#define MCU_ETH_MII_MDIO                                MCU_PIN_A_2
#define MCU_ETH_MII_COL                                 MCU_PIN_A_3
#define MCU_ETH_MII_RX_DV                               MCU_PIN_A_7
#define MCU_ETH_MII_RXD2                                MCU_PIN_B_0
#define MCU_ETH_MII_RXD3                                MCU_PIN_B_1
#define MCU_ETH_MII_TXD3                                MCU_PIN_B_8
#define MCU_ETH_MII_RX_ER                               MCU_PIN_B_10
#define MCU_ETH_MII_TX_EN                               MCU_PIN_B_11
#define MCU_ETH_MII_TXD0                                MCU_PIN_B_12
#define MCU_ETH_MII_TXD1                                MCU_PIN_B_13
#define MCU_ETH_MII_MDC                                 MCU_PIN_C_1
#define MCU_ETH_MII_TXD2                                MCU_PIN_C_2
#define MCU_ETH_MII_TX_CLK                              MCU_PIN_C_3
#define MCU_ETH_MII_RXD0                                MCU_PIN_C_4
#define MCU_ETH_MII_RXD1                                MCU_PIN_C_5



#endif



















