/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-08-23 09:44:56 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-10-25 17:49:57 +0800
************************************************************************************************/ 
#ifndef __DRIVER_CONFIG_CAN_H_
#define __DRIVER_CONFIG_CAN_H_
#include "DriverConfigCore.h"

/*-----------------------------------------------芯片CAN--------------------------------------------*/
//单包CAN最大容量
#define MCU_CAN_SINGLE_PACK_DATA_MAX                    8
//总的ID过滤器的数量
#define MCU_CAN_FILTER_COUNT_MAX                        28
//CAN1使用的过滤器的数量
#define MCU_CAN1_FILTER_COUNT_SET                       14
//CAN2使用的过滤器的数量
#define MCU_CAN2_FILTER_COUNT_SET                       (MCU_CAN_FILTER_COUNT_MAX - MCU_CAN1_FILTER_COUNT_SET)
//创建过滤标准帧ID低位
#define MCU_CAN_CALC_STD_ID_LOW(idCode)                 ((((uint32_t)(idCode)<<3)|CAN_ID_STD|CAN_RTR_DATA)&0xFFFF)
//创建过滤标准帧ID高位      
#define MCU_CAN_CALC_STD_ID_HIGH(idCode)                (((((uint32_t)(idCode)<<3)|CAN_ID_STD|CAN_RTR_DATA)&0xFFFF0000)>>16)
//创建过滤标准帧掩码低位,数据帧标准帧帧必须匹配     
#define MCU_CAN_CALC_STD_MASK_LOW(maskCode)             ((((uint32_t)(maskCode)<<3)|0X04|0X03)&0xFFFF)
//创建过滤标准帧掩码高位,数据帧标准帧帧必须匹配     
#define MCU_CAN_CALC_STD_MASK_HIGH(maskCode)            (((((uint32_t)(maskCode)<<3)|0X04|0X03)&0xFFFF0000)>>16)
//创建过滤扩展帧ID低位
#define MCU_CAN_CALC_EXT_ID_LOW(idCode)                 ((((uint32_t)(idCode)<<3)|CAN_ID_EXT|CAN_RTR_DATA)&0xFFFF)
//创建过滤扩展帧ID高位      
#define MCU_CAN_CALC_EXT_ID_HIGH(idCode)                (((((uint32_t)(idCode)<<3)|CAN_ID_EXT|CAN_RTR_DATA)&0xFFFF0000)>>16)
//创建过滤扩展帧掩码低位,数据帧扩展帧必须匹配       
#define MCU_CAN_CALC_EXT_MASK_LOW(maskCode)             ((((uint32_t)(maskCode)<<3)|0X04|0X03)&0xFFFF)
//创建过滤扩展帧掩码高位,数据帧扩展帧必须匹配       
#define MCU_CAN_CALC_EXT_MASK_HIGH(maskCode)            (((((uint32_t)(maskCode)<<3)|0X04|0X03)&0xFFFF0000)>>16)
//发送消息的FIFO长度
#define MCU_CAN_TRANS_FIFO_NBR_COUNT                    3
//必须使用CAN回调
#if(USE_HAL_CAN_REGISTER_CALLBACKS == 0)
#error "Lib CAN Must Use CallBack FuncPtr !!!"
#endif


/*---------------------------------------CAN1总线端口与波特率设置---------------------------------------*/
//CAN端口配置
#define MCU_CAN1_TX_PIN                                 MCU_PIN_A_12
#define MCU_CAN1_RX_PIN                                 MCU_PIN_A_11
//CAN波特率设置
#define MCU_CAN1_BAUD_USER                              MCU_CAN_BPS_1000K
//CAN口收发缓冲区的长度以及消息队列的长度
#define MCU_CAN1_MSG_FIFO_LENGTH                        256
//是否需要配置11位标准帧过滤器
#define MCU_CAN1_STD_FILTER_NBR_EXIST                   CONFIG_FUNC_DISABLE
//是否需要配置29位扩展帧过滤器
#define MCU_CAN1_EXT_FILTER_NBR_EXIST                   CONFIG_FUNC_DISABLE
//自动重发配置,ENABLE标识打开自动重发
#define MCU_CAN1_AUTO_RETRANS                           ENABLE
//FIFO溢出的时候是丢掉原来的数据还是丢掉新的数据,ENABLE标识丢掉新数据
#define MCU_CAN1_RX_FIFO_OVERRUN_IGNORE_NEW             ENABLE 
//自动离线管理是否打开,一旦硬件检测到128次11位连续的隐性位，则自动退出离线状态
#define MCU_CAN1_AUTO_BUS_OFF_MANAGE                    ENABLE  
//自动唤醒是否打开,睡眠模式通过检测CAN报文，由硬件自动唤醒。唤醒的同时，硬件自动对CAN_MSR寄存
//器的SLEEP和SLAK位清’0’
#define MCU_CAN1_AUTO_WAKE_UP_MANAGE                    ENABLE     
//发送优先级由报文ID优先级决定还是发送请求顺序决定,ENABLE表示发送请求顺序决定           
#define MCU_CAN1_TRANS_FIFO_ORDER_FIRST                 ENABLE   
//等待CAN FIFO最长时间,数据发包的时候如果超时还没有缓冲区就认为是异常
#define MCU_CAN1_WAIT_FIFO_SINGLE_PACK_MS               20
//CAN总线离线之后等待再次上线的时间
#define MCU_CAN1_WAIT_BUS_OFF_TIME_MS                   100

/*---------------------------------------CAN2总线端口与波特率设置---------------------------------------*/
//CAN端口配置
#define MCU_CAN2_TX_PIN                                 MCU_PIN_B_6
#define MCU_CAN2_RX_PIN                                 MCU_PIN_B_5
//CAN波特率设置
#define MCU_CAN2_BAUD_USER                              MCU_CAN_BPS_1000K
//CAN口收发缓冲区的长度以及消息队列的长度
#define MCU_CAN2_MSG_FIFO_LENGTH                        256
//是否需要配置11位标准帧过滤器
#define MCU_CAN2_STD_FILTER_NBR_EXIST                   CONFIG_FUNC_DISABLE
//是否需要配置29位扩展帧过滤器
#define MCU_CAN2_EXT_FILTER_NBR_EXIST                   CONFIG_FUNC_DISABLE
//自动重发配置,ENABLE标识打开自动重发
#define MCU_CAN2_AUTO_RETRANS                           ENABLE
//FIFO溢出的时候是丢掉原来的数据还是丢掉新的数据,ENABLE标识丢掉新数据
#define MCU_CAN2_RX_FIFO_OVERRUN_IGNORE_NEW             ENABLE 
//自动离线管理是否打开,一旦硬件检测到128次11位连续的隐性位，则自动退出离线状态
#define MCU_CAN2_AUTO_BUS_OFF_MANAGE                    ENABLE  
//自动唤醒是否打开,睡眠模式通过检测CAN报文，由硬件自动唤醒。唤醒的同时，硬件自动对CAN_MSR寄存
//器的SLEEP和SLAK位清’0’
#define MCU_CAN2_AUTO_WAKE_UP_MANAGE                    ENABLE     
//发送优先级由报文ID优先级决定还是发送请求顺序决定,ENABLE表示发送请求顺序决定           
#define MCU_CAN2_TRANS_FIFO_ORDER_FIRST                 ENABLE   
//等待CAN FIFO最长时间,数据发包的时候如果超时还没有缓冲区就认为是异常
#define MCU_CAN2_WAIT_FIFO_SINGLE_PACK_MS               20
//CAN总线离线之后等待再次上线的时间
#define MCU_CAN2_WAIT_BUS_OFF_TIME_MS                   100
#endif












