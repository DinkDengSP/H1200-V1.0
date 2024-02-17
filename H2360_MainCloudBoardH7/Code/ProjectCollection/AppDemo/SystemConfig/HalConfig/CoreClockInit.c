/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-07-18 16:09:51 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-07-18 20:51:12 +0800
************************************************************************************************/ 
#include "DriverConfigCore.h"

//系统主PLL锁相环初始化,该时钟主要供给总线矩阵和系统内核
static void CoreClock_MainPLL_Init(void)
{
    HAL_StatusTypeDef ret = HAL_OK;
    //初始化需要的结构体
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    //启用电源配置更新,打开LDO,系统电源运行到VORE域,全功率供电
	ret = HAL_PWREx_ConfigSupply(PWR_LDO_SUPPLY);
    if(ret != HAL_OK)
    {
        //等待LDO稳定超时或者失败
        while(1);
    }
    //使能SYSCFG
    __HAL_RCC_SYSCFG_CLK_ENABLE();
    //配置主内部稳压器输出电压,使用SCALE0,最大能跑到480MHZ,SCALE1最大能跑到400MHZ
    //这个情况下,Flash 0个等待周期70MHZ, 多1个周期多70M,480M下是6,400M下是5
    ret = HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1);
    if(ret != HAL_OK)
    {
        //设置LDO调压器级别失败
        while(1);
    }
    //等待D3区域VOS 输出准备就绪
    while ((PWR->D3CR & (PWR_D3CR_VOSRDY)) != PWR_D3CR_VOSRDY) 
    //配置外部时钟的驱动能力，在LSE启动前
	__HAL_RCC_LSEDRIVE_CONFIG(RCC_LSEDRIVE_HIGH);
    //初始化CPU AHB APB时钟
    //使能HSE，并选择HSE作为PLL时钟源
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE|RCC_OSCILLATORTYPE_LSI|
                                        RCC_OSCILLATORTYPE_LSE|RCC_OSCILLATORTYPE_HSI|
                                        RCC_OSCILLATORTYPE_CSI|RCC_OSCILLATORTYPE_HSI48;
    RCC_OscInitStruct.HSEState = RCC_HSE_ON;
    RCC_OscInitStruct.HSIState = RCC_HSI_ON;
    RCC_OscInitStruct.LSIState = RCC_LSI_ON;
#if(CORE_CLK_LSE_SET == CONFIG_FUNC_ENABLE)
    RCC_OscInitStruct.LSEState = RCC_LSE_ON;
#else
    RCC_OscInitStruct.LSEState = RCC_LSE_OFF;
#endif
    RCC_OscInitStruct.CSIState = RCC_CSI_ON;
    RCC_OscInitStruct.HSI48State = RCC_HSI48_ON;
    //PLL配置开始
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    //PLL使用HSE锁相环
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
    //配置PLL锁相环的配置 
    //pllm:PLL1 预分频系数(进PLL 之前的分频),分频后频率1-16M之间,取值范围:2~63.  
    RCC_OscInitStruct.PLL.PLLM = CORE_MAIN_PLL_M;
    //plln:PLL1 倍频系数(PLL 倍频),取值范围:4~512. 
    RCC_OscInitStruct.PLL.PLLN = CORE_MAIN_PLL_N;
    //pllp:PLL1 的p分频系数(PLL 之后的分频),plln分频后作为系统时钟,取值范围:2~128 
    RCC_OscInitStruct.PLL.PLLP = CORE_MAIN_PLL_P;
    //pllr:PLL1 的r分频系数(PLL 之后的分频),分频后作为系统时钟,取值范围:2~128 
    //PLL1的R可选作为trace时钟,可以与CPU时钟一致
    RCC_OscInitStruct.PLL.PLLR = CORE_MAIN_PLL_R;
    //pllq:PLL1 的q 分频系数(PLL 之后的分频),取值范围:1~128. 
    RCC_OscInitStruct.PLL.PLLQ = CORE_MAIN_PLL_Q;	
    //开始初始化PLL锁相环
    //设置调压器输出电压级别,选择PLL1 VCO 为宽VCO范围 192M到836M,中等VCO范围是150-420M
    RCC_OscInitStruct.PLL.PLLVCOSEL = RCC_PLL1VCOWIDE;
    //输入PLL的频率,RCC_PLL1VCIRANGE_2支持4-8M
    RCC_OscInitStruct.PLL.PLLRGE = RCC_PLL1VCIRANGE_2;	
    //PLL1 VCO 乘数因子的小数部分
    RCC_OscInitStruct.PLL.PLLFRACN = 0;
    //PLL初始化
    ret = HAL_RCC_OscConfig(&RCC_OscInitStruct);
    if(ret != HAL_OK)
    {
        while(1);
    }
}


//总线时钟初始化
//系统主时钟初始化
//System Clock source  = PLL1 (HSE)
//SYSCLK(Hz)           = PLL1 P (CPU Clock)
//AHB Prescaler        = 2(AHB Clock MAX 240M)
//HCLK(Hz)             = AXI and AHBs Clock MAX 240M
//D1 APB3 Prescaler    = 2 (APB3 Clock  MAX 120MHz)
//D2 APB1 Prescaler    = 2 (APB1 Clock  MAX 120MHz)
//D2 APB2 Prescaler    = 2 (APB2 Clock  MAX 120MHz)
//D3 APB4 Prescaler    = 2 (APB4 Clock  MAX 120MHz)
//VDD(V)               = 3.3
//Flash Latency(WS)    = 6 ~~~(6+1)*7=49
static void CoreClock_MainBus_Init(void)
{
    HAL_StatusTypeDef ret = HAL_OK;
    //初始化需要的结构体
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
    //选择PLL的输出作为系统时钟
    //配置RCC_CLOCKTYPE_SYSCLK系统时钟
    //配置RCC_CLOCKTYPE_HCLK 时钟，对应AHB1，AHB2，AHB3和AHB4总线
    //配置RCC_CLOCKTYPE_PCLK1时钟，对应APB1总线
    //配置RCC_CLOCKTYPE_PCLK2时钟，对应APB2总线
    //配置RCC_CLOCKTYPE_D1PCLK1时钟，对应APB3总线
    //配置RCC_CLOCKTYPE_D3PCLK1时钟，对应APB4总线  
	RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | 
                                   RCC_CLOCKTYPE_D1PCLK1 | RCC_CLOCKTYPE_PCLK1 | 
                                   RCC_CLOCKTYPE_PCLK2  | RCC_CLOCKTYPE_D3PCLK1);
    //系统时钟就是锁相环时钟 400MHZ
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    //内核时钟分频
	RCC_ClkInitStruct.SYSCLKDivider = RCC_SYSCLK_DIV1;
    //HCLK为系统时钟2分频,AHB均为,AXI BUS AHB1 2 4 HCLK3
	RCC_ClkInitStruct.AHBCLKDivider = RCC_HCLK_DIV2;
    //APB1 2 3 4均从AHB二分频率
    //定时器的时钟都是在D2域进行控制，而且当 D2PPRE1或 D2PPRE2 的分频系数不为1的时候，
    //定时器的时钟频率为rcc_pclk1或rcc_pclk2的2倍,也就是APB*2MHZ定时器
    //APB3 
    RCC_ClkInitStruct.APB3CLKDivider = RCC_APB3_DIV2;  
    //APB1  APB1上的定时器*2
    RCC_ClkInitStruct.APB1CLKDivider = RCC_APB1_DIV2; 
    //APB2  APB2上的定时器*2
    RCC_ClkInitStruct.APB2CLKDivider = RCC_APB2_DIV2; 
    //APB4
    RCC_ClkInitStruct.APB4CLKDivider = RCC_APB4_DIV2; 
	//此函数会更新SystemCoreClock，并重新配置HAL_InitTick,Flash 0个等待周期70MHZ, 多1个周期多70M.
    //480M下是6,400M下是5*/
    ret = HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5);
    if(ret != HAL_OK)
    {
        while(1);//配置失败,死循环
    }
    //使用IO的高速模式，要使能IO补偿，即调用下面三个函数 
    //使能CSI clock
	__HAL_RCC_CSI_ENABLE() ;
    //使能时钟安全机制
    HAL_RCC_EnableCSS();
    //使能SYSCFG clock,系统配置时钟使能,该单元负责补偿
	__HAL_RCC_SYSCFG_CLK_ENABLE();
    //使能I/O补偿单元， 设置SYSCFG_CCCSR寄存器的bit0
	HAL_EnableCompensationCell();
    /* AXI SRAM的时钟是上电自动使能的，而D2域的SRAM1，SRAM2和SRAM3要单独使能 */	
    __HAL_RCC_D2SRAM1_CLK_ENABLE();
    __HAL_RCC_D2SRAM2_CLK_ENABLE();
    __HAL_RCC_D2SRAM3_CLK_ENABLE();
    //D3域内存访问时钟使能
	__HAL_RCC_BKPRAM_CLKAM_ENABLE();       
	__HAL_RCC_D3SRAM1_CLKAM_ENABLE();
    //定时器时钟设置在总线1,2分频的情况下,定时器时钟为总线频率,否则为两倍总线频率
    //如果设置为 RCC_TIMPRES_ACTIVATED,定时器时钟设置在总线1,2,4分频的情况下,定时器时钟为总线频率
    //否则为四倍总线频率
    __HAL_RCC_TIMCLKPRESCALER(RCC_TIMPRES_DESACTIVATED);
}

//FMC外设PLL初始化
void CoreClock_FMC_Init(void)
{
    RCC_PeriphCLKInitTypeDef RCC_PeriphClkInit = {0};
    //FMC时钟,使用PLL2R作为FMC时钟源
    RCC_PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_FMC;
    //选择PLL2实际使用的是PLL2R
    RCC_PeriphClkInit.FmcClockSelection = RCC_FMCCLKSOURCE_PLL2;
    RCC_PeriphClkInit.PLL2.PLL2M = CLK_PLL2_M_PARAM;
    RCC_PeriphClkInit.PLL2.PLL2N = CLK_PLL2_N_PARAM;
    RCC_PeriphClkInit.PLL2.PLL2P = CLK_PLL2_P_PARAM;
    RCC_PeriphClkInit.PLL2.PLL2Q = CLK_PLL2_Q_PARAM;
    RCC_PeriphClkInit.PLL2.PLL2R = CLK_PLL2_R_PARAM;
    //输入PLL的频率,RCC_PLL1VCIRANGE_2支持4-8M
    RCC_PeriphClkInit.PLL2.PLL2RGE = RCC_PLL1VCIRANGE_2;
    //设置调压器输出电压级别,选择PLL1 VCO 为宽VCO范围 192M到836M,中等VCO范围是150-420M
    RCC_PeriphClkInit.PLL2.PLL2VCOSEL = RCC_PLL2VCOWIDE;
    RCC_PeriphClkInit.PLL2.PLL2FRACN = 0;
    //时钟配置
    if (HAL_RCCEx_PeriphCLKConfig(&RCC_PeriphClkInit) != HAL_OK)
    {
        while(1);
    }
    /* 使能 FMC 时钟 */
    __FMC_CLK_ENABLE();
}

//LTDC初始化,IIC也要用这个时钟
void CoreClock_LTDC_USB_Init(void)
{
    RCC_PeriphCLKInitTypeDef RCC_PeriphClkInit = {0};
    //先停止PLL3
    __HAL_RCC_PLL3_DISABLE();
    //然后设置PLL输出
    __HAL_RCC_PLL3CLKOUT_ENABLE(RCC_PLL3_DIVP);
    __HAL_RCC_PLL3CLKOUT_ENABLE(RCC_PLL3_DIVQ);
    __HAL_RCC_PLL3CLKOUT_ENABLE(RCC_PLL3_DIVR);
    //LTDC时钟,仅有一个PLL3R可以选择,IIC使用PLL3提前在这里初始化
    RCC_PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_LTDC;
    //PLL设置
    RCC_PeriphClkInit.PLL3.PLL3M = CLK_PLL3_M_PARAM;
    RCC_PeriphClkInit.PLL3.PLL3N = CLK_PLL3_N_PARAM;
    RCC_PeriphClkInit.PLL3.PLL3P = CLK_PLL3_P_PARAM;
    RCC_PeriphClkInit.PLL3.PLL3Q = CLK_PLL3_Q_PARAM;
    RCC_PeriphClkInit.PLL3.PLL3R = CLK_PLL3_R_PARAM;
    //输入PLL的频率,我们是25M晶振5细分,所以是5M,RCC_PLL1VCIRANGE_2支持4-8M
    RCC_PeriphClkInit.PLL3.PLL3RGE = RCC_PLL3VCIRANGE_2;
    //设置调压器输出电压级别,选择PLL1 VCO 为宽VCO范围 192M到836M,中等VCO范围是150-420M
    RCC_PeriphClkInit.PLL3.PLL3VCOSEL = RCC_PLL3VCOMEDIUM;
    RCC_PeriphClkInit.PLL3.PLL3FRACN = 0;
    if (HAL_RCCEx_PeriphCLKConfig(&RCC_PeriphClkInit) != HAL_OK)
    {
        while(1);
    }
    //输出LTDC时钟
	__HAL_RCC_LTDC_CLK_ENABLE();
}


//主时钟初始化
void CoreClockInit(void)
{
    //内核PLL初始化
    CoreClock_MainPLL_Init();
    //内核总线初始化
    CoreClock_MainBus_Init();
    //FMC时钟初始化
    CoreClock_FMC_Init();
    //LTDC USB初始化
    CoreClock_LTDC_USB_Init();
}











