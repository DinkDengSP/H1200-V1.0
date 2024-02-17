/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-08-23 09:44:56 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-08-23 13:57:52 +0800
************************************************************************************************/ 
#include "MCU_Random.h"
#include "MCU_LogShow.h"

//RNG句柄
static RNG_HandleTypeDef RNG_Handler;  
//初始化等待完成的超时时间
#define TIME_MS_WAIT_RNG_STABLE                 10

//随机数初始化
void MCU_RandomInit(void)
{
    HAL_StatusTypeDef halState = HAL_OK;
    uint32_t waitStartTimeStamp = 0;
	//使能时钟
	__HAL_RCC_RNG_CLK_ENABLE();
    //设定
    RNG_Handler.Instance=RNG;  
    //复位 
    HAL_RNG_DeInit(&RNG_Handler);
    //初始化RNG
    halState = HAL_RNG_Init(&RNG_Handler);
    if(halState != HAL_OK)
    {
        MCU_LogShowHalState(halState,RNG_Handler.ErrorCode);
        while(1);
    }
    //设置开始等待时间戳
    waitStartTimeStamp = tx_time_get();
    //等待RNG准备就绪
    while(__HAL_RNG_GET_FLAG(&RNG_Handler,RNG_FLAG_DRDY)==RESET)
    {
        asm("nop");
        if(tx_time_get() - waitStartTimeStamp > MS_TO_OS_TICK(TIME_MS_WAIT_RNG_STABLE))
        {
            //错误
            MCU_LogPrintf("%s,%d,WaitTimeOut!!!\r\n",__func__,__LINE__);
            while(1);
        }
    }
    //初始化随机数函数库
    srand((int32_t)MCU_RandomGetNext());
}

//得到随机数
uint32_t MCU_RandomGetNext(void)
{
    HAL_StatusTypeDef halState = HAL_OK;
    uint32_t randomnum;
    //锁定线程
    uint32_t lastIntState = CoreLockSchedule();
    //生成随机数
    halState = HAL_RNG_GenerateRandomNumber(&RNG_Handler,&randomnum);
    if(halState != HAL_OK)
    {
        MCU_LogShowHalState(halState,RNG_Handler.ErrorCode);
        //解除锁定
        CoreUnlockSchedule(lastIntState);
        while(1);
    }
    //解除锁定
    CoreUnlockSchedule(lastIntState);
    //返回数据
	return randomnum;
}

//正确计算随机数的方法
int32_t MCU_RandomGetNextInteger(int32_t min, int32_t max)
{
    if (max <= min)
    {
        return 0;
    }
    //基础值
    uint32_t baseResult = MCU_RandomGetNext();
    //将取得的值在32位数据中计算到的位置计算出来,阈值在1
    //然后根据本次阈值扩张,然后取整
    uint32_t rangeBase = (uint32_t)((max - min) * (((float)baseResult) / ((float)UINT32_MAX)));
    //最后确定符号位
    return (int32_t)(min + rangeBase);    
}

//正确计算随机数的方法
uint32_t MCU_RandomGetNextUnsigned(uint32_t min, uint32_t max)
{
    if (max <= min)
    {
        return 0;
    }
    //基础值
    uint32_t baseResult = MCU_RandomGetNext();
    uint32_t rangeBase = (uint32_t)((max - min) * (((float)baseResult) / ((float)UINT32_MAX)));
    return min + rangeBase;   
}










