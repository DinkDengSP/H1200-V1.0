/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-10-30 16:33:45 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-10-30 17:02:38 +0800
************************************************************************************************/ 
#include "BoardPortIn.h"
#include "BoardCPLD.h"

//外部导入初始化配置
extern const MCU_PIN_PULL boardPortInInitPullConfigArray[];

//输入初始化函数指针
typedef void (*BoardPortInInitFuncPtr)(void);
//读取输入值状态
typedef GPIO_PinState (*BoardPortInReadValueFuncPtr)(void);

/***************************************输入端口初始化*******************************************/
void BoardPortInInit_CPLD_NOP1(void)
{
    return;
}

void BoardPortInInit_CPLD_NOP2(void)
{
    return;
}

void BoardPortInInit_CPLD_NOP3(void)
{
    return;
}

void BoardPortInInit_CPLD_NOP4(void)
{
    return;
}

void BoardPortInInit_CPLD_NOP5(void)
{
    return;
}

void BoardPortInInit_CPLD_NOP6(void)
{
    return;
}

void BoardPortInInit_CPLD_NOP7(void)
{
    return;
}

void BoardPortInInit_CPLD_NOP8(void)
{
    return;
}

void BoardPortInInit_CPLD_NOP9(void)
{
    return;
}

void BoardPortInInit_CPLD_NOP10(void)
{
    return;
}

void BoardPortInInit_CPLD_NOP11(void)
{
    return;
}

void BoardPortInInit_CPLD_NOP12(void)
{
    return;
}

void BoardPortInInit_CPLD_NOP13(void)
{
    return;
}

//MCU输入端口初始化
void BoardPortInInit_ARM_SEN1_PC5(void)
{
    MCU_PinInit(MCU_PIN_C_5, MCU_PIN_MODE_INPUT,boardPortInInitPullConfigArray[BOARD_PORT_IN_ARM_SEN1_PC5], 
                    MCU_PIN_SPEED_FREQ_MEDIUM,0);
}

//MCU输入端口初始化
void BoardPortInInit_ARM_SEN2_PB0(void)
{
    MCU_PinInit(MCU_PIN_B_0, MCU_PIN_MODE_INPUT,boardPortInInitPullConfigArray[BOARD_PORT_IN_ARM_SEN2_PB0], 
                    MCU_PIN_SPEED_FREQ_MEDIUM,0);
}

//MCU输入端口初始化
void BoardPortInInit_ARM_SEN3_PB1(void)
{
    MCU_PinInit(MCU_PIN_B_1, MCU_PIN_MODE_INPUT,boardPortInInitPullConfigArray[BOARD_PORT_IN_ARM_SEN3_PB1], 
                    MCU_PIN_SPEED_FREQ_MEDIUM,0);
}

//MCU输入端口初始化
void BoardPortInInit_ARM_SEN4_PB10(void)
{
    MCU_PinInit(MCU_PIN_B_10, MCU_PIN_MODE_INPUT,boardPortInInitPullConfigArray[BOARD_PORT_IN_ARM_SEN4_PB10], 
                    MCU_PIN_SPEED_FREQ_MEDIUM,0);
}

//MCU输入端口初始化
void BoardPortInInit_ARM_SEN5_PB11(void)
{
    MCU_PinInit(MCU_PIN_B_11, MCU_PIN_MODE_INPUT,boardPortInInitPullConfigArray[BOARD_PORT_IN_ARM_SEN5_PB11], 
                    MCU_PIN_SPEED_FREQ_MEDIUM,0);
}

//MCU输入端口初始化
void BoardPortInInit_ARM_SEN6_PH6(void)
{
    MCU_PinInit(MCU_PIN_H_6, MCU_PIN_MODE_INPUT,boardPortInInitPullConfigArray[BOARD_PORT_IN_ARM_SEN6_PH6], 
                    MCU_PIN_SPEED_FREQ_MEDIUM,0);
}

//MCU输入端口初始化
void BoardPortInInit_ARM_SEN7_PH7(void)
{
    MCU_PinInit(MCU_PIN_H_7, MCU_PIN_MODE_INPUT,boardPortInInitPullConfigArray[BOARD_PORT_IN_ARM_SEN7_PH7], 
                    MCU_PIN_SPEED_FREQ_MEDIUM,0);
}

//MCU输入端口初始化
void BoardPortInInit_ARM_SEN8_PH8(void)
{
    MCU_PinInit(MCU_PIN_H_8, MCU_PIN_MODE_INPUT,boardPortInInitPullConfigArray[BOARD_PORT_IN_ARM_SEN8_PH8], 
                    MCU_PIN_SPEED_FREQ_MEDIUM,0);
}

//MCU输入端口初始化
void BoardPortInInit_ARM_SEN9_PH9(void)
{
    MCU_PinInit(MCU_PIN_H_9, MCU_PIN_MODE_INPUT,boardPortInInitPullConfigArray[BOARD_PORT_IN_ARM_SEN9_PH9], 
                    MCU_PIN_SPEED_FREQ_MEDIUM,0);
}

//MCU输入端口初始化
void BoardPortInInit_ARM_SEN10_PH10(void)
{
    MCU_PinInit(MCU_PIN_H_10, MCU_PIN_MODE_INPUT,boardPortInInitPullConfigArray[BOARD_PORT_IN_ARM_SEN10_PH10], 
                    MCU_PIN_SPEED_FREQ_MEDIUM,0);
}

//MCU输入端口初始化
void BoardPortInInit_ARM_SEN11_PH11(void)
{
    MCU_PinInit(MCU_PIN_H_11, MCU_PIN_MODE_INPUT,boardPortInInitPullConfigArray[BOARD_PORT_IN_ARM_SEN11_PH11], 
                    MCU_PIN_SPEED_FREQ_MEDIUM,0);
}

//MCU输入端口初始化
void BoardPortInInit_ARM_SEN12_PH12(void)
{
    MCU_PinInit(MCU_PIN_H_12, MCU_PIN_MODE_INPUT,boardPortInInitPullConfigArray[BOARD_PORT_IN_ARM_SEN12_PH12], 
                    MCU_PIN_SPEED_FREQ_MEDIUM,0);
}

//MCU输入端口初始化
void BoardPortInInit_ARM_SEN13_PA4(void)
{
    MCU_PinInit(MCU_PIN_A_4, MCU_PIN_MODE_INPUT,boardPortInInitPullConfigArray[BOARD_PORT_IN_ARM_SEN13_PA4], 
                    MCU_PIN_SPEED_FREQ_MEDIUM,0);
}

//MCU输入端口初始化
void BoardPortInInit_ARM_SEN14_PA5(void)
{
    MCU_PinInit(MCU_PIN_A_5, MCU_PIN_MODE_INPUT,boardPortInInitPullConfigArray[BOARD_PORT_IN_ARM_SEN14_PA5], 
                    MCU_PIN_SPEED_FREQ_MEDIUM,0);
}

//MCU输入端口初始化
void BoardPortInInit_ARM_SEN15_PA6(void)
{
    MCU_PinInit(MCU_PIN_A_6, MCU_PIN_MODE_INPUT,boardPortInInitPullConfigArray[BOARD_PORT_IN_ARM_SEN15_PA6], 
                    MCU_PIN_SPEED_FREQ_MEDIUM,0);
}

//MCU输入端口初始化
void BoardPortInInit_ARM_SEN16_PA7(void)
{
    MCU_PinInit(MCU_PIN_A_7, MCU_PIN_MODE_INPUT,boardPortInInitPullConfigArray[BOARD_PORT_IN_ARM_SEN16_PA7], 
                    MCU_PIN_SPEED_FREQ_MEDIUM,0);
}

//MCU输入端口初始化
void BoardPortInInit_ARM_SEN17_PC4(void)
{
    MCU_PinInit(MCU_PIN_C_4, MCU_PIN_MODE_INPUT,boardPortInInitPullConfigArray[BOARD_PORT_IN_ARM_SEN17_PC4], 
                    MCU_PIN_SPEED_FREQ_MEDIUM,0);
}

//MCU输入端口初始化
void BoardPortInInit_ARM_SEN18_PH3(void)
{
    MCU_PinInit(MCU_PIN_H_3, MCU_PIN_MODE_INPUT,boardPortInInitPullConfigArray[BOARD_PORT_IN_ARM_SEN18_PH3], 
                    MCU_PIN_SPEED_FREQ_MEDIUM,0);
}

//MCU输入端口初始化
void BoardPortInInit_ARM_SEN19_PH2(void)
{
    MCU_PinInit(MCU_PIN_H_2, MCU_PIN_MODE_INPUT,boardPortInInitPullConfigArray[BOARD_PORT_IN_ARM_SEN19_PH2], 
                    MCU_PIN_SPEED_FREQ_MEDIUM,0);
}

//MCU输入端口初始化
void BoardPortInInit_ARM_SEN20_PC3(void)
{
    MCU_PinInit(MCU_PIN_C_3, MCU_PIN_MODE_INPUT,boardPortInInitPullConfigArray[BOARD_PORT_IN_ARM_SEN20_PC3], 
                    MCU_PIN_SPEED_FREQ_MEDIUM,0);
}

//MCU输入端口初始化
void BoardPortInInit_ARM_SEN21_PC2(void)
{
    MCU_PinInit(MCU_PIN_C_2, MCU_PIN_MODE_INPUT,boardPortInInitPullConfigArray[BOARD_PORT_IN_ARM_SEN21_PC2], 
                    MCU_PIN_SPEED_FREQ_MEDIUM,0);
}

//MCU输入端口初始化
void BoardPortInInit_ARM_SEN22_PC1(void)
{
    MCU_PinInit(MCU_PIN_C_1, MCU_PIN_MODE_INPUT,boardPortInInitPullConfigArray[BOARD_PORT_IN_ARM_SEN22_PC1], 
                    MCU_PIN_SPEED_FREQ_MEDIUM,0);
}

//MCU输入端口初始化
void BoardPortInInit_ARM_SEN23_PC0(void)
{
    MCU_PinInit(MCU_PIN_C_0, MCU_PIN_MODE_INPUT,boardPortInInitPullConfigArray[BOARD_PORT_IN_ARM_SEN23_PC0], 
                    MCU_PIN_SPEED_FREQ_MEDIUM,0);
}

//MCU输入端口初始化
void BoardPortInInit_ARM_SEN24_PF10(void)
{
    MCU_PinInit(MCU_PIN_F_10, MCU_PIN_MODE_INPUT,boardPortInInitPullConfigArray[BOARD_PORT_IN_ARM_SEN24_PF10], 
                    MCU_PIN_SPEED_FREQ_MEDIUM,0);
}

//MCU输入端口初始化
void BoardPortInInit_ARM_SEN25_PB3(void)
{
    MCU_PinInit(MCU_PIN_B_3, MCU_PIN_MODE_INPUT,boardPortInInitPullConfigArray[BOARD_PORT_IN_ARM_SEN25_PB3], 
                    MCU_PIN_SPEED_FREQ_MEDIUM,0);
}
/***************************************读取状态*******************************************/
static GPIO_PinState BoardPortInReadValue_CPLD_NOP1(void)
{
    return BoardCPLD_ReadInputPin(BOARD_CPLD_IN_PIN_1);
}

static GPIO_PinState BoardPortInReadValue_CPLD_NOP2(void)
{
    return BoardCPLD_ReadInputPin(BOARD_CPLD_IN_PIN_2);
}

static GPIO_PinState BoardPortInReadValue_CPLD_NOP3(void)
{
    return BoardCPLD_ReadInputPin(BOARD_CPLD_IN_PIN_3);
}

static GPIO_PinState BoardPortInReadValue_CPLD_NOP4(void)
{
    return BoardCPLD_ReadInputPin(BOARD_CPLD_IN_PIN_4);
}

static GPIO_PinState BoardPortInReadValue_CPLD_NOP5(void)
{
    return BoardCPLD_ReadInputPin(BOARD_CPLD_IN_PIN_5);
}

static GPIO_PinState BoardPortInReadValue_CPLD_NOP6(void)
{
    return BoardCPLD_ReadInputPin(BOARD_CPLD_IN_PIN_6);
}

static GPIO_PinState BoardPortInReadValue_CPLD_NOP7(void)
{
    return BoardCPLD_ReadInputPin(BOARD_CPLD_IN_PIN_7);
}

static GPIO_PinState BoardPortInReadValue_CPLD_NOP8(void)
{
    return BoardCPLD_ReadInputPin(BOARD_CPLD_IN_PIN_8);
}

static GPIO_PinState BoardPortInReadValue_CPLD_NOP9(void)
{
    return BoardCPLD_ReadInputPin(BOARD_CPLD_IN_PIN_9);
}

static GPIO_PinState BoardPortInReadValue_CPLD_NOP10(void)
{
    return BoardCPLD_ReadInputPin(BOARD_CPLD_IN_PIN_10);
}

static GPIO_PinState BoardPortInReadValue_CPLD_NOP11(void)
{
    return BoardCPLD_ReadInputPin(BOARD_CPLD_IN_PIN_11);
}

static GPIO_PinState BoardPortInReadValue_CPLD_NOP12(void)
{
    return BoardCPLD_ReadInputPin(BOARD_CPLD_IN_PIN_12);
}

static GPIO_PinState BoardPortInReadValue_CPLD_NOP13(void)
{
    return BoardCPLD_ReadInputPin(BOARD_CPLD_IN_PIN_13);
}


static GPIO_PinState BoardPortInReadValue_ARM_SEN1_PC5(void)
{
    //读取当前状态
    return MCU_PinReadSingle(MCU_PIN_C_5);
}

static GPIO_PinState BoardPortInReadValue_ARM_SEN2_PB0(void)
{
    //读取当前状态
    return MCU_PinReadSingle(MCU_PIN_B_0);
}

static GPIO_PinState BoardPortInReadValue_ARM_SEN3_PB1(void)
{
    //读取当前状态
    return MCU_PinReadSingle(MCU_PIN_B_1);
}

static GPIO_PinState BoardPortInReadValue_ARM_SEN4_PB10(void)
{
    //读取当前状态
    return MCU_PinReadSingle(MCU_PIN_B_10);
}

static GPIO_PinState BoardPortInReadValue_ARM_SEN5_PB11(void)
{
    //读取当前状态
    return MCU_PinReadSingle(MCU_PIN_B_11);
}

static GPIO_PinState BoardPortInReadValue_ARM_SEN6_PH6(void)
{
    //读取当前状态
    return MCU_PinReadSingle(MCU_PIN_H_6);
}

static GPIO_PinState BoardPortInReadValue_ARM_SEN7_PH7(void)
{
    //读取当前状态
    return MCU_PinReadSingle(MCU_PIN_H_7);
}

static GPIO_PinState BoardPortInReadValue_ARM_SEN8_PH8(void)
{
    //读取当前状态
    return MCU_PinReadSingle(MCU_PIN_H_8);
}

static GPIO_PinState BoardPortInReadValue_ARM_SEN9_PH9(void)
{
    //读取当前状态
    return MCU_PinReadSingle(MCU_PIN_H_9);
}

static GPIO_PinState BoardPortInReadValue_ARM_SEN10_PH10(void)
{   
    //读取当前状态
    return MCU_PinReadSingle(MCU_PIN_H_10);
}

static GPIO_PinState BoardPortInReadValue_ARM_SEN11_PH11(void)
{
    //读取当前状态
    return MCU_PinReadSingle(MCU_PIN_H_11);
}

static GPIO_PinState BoardPortInReadValue_ARM_SEN12_PH12(void)
{
    //读取当前状态
    return MCU_PinReadSingle(MCU_PIN_H_12);
}

static GPIO_PinState BoardPortInReadValue_ARM_SEN13_PA4(void)
{
    //读取当前状态
    return MCU_PinReadSingle(MCU_PIN_A_4);
}

static GPIO_PinState BoardPortInReadValue_ARM_SEN14_PA5(void)
{
    //读取当前状态
    return MCU_PinReadSingle(MCU_PIN_A_5);
}

static GPIO_PinState BoardPortInReadValue_ARM_SEN15_PA6(void)
{
    //读取当前状态
    return MCU_PinReadSingle(MCU_PIN_A_6);
}

static GPIO_PinState BoardPortInReadValue_ARM_SEN16_PA7(void)
{
    //读取当前状态
    return MCU_PinReadSingle(MCU_PIN_A_7);
}

static GPIO_PinState BoardPortInReadValue_ARM_SEN17_PC4(void)
{
    //读取当前状态
    return MCU_PinReadSingle(MCU_PIN_C_4);
}

static GPIO_PinState BoardPortInReadValue_ARM_SEN18_PH3(void)
{
    //读取当前状态
    return MCU_PinReadSingle(MCU_PIN_H_3);
}

static GPIO_PinState BoardPortInReadValue_ARM_SEN19_PH2(void)
{
    //读取当前状态
    return MCU_PinReadSingle(MCU_PIN_H_2);
}

static GPIO_PinState BoardPortInReadValue_ARM_SEN20_PC3(void)
{
    //读取当前状态
    return MCU_PinReadSingle(MCU_PIN_C_3);
}

static GPIO_PinState BoardPortInReadValue_ARM_SEN21_PC2(void)
{
    //读取当前状态
    return MCU_PinReadSingle(MCU_PIN_C_2);
}

static GPIO_PinState BoardPortInReadValue_ARM_SEN22_PC1(void)
{
    //读取当前状态
    return MCU_PinReadSingle(MCU_PIN_C_1);
}

static GPIO_PinState BoardPortInReadValue_ARM_SEN23_PC0(void)
{
    //读取当前状态
    return MCU_PinReadSingle(MCU_PIN_C_0);
}

static GPIO_PinState BoardPortInReadValue_ARM_SEN24_PF10(void)
{
    //读取当前状态
    return MCU_PinReadSingle(MCU_PIN_F_10);
}

static GPIO_PinState BoardPortInReadValue_ARM_SEN25_PB3(void)
{
    //读取当前状态
    return MCU_PinReadSingle(MCU_PIN_B_3);
}

/**********************************函数指针数组*************************************/
static const BoardPortInInitFuncPtr BoardPortInInitFuncPtrArray[BOARD_PORT_IN_COUNT] = {
    BoardPortInInit_CPLD_NOP1,
    BoardPortInInit_CPLD_NOP2,
    BoardPortInInit_CPLD_NOP3,
    BoardPortInInit_CPLD_NOP4,
    BoardPortInInit_CPLD_NOP5,
    BoardPortInInit_CPLD_NOP6,
    BoardPortInInit_CPLD_NOP7,
    BoardPortInInit_CPLD_NOP8,
    BoardPortInInit_CPLD_NOP9,
    BoardPortInInit_CPLD_NOP10,
    BoardPortInInit_CPLD_NOP11,
    BoardPortInInit_CPLD_NOP12,
    BoardPortInInit_CPLD_NOP13,
    BoardPortInInit_ARM_SEN1_PC5,
    BoardPortInInit_ARM_SEN2_PB0,
    BoardPortInInit_ARM_SEN3_PB1,
    BoardPortInInit_ARM_SEN4_PB10,
    BoardPortInInit_ARM_SEN5_PB11,
    BoardPortInInit_ARM_SEN6_PH6,
    BoardPortInInit_ARM_SEN7_PH7,
    BoardPortInInit_ARM_SEN8_PH8,
    BoardPortInInit_ARM_SEN9_PH9,
    BoardPortInInit_ARM_SEN10_PH10,
    BoardPortInInit_ARM_SEN11_PH11,
    BoardPortInInit_ARM_SEN12_PH12,
    BoardPortInInit_ARM_SEN13_PA4,
    BoardPortInInit_ARM_SEN14_PA5,
    BoardPortInInit_ARM_SEN15_PA6,
    BoardPortInInit_ARM_SEN16_PA7,
    BoardPortInInit_ARM_SEN17_PC4,
    BoardPortInInit_ARM_SEN18_PH3,
    BoardPortInInit_ARM_SEN19_PH2,
    BoardPortInInit_ARM_SEN20_PC3,
    BoardPortInInit_ARM_SEN21_PC2,
    BoardPortInInit_ARM_SEN22_PC1,
    BoardPortInInit_ARM_SEN23_PC0,
    BoardPortInInit_ARM_SEN24_PF10,
    BoardPortInInit_ARM_SEN25_PB3,
};

//函数指针数组
static const BoardPortInReadValueFuncPtr BoardPortInReadValueFuncPtrArray[BOARD_PORT_IN_COUNT] = {
    BoardPortInReadValue_CPLD_NOP1,
    BoardPortInReadValue_CPLD_NOP2,
    BoardPortInReadValue_CPLD_NOP3,
    BoardPortInReadValue_CPLD_NOP4,
    BoardPortInReadValue_CPLD_NOP5,
    BoardPortInReadValue_CPLD_NOP6,
    BoardPortInReadValue_CPLD_NOP7,
    BoardPortInReadValue_CPLD_NOP8,
    BoardPortInReadValue_CPLD_NOP9,
    BoardPortInReadValue_CPLD_NOP10,
    BoardPortInReadValue_CPLD_NOP11,
    BoardPortInReadValue_CPLD_NOP12,
    BoardPortInReadValue_CPLD_NOP13,
    BoardPortInReadValue_ARM_SEN1_PC5,
    BoardPortInReadValue_ARM_SEN2_PB0,
    BoardPortInReadValue_ARM_SEN3_PB1,
    BoardPortInReadValue_ARM_SEN4_PB10,
    BoardPortInReadValue_ARM_SEN5_PB11,
    BoardPortInReadValue_ARM_SEN6_PH6,
    BoardPortInReadValue_ARM_SEN7_PH7,
    BoardPortInReadValue_ARM_SEN8_PH8,
    BoardPortInReadValue_ARM_SEN9_PH9,
    BoardPortInReadValue_ARM_SEN10_PH10,
    BoardPortInReadValue_ARM_SEN11_PH11,
    BoardPortInReadValue_ARM_SEN12_PH12,
    BoardPortInReadValue_ARM_SEN13_PA4,
    BoardPortInReadValue_ARM_SEN14_PA5,
    BoardPortInReadValue_ARM_SEN15_PA6,
    BoardPortInReadValue_ARM_SEN16_PA7,
    BoardPortInReadValue_ARM_SEN17_PC4,
    BoardPortInReadValue_ARM_SEN18_PH3,
    BoardPortInReadValue_ARM_SEN19_PH2,
    BoardPortInReadValue_ARM_SEN20_PC3,
    BoardPortInReadValue_ARM_SEN21_PC2,
    BoardPortInReadValue_ARM_SEN22_PC1,
    BoardPortInReadValue_ARM_SEN23_PC0,
    BoardPortInReadValue_ARM_SEN24_PF10,
    BoardPortInReadValue_ARM_SEN25_PB3,
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





