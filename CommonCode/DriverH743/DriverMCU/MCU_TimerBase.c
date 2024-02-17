/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-08-21 10:19:22 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-08-21 13:56:28 +0800
************************************************************************************************/ 
#include "MCU_TimerBase.h"

//通过频率计算定时器分频系数和计数值,频率上限是输入频率的二分之一,超过直接设置为二分之一
void MCU_TimerCalcClkDivCountForFreq(uint32_t timSrc,uint32_t freq,uint16_t* divResult,uint16_t* countResult)
{
    double calcUtilFloat = 0.0;
    uint32_t divResultLocal = 0;
    uint32_t countResultLocal = 0;
    //防止除0异常
    if(freq == 0)freq = 1;
    //保证计数器的最小值为10,也就是最大频率为定时器源频率的十分之一
    calcUtilFloat = timSrc/freq;
    if(calcUtilFloat < 2.0)
    {
        //最大设计120M
        *divResult = 0;
        *countResult = 1;
        return;
    }
    if(freq <= 10)
    {
        divResultLocal = 3700;
        calcUtilFloat = ((double)timSrc)/((double)divResultLocal);
        countResultLocal = (uint32_t)((calcUtilFloat/((double)freq)));
    }
    else if(freq <= 100)
    {
        divResultLocal = 370;
        calcUtilFloat = ((double)timSrc)/((double)divResultLocal);
        countResultLocal = (uint32_t)((calcUtilFloat/((double)freq)));
    }
    else if(freq <= 1000)
    {
        divResultLocal = 37;
        calcUtilFloat = ((double)timSrc)/((double)divResultLocal);
        countResultLocal = (uint32_t)((calcUtilFloat/((double)freq)));
    }
    else if(freq <= 10000)
    {
        divResultLocal = 4;
        calcUtilFloat = ((double)timSrc)/((double)divResultLocal);
        countResultLocal = (uint32_t)((calcUtilFloat/((double)freq)));
    }
    else
    {
        divResultLocal = 1;
        calcUtilFloat = ((double)timSrc)/((double)divResultLocal);
        countResultLocal = (uint32_t)((calcUtilFloat/((double)freq)));
    }
    //最终结果为计算结果-1
    *divResult = (uint16_t)(divResultLocal - 1);
    *countResult = (uint16_t)(countResultLocal - 1);
    return;
}

//通过频率计算定时器分频系数和计数值,频率上线是输入频率的百分之一,主要是用于PWM输出,好设置占空比
void MCU_TimerCalcClkDivCountForFreqPwmOut(uint32_t timSrc,uint32_t freq,uint16_t* divResult,uint16_t* countResult)
{
    double calcUtilFloat = 0.0;
    uint32_t divResultLocal = 0;
    uint32_t countResultLocal = 0;
    //防止除0异常
    if(freq == 0)freq = 1;
    if(freq >= timSrc/100)
    {
        countResultLocal = 100;
        divResultLocal = 1;
    }
    else if(freq >= 1000000)
    {
        countResultLocal = 100;
        calcUtilFloat = ((double)timSrc)/((double)countResultLocal);
        divResultLocal = (uint32_t)((calcUtilFloat/((double)freq)));
    }
    else if(freq >= 100000)
    {
        countResultLocal = 100;
        calcUtilFloat = ((double)timSrc)/((double)countResultLocal);
        divResultLocal = (uint32_t)((calcUtilFloat/((double)freq)));
    }
    else if(freq >= 10000)
    {
        countResultLocal = 100;
        calcUtilFloat = ((double)timSrc)/((double)countResultLocal);
        divResultLocal = (uint32_t)((calcUtilFloat/((double)freq)));
    }
    else if(freq >= 1000)
    {
        countResultLocal = 100;
        calcUtilFloat = ((double)timSrc)/((double)countResultLocal);
        divResultLocal = (uint32_t)((calcUtilFloat/((double)freq)));
    }
    else if(freq >= 100)
    {
        countResultLocal = 1000;
        calcUtilFloat = ((double)timSrc)/((double)countResultLocal);
        divResultLocal = (uint32_t)((calcUtilFloat/((double)freq)));
    }
    else if(freq >= 10)
    {
        countResultLocal = 1000;
        calcUtilFloat = ((double)timSrc)/((double)countResultLocal);
        divResultLocal = (uint32_t)((calcUtilFloat/((double)freq)));
    }
    else
    {
        countResultLocal = 10000;
        calcUtilFloat = ((double)timSrc)/((double)countResultLocal);
        divResultLocal = (uint32_t)((calcUtilFloat/((double)freq)));
    }
    //最终结果为计算结果-1
    *divResult = (uint16_t)(divResultLocal - 1);
    *countResult = (uint16_t)(countResultLocal - 1);
    return;
}







