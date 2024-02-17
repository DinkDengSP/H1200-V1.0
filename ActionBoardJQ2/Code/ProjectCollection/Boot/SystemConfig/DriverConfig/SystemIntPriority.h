/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-08-23 09:44:56 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-08-23 20:00:15 +0800
************************************************************************************************/ 
#ifndef __SYSTEM_INT_PRIORITY_H_
#define __SYSTEM_INT_PRIORITY_H_
#include "stm32f4xx_hal.h"

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
#define MCU_WWDG_SUB_PRI                            NVIC_INT_SUB_PRI_0

//定时器1PWM输出中断
#define MCU_TIMER1_UP_BASE_PREE_PRI                 NVIC_INT_PRE_PRI_0
#define MCU_TIMER1_UP_BASE_SUB_PRI                  NVIC_INT_SUB_PRI_3

//定时器9PWM输出中断
#define MCU_TIMER9_UP_BASE_PREE_PRI                 NVIC_INT_PRE_PRI_0
#define MCU_TIMER9_UP_BASE_SUB_PRI                  NVIC_INT_SUB_PRI_3
/*-------------------------需要操作系统关注的中断-----------------------------*/
//滴答定时器中断
#define MCU_SYS_TICK_PREE_PRI                       NVIC_INT_PRE_PRI_1
#define MCU_SYS_TICK_SUB_PRI                        NVIC_INT_SUB_PRI_0

//串口1接收DMA中断
#define MCU_UART1_RX_DMA_PREE_PRI                   NVIC_INT_PRE_PRI_1
#define MCU_UART1_RX_DMA_SUB_PRI                    NVIC_INT_SUB_PRI_2

//串口1中断,不使用DMA的情况下,将中断设定为无需系统关注
#define MCU_UART1_PREE_PRI                          NVIC_INT_PRE_PRI_1
#define MCU_UART1_SUB_PRI                           NVIC_INT_SUB_PRI_3

//串口3接收DMA中断
#define MCU_UART3_RX_DMA_PREE_PRI                   NVIC_INT_PRE_PRI_1
#define MCU_UART3_RX_DMA_SUB_PRI                    NVIC_INT_SUB_PRI_2

//串口3中断,不使用DMA的情况下,将中断设定为无需系统关注
#define MCU_UART3_PREE_PRI                          NVIC_INT_PRE_PRI_1
#define MCU_UART3_SUB_PRI                           NVIC_INT_SUB_PRI_3

//CAN1 RX0中断
#define CAN1_RX0_PREE_PRI                           NVIC_INT_PRE_PRI_2
#define CAN1_RX0_SUB_PRI                            NVIC_INT_SUB_PRI_0

//CAN1 RX1中断
#define CAN1_RX1_PREE_PRI                           NVIC_INT_PRE_PRI_2
#define CAN1_RX1_SUB_PRI                            NVIC_INT_SUB_PRI_0

//CAN2 RX0中断
#define CAN2_RX0_PREE_PRI                           NVIC_INT_PRE_PRI_2
#define CAN2_RX0_SUB_PRI                            NVIC_INT_SUB_PRI_0

//CAN2 RX1中断
#define CAN2_RX1_PREE_PRI                           NVIC_INT_PRE_PRI_2
#define CAN2_RX1_SUB_PRI                            NVIC_INT_SUB_PRI_0

//SPI3中断,使用DMA模式的时候的优先级      
#define SPI3_PREE_PRI_DMA                           NVIC_INT_PRE_PRI_2
#define SPI3_SUB_PRI_DMA                            NVIC_INT_SUB_PRI_1

//SPI3 DMA发送中断
#define SPI3_DMA_TX_PREE_PRI                        NVIC_INT_PRE_PRI_2
#define SPI3_DMA_TX_SUB_PRI                         NVIC_INT_SUB_PRI_2

//SPI3 DMA接收中断  
#define SPI3_DMA_RX_PREE_PRI                        NVIC_INT_PRE_PRI_2
#define SPI3_DMA_RX_SUB_PRI                         NVIC_INT_SUB_PRI_2

//用户外部中断
#define USER_EXT_INT_PREE_PRI                       NVIC_INT_PRE_PRI_2
#define USER_EXT_INT_SUB_PRI                        NVIC_INT_SUB_PRI_3

//TIMER6中断.用于定时器模式,基础模式下,中断响应级别不用那么高
#define MCU_TIMER6_BASE_PREE_PRI                    NVIC_INT_PRE_PRI_3
#define MCU_TIMER6_BASE_SUB_PRI                     NVIC_INT_SUB_PRI_2

//TIMER7中断,用于定时器模式,基础模式下,中断响应级别不用那么高
#define MCU_TIMER7_BASE_PREE_PRI                    NVIC_INT_PRE_PRI_3
#define MCU_TIMER7_BASE_SUB_PRI                     NVIC_INT_SUB_PRI_2

//RTC闹钟中断
#define MCU_RTC_ALARM_PREE_PRI                      NVIC_INT_PRE_PRI_3
#define MCU_RTC_ALARM_SUB_PRI                       NVIC_INT_SUB_PRI_3

//串口1发送DMA中断
#define MCU_UART1_TX_DMA_PREE_PRI                   NVIC_INT_PRE_PRI_3
#define MCU_UART1_TX_DMA_SUB_PRI                    NVIC_INT_SUB_PRI_3

//串口3发送DMA中断
#define MCU_UART3_TX_DMA_PREE_PRI                   NVIC_INT_PRE_PRI_3
#define MCU_UART3_TX_DMA_SUB_PRI                    NVIC_INT_SUB_PRI_3

//CAN1 TX中断
#define CAN1_TX_PREE_PRI                            NVIC_INT_PRE_PRI_3
#define CAN1_TX_SUB_PRI                             NVIC_INT_SUB_PRI_3

//CAN1 SEC中断
#define CAN1_SEC_PREE_PRI                           NVIC_INT_PRE_PRI_3
#define CAN1_SEC_SUB_PRI                            NVIC_INT_SUB_PRI_3

//CAN2 TX中断
#define CAN2_TX_PREE_PRI                            NVIC_INT_PRE_PRI_3
#define CAN2_TX_SUB_PRI                             NVIC_INT_SUB_PRI_3

//CAN2 SEC中断
#define CAN2_SEC_PREE_PRI                           NVIC_INT_PRE_PRI_3
#define CAN2_SEC_SUB_PRI                            NVIC_INT_SUB_PRI_3

//RTC周期唤醒
#define MCU_RTC_WAKEUP_PREE_PRI                     NVIC_INT_PRE_PRI_3
#define MCU_RTC_WAKEUP_SUB_PRI                      NVIC_INT_SUB_PRI_3

#endif




