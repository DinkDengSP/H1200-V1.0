/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-02-11 14:19:50 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-02-15 11:00:41 +0800
************************************************************************************************/ 
#ifndef __MCU_ADC1_H_
#define __MCU_ADC1_H_
#include "MCU_ADC_Universal.h"
#include "DriverConfigADC.h"

//单通道查询模式ADC触发
#if(MODE_ADC1_USER == MODE_ADC1_POOL)
//初始化
void MCU_ADC1_Init(void);
//ADC通道配置
void MCU_ADC1_ConfigChannel(MCU_ADC_CHANNEL_CONFIG* channelConfigs,uint8_t configCount);
//ADC转换
void MCU_ADC1_ConvertOnce(uint16_t* dataBuffer,uint8_t rankCount);
//获取ADC模块温度值
float MCU_ADC1_GetChipTemp(uint16_t sampleCount);

#endif

//DMA模式ADC触发
#if(MODE_ADC1_USER == MODE_ADC1_DMA)
//ADC初始化
void MCU_ADC1_Init(void);
//ADC通道配置,设置采样点数
void MCU_ADC1_ConfigChannel(MCU_ADC_CHANNEL_CONFIG* channelConfigs,uint8_t rankCount);
//启动转换
void MCU_ADC1_ConvertStart(uint8_t rankCount,uint16_t exceptSampleCount);
//检查转换是否完成
FlagStatus MCU_ADC1_CheckConvertIsOver(void);
//获取转换数据并且清除内存储存的转换历史
void MCU_ADC1_GetConvertResultClearHistory(uint16_t** dataBufferPtrPtr,uint16_t* dataLength);
//释放读取数据
void MCU_ADC1_ReleaseConvertResult(uint16_t* dataBufferPtrPtr);
//获取ADC模块温度值
float MCU_ADC1_GetChipTemp(uint16_t sampleCount);
#endif


#endif



