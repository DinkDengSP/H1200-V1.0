/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-08-21 10:49:58 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-10-30 17:23:09 +0800
************************************************************************************************/ 
#include "DriverConfigMCU.h"
#include "BoardPortIn.h"
#include "BoardPortOut.h"



//主板输入初始化
//配置输入上拉还是下拉
const MCU_PIN_PULL boardPortInInitPullConfigArray[BOARD_PORT_IN_COUNT] = {
    MCU_PIN_PULL_PULLDOWN,//BOARD_PORT_IN_CPLD_NOP1 
    MCU_PIN_PULL_PULLDOWN,//BOARD_PORT_IN_CPLD_NOP2 
    MCU_PIN_PULL_PULLDOWN,//BOARD_PORT_IN_CPLD_NOP3 
    MCU_PIN_PULL_PULLDOWN,//BOARD_PORT_IN_CPLD_NOP4 
    MCU_PIN_PULL_PULLDOWN,//BOARD_PORT_IN_CPLD_NOP5 
    MCU_PIN_PULL_PULLDOWN,//BOARD_PORT_IN_CPLD_NOP6 
    MCU_PIN_PULL_PULLDOWN,//BOARD_PORT_IN_CPLD_NOP7 
    MCU_PIN_PULL_PULLDOWN,//BOARD_PORT_IN_CPLD_NOP8 
    MCU_PIN_PULL_PULLDOWN,//BOARD_PORT_IN_CPLD_NOP9 
    MCU_PIN_PULL_PULLDOWN,//BOARD_PORT_IN_CPLD_NOP10
    MCU_PIN_PULL_PULLDOWN,//BOARD_PORT_IN_CPLD_NOP11
    MCU_PIN_PULL_PULLDOWN,//BOARD_PORT_IN_CPLD_NOP12
    MCU_PIN_PULL_PULLDOWN,//BOARD_PORT_IN_CPLD_NOP13
    MCU_PIN_PULL_PULLDOWN,//BOARD_PORT_IN_ARM_SEN1_PC5   
    MCU_PIN_PULL_PULLDOWN,//BOARD_PORT_IN_ARM_SEN2_PB0   
    MCU_PIN_PULL_PULLDOWN,//BOARD_PORT_IN_ARM_SEN3_PB1   
    MCU_PIN_PULL_PULLDOWN,//BOARD_PORT_IN_ARM_SEN4_PB10  
    MCU_PIN_PULL_PULLDOWN,//BOARD_PORT_IN_ARM_SEN5_PB11  
    MCU_PIN_PULL_PULLDOWN,//BOARD_PORT_IN_ARM_SEN6_PH6   
    MCU_PIN_PULL_PULLDOWN,//BOARD_PORT_IN_ARM_SEN7_PH7   
    MCU_PIN_PULL_PULLDOWN,//BOARD_PORT_IN_ARM_SEN8_PH8   
    MCU_PIN_PULL_PULLDOWN,//BOARD_PORT_IN_ARM_SEN9_PH9   
    MCU_PIN_PULL_PULLDOWN,//BOARD_PORT_IN_ARM_SEN10_PH10 
    MCU_PIN_PULL_PULLDOWN,//BOARD_PORT_IN_ARM_SEN11_PH11 
    MCU_PIN_PULL_PULLDOWN,//BOARD_PORT_IN_ARM_SEN12_PH12 
    MCU_PIN_PULL_PULLDOWN,//BOARD_PORT_IN_ARM_SEN13_PA4  
    MCU_PIN_PULL_PULLDOWN,//BOARD_PORT_IN_ARM_SEN14_PA5  
    MCU_PIN_PULL_PULLDOWN,//BOARD_PORT_IN_ARM_SEN15_PA6  
    MCU_PIN_PULL_PULLDOWN,//BOARD_PORT_IN_ARM_SEN16_PA7  
    MCU_PIN_PULL_PULLDOWN,//BOARD_PORT_IN_ARM_SEN17_PC4  
    MCU_PIN_PULL_PULLDOWN,//BOARD_PORT_IN_ARM_SEN18_PH3  
    MCU_PIN_PULL_PULLDOWN,//BOARD_PORT_IN_ARM_SEN19_PH2  
    MCU_PIN_PULL_PULLDOWN,//BOARD_PORT_IN_ARM_SEN20_PC3  
    MCU_PIN_PULL_PULLDOWN,//BOARD_PORT_IN_ARM_SEN21_PC2  
    MCU_PIN_PULL_PULLDOWN,//BOARD_PORT_IN_ARM_SEN22_PC1  
    MCU_PIN_PULL_PULLDOWN,//BOARD_PORT_IN_ARM_SEN23_PC0  
    MCU_PIN_PULL_PULLDOWN,//BOARD_PORT_IN_ARM_SEN24_PF10 
    MCU_PIN_PULL_PULLDOWN,//BOARD_PORT_IN_ARM_SEN25_PB3  
};


//板上IO初始化的电平状态配置
const GPIO_PinState  boardPortOutInitLevelConfigArray[BOARD_PORT_OUT_COUNT] = {
    GPIO_PIN_SET,//BOARD_PORT_OUT_CPLD_CDC1     
    GPIO_PIN_SET,//BOARD_PORT_OUT_CPLD_CDC2     
    GPIO_PIN_SET,//BOARD_PORT_OUT_CPLD_CDC3     
    GPIO_PIN_SET,//BOARD_PORT_OUT_CPLD_CDC4     
    GPIO_PIN_SET,//BOARD_PORT_OUT_CPLD_CDC5     
    GPIO_PIN_SET,//BOARD_PORT_OUT_CPLD_CDC6     
    GPIO_PIN_SET,//BOARD_PORT_OUT_CPLD_CDC7     
    GPIO_PIN_SET,//BOARD_PORT_OUT_CPLD_CDC8     
    GPIO_PIN_SET,//BOARD_PORT_OUT_CPLD_CDC9     
    GPIO_PIN_SET,//BOARD_PORT_OUT_CPLD_CDC10    
    GPIO_PIN_SET,//BOARD_PORT_OUT_CPLD_CDC11    
    GPIO_PIN_SET,//BOARD_PORT_OUT_CPLD_CDC12    
    GPIO_PIN_SET,//BOARD_PORT_OUT_CPLD_CDC13    
    GPIO_PIN_SET,//BOARD_PORT_OUT_CPLD_CDC14    
    GPIO_PIN_SET,//BOARD_PORT_OUT_CPLD_CDC15    
    GPIO_PIN_SET,//BOARD_PORT_OUT_CPLD_CDC16    
    GPIO_PIN_SET,//BOARD_PORT_OUT_CPLD_CDC17    
    GPIO_PIN_SET,//BOARD_PORT_OUT_CPLD_CDC18    
    GPIO_PIN_SET,//BOARD_PORT_OUT_CPLD_CDC19    
    GPIO_PIN_SET,//BOARD_PORT_OUT_CPLD_CDC20    
    GPIO_PIN_SET,//BOARD_PORT_OUT_CPLD_CDC21    
    GPIO_PIN_SET,//BOARD_PORT_OUT_CPLD_CDC22    
    GPIO_PIN_SET,//BOARD_PORT_OUT_CPLD_CDC23    
    GPIO_PIN_SET,//BOARD_PORT_OUT_CPLD_CDC24    
    GPIO_PIN_SET,//BOARD_PORT_OUT_CPLD_CDC25    
    GPIO_PIN_SET,//BOARD_PORT_OUT_CPLD_CDC26    
    GPIO_PIN_SET,//BOARD_PORT_OUT_CPLD_CDC27    
    GPIO_PIN_SET,//BOARD_PORT_OUT_CPLD_CDC28    
    GPIO_PIN_SET,//BOARD_PORT_OUT_CPLD_CDC29    
    GPIO_PIN_SET,//BOARD_PORT_OUT_CPLD_CDC30    
    GPIO_PIN_SET,//BOARD_PORT_OUT_CPLD_CDC31    
    GPIO_PIN_SET,//BOARD_PORT_OUT_CPLD_CDC32    
    GPIO_PIN_SET,//BOARD_PORT_OUT_CPLD_CDC33    
    GPIO_PIN_SET,//BOARD_PORT_OUT_CPLD_CDC34    
    GPIO_PIN_SET,//BOARD_PORT_OUT_CPLD_CDC35    
    GPIO_PIN_SET,//BOARD_PORT_OUT_CPLD_CDC36    
    GPIO_PIN_RESET,//BOARD_PORT_OUT_MCU_SEN1_PF6  
    GPIO_PIN_RESET,//BOARD_PORT_OUT_MCU_SEN2_PI10 
    GPIO_PIN_RESET,//BOARD_PORT_OUT_MCU_SEN3_PI9  
    GPIO_PIN_RESET,//BOARD_PORT_OUT_MCU_SEN4_PC13 
    GPIO_PIN_RESET,//BOARD_PORT_OUT_MCU_SEN5_PI11 
    GPIO_PIN_SET,//BOARD_PORT_OUT_MCU_SEN6_PG13 
    GPIO_PIN_SET,//BOARD_PORT_OUT_MCU_SEN7_PG11 
    GPIO_PIN_SET,//BOARD_PORT_OUT_MCU_SEN8_PG10 
    GPIO_PIN_SET,//BOARD_PORT_OUT_MCU_SEN9_PG9  
    GPIO_PIN_SET,//BOARD_PORT_OUT_MCU_SEN10_PD6 
    GPIO_PIN_SET,//BOARD_PORT_OUT_MCU_SEN11_PF8 
    GPIO_PIN_SET,//BOARD_PORT_OUT_MCU_SEN12_PF9 
    GPIO_PIN_SET,//BOARD_PORT_OUT_MCU_SEN13_PF7 
    GPIO_PIN_SET,//BOARD_PORT_OUT_MCU_SEN14_PB4 
    GPIO_PIN_SET,//BOARD_PORT_OUT_MCU_SEN15_PB8 
    GPIO_PIN_SET,//BOARD_PORT_OUT_MCU_SEN16_PB9 
    GPIO_PIN_SET,//BOARD_PORT_OUT_MCU_SEN17_PE3 
    GPIO_PIN_SET,//BOARD_PORT_OUT_MCU_SEN18_PE4 
    GPIO_PIN_SET,//BOARD_PORT_OUT_MCU_SEN19_PG14
    GPIO_PIN_SET,//BOARD_PORT_OUT_MCU_SEN20_PI5 
    GPIO_PIN_SET,//BOARD_PORT_OUT_MCU_SEN21_PI6 
    GPIO_PIN_SET,//BOARD_PORT_OUT_MCU_SEN22_PI7 
};


//板上IO初始化的上下拉配置
const MCU_PIN_PULL  boardPortOutInitPullConfigArray[BOARD_PORT_OUT_COUNT] = {
    MCU_PIN_PULL_PULLUP,//BOARD_PORT_OUT_CPLD_CDC1     
    MCU_PIN_PULL_PULLUP,//BOARD_PORT_OUT_CPLD_CDC2     
    MCU_PIN_PULL_PULLUP,//BOARD_PORT_OUT_CPLD_CDC3     
    MCU_PIN_PULL_PULLUP,//BOARD_PORT_OUT_CPLD_CDC4     
    MCU_PIN_PULL_PULLUP,//BOARD_PORT_OUT_CPLD_CDC5     
    MCU_PIN_PULL_PULLUP,//BOARD_PORT_OUT_CPLD_CDC6     
    MCU_PIN_PULL_PULLUP,//BOARD_PORT_OUT_CPLD_CDC7     
    MCU_PIN_PULL_PULLUP,//BOARD_PORT_OUT_CPLD_CDC8     
    MCU_PIN_PULL_PULLUP,//BOARD_PORT_OUT_CPLD_CDC9     
    MCU_PIN_PULL_PULLUP,//BOARD_PORT_OUT_CPLD_CDC10    
    MCU_PIN_PULL_PULLUP,//BOARD_PORT_OUT_CPLD_CDC11    
    MCU_PIN_PULL_PULLUP,//BOARD_PORT_OUT_CPLD_CDC12    
    MCU_PIN_PULL_PULLUP,//BOARD_PORT_OUT_CPLD_CDC13    
    MCU_PIN_PULL_PULLUP,//BOARD_PORT_OUT_CPLD_CDC14    
    MCU_PIN_PULL_PULLUP,//BOARD_PORT_OUT_CPLD_CDC15    
    MCU_PIN_PULL_PULLUP,//BOARD_PORT_OUT_CPLD_CDC16    
    MCU_PIN_PULL_PULLUP,//BOARD_PORT_OUT_CPLD_CDC17    
    MCU_PIN_PULL_PULLUP,//BOARD_PORT_OUT_CPLD_CDC18    
    MCU_PIN_PULL_PULLUP,//BOARD_PORT_OUT_CPLD_CDC19    
    MCU_PIN_PULL_PULLUP,//BOARD_PORT_OUT_CPLD_CDC20    
    MCU_PIN_PULL_PULLUP,//BOARD_PORT_OUT_CPLD_CDC21    
    MCU_PIN_PULL_PULLUP,//BOARD_PORT_OUT_CPLD_CDC22    
    MCU_PIN_PULL_PULLUP,//BOARD_PORT_OUT_CPLD_CDC23    
    MCU_PIN_PULL_PULLUP,//BOARD_PORT_OUT_CPLD_CDC24    
    MCU_PIN_PULL_PULLUP,//BOARD_PORT_OUT_CPLD_CDC25    
    MCU_PIN_PULL_PULLUP,//BOARD_PORT_OUT_CPLD_CDC26    
    MCU_PIN_PULL_PULLUP,//BOARD_PORT_OUT_CPLD_CDC27    
    MCU_PIN_PULL_PULLUP,//BOARD_PORT_OUT_CPLD_CDC28    
    MCU_PIN_PULL_PULLUP,//BOARD_PORT_OUT_CPLD_CDC29    
    MCU_PIN_PULL_PULLUP,//BOARD_PORT_OUT_CPLD_CDC30    
    MCU_PIN_PULL_PULLUP,//BOARD_PORT_OUT_CPLD_CDC31    
    MCU_PIN_PULL_PULLUP,//BOARD_PORT_OUT_CPLD_CDC32    
    MCU_PIN_PULL_PULLUP,//BOARD_PORT_OUT_CPLD_CDC33    
    MCU_PIN_PULL_PULLUP,//BOARD_PORT_OUT_CPLD_CDC34    
    MCU_PIN_PULL_PULLUP,//BOARD_PORT_OUT_CPLD_CDC35    
    MCU_PIN_PULL_PULLUP,//BOARD_PORT_OUT_CPLD_CDC36    
    MCU_PIN_PULL_PULLUP,//BOARD_PORT_OUT_MCU_SEN1_PF6  
    MCU_PIN_PULL_PULLUP,//BOARD_PORT_OUT_MCU_SEN2_PI10 
    MCU_PIN_PULL_PULLUP,//BOARD_PORT_OUT_MCU_SEN3_PI9  
    MCU_PIN_PULL_PULLUP,//BOARD_PORT_OUT_MCU_SEN4_PC13 
    MCU_PIN_PULL_PULLUP,//BOARD_PORT_OUT_MCU_SEN5_PI11 
    MCU_PIN_PULL_PULLUP,//BOARD_PORT_OUT_MCU_SEN6_PG13 
    MCU_PIN_PULL_PULLUP,//BOARD_PORT_OUT_MCU_SEN7_PG11 
    MCU_PIN_PULL_PULLUP,//BOARD_PORT_OUT_MCU_SEN8_PG10 
    MCU_PIN_PULL_PULLUP,//BOARD_PORT_OUT_MCU_SEN9_PG9  
    MCU_PIN_PULL_PULLUP,//BOARD_PORT_OUT_MCU_SEN10_PD6 
    MCU_PIN_PULL_PULLUP,//BOARD_PORT_OUT_MCU_SEN11_PF8 
    MCU_PIN_PULL_PULLUP,//BOARD_PORT_OUT_MCU_SEN12_PF9 
    MCU_PIN_PULL_PULLUP,//BOARD_PORT_OUT_MCU_SEN13_PF7 
    MCU_PIN_PULL_PULLUP,//BOARD_PORT_OUT_MCU_SEN14_PB4 
    MCU_PIN_PULL_PULLUP,//BOARD_PORT_OUT_MCU_SEN15_PB8 
    MCU_PIN_PULL_PULLUP,//BOARD_PORT_OUT_MCU_SEN16_PB9 
    MCU_PIN_PULL_PULLUP,//BOARD_PORT_OUT_MCU_SEN17_PE3 
    MCU_PIN_PULL_PULLUP,//BOARD_PORT_OUT_MCU_SEN18_PE4 
    MCU_PIN_PULL_PULLUP,//BOARD_PORT_OUT_MCU_SEN19_PG14
    MCU_PIN_PULL_PULLUP,//BOARD_PORT_OUT_MCU_SEN20_PI5 
    MCU_PIN_PULL_PULLUP,//BOARD_PORT_OUT_MCU_SEN21_PI6 
    MCU_PIN_PULL_PULLUP,//BOARD_PORT_OUT_MCU_SEN22_PI7 
};

















