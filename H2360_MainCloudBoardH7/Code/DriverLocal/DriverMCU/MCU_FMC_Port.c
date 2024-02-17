/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-11-03 16:12:17 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-11-06 13:32:11 +0800
************************************************************************************************/ 
#include "MCU_FMC_Port.h"
#include "MCU_PIN.h"

//初始化标志,防止反复初始化
static uint8_t mcu_FmcPinInitFlag = 0;

//板上FMC端口初始化
void MCU_FMC_PortInit(void)
{
    //防止重复初始化
    if(mcu_FmcPinInitFlag != 0)
    {
        return;
    }
    mcu_FmcPinInitFlag = 1;
	__HAL_RCC_FMC_CLK_ENABLE();
    /* 复位FMC */
    __HAL_RCC_FMC_FORCE_RESET();
    __HAL_RCC_FMC_RELEASE_RESET();
    //对应IO口初始化
    //A0 - A18
    MCU_PinInit(MCU_PIN_F_0, MCU_PIN_MODE_AF_PP, MCU_PIN_PULL_PULLUP, MCU_PIN_SPEED_FREQ_VERY_HIGH,GPIO_AF12_FMC);
    MCU_PinInit(MCU_PIN_F_1, MCU_PIN_MODE_AF_PP, MCU_PIN_PULL_PULLUP, MCU_PIN_SPEED_FREQ_VERY_HIGH,GPIO_AF12_FMC);
    MCU_PinInit(MCU_PIN_F_2, MCU_PIN_MODE_AF_PP, MCU_PIN_PULL_PULLUP, MCU_PIN_SPEED_FREQ_VERY_HIGH,GPIO_AF12_FMC);
    MCU_PinInit(MCU_PIN_F_3, MCU_PIN_MODE_AF_PP, MCU_PIN_PULL_PULLUP, MCU_PIN_SPEED_FREQ_VERY_HIGH,GPIO_AF12_FMC);
    MCU_PinInit(MCU_PIN_F_4, MCU_PIN_MODE_AF_PP, MCU_PIN_PULL_PULLUP, MCU_PIN_SPEED_FREQ_VERY_HIGH,GPIO_AF12_FMC);
    MCU_PinInit(MCU_PIN_F_5, MCU_PIN_MODE_AF_PP, MCU_PIN_PULL_PULLUP, MCU_PIN_SPEED_FREQ_VERY_HIGH,GPIO_AF12_FMC);
    MCU_PinInit(MCU_PIN_F_12, MCU_PIN_MODE_AF_PP, MCU_PIN_PULL_PULLUP, MCU_PIN_SPEED_FREQ_VERY_HIGH,GPIO_AF12_FMC);
    MCU_PinInit(MCU_PIN_F_13, MCU_PIN_MODE_AF_PP, MCU_PIN_PULL_PULLUP, MCU_PIN_SPEED_FREQ_VERY_HIGH,GPIO_AF12_FMC);
    MCU_PinInit(MCU_PIN_F_14, MCU_PIN_MODE_AF_PP, MCU_PIN_PULL_PULLUP, MCU_PIN_SPEED_FREQ_VERY_HIGH,GPIO_AF12_FMC);
    MCU_PinInit(MCU_PIN_F_15, MCU_PIN_MODE_AF_PP, MCU_PIN_PULL_PULLUP, MCU_PIN_SPEED_FREQ_VERY_HIGH,GPIO_AF12_FMC);
    MCU_PinInit(MCU_PIN_G_0, MCU_PIN_MODE_AF_PP, MCU_PIN_PULL_PULLUP, MCU_PIN_SPEED_FREQ_VERY_HIGH,GPIO_AF12_FMC);
    MCU_PinInit(MCU_PIN_G_1, MCU_PIN_MODE_AF_PP, MCU_PIN_PULL_PULLUP, MCU_PIN_SPEED_FREQ_VERY_HIGH,GPIO_AF12_FMC);
    MCU_PinInit(MCU_PIN_G_2, MCU_PIN_MODE_AF_PP, MCU_PIN_PULL_PULLUP, MCU_PIN_SPEED_FREQ_VERY_HIGH,GPIO_AF12_FMC);
    MCU_PinInit(MCU_PIN_G_3, MCU_PIN_MODE_AF_PP, MCU_PIN_PULL_PULLUP, MCU_PIN_SPEED_FREQ_VERY_HIGH,GPIO_AF12_FMC);
    MCU_PinInit(MCU_PIN_G_4, MCU_PIN_MODE_AF_PP, MCU_PIN_PULL_PULLUP, MCU_PIN_SPEED_FREQ_VERY_HIGH,GPIO_AF12_FMC);
    MCU_PinInit(MCU_PIN_G_5, MCU_PIN_MODE_AF_PP, MCU_PIN_PULL_PULLUP, MCU_PIN_SPEED_FREQ_VERY_HIGH,GPIO_AF12_FMC);
    MCU_PinInit(MCU_PIN_D_11, MCU_PIN_MODE_AF_PP, MCU_PIN_PULL_PULLUP, MCU_PIN_SPEED_FREQ_VERY_HIGH,GPIO_AF12_FMC);
    MCU_PinInit(MCU_PIN_D_12, MCU_PIN_MODE_AF_PP, MCU_PIN_PULL_PULLUP, MCU_PIN_SPEED_FREQ_VERY_HIGH,GPIO_AF12_FMC);
    MCU_PinInit(MCU_PIN_D_13, MCU_PIN_MODE_AF_PP, MCU_PIN_PULL_PULLUP, MCU_PIN_SPEED_FREQ_VERY_HIGH,GPIO_AF12_FMC);

    //D0 - D15
    MCU_PinInit(MCU_PIN_D_14, MCU_PIN_MODE_AF_PP, MCU_PIN_PULL_PULLUP, MCU_PIN_SPEED_FREQ_VERY_HIGH,GPIO_AF12_FMC);
    MCU_PinInit(MCU_PIN_D_15, MCU_PIN_MODE_AF_PP, MCU_PIN_PULL_PULLUP, MCU_PIN_SPEED_FREQ_VERY_HIGH,GPIO_AF12_FMC);
    MCU_PinInit(MCU_PIN_D_0, MCU_PIN_MODE_AF_PP, MCU_PIN_PULL_PULLUP, MCU_PIN_SPEED_FREQ_VERY_HIGH,GPIO_AF12_FMC);
    MCU_PinInit(MCU_PIN_D_1, MCU_PIN_MODE_AF_PP, MCU_PIN_PULL_PULLUP, MCU_PIN_SPEED_FREQ_VERY_HIGH,GPIO_AF12_FMC);
    MCU_PinInit(MCU_PIN_E_7, MCU_PIN_MODE_AF_PP, MCU_PIN_PULL_PULLUP, MCU_PIN_SPEED_FREQ_VERY_HIGH,GPIO_AF12_FMC);
    MCU_PinInit(MCU_PIN_E_8, MCU_PIN_MODE_AF_PP, MCU_PIN_PULL_PULLUP, MCU_PIN_SPEED_FREQ_VERY_HIGH,GPIO_AF12_FMC);
    MCU_PinInit(MCU_PIN_E_9, MCU_PIN_MODE_AF_PP, MCU_PIN_PULL_PULLUP, MCU_PIN_SPEED_FREQ_VERY_HIGH,GPIO_AF12_FMC);
    MCU_PinInit(MCU_PIN_E_10, MCU_PIN_MODE_AF_PP, MCU_PIN_PULL_PULLUP, MCU_PIN_SPEED_FREQ_VERY_HIGH,GPIO_AF12_FMC);
    MCU_PinInit(MCU_PIN_E_11, MCU_PIN_MODE_AF_PP, MCU_PIN_PULL_PULLUP, MCU_PIN_SPEED_FREQ_VERY_HIGH,GPIO_AF12_FMC);
    MCU_PinInit(MCU_PIN_E_12, MCU_PIN_MODE_AF_PP, MCU_PIN_PULL_PULLUP, MCU_PIN_SPEED_FREQ_VERY_HIGH,GPIO_AF12_FMC);
    MCU_PinInit(MCU_PIN_E_13, MCU_PIN_MODE_AF_PP, MCU_PIN_PULL_PULLUP, MCU_PIN_SPEED_FREQ_VERY_HIGH,GPIO_AF12_FMC);
    MCU_PinInit(MCU_PIN_E_14, MCU_PIN_MODE_AF_PP, MCU_PIN_PULL_PULLUP, MCU_PIN_SPEED_FREQ_VERY_HIGH,GPIO_AF12_FMC);
    MCU_PinInit(MCU_PIN_E_15, MCU_PIN_MODE_AF_PP, MCU_PIN_PULL_PULLUP, MCU_PIN_SPEED_FREQ_VERY_HIGH,GPIO_AF12_FMC);
    MCU_PinInit(MCU_PIN_D_8, MCU_PIN_MODE_AF_PP, MCU_PIN_PULL_PULLUP, MCU_PIN_SPEED_FREQ_VERY_HIGH,GPIO_AF12_FMC);
    MCU_PinInit(MCU_PIN_D_9, MCU_PIN_MODE_AF_PP, MCU_PIN_PULL_PULLUP, MCU_PIN_SPEED_FREQ_VERY_HIGH,GPIO_AF12_FMC);
    MCU_PinInit(MCU_PIN_D_10, MCU_PIN_MODE_AF_PP, MCU_PIN_PULL_PULLUP, MCU_PIN_SPEED_FREQ_VERY_HIGH,GPIO_AF12_FMC);

    //NE3
    MCU_PinInit(MCU_PIN_G_10, MCU_PIN_MODE_AF_PP, MCU_PIN_PULL_PULLUP, MCU_PIN_SPEED_FREQ_VERY_HIGH,GPIO_AF12_FMC);
    //NWE
    MCU_PinInit(MCU_PIN_D_5, MCU_PIN_MODE_AF_PP, MCU_PIN_PULL_PULLUP, MCU_PIN_SPEED_FREQ_VERY_HIGH,GPIO_AF12_FMC);
    //NOE
    MCU_PinInit(MCU_PIN_D_4, MCU_PIN_MODE_AF_PP, MCU_PIN_PULL_PULLUP, MCU_PIN_SPEED_FREQ_VERY_HIGH,GPIO_AF12_FMC);
    //NBL0
    MCU_PinInit(MCU_PIN_E_0, MCU_PIN_MODE_AF_PP, MCU_PIN_PULL_PULLUP, MCU_PIN_SPEED_FREQ_VERY_HIGH,GPIO_AF12_FMC);
    //NBL1
    MCU_PinInit(MCU_PIN_E_1, MCU_PIN_MODE_AF_PP, MCU_PIN_PULL_PULLUP, MCU_PIN_SPEED_FREQ_VERY_HIGH,GPIO_AF12_FMC);
}


//板上FMC端口初始化
void MCU_FMC_PortDeInit(void)
{
    mcu_FmcPinInitFlag = 0;
    //模块复位
    __HAL_RCC_FMC_FORCE_RESET();
    __HAL_RCC_FMC_RELEASE_RESET();
    /* 禁止FMC时钟 */
	__HAL_RCC_FMC_CLK_DISABLE();

    //A0 - A18
    MCU_PinDeInit(MCU_PIN_F_0);
    MCU_PinDeInit(MCU_PIN_F_1);
    MCU_PinDeInit(MCU_PIN_F_2);
    MCU_PinDeInit(MCU_PIN_F_3);
    MCU_PinDeInit(MCU_PIN_F_4);
    MCU_PinDeInit(MCU_PIN_F_5);
    MCU_PinDeInit(MCU_PIN_F_12);
    MCU_PinDeInit(MCU_PIN_F_13);
    MCU_PinDeInit(MCU_PIN_F_14);
    MCU_PinDeInit(MCU_PIN_F_15);
    MCU_PinDeInit(MCU_PIN_G_0);
    MCU_PinDeInit(MCU_PIN_G_1);
    MCU_PinDeInit(MCU_PIN_G_2);
    MCU_PinDeInit(MCU_PIN_G_3);
    MCU_PinDeInit(MCU_PIN_G_4);
    MCU_PinDeInit(MCU_PIN_G_5);
    MCU_PinDeInit(MCU_PIN_D_11);
    MCU_PinDeInit(MCU_PIN_D_12);
    MCU_PinDeInit(MCU_PIN_D_13);

    //D0 - D15
    MCU_PinDeInit(MCU_PIN_D_14);
    MCU_PinDeInit(MCU_PIN_D_15);
    MCU_PinDeInit(MCU_PIN_D_0);
    MCU_PinDeInit(MCU_PIN_D_1);
    MCU_PinDeInit(MCU_PIN_E_7);
    MCU_PinDeInit(MCU_PIN_E_8);
    MCU_PinDeInit(MCU_PIN_E_9);
    MCU_PinDeInit(MCU_PIN_E_10);
    MCU_PinDeInit(MCU_PIN_E_11);
    MCU_PinDeInit(MCU_PIN_E_12);
    MCU_PinDeInit(MCU_PIN_E_13);
    MCU_PinDeInit(MCU_PIN_E_14);
    MCU_PinDeInit(MCU_PIN_E_15);
    MCU_PinDeInit(MCU_PIN_D_8);
    MCU_PinDeInit(MCU_PIN_D_9);
    MCU_PinDeInit(MCU_PIN_D_10);

    //NE3
    MCU_PinDeInit(MCU_PIN_G_10);
    //NWE
    MCU_PinDeInit(MCU_PIN_D_5);
    //NOE
    MCU_PinDeInit(MCU_PIN_D_4);
    //NBL0
    MCU_PinDeInit(MCU_PIN_E_0);
    //NBL1
    MCU_PinDeInit(MCU_PIN_E_1);
}
















