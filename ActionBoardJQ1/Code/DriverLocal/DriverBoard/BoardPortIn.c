/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-08-21 10:49:58 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-08-29 10:22:31 +0800
************************************************************************************************/ 
#include "BoardPortIn.h"

//外部导入初始化配置
extern const MCU_PIN_PULL boardPortInInitPullConfigArray[];

//输入初始化函数指针
typedef void (*BoardPortInInitFuncPtr)(void);
//读取输入值状态
typedef GPIO_PinState (*BoardPortInReadValueFuncPtr)(void);

//输入端口初始化
static void BoardPortInInit_PH6_SEN1(void)
{
    MCU_PinInit(MCU_PIN_H_6, MCU_PIN_MODE_INPUT, boardPortInInitPullConfigArray[BOARD_PORT_IN_PH6_SEN1], MCU_PIN_SPEED_FREQ_MEDIUM,0);
}

static void BoardPortInInit_PH7_SEN2(void)
{
    MCU_PinInit(MCU_PIN_H_7, MCU_PIN_MODE_INPUT, boardPortInInitPullConfigArray[BOARD_PORT_IN_PH7_SEN2], MCU_PIN_SPEED_FREQ_MEDIUM,0);
}

static void BoardPortInInit_PH8_SEN3(void)
{
    MCU_PinInit(MCU_PIN_H_8, MCU_PIN_MODE_INPUT, boardPortInInitPullConfigArray[BOARD_PORT_IN_PH8_SEN3], MCU_PIN_SPEED_FREQ_MEDIUM,0);
}
  
static void BoardPortInInit_PH9_SEN4(void)
{
    MCU_PinInit(MCU_PIN_H_9, MCU_PIN_MODE_INPUT, boardPortInInitPullConfigArray[BOARD_PORT_IN_PH9_SEN4], MCU_PIN_SPEED_FREQ_MEDIUM,0);
}
  
static void BoardPortInInit_PG0_SEN5(void)
{
    MCU_PinInit(MCU_PIN_G_0, MCU_PIN_MODE_INPUT, boardPortInInitPullConfigArray[BOARD_PORT_IN_PG0_SEN5], MCU_PIN_SPEED_FREQ_MEDIUM,0);
}
  
static void BoardPortInInit_PG1_SEN6(void)
{
    MCU_PinInit(MCU_PIN_G_1, MCU_PIN_MODE_INPUT, boardPortInInitPullConfigArray[BOARD_PORT_IN_PG1_SEN6], MCU_PIN_SPEED_FREQ_MEDIUM,0);
}
  
static void BoardPortInInit_PE7_SEN7(void)
{
    MCU_PinInit(MCU_PIN_E_7, MCU_PIN_MODE_INPUT, boardPortInInitPullConfigArray[BOARD_PORT_IN_PE7_SEN7], MCU_PIN_SPEED_FREQ_MEDIUM,0);
}
  
static void BoardPortInInit_PE12_SEN8(void)
{
    MCU_PinInit(MCU_PIN_E_12, MCU_PIN_MODE_INPUT, boardPortInInitPullConfigArray[BOARD_PORT_IN_PE12_SEN8], MCU_PIN_SPEED_FREQ_MEDIUM,0);
}
  
static void BoardPortInInit_PF12_SEN9(void)
{
    MCU_PinInit(MCU_PIN_F_12, MCU_PIN_MODE_INPUT, boardPortInInitPullConfigArray[BOARD_PORT_IN_PF12_SEN9], MCU_PIN_SPEED_FREQ_MEDIUM,0);
}
  
static void BoardPortInInit_PF13_SEN10(void)
{
    MCU_PinInit(MCU_PIN_F_13, MCU_PIN_MODE_INPUT, boardPortInInitPullConfigArray[BOARD_PORT_IN_PF13_SEN10], MCU_PIN_SPEED_FREQ_MEDIUM,0);
}
 
static void BoardPortInInit_PF14_SEN11(void)
{
    MCU_PinInit(MCU_PIN_F_14, MCU_PIN_MODE_INPUT, boardPortInInitPullConfigArray[BOARD_PORT_IN_PF14_SEN11], MCU_PIN_SPEED_FREQ_MEDIUM,0);
}
 
static void BoardPortInInit_PF15_SEN12(void)
{
    MCU_PinInit(MCU_PIN_F_15, MCU_PIN_MODE_INPUT, boardPortInInitPullConfigArray[BOARD_PORT_IN_PF15_SEN12], MCU_PIN_SPEED_FREQ_MEDIUM,0);
}
 
static void BoardPortInInit_PA3_SEN13(void)
{
    MCU_PinInit(MCU_PIN_A_3, MCU_PIN_MODE_INPUT, boardPortInInitPullConfigArray[BOARD_PORT_IN_PA3_SEN13], MCU_PIN_SPEED_FREQ_MEDIUM,0);
}

static void BoardPortInInit_PB0_SEN14(void)
{
    MCU_PinInit(MCU_PIN_B_0, MCU_PIN_MODE_INPUT, boardPortInInitPullConfigArray[BOARD_PORT_IN_PB0_SEN14], MCU_PIN_SPEED_FREQ_MEDIUM,0);
}

static void BoardPortInInit_PB1_SEN15(void)
{
    MCU_PinInit(MCU_PIN_B_1, MCU_PIN_MODE_INPUT, boardPortInInitPullConfigArray[BOARD_PORT_IN_PB1_SEN15], MCU_PIN_SPEED_FREQ_MEDIUM,0);
}

static void BoardPortInInit_PF11_SEN16(void)
{
    MCU_PinInit(MCU_PIN_F_11, MCU_PIN_MODE_INPUT, boardPortInInitPullConfigArray[BOARD_PORT_IN_PF11_SEN16], MCU_PIN_SPEED_FREQ_MEDIUM,0);
}

/***************************************读取状态*******************************************/
static GPIO_PinState BoardInReadValue_PH6_SEN1(void)
{
    //读取当前状态
    return MCU_PinReadSingle(MCU_PIN_H_6);
}

static GPIO_PinState BoardInReadValue_PH7_SEN2(void)
{
    //读取当前状态
    return MCU_PinReadSingle(MCU_PIN_H_7);
}
  
static GPIO_PinState BoardInReadValue_PH8_SEN3(void)
{
    //读取当前状态
    return MCU_PinReadSingle(MCU_PIN_H_8);
}
  
static GPIO_PinState BoardInReadValue_PH9_SEN4(void)
{
    //读取当前状态
    return MCU_PinReadSingle(MCU_PIN_H_9);
}
  
static GPIO_PinState BoardInReadValue_PG0_SEN5(void)
{
    //读取当前状态
    return MCU_PinReadSingle(MCU_PIN_G_0);
}

static GPIO_PinState BoardInReadValue_PG1_SEN6(void)
{
    //读取当前状态
    return MCU_PinReadSingle(MCU_PIN_G_1);
}
  
static GPIO_PinState BoardInReadValue_PE7_SEN7(void)
{
    //读取当前状态
    return MCU_PinReadSingle(MCU_PIN_E_7);
}
  
static GPIO_PinState BoardInReadValue_PE12_SEN8(void)
{
    //读取当前状态
    return MCU_PinReadSingle(MCU_PIN_E_12);
}
  
static GPIO_PinState BoardInReadValue_PF12_SEN9(void)
{
    //读取当前状态
    return MCU_PinReadSingle(MCU_PIN_F_12);
}
  
static GPIO_PinState BoardInReadValue_PF13_SEN10(void)
{
    //读取当前状态
    return MCU_PinReadSingle(MCU_PIN_F_13);
}
 
static GPIO_PinState BoardInReadValue_PF14_SEN11(void)
{
    //读取当前状态
    return MCU_PinReadSingle(MCU_PIN_F_14);
}
 
static GPIO_PinState BoardInReadValue_PF15_SEN12(void)
{
    //读取当前状态
    return MCU_PinReadSingle(MCU_PIN_F_15);
}
 
static GPIO_PinState BoardInReadValue_PA3_SEN13(void)
{
    //读取当前状态
    return MCU_PinReadSingle(MCU_PIN_A_3);
}

static GPIO_PinState BoardInReadValue_PB0_SEN14(void)
{
    //读取当前状态
    return MCU_PinReadSingle(MCU_PIN_B_0);
}

static GPIO_PinState BoardInReadValue_PB1_SEN15(void)
{
    //读取当前状态
    return MCU_PinReadSingle(MCU_PIN_B_1);
}

static GPIO_PinState BoardInReadValue_PF11_SEN16(void)
{
    //读取当前状态
    return MCU_PinReadSingle(MCU_PIN_F_11);
}

/**********************************函数指针数组*************************************/
static const BoardPortInInitFuncPtr BoardPortInInitFuncPtrArray[BOARD_PORT_IN_COUNT] = {
    BoardPortInInit_PH6_SEN1   ,
    BoardPortInInit_PH7_SEN2   ,
    BoardPortInInit_PH8_SEN3   ,
    BoardPortInInit_PH9_SEN4   ,
    BoardPortInInit_PG0_SEN5   ,
    BoardPortInInit_PG1_SEN6   ,
    BoardPortInInit_PE7_SEN7   ,
    BoardPortInInit_PE12_SEN8  ,
    BoardPortInInit_PF12_SEN9  ,
    BoardPortInInit_PF13_SEN10 ,
    BoardPortInInit_PF14_SEN11 ,
    BoardPortInInit_PF15_SEN12 ,
    BoardPortInInit_PA3_SEN13  ,
    BoardPortInInit_PB0_SEN14  ,
    BoardPortInInit_PB1_SEN15  ,
    BoardPortInInit_PF11_SEN16 ,
};

//函数指针数组
static const BoardPortInReadValueFuncPtr BoardPortInReadValueFuncPtrArray[BOARD_PORT_IN_COUNT] = {
    BoardInReadValue_PH6_SEN1   ,
    BoardInReadValue_PH7_SEN2   ,
    BoardInReadValue_PH8_SEN3   ,
    BoardInReadValue_PH9_SEN4   ,
    BoardInReadValue_PG0_SEN5   ,
    BoardInReadValue_PG1_SEN6   ,
    BoardInReadValue_PE7_SEN7   ,
    BoardInReadValue_PE12_SEN8  ,
    BoardInReadValue_PF12_SEN9  ,
    BoardInReadValue_PF13_SEN10 ,
    BoardInReadValue_PF14_SEN11 ,
    BoardInReadValue_PF15_SEN12 ,
    BoardInReadValue_PA3_SEN13  ,
    BoardInReadValue_PB0_SEN14  ,
    BoardInReadValue_PB1_SEN15  ,
    BoardInReadValue_PF11_SEN16 ,
};


//输入端口初始化
void BoardPortInInitAll(void)
{
    uint16_t indexUtil = 0; 
    for(indexUtil = 0; indexUtil < BOARD_PORT_IN_COUNT; indexUtil++)
    {
        BoardPortInInitSingle((BOARD_PORT_IN)indexUtil);
    }
}

//指定端口初始化
void BoardPortInInitSingle(BOARD_PORT_IN pinIndex)
{
    if(pinIndex >= BOARD_PORT_IN_COUNT)
    {
        return;
    }
    BoardPortInInitFuncPtrArray[pinIndex]();
}

//输出端口读取指定端口当前状态
GPIO_PinState BoardPortInReadState(BOARD_PORT_IN pinIndex)
{
    if(pinIndex >= BOARD_PORT_IN_COUNT)
    {
        return GPIO_PIN_RESET;
    }
    return BoardPortInReadValueFuncPtrArray[pinIndex]();
}












