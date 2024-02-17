/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-08-23 09:44:56 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-08-23 19:47:03 +0800
************************************************************************************************/ 
#ifndef __MCU_TIMER_BASE_H_
#define __MCU_TIMER_BASE_H_
#include "DriverHeaderCore.h"

//通道状态
typedef enum MCU_TIMER_PWM_CHANNEL_STATE
{
    MCU_TIMER_PWM_CHANNEL_STATE_OFF = 0X00,//停止状态
    MCU_TIMER_PWM_CHANNEL_STATE_ON  = 0X01,//正在运行状态
}MCU_TIMER_PWM_CHANNEL_STATE;

//主定时器状态
typedef enum MCU_TIMER_PWM_MAIN_STATE
{
    MCU_TIMER_PWM_MAIN_STATE_OFF = 0X00,//停止状态
    MCU_TIMER_PWM_MAIN_STATE_ON  = 0X01,//正在运行状态
}MCU_TIMER_PWM_MAIN_STATE;


//定时器输出通道配置
typedef struct MCU_TIMER_PWM_CHANNEL_BASE16_CONFIG
{
    uint16_t countOutMatch;
    uint8_t duty;
    MCU_TIMER_PWM_CHANNEL_STATE channelState;
    MCU_TimerPWM_MatchCallBack channelOutCallBack;
}MCU_TIMER_PWM_CHANNEL_BASE16_CONFIG;

//定时器输出通道配置
typedef struct MCU_TIMER_PWM_CHANNEL_BASE32_CONFIG
{
    uint32_t countOutMatch;
    uint8_t duty;
    MCU_TIMER_PWM_CHANNEL_STATE channelState;
    MCU_TimerPWM_MatchCallBack channelOutCallBack;
}MCU_TIMER_PWM_CHANNEL_BASE32_CONFIG;

//定时器PWM基础配置
typedef struct MCU_TIMER_PWM_MAIN_BASE16_CONFIG
{
    uint16_t freqDivParam;//分频数值
    uint16_t countUpValue;//计数溢出值
    uint32_t freq;//计数频率
    MCU_TIMER_PWM_MAIN_STATE mainState;//主定时器状态
    MCU_TimerBaseCallBack callBackFuncPtr;//回调函数
}MCU_TIMER_PWM_MAIN_BASE16_CONFIG;

//定时器PWM基础配置
typedef struct MCU_TIMER_PWM_MAIN_BASE32_CONFIG
{
    uint16_t freqDivParam;//分频数值
    uint32_t countUpValue;//计数溢出值
    uint32_t freq;//计数频率
    MCU_TIMER_PWM_MAIN_STATE mainState;//主定时器状态
    MCU_TimerBaseCallBack callBackFuncPtr;//回调函数
}MCU_TIMER_PWM_MAIN_BASE32_CONFIG;

//定时器PWM模式配置变量
typedef struct MCU_TIMER_PWM_BASE16_CONFIG
{
    MCU_TIMER_PWM_MAIN_BASE16_CONFIG mainConfig;
    MCU_TIMER_PWM_CHANNEL_BASE16_CONFIG channel1Config;
    MCU_TIMER_PWM_CHANNEL_BASE16_CONFIG channel2Config;
    MCU_TIMER_PWM_CHANNEL_BASE16_CONFIG channel3Config;
    MCU_TIMER_PWM_CHANNEL_BASE16_CONFIG channel4Config;
}MCU_TIMER_PWM_BASE16_CONFIG;

//定时器PWM模式配置变量
typedef struct MCU_TIMER_PWM_ADV16_CONFIG
{
    MCU_TIMER_PWM_MAIN_BASE16_CONFIG mainConfig;
    MCU_TIMER_PWM_CHANNEL_BASE16_CONFIG channel1Config;
    MCU_TIMER_PWM_CHANNEL_BASE16_CONFIG channel2Config;
    MCU_TIMER_PWM_CHANNEL_BASE16_CONFIG channel3Config;
    MCU_TIMER_PWM_CHANNEL_BASE16_CONFIG channel4Config;
    MCU_TIMER_PWM_CHANNEL_BASE16_CONFIG channel5Config;
    MCU_TIMER_PWM_CHANNEL_BASE16_CONFIG channel6Config;
}MCU_TIMER_PWM_ADV16_CONFIG;

//定时器PWM模式配置变量
typedef struct MCU_TIMER_PWM_BASE32_CONFIG
{
    MCU_TIMER_PWM_MAIN_BASE32_CONFIG mainConfig;
    MCU_TIMER_PWM_CHANNEL_BASE32_CONFIG channel1Config;
    MCU_TIMER_PWM_CHANNEL_BASE32_CONFIG channel2Config;
    MCU_TIMER_PWM_CHANNEL_BASE32_CONFIG channel3Config;
    MCU_TIMER_PWM_CHANNEL_BASE32_CONFIG channel4Config;
}MCU_TIMER_PWM_BASE32_CONFIG;



#endif







