/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-03-16 14:46:33 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-03-16 14:46:34 +0800
************************************************************************************************/ 
#include "DriverConfigCore.h"
#include "stm32f4xx_hal.h"

//系统主PLL初始化
static void CoreClock_MainPLL_Init(void)
{
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};

	//打开芯片电源管理时钟
	__HAL_RCC_PWR_CLK_ENABLE();
	//选用的PWR_REGULATOR_VOLTAGE_SCALE1,正常可以到168M,开始OVER_DRIVER的情况下可以到168M
	__HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
    /* 使能HSE，并选择HSE作为PLL时钟源 */
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE|RCC_OSCILLATORTYPE_HSI|
                                        RCC_OSCILLATORTYPE_LSE|RCC_OSCILLATORTYPE_LSI;
    //将内外四个时钟都打开
	RCC_OscInitStruct.HSEState = RCC_HSE_ON;
    RCC_OscInitStruct.HSIState = RCC_HSI_ON;
    RCC_OscInitStruct.LSIState = RCC_LSI_ON;
#if(CORE_RTC_USE_LSE == CONFIG_FUNC_ENABLE)
    RCC_OscInitStruct.LSEState = RCC_LSE_ON;
#else
    RCC_OscInitStruct.LSEState = RCC_LSE_OFF;
#endif
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    //选择HSE作为PLL输出时钟
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
	RCC_OscInitStruct.PLL.PLLM = CORE_CLOCK_MAIN_PLL_M;
	RCC_OscInitStruct.PLL.PLLN = CORE_CLOCK_MAIN_PLL_N;
	RCC_OscInitStruct.PLL.PLLP = CORE_CLOCK_MAIN_PLL_P;
	RCC_OscInitStruct.PLL.PLLQ = CORE_CLOCK_MAIN_PLL_Q;
	if(HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
	{
        //PLL初始化失败,卡在这里
        while(1);
	}
}


//系统内核时钟和总线时钟初始化
static void CoreClock_Bus_Init(void)
{
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
    //各个设备总线分频,选中PLL为系统时钟源
    RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK|RCC_CLOCKTYPE_HCLK|
                                        RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2);
    //设置系统时钟时钟源为PLL,168MHZ
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    //AHB分频系数为1 168MHZ
    RCC_ClkInitStruct.AHBCLKDivider = CORE_CLOCK_BUS_AHB;
    //APB1分频系数
    RCC_ClkInitStruct.APB1CLKDivider = CORE_CLOCK_BUS_APB1; 
    //APB2分频系数
    RCC_ClkInitStruct.APB2CLKDivider = CORE_CLOCK_BUS_APB2; 
    //供电电压3.3V的时候,HCLK为168M情况下需要5WS的Flash访问周期,也就是6个CPU周期
    if(HAL_RCC_ClockConfig(&RCC_ClkInitStruct,CORE_FLASH_WAIT_CYCLE) != HAL_OK)
    {
        //内核时钟设置以及分频失败,卡在这里
        while(1);
    }
    //使用IO的高速模式，要使能IO补偿，即调用下面三个函数 
    //使能时钟安全机制
    HAL_RCC_EnableCSS();
    //使能SYSCFG clock,系统配置时钟使能,该单元负责补偿
	__HAL_RCC_SYSCFG_CLK_ENABLE();
    //使能I/O补偿单元， 设置SYSCFG_CCCSR寄存器的bit0
	HAL_EnableCompensationCell();
}

//系统时钟初始化
void CoreClockInit(void)
{
    //系统主PLL时钟初始化
    CoreClock_MainPLL_Init();
    //系统内核与总线初始化
    CoreClock_Bus_Init();
    //使能备份域访问
    HAL_PWR_EnableBkUpAccess();
    //使能备份SRAM时钟
    __HAL_RCC_BKPSRAM_CLK_ENABLE();
    //等待备份调压器就绪
    HAL_PWREx_EnableBkUpReg();
}