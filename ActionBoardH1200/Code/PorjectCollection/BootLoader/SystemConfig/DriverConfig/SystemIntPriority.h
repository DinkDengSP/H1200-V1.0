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
#ifndef __SYSTEM_INT_PRIORITY_H_
#define __SYSTEM_INT_PRIORITY_H_
#include "stm32h7xx_hal.h"

/**定义系统中断抢占优先级 数值越小,优先级越高*/
typedef enum NVIC_INT_PRE_PRI
{
    NVIC_INT_PRE_PRI_0 = 0,
    NVIC_INT_PRE_PRI_1 = 1,
    NVIC_INT_PRE_PRI_2 = 2,
    NVIC_INT_PRE_PRI_3 = 3,
} NVIC_INT_PRE_PRI;

/**定义系统中断子优先级 数值越小,优先级越高*/
typedef enum NVIC_INT_SUB_PRI
{
    NVIC_INT_SUB_PRI_0 = 0,
    NVIC_INT_SUB_PRI_1 = 1,
    NVIC_INT_SUB_PRI_2 = 2,
    NVIC_INT_SUB_PRI_3 = 3,
} NVIC_INT_SUB_PRI;

//主板中断分组设计,可选值 : NVIC_PRIORITYGROUP_0 -- NVIC_PRIORITYGROUP_4
#define SYSTEM_DEFAULT_NVIC_GROUP                   NVIC_PRIORITYGROUP_2

/*-------------------------不需要操作系统关注的中断-----------------------------*/
//窗口看门狗喂狗事件
#define MCU_WWDG_PREE_PRI                           NVIC_INT_PRE_PRI_0
#define MCU_WWDG_SUB_PRI                            NVIC_INT_PRE_PRI_0

//串口1中断
#define MCU_UART1_PREE_PRI                          NVIC_INT_PRE_PRI_0
#define MCU_UART1_SUB_PRI                           NVIC_INT_SUB_PRI_2

//串口3中断
#define MCU_UART3_PREE_PRI                          NVIC_INT_PRE_PRI_0
#define MCU_UART3_SUB_PRI                           NVIC_INT_SUB_PRI_2

//串口6中断
#define MCU_UART6_PREE_PRI                          NVIC_INT_PRE_PRI_0
#define MCU_UART6_SUB_PRI                           NVIC_INT_SUB_PRI_2

//主板高精度时间戳
#define BOARD_HP_TIME_STAMP_PREE_PRI                NVIC_INT_PRE_PRI_0
#define BOARD_HP_TIME_STAMP_SUB_PRI                 NVIC_INT_SUB_PRI_3

/*-------------------------需要操作系统关注的中断-----------------------------*/
//滴答定时器中断
#define MCU_SYS_TICK_PREE_PRI                       NVIC_INT_PRE_PRI_1
#define MCU_SYS_TICK_SUB_PRI                        NVIC_INT_SUB_PRI_0

//FDCAN1 FIFO0中断
#define CAN1_INT0_PREE_PRI                          NVIC_INT_PRE_PRI_1
#define CAN1_INT0_SUB_PRI                           NVIC_INT_SUB_PRI_1

//FDCAN1 FIFO1中断
#define CAN1_INT1_PREE_PRI                          NVIC_INT_PRE_PRI_1
#define CAN1_INT1_SUB_PRI                           NVIC_INT_SUB_PRI_2

//FDCAN时钟校准中断
#define CAN_CAL_PREE_PRI                            NVIC_INT_PRE_PRI_1
#define CAN_CAL_SUB_PRI                             NVIC_INT_SUB_PRI_3

//串口1接收DMA中断
#define MCU_UART1_RX_DMA_PREE_PRI                   NVIC_INT_PRE_PRI_1
#define MCU_UART1_RX_DMA_SUB_PRI                    NVIC_INT_SUB_PRI_3

//串口3接收DMA中断
#define MCU_UART3_RX_DMA_PREE_PRI                   NVIC_INT_PRE_PRI_1
#define MCU_UART3_RX_DMA_SUB_PRI                    NVIC_INT_SUB_PRI_3

//串口6 DMA接收中断     
#define MCU_UART6_RX_DMA_PREE_PRI                   NVIC_INT_PRE_PRI_1
#define MCU_UART6_RX_DMA_SUB_PRI                    NVIC_INT_SUB_PRI_3

//SPI4中断,使用DMA模式的时候的优先级      
#define SPI4_PREE_PRI_DMA                           NVIC_INT_PRE_PRI_2
#define SPI4_SUB_PRI_DMA                            NVIC_INT_SUB_PRI_1

//SPI4 DMA发送中断
#define SPI4_DMA_TX_PREE_PRI                        NVIC_INT_PRE_PRI_2
#define SPI4_DMA_TX_SUB_PRI                         NVIC_INT_SUB_PRI_2

//SPI4 DMA接收中断  
#define SPI4_DMA_RX_PREE_PRI                        NVIC_INT_PRE_PRI_2
#define SPI4_DMA_RX_SUB_PRI                         NVIC_INT_SUB_PRI_2

//SPI5中断,使用DMA模式的时候的优先级      
#define SPI5_PREE_PRI_DMA                           NVIC_INT_PRE_PRI_2
#define SPI5_SUB_PRI_DMA                            NVIC_INT_SUB_PRI_1

//SPI5 DMA发送中断
#define SPI5_DMA_TX_PREE_PRI                        NVIC_INT_PRE_PRI_2
#define SPI5_DMA_TX_SUB_PRI                         NVIC_INT_SUB_PRI_2

//SPI5 DMA接收中断  
#define SPI5_DMA_RX_PREE_PRI                        NVIC_INT_PRE_PRI_2
#define SPI5_DMA_RX_SUB_PRI                         NVIC_INT_SUB_PRI_2

//IIC1通讯中断
#define IIC1_EVENT_PREE_PRI                         NVIC_INT_PRE_PRI_3
#define IIC1_EVENT_SUB_PRI                          NVIC_INT_SUB_PRI_0

//IIC1 DMA发送中断
#define IIC1_DMA_TX_PREE_PRI                        NVIC_INT_PRE_PRI_3
#define IIC1_DMA_TX_SUB_PRI                         NVIC_INT_SUB_PRI_1

//IIC1 DMA接收中断  
#define IIC1_DMA_RX_PREE_PRI                        NVIC_INT_PRE_PRI_3
#define IIC1_DMA_RX_SUB_PRI                         NVIC_INT_SUB_PRI_1

//IIC1错误中断
#define IIC1_ERR_PREE_PRI                           NVIC_INT_PRE_PRI_3
#define IIC1_ERR_SUB_PRI                            NVIC_INT_SUB_PRI_0

//IIC2通讯中断
#define IIC2_EVENT_PREE_PRI                         NVIC_INT_PRE_PRI_3
#define IIC2_EVENT_SUB_PRI                          NVIC_INT_SUB_PRI_0

//IIC2 DMA发送中断
#define IIC2_DMA_TX_PREE_PRI                        NVIC_INT_PRE_PRI_3
#define IIC2_DMA_TX_SUB_PRI                         NVIC_INT_SUB_PRI_1

//IIC2 DMA接收中断  
#define IIC2_DMA_RX_PREE_PRI                        NVIC_INT_PRE_PRI_3
#define IIC2_DMA_RX_SUB_PRI                         NVIC_INT_SUB_PRI_1

//IIC2错误中断
#define IIC2_ERR_PREE_PRI                           NVIC_INT_PRE_PRI_3
#define IIC2_ERR_SUB_PRI                            NVIC_INT_SUB_PRI_0

//串口1 发送DMA中断
#define MCU_UART1_TX_DMA_PREE_PRI                   NVIC_INT_PRE_PRI_3
#define MCU_UART1_TX_DMA_SUB_PRI                    NVIC_INT_SUB_PRI_3

//串口3 发送DMA中断
#define MCU_UART3_TX_DMA_PREE_PRI                   NVIC_INT_PRE_PRI_3
#define MCU_UART3_TX_DMA_SUB_PRI                    NVIC_INT_SUB_PRI_3

//串口6 DMA发送中断
#define MCU_UART6_TX_DMA_PREE_PRI                   NVIC_INT_PRE_PRI_3
#define MCU_UART6_TX_DMA_SUB_PRI                    NVIC_INT_SUB_PRI_3

//RTC闹钟中断
#define MCU_RTC_ALARM_PREE_PRI                      NVIC_INT_PRE_PRI_3
#define MCU_RTC_ALARM_SUB_PRI                       NVIC_INT_SUB_PRI_3

//RTC周期唤醒
#define MCU_RTC_WAKEUP_PREE_PRI                     NVIC_INT_PRE_PRI_3
#define MCU_RTC_WAKEUP_SUB_PRI                      NVIC_INT_SUB_PRI_3

//定时器6中断
#define MCU_TIMER6_BASE_PREE_PRI                    NVIC_INT_PRE_PRI_3
#define MCU_TIMER6_BASE_SUB_PRI                     NVIC_INT_SUB_PRI_3

//定时器7中断
#define MCU_TIMER7_BASE_PREE_PRI                    NVIC_INT_PRE_PRI_3
#define MCU_TIMER7_BASE_SUB_PRI                     NVIC_INT_SUB_PRI_3

#endif








