/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-08-21 10:19:22 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-08-21 13:56:28 +0800
************************************************************************************************/ 
#include "MCU_IWDG.h"
#include "MCU_LogShow.h"


//独立看门狗句柄
static IWDG_HandleTypeDef IWDG_Handler; 

//初始化独立看门狗
//rlr:自动重装载值,0~0XFFF.最大4095
//默认使用32分频,
//时间计算(大概):Tout=((4*2^prer)*rlr)/32 (ms).
void MCU_IWDG_Init(uint16_t periodMs)
{
    //使能LSI
    __HAL_RCC_LSI_ENABLE();
    //等待LSI Ready
    while(__HAL_RCC_GET_FLAG(RCC_FLAG_LSIRDY) == 0U);
    //检查独立看门狗复位标志位
	if (__HAL_RCC_GET_FLAG(RCC_FLAG_IWDG1RST) != RESET)
	{
        MCU_LogSendString("Last Reset Src IWDG\r\n");
		//清除复位标志位
		__HAL_RCC_CLEAR_RESET_FLAGS();
	}
    IWDG_Handler.Instance=IWDG1;
    //设置IWDG分频系数,使用LSI供给时钟,大约32KHZ,分频32,代表1MS一次计数
    IWDG_Handler.Init.Prescaler=IWDG_PRESCALER_32;	
    //重装载值,超过多少毫秒会复位
    if(periodMs > (0X0FFF-1))
    {
        periodMs = (0X0FFF-1);
    }
    IWDG_Handler.Init.Reload=periodMs;	
    IWDG_Handler.Init.Window = IWDG_WINDOW_DISABLE;
    //初始化IWDG  	
    HAL_IWDG_Init(&IWDG_Handler);	
    //启动看门狗
    __HAL_IWDG_START(&IWDG_Handler);	
    //设置调试模式看门狗冻结
    __HAL_DBGMCU_FREEZE_IWDG1();
}
    
//喂独立看门狗
void MCU_IWDG_Feed(void)
{   
    HAL_IWDG_Refresh(&IWDG_Handler); 	
}






