/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-08-23 09:44:56 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-10-25 13:37:51 +0800
************************************************************************************************/ 
#include "MCU_WWDG.h"
#include "MCU_LogShow.h"

//窗口看门狗句柄
static WWDG_HandleTypeDef WWDG_Handle;     
#if(MCU_WWDG_ENABLE_EWI_SET != CONFIG_FUNC_DISABLE)
static MCU_WWDG_EarlyFeedCallBack callBackFunc = NULL;
#endif


//初始化窗口看门狗 	
//分频数值默认为8
//当使用168M主频的时候,PCLK为42MHZ,计数一次的时间为 1000MS/(42000000/4096/8)
//=1000MS/1281.73 = 0.78MS,当计数值最大0X7F的时候,复位周期(0X7F-0X40=0X3F)=49.15MS
//Fwwdg=PCLK1/(4096*2^fprer). 一般PCLK1=45Mhz
#if(MCU_WWDG_ENABLE_EWI_SET == CONFIG_FUNC_DISABLE)
void MCU_WWDG_Init(void)
#else
void MCU_WWDG_Init(MCU_WWDG_EarlyFeedCallBack callBackPtr)
#endif
{
    //检查独立看门狗复位标志位
	if (__HAL_RCC_GET_FLAG(RCC_FLAG_WWDGRST) != RESET)
	{
        MCU_LogSendString("Last Reset Src WWDG\r\n");
		//清除复位标志位
		__HAL_RCC_CLEAR_RESET_FLAGS();
	}
    //开启 WWDG 时钟
	__HAL_RCC_WWDG_CLK_ENABLE();  
    // 配置WWDG句柄即寄存器基地址
	WWDG_Handle.Instance = WWDG;
    //清除中断标志
    __HAL_WWDG_CLEAR_FLAG(&WWDG_Handle,WWDG_FLAG_EWIF);
    __HAL_WWDG_CLEAR_IT(&WWDG_Handle,WWDG_IT_EWI);
#if(MCU_WWDG_ENABLE_EWI_SET != CONFIG_FUNC_DISABLE)
    //开启中断
    HAL_NVIC_SetPriority(WWDG_IRQn,MCU_WWDG_PREE_PRI,MCU_WWDG_SUB_PRI);  
    //使能窗口看门狗中断 
    HAL_NVIC_EnableIRQ(WWDG_IRQn);
#endif
    //设置调试模式看门狗冻结
    __HAL_DBGMCU_FREEZE_WWDG(); 
#if(MCU_WWDG_ENABLE_EWI_SET != CONFIG_FUNC_DISABLE)
    //设置回调函数
    callBackFunc = callBackPtr;
#endif
    //设置分频系数,默认8细分
    WWDG_Handle.Init.Prescaler=WWDG_PRESCALER_8;   
    //设置窗口值
    WWDG_Handle.Init.Window=MCU_WWDG_WINDOW_VAL;    
    //设置计数器值     
    WWDG_Handle.Init.Counter=MCU_WWDG_RELOAD_VAL; 
#if(MCU_WWDG_ENABLE_EWI_SET == CONFIG_FUNC_DISABLE)
    //设置提前唤醒中断
    WWDG_Handle.Init.EWIMode = WWDG_EWI_DISABLE;
#else
    //设置提前唤醒中断
    WWDG_Handle.Init.EWIMode = WWDG_EWI_ENABLE;
#endif
    //初始化WWDG     
    HAL_WWDG_Init(&WWDG_Handle);   
}

//窗口看门狗喂狗
void MCU_WWDG_Feed(void)
{
    uint32_t currentCount = READ_REG(WWDG_Handle.Instance->CR);
    //获取当前计数值
    currentCount &= 0X007F;
    //检查是否在窗口上
    if((currentCount >= 0X40)&&(currentCount <= MCU_WWDG_WINDOW_VAL))
    {
        //可以喂狗
        HAL_WWDG_Refresh(&WWDG_Handle);
    }
}

#if(MCU_WWDG_ENABLE_EWI_SET == CONFIG_FUNC_ENABLE)
//窗口看门狗中断服务函数
void WWDG_IRQHandler(void)
{
    //调用WWDG共用中断处理函数
    HAL_WWDG_IRQHandler(&WWDG_Handle);
}

//中断服务函数处理过程
//此函数会被HAL_WWDG_IRQHandler()调用
void HAL_WWDG_EarlyWakeupCallback(WWDG_HandleTypeDef *hwwdg)
{
    //更新窗口看门狗值
    HAL_WWDG_Refresh(&WWDG_Handle);
    //调用用户函数
    if(callBackFunc != NULL)
    {
        //函数回调
        callBackFunc();
    }
}
#endif





