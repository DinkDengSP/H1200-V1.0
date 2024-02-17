/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-11-21 09:14:05 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-11-21 09:14:05 +0800
************************************************************************************************/ 
#include "MCU_ADC_Universal.h"
#include "MCU_ADC1.h"
#include "DriverConfigADC.h"

//DMA中断模式ADC触发,导入回调函数
#if(MODE_ADC1_USER == MODE_ADC1_DMA)
void MCU_ADC1_ConvCpltCallbackDMA(void);
void MCU_ADC1_ConvHalfCpltCallbackDMA(void);
void MCU_ADC1_LevelOutOfWindowCallbackDMA(void);
void MCU_ADC1_ErrorCallbackDMA(void);
void MCU_ADC1_InjectedConvCpltCallbackDMA(void);
#endif



//规则转换完成回调
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{
    if(hadc->Instance == ADC1)
    {
        //DMA模式
#if(MODE_ADC1_USER == MODE_ADC1_DMA)
        MCU_ADC1_ConvCpltCallbackDMA();
#endif
    }
}

//规则转换半完成回调
void HAL_ADC_ConvHalfCpltCallback(ADC_HandleTypeDef* hadc)
{
    if(hadc->Instance == ADC1)
    {
        //DMA模式
#if(MODE_ADC1_USER == MODE_ADC1_DMA)
        MCU_ADC1_ConvHalfCpltCallbackDMA();
#endif
    }
}
//规则数据溢出回调
void HAL_ADC_LevelOutOfWindowCallback(ADC_HandleTypeDef* hadc)
{
    if(hadc->Instance == ADC1)
    {
        //DMA模式
#if(MODE_ADC1_USER == MODE_ADC1_DMA)
        MCU_ADC1_LevelOutOfWindowCallbackDMA();
#endif
    }
}
//规则错误回调
void HAL_ADC_ErrorCallback(ADC_HandleTypeDef *hadc)
{
    if(hadc->Instance == ADC1)
    {
        //DMA模式
#if(MODE_ADC1_USER == MODE_ADC1_DMA)
        MCU_ADC1_ErrorCallbackDMA();
#endif
    }
}

void HAL_ADCEx_InjectedConvCpltCallback(ADC_HandleTypeDef *hadc)
{
    if(hadc->Instance == ADC1)
    {
        //DMA模式
#if(MODE_ADC1_USER == MODE_ADC1_DMA)
        MCU_ADC1_InjectedConvCpltCallbackDMA();
#endif
    }
}





