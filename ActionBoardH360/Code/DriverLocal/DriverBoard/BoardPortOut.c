/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-10-30 16:33:45 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-11-10 13:27:40 +0800
************************************************************************************************/ 
#include "BoardPortOut.h"
#include "BoardCPLD.h"

//外部导入初始化配置
extern const GPIO_PinState boardPortOutInitLevelConfigArray[];
extern const MCU_PIN_PULL boardPortOutInitPullConfigArray[];

//输出初始化函数指针
typedef void (*BoardPortOutInitFuncPtr)(void);
//输出指定值
typedef void (*BoardPortOutWriteFuncPtr)(GPIO_PinState value);

/*********************************************输出端口初始化*****************************************************/

//初始化,全部默认高电平
void BoardPortOutInit_CPLD_CDC1(void)
{
    BoardCPLD_WriteOutputPin(BOARD_CPLD_OUT_PIN_1,boardPortOutInitLevelConfigArray[BOARD_PORT_OUT_CPLD_CDC1]);
}

void BoardPortOutInit_CPLD_CDC2(void)
{
    BoardCPLD_WriteOutputPin(BOARD_CPLD_OUT_PIN_2,boardPortOutInitLevelConfigArray[BOARD_PORT_OUT_CPLD_CDC2]);
}

void BoardPortOutInit_CPLD_CDC3(void)
{
    BoardCPLD_WriteOutputPin(BOARD_CPLD_OUT_PIN_3,boardPortOutInitLevelConfigArray[BOARD_PORT_OUT_CPLD_CDC3]);
}

void BoardPortOutInit_CPLD_CDC4(void)
{
    BoardCPLD_WriteOutputPin(BOARD_CPLD_OUT_PIN_4,boardPortOutInitLevelConfigArray[BOARD_PORT_OUT_CPLD_CDC4]);
}

void BoardPortOutInit_CPLD_CDC5(void)
{
    BoardCPLD_WriteOutputPin(BOARD_CPLD_OUT_PIN_5,boardPortOutInitLevelConfigArray[BOARD_PORT_OUT_CPLD_CDC5]);
}

void BoardPortOutInit_CPLD_CDC6(void)
{
    BoardCPLD_WriteOutputPin(BOARD_CPLD_OUT_PIN_6,boardPortOutInitLevelConfigArray[BOARD_PORT_OUT_CPLD_CDC6]);
}

void BoardPortOutInit_CPLD_CDC7(void)
{
    BoardCPLD_WriteOutputPin(BOARD_CPLD_OUT_PIN_7,boardPortOutInitLevelConfigArray[BOARD_PORT_OUT_CPLD_CDC7]);
}

void BoardPortOutInit_CPLD_CDC8(void)
{
    BoardCPLD_WriteOutputPin(BOARD_CPLD_OUT_PIN_8,boardPortOutInitLevelConfigArray[BOARD_PORT_OUT_CPLD_CDC8]);
}

void BoardPortOutInit_CPLD_CDC9(void)
{
    BoardCPLD_WriteOutputPin(BOARD_CPLD_OUT_PIN_9,boardPortOutInitLevelConfigArray[BOARD_PORT_OUT_CPLD_CDC9]);
}

void BoardPortOutInit_CPLD_CDC10(void)
{
    BoardCPLD_WriteOutputPin(BOARD_CPLD_OUT_PIN_10,boardPortOutInitLevelConfigArray[BOARD_PORT_OUT_CPLD_CDC10]);
}

void BoardPortOutInit_CPLD_CDC11(void)
{
    BoardCPLD_WriteOutputPin(BOARD_CPLD_OUT_PIN_11,boardPortOutInitLevelConfigArray[BOARD_PORT_OUT_CPLD_CDC11]);
}

void BoardPortOutInit_CPLD_CDC12(void)
{
    BoardCPLD_WriteOutputPin(BOARD_CPLD_OUT_PIN_12,boardPortOutInitLevelConfigArray[BOARD_PORT_OUT_CPLD_CDC12]);
}

void BoardPortOutInit_CPLD_CDC13(void)
{
    BoardCPLD_WriteOutputPin(BOARD_CPLD_OUT_PIN_13,boardPortOutInitLevelConfigArray[BOARD_PORT_OUT_CPLD_CDC13]);
}

void BoardPortOutInit_CPLD_CDC14(void)
{
    BoardCPLD_WriteOutputPin(BOARD_CPLD_OUT_PIN_14,boardPortOutInitLevelConfigArray[BOARD_PORT_OUT_CPLD_CDC14]);
}

void BoardPortOutInit_CPLD_CDC15(void)
{
    BoardCPLD_WriteOutputPin(BOARD_CPLD_OUT_PIN_15,boardPortOutInitLevelConfigArray[BOARD_PORT_OUT_CPLD_CDC15]);
}

void BoardPortOutInit_CPLD_CDC16(void)
{
    BoardCPLD_WriteOutputPin(BOARD_CPLD_OUT_PIN_16,boardPortOutInitLevelConfigArray[BOARD_PORT_OUT_CPLD_CDC16]);
}

void BoardPortOutInit_CPLD_CDC17(void)
{
    BoardCPLD_WriteOutputPin(BOARD_CPLD_OUT_PIN_17,boardPortOutInitLevelConfigArray[BOARD_PORT_OUT_CPLD_CDC17]);
}

void BoardPortOutInit_CPLD_CDC18(void)
{
    BoardCPLD_WriteOutputPin(BOARD_CPLD_OUT_PIN_18,boardPortOutInitLevelConfigArray[BOARD_PORT_OUT_CPLD_CDC18]);
}

void BoardPortOutInit_CPLD_CDC19(void)
{
    BoardCPLD_WriteOutputPin(BOARD_CPLD_OUT_PIN_19,boardPortOutInitLevelConfigArray[BOARD_PORT_OUT_CPLD_CDC19]);
}

void BoardPortOutInit_CPLD_CDC20(void)
{
    BoardCPLD_WriteOutputPin(BOARD_CPLD_OUT_PIN_20,boardPortOutInitLevelConfigArray[BOARD_PORT_OUT_CPLD_CDC20]);
}

void BoardPortOutInit_CPLD_CDC21(void)
{
    BoardCPLD_WriteOutputPin(BOARD_CPLD_OUT_PIN_21,boardPortOutInitLevelConfigArray[BOARD_PORT_OUT_CPLD_CDC21]);
}

void BoardPortOutInit_CPLD_CDC22(void)
{
    BoardCPLD_WriteOutputPin(BOARD_CPLD_OUT_PIN_22,boardPortOutInitLevelConfigArray[BOARD_PORT_OUT_CPLD_CDC22]);
}

void BoardPortOutInit_CPLD_CDC23(void)
{
    BoardCPLD_WriteOutputPin(BOARD_CPLD_OUT_PIN_23,boardPortOutInitLevelConfigArray[BOARD_PORT_OUT_CPLD_CDC23]);
}

void BoardPortOutInit_CPLD_CDC24(void)
{
    BoardCPLD_WriteOutputPin(BOARD_CPLD_OUT_PIN_24,boardPortOutInitLevelConfigArray[BOARD_PORT_OUT_CPLD_CDC24]);
}

void BoardPortOutInit_CPLD_CDC25(void)
{
    BoardCPLD_WriteOutputPin(BOARD_CPLD_OUT_PIN_25,boardPortOutInitLevelConfigArray[BOARD_PORT_OUT_CPLD_CDC25]);
}

void BoardPortOutInit_CPLD_CDC26(void)
{
    BoardCPLD_WriteOutputPin(BOARD_CPLD_OUT_PIN_26,boardPortOutInitLevelConfigArray[BOARD_PORT_OUT_CPLD_CDC26]);
}

void BoardPortOutInit_CPLD_CDC27(void)
{
    BoardCPLD_WriteOutputPin(BOARD_CPLD_OUT_PIN_27,boardPortOutInitLevelConfigArray[BOARD_PORT_OUT_CPLD_CDC27]);
}

void BoardPortOutInit_CPLD_CDC28(void)
{
    BoardCPLD_WriteOutputPin(BOARD_CPLD_OUT_PIN_28,boardPortOutInitLevelConfigArray[BOARD_PORT_OUT_CPLD_CDC28]);
}

void BoardPortOutInit_CPLD_CDC29(void)
{
    BoardCPLD_WriteOutputPin(BOARD_CPLD_OUT_PIN_29,boardPortOutInitLevelConfigArray[BOARD_PORT_OUT_CPLD_CDC29]);
}

void BoardPortOutInit_CPLD_CDC30(void)
{
    BoardCPLD_WriteOutputPin(BOARD_CPLD_OUT_PIN_30,boardPortOutInitLevelConfigArray[BOARD_PORT_OUT_CPLD_CDC30]);
}

void BoardPortOutInit_CPLD_CDC31(void)
{
    BoardCPLD_WriteOutputPin(BOARD_CPLD_OUT_PIN_31,boardPortOutInitLevelConfigArray[BOARD_PORT_OUT_CPLD_CDC31]);
}

void BoardPortOutInit_CPLD_CDC32(void)
{
    BoardCPLD_WriteOutputPin(BOARD_CPLD_OUT_PIN_32,boardPortOutInitLevelConfigArray[BOARD_PORT_OUT_CPLD_CDC32]);
}

void BoardPortOutInit_CPLD_CDC33(void)
{
    BoardCPLD_WriteOutputPin(BOARD_CPLD_OUT_PIN_33,boardPortOutInitLevelConfigArray[BOARD_PORT_OUT_CPLD_CDC33]);
}

void BoardPortOutInit_CPLD_CDC34(void)
{
    BoardCPLD_WriteOutputPin(BOARD_CPLD_OUT_PIN_34,boardPortOutInitLevelConfigArray[BOARD_PORT_OUT_CPLD_CDC34]);
}

void BoardPortOutInit_CPLD_CDC35(void)
{
    BoardCPLD_WriteOutputPin(BOARD_CPLD_OUT_PIN_35,boardPortOutInitLevelConfigArray[BOARD_PORT_OUT_CPLD_CDC35]);
}

void BoardPortOutInit_CPLD_CDC36(void)
{
    BoardCPLD_WriteOutputPin(BOARD_CPLD_OUT_PIN_36,boardPortOutInitLevelConfigArray[BOARD_PORT_OUT_CPLD_CDC36]);
}

static void BoardPortOutInit_SLAVE_SEN17_PC4(void)
{
    MCU_PinInit(MCU_PIN_C_4, MCU_PIN_MODE_OUTPUT_PP, 
                    boardPortOutInitPullConfigArray[BOARD_PORT_OUT_SLAVE_SEN17_PC4], 
                    MCU_PIN_SPEED_FREQ_MEDIUM,0);
    MCU_PinWriteSingle(MCU_PIN_C_4,boardPortOutInitLevelConfigArray[BOARD_PORT_OUT_SLAVE_SEN17_PC4]);
}

static void BoardPortOutInit_SLAVE_SEN18_PA7(void)
{
    MCU_PinInit(MCU_PIN_A_7, MCU_PIN_MODE_OUTPUT_PP, 
                    boardPortOutInitPullConfigArray[BOARD_PORT_OUT_SLAVE_SEN18_PA7], 
                    MCU_PIN_SPEED_FREQ_MEDIUM,0);
    MCU_PinWriteSingle(MCU_PIN_A_7,boardPortOutInitLevelConfigArray[BOARD_PORT_OUT_SLAVE_SEN18_PA7]);
}

static void BoardPortOutInit_SLAVE_SEN19_PA6(void)
{
    MCU_PinInit(MCU_PIN_A_6, MCU_PIN_MODE_OUTPUT_PP, 
                    boardPortOutInitPullConfigArray[BOARD_PORT_OUT_SLAVE_SEN19_PA6], 
                    MCU_PIN_SPEED_FREQ_MEDIUM,0);
    MCU_PinWriteSingle(MCU_PIN_A_6,boardPortOutInitLevelConfigArray[BOARD_PORT_OUT_SLAVE_SEN19_PA6]);
}

static void BoardPortOutInit_SLAVE_SEN20_PA5(void)
{
    MCU_PinInit(MCU_PIN_A_5, MCU_PIN_MODE_OUTPUT_PP, 
                    boardPortOutInitPullConfigArray[BOARD_PORT_OUT_SLAVE_SEN20_PA5], 
                    MCU_PIN_SPEED_FREQ_MEDIUM,0);
    MCU_PinWriteSingle(MCU_PIN_A_5,boardPortOutInitLevelConfigArray[BOARD_PORT_OUT_SLAVE_SEN20_PA5]);
}

static void BoardPortOutInit_SLAVE_SEN21_PA4(void)
{
    MCU_PinInit(MCU_PIN_A_4, MCU_PIN_MODE_OUTPUT_PP, 
                    boardPortOutInitPullConfigArray[BOARD_PORT_OUT_SLAVE_SEN21_PA4], 
                    MCU_PIN_SPEED_FREQ_MEDIUM,0);
    MCU_PinWriteSingle(MCU_PIN_A_4,boardPortOutInitLevelConfigArray[BOARD_PORT_OUT_SLAVE_SEN21_PA4]);
}

static void BoardPortOutInit_SLAVE_SEN22_PA3(void)
{
    MCU_PinInit(MCU_PIN_A_3, MCU_PIN_MODE_OUTPUT_PP, 
                    boardPortOutInitPullConfigArray[BOARD_PORT_OUT_SLAVE_SEN22_PA3], 
                    MCU_PIN_SPEED_FREQ_MEDIUM,0);
    MCU_PinWriteSingle(MCU_PIN_A_3,boardPortOutInitLevelConfigArray[BOARD_PORT_OUT_SLAVE_SEN22_PA3]);
}

static void BoardPortOutInit_SLAVE_SEN23_PF8(void)
{
    MCU_PinInit(MCU_PIN_F_8, MCU_PIN_MODE_OUTPUT_PP, 
                    boardPortOutInitPullConfigArray[BOARD_PORT_OUT_SLAVE_SEN23_PF8], 
                    MCU_PIN_SPEED_FREQ_MEDIUM,0);
    MCU_PinWriteSingle(MCU_PIN_F_8,boardPortOutInitLevelConfigArray[BOARD_PORT_OUT_SLAVE_SEN23_PF8]);
}

static void BoardPortOutInit_SLAVE_SEN24_PF9(void)
{   
    MCU_PinInit(MCU_PIN_F_9, MCU_PIN_MODE_OUTPUT_PP, 
                    boardPortOutInitPullConfigArray[BOARD_PORT_OUT_SLAVE_SEN24_PF9], 
                    MCU_PIN_SPEED_FREQ_MEDIUM,0);
    MCU_PinWriteSingle(MCU_PIN_F_9,boardPortOutInitLevelConfigArray[BOARD_PORT_OUT_SLAVE_SEN24_PF9]);
}

static void BoardPortOutInit_SLAVE_SEN25_PF10(void)
{
    MCU_PinInit(MCU_PIN_F_10, MCU_PIN_MODE_OUTPUT_PP, 
                    boardPortOutInitPullConfigArray[BOARD_PORT_OUT_SLAVE_SEN25_PF10], 
                    MCU_PIN_SPEED_FREQ_MEDIUM,0);
    MCU_PinWriteSingle(MCU_PIN_F_10,boardPortOutInitLevelConfigArray[BOARD_PORT_OUT_SLAVE_SEN25_PF10]);
}

static void BoardPortOutInit_SLAVE_SEN26_PH3(void)
{
    MCU_PinInit(MCU_PIN_H_3, MCU_PIN_MODE_OUTPUT_PP, 
                    boardPortOutInitPullConfigArray[BOARD_PORT_OUT_SLAVE_SEN26_PH3], 
                    MCU_PIN_SPEED_FREQ_MEDIUM,0);
    MCU_PinWriteSingle(MCU_PIN_H_3,boardPortOutInitLevelConfigArray[BOARD_PORT_OUT_SLAVE_SEN26_PH3]);
}

static void BoardPortOutInit_SLAVE_SEN27_PH2(void)
{
    MCU_PinInit(MCU_PIN_H_2, MCU_PIN_MODE_OUTPUT_PP, 
                    boardPortOutInitPullConfigArray[BOARD_PORT_OUT_SLAVE_SEN27_PH2], 
                    MCU_PIN_SPEED_FREQ_MEDIUM,0);
    MCU_PinWriteSingle(MCU_PIN_H_2,boardPortOutInitLevelConfigArray[BOARD_PORT_OUT_SLAVE_SEN27_PH2]);
}

static void BoardPortOutInit_SLAVE_SEN28_PA2(void)
{
    MCU_PinInit(MCU_PIN_A_2, MCU_PIN_MODE_OUTPUT_PP, 
                    boardPortOutInitPullConfigArray[BOARD_PORT_OUT_SLAVE_SEN28_PA2], 
                    MCU_PIN_SPEED_FREQ_MEDIUM,0);
    MCU_PinWriteSingle(MCU_PIN_A_2,boardPortOutInitLevelConfigArray[BOARD_PORT_OUT_SLAVE_SEN28_PA2]);
}

static void BoardPortOutInit_SLAVE_DC1_VAVLE_PC8(void)
{
    MCU_PinInit(MCU_PIN_C_8, MCU_PIN_MODE_OUTPUT_PP, 
                    boardPortOutInitPullConfigArray[BOARD_PORT_OUT_SLAVE_DC1_VAVLE_PC8], 
                    MCU_PIN_SPEED_FREQ_MEDIUM,0);
    MCU_PinWriteSingle(MCU_PIN_C_8,boardPortOutInitLevelConfigArray[BOARD_PORT_OUT_SLAVE_DC1_VAVLE_PC8]);
}

static void BoardPortOutInit_SLAVE_DC2_RUBBISH_MOTOR1_PG9(void)
{
    MCU_PinInit(MCU_PIN_G_9, MCU_PIN_MODE_OUTPUT_PP, 
                    boardPortOutInitPullConfigArray[BOARD_PORT_OUT_SLAVE_DC2_RUBBISH_MOTOR1_PG9], 
                    MCU_PIN_SPEED_FREQ_MEDIUM,0);
    MCU_PinWriteSingle(MCU_PIN_G_9,boardPortOutInitLevelConfigArray[BOARD_PORT_OUT_SLAVE_DC2_RUBBISH_MOTOR1_PG9]);
}

static void BoardPortOutInit_SLAVE_DC3_RUBBISH_MOTOR2_PD6(void)
{
    MCU_PinInit(MCU_PIN_D_6, MCU_PIN_MODE_OUTPUT_PP, 
                    boardPortOutInitPullConfigArray[BOARD_PORT_OUT_SLAVE_DC3_RUBBISH_MOTOR2_PD6], 
                    MCU_PIN_SPEED_FREQ_MEDIUM,0);
    MCU_PinWriteSingle(MCU_PIN_D_6,boardPortOutInitLevelConfigArray[BOARD_PORT_OUT_SLAVE_DC3_RUBBISH_MOTOR2_PD6]);
}

static void BoardPortOutInit_SLAVE_DC4_RUBBISH_MOTOR3_PI3(void)
{
    MCU_PinInit(MCU_PIN_I_3, MCU_PIN_MODE_OUTPUT_PP, 
                    boardPortOutInitPullConfigArray[BOARD_PORT_OUT_SLAVE_DC4_RUBBISH_MOTOR3_PI3], 
                    MCU_PIN_SPEED_FREQ_MEDIUM,0);
    MCU_PinWriteSingle(MCU_PIN_I_3,boardPortOutInitLevelConfigArray[BOARD_PORT_OUT_SLAVE_DC4_RUBBISH_MOTOR3_PI3]);
}

static void BoardPortOutInit_SLAVE_DC5_RUBBISH_MOTOR4_PI2(void)
{
    MCU_PinInit(MCU_PIN_I_2, MCU_PIN_MODE_OUTPUT_PP, 
                    boardPortOutInitPullConfigArray[BOARD_PORT_OUT_SLAVE_DC5_RUBBISH_MOTOR4_PI2], 
                    MCU_PIN_SPEED_FREQ_MEDIUM,0);
    MCU_PinWriteSingle(MCU_PIN_I_2,boardPortOutInitLevelConfigArray[BOARD_PORT_OUT_SLAVE_DC5_RUBBISH_MOTOR4_PI2]);
}

static void BoardPortOutInit_YM1_ARM3_PB4(void)
{
    MCU_PinInit(MCU_PIN_B_4, MCU_PIN_MODE_OUTPUT_PP, 
                    boardPortOutInitPullConfigArray[BOARD_PORT_OUT_YM1_ARM3_PB4], 
                    MCU_PIN_SPEED_FREQ_MEDIUM,0);
    MCU_PinWriteSingle(MCU_PIN_B_4,boardPortOutInitLevelConfigArray[BOARD_PORT_OUT_YM1_ARM3_PB4]);
}

static void BoardPortOutInit_YM1_ARM4_PG15(void)
{
    MCU_PinInit(MCU_PIN_G_15, MCU_PIN_MODE_OUTPUT_PP, 
                    boardPortOutInitPullConfigArray[BOARD_PORT_OUT_YM1_ARM4_PG15], 
                    MCU_PIN_SPEED_FREQ_MEDIUM,0);
    MCU_PinWriteSingle(MCU_PIN_G_15,boardPortOutInitLevelConfigArray[BOARD_PORT_OUT_YM1_ARM4_PG15]);
}

/********************************************设置电平状态*********************************/
void BoardPortOutWrite_CPLD_CDC1(GPIO_PinState setValue)
{
    BoardCPLD_WriteOutputPin(BOARD_CPLD_OUT_PIN_1,setValue);
}

void BoardPortOutWrite_CPLD_CDC2(GPIO_PinState setValue)
{
    BoardCPLD_WriteOutputPin(BOARD_CPLD_OUT_PIN_2,setValue);
}

void BoardPortOutWrite_CPLD_CDC3(GPIO_PinState setValue)
{
    BoardCPLD_WriteOutputPin(BOARD_CPLD_OUT_PIN_3,setValue);
}

void BoardPortOutWrite_CPLD_CDC4(GPIO_PinState setValue)
{
    BoardCPLD_WriteOutputPin(BOARD_CPLD_OUT_PIN_4,setValue);
}

void BoardPortOutWrite_CPLD_CDC5(GPIO_PinState setValue)
{
    BoardCPLD_WriteOutputPin(BOARD_CPLD_OUT_PIN_5,setValue);
}

void BoardPortOutWrite_CPLD_CDC6(GPIO_PinState setValue)
{
    BoardCPLD_WriteOutputPin(BOARD_CPLD_OUT_PIN_6,setValue);
}

void BoardPortOutWrite_CPLD_CDC7(GPIO_PinState setValue)
{
    BoardCPLD_WriteOutputPin(BOARD_CPLD_OUT_PIN_7,setValue);
}

void BoardPortOutWrite_CPLD_CDC8(GPIO_PinState setValue)
{
    BoardCPLD_WriteOutputPin(BOARD_CPLD_OUT_PIN_8,setValue);
}

void BoardPortOutWrite_CPLD_CDC9(GPIO_PinState setValue)
{
    BoardCPLD_WriteOutputPin(BOARD_CPLD_OUT_PIN_9,setValue);
}

void BoardPortOutWrite_CPLD_CDC10(GPIO_PinState setValue)
{
    BoardCPLD_WriteOutputPin(BOARD_CPLD_OUT_PIN_10,setValue);
}

void BoardPortOutWrite_CPLD_CDC11(GPIO_PinState setValue)
{
    BoardCPLD_WriteOutputPin(BOARD_CPLD_OUT_PIN_11,setValue);
}

void BoardPortOutWrite_CPLD_CDC12(GPIO_PinState setValue)
{
    BoardCPLD_WriteOutputPin(BOARD_CPLD_OUT_PIN_12,setValue);
}

void BoardPortOutWrite_CPLD_CDC13(GPIO_PinState setValue)
{
    BoardCPLD_WriteOutputPin(BOARD_CPLD_OUT_PIN_13,setValue);
}

void BoardPortOutWrite_CPLD_CDC14(GPIO_PinState setValue)
{
    BoardCPLD_WriteOutputPin(BOARD_CPLD_OUT_PIN_14,setValue);
}

void BoardPortOutWrite_CPLD_CDC15(GPIO_PinState setValue)
{
    BoardCPLD_WriteOutputPin(BOARD_CPLD_OUT_PIN_15,setValue);
}

void BoardPortOutWrite_CPLD_CDC16(GPIO_PinState setValue)
{
    BoardCPLD_WriteOutputPin(BOARD_CPLD_OUT_PIN_16,setValue);
}

void BoardPortOutWrite_CPLD_CDC17(GPIO_PinState setValue)
{
    BoardCPLD_WriteOutputPin(BOARD_CPLD_OUT_PIN_17,setValue);
}

void BoardPortOutWrite_CPLD_CDC18(GPIO_PinState setValue)
{
    BoardCPLD_WriteOutputPin(BOARD_CPLD_OUT_PIN_18,setValue);
}

void BoardPortOutWrite_CPLD_CDC19(GPIO_PinState setValue)
{
    BoardCPLD_WriteOutputPin(BOARD_CPLD_OUT_PIN_19,setValue);
}

void BoardPortOutWrite_CPLD_CDC20(GPIO_PinState setValue)
{
    BoardCPLD_WriteOutputPin(BOARD_CPLD_OUT_PIN_20,setValue);
}

void BoardPortOutWrite_CPLD_CDC21(GPIO_PinState setValue)
{
    BoardCPLD_WriteOutputPin(BOARD_CPLD_OUT_PIN_21,setValue);
}

void BoardPortOutWrite_CPLD_CDC22(GPIO_PinState setValue)
{
    BoardCPLD_WriteOutputPin(BOARD_CPLD_OUT_PIN_22,setValue);
}

void BoardPortOutWrite_CPLD_CDC23(GPIO_PinState setValue)
{
    BoardCPLD_WriteOutputPin(BOARD_CPLD_OUT_PIN_23,setValue);
}

void BoardPortOutWrite_CPLD_CDC24(GPIO_PinState setValue)
{
    BoardCPLD_WriteOutputPin(BOARD_CPLD_OUT_PIN_24,setValue);
}

void BoardPortOutWrite_CPLD_CDC25(GPIO_PinState setValue)
{
    BoardCPLD_WriteOutputPin(BOARD_CPLD_OUT_PIN_25,setValue);
}

void BoardPortOutWrite_CPLD_CDC26(GPIO_PinState setValue)
{
    BoardCPLD_WriteOutputPin(BOARD_CPLD_OUT_PIN_26,setValue);
}

void BoardPortOutWrite_CPLD_CDC27(GPIO_PinState setValue)
{
    BoardCPLD_WriteOutputPin(BOARD_CPLD_OUT_PIN_27,setValue);
}

void BoardPortOutWrite_CPLD_CDC28(GPIO_PinState setValue)
{
    BoardCPLD_WriteOutputPin(BOARD_CPLD_OUT_PIN_28,setValue);
}

void BoardPortOutWrite_CPLD_CDC29(GPIO_PinState setValue)
{
    BoardCPLD_WriteOutputPin(BOARD_CPLD_OUT_PIN_29,setValue);
}

void BoardPortOutWrite_CPLD_CDC30(GPIO_PinState setValue)
{
    BoardCPLD_WriteOutputPin(BOARD_CPLD_OUT_PIN_30,setValue);
}

void BoardPortOutWrite_CPLD_CDC31(GPIO_PinState setValue)
{
    BoardCPLD_WriteOutputPin(BOARD_CPLD_OUT_PIN_31,setValue);
}

void BoardPortOutWrite_CPLD_CDC32(GPIO_PinState setValue)
{
    BoardCPLD_WriteOutputPin(BOARD_CPLD_OUT_PIN_32,setValue);
}

void BoardPortOutWrite_CPLD_CDC33(GPIO_PinState setValue)
{
    BoardCPLD_WriteOutputPin(BOARD_CPLD_OUT_PIN_33,setValue);
}

void BoardPortOutWrite_CPLD_CDC34(GPIO_PinState setValue)
{
    BoardCPLD_WriteOutputPin(BOARD_CPLD_OUT_PIN_34,setValue);
}

void BoardPortOutWrite_CPLD_CDC35(GPIO_PinState setValue)
{
    BoardCPLD_WriteOutputPin(BOARD_CPLD_OUT_PIN_35,setValue);
}

void BoardPortOutWrite_CPLD_CDC36(GPIO_PinState setValue)
{
    BoardCPLD_WriteOutputPin(BOARD_CPLD_OUT_PIN_36,setValue);
}

static void BoardPortOutWrite_SLAVE_SEN17_PC4(GPIO_PinState setValue)
{
    MCU_PinWriteSingle(MCU_PIN_C_4,setValue);
}

static void BoardPortOutWrite_SLAVE_SEN18_PA7(GPIO_PinState setValue)
{
    MCU_PinWriteSingle(MCU_PIN_A_7,setValue);
}

static void BoardPortOutWrite_SLAVE_SEN19_PA6(GPIO_PinState setValue)
{
    MCU_PinWriteSingle(MCU_PIN_A_6,setValue);
}

static void BoardPortOutWrite_SLAVE_SEN20_PA5(GPIO_PinState setValue)
{
    MCU_PinWriteSingle(MCU_PIN_A_5,setValue);
}

static void BoardPortOutWrite_SLAVE_SEN21_PA4(GPIO_PinState setValue)
{
    MCU_PinWriteSingle(MCU_PIN_A_4,setValue);
}

static void BoardPortOutWrite_SLAVE_SEN22_PA3(GPIO_PinState setValue)
{
    MCU_PinWriteSingle(MCU_PIN_A_3,setValue);
}

static void BoardPortOutWrite_SLAVE_SEN23_PF8(GPIO_PinState setValue)
{
    MCU_PinWriteSingle(MCU_PIN_F_8,setValue);
}

static void BoardPortOutWrite_SLAVE_SEN24_PF9(GPIO_PinState setValue)
{
    MCU_PinWriteSingle(MCU_PIN_F_9,setValue);
}

static void BoardPortOutWrite_SLAVE_SEN25_PF10(GPIO_PinState setValue)
{
    MCU_PinWriteSingle(MCU_PIN_F_10,setValue);
}

static void BoardPortOutWrite_SLAVE_SEN26_PH3(GPIO_PinState setValue)
{
    MCU_PinWriteSingle(MCU_PIN_H_3,setValue);
}

static void BoardPortOutWrite_SLAVE_SEN27_PH2(GPIO_PinState setValue)
{
    MCU_PinWriteSingle(MCU_PIN_H_2,setValue);
}

static void BoardPortOutWrite_SLAVE_SEN28_PA2(GPIO_PinState setValue)
{
    MCU_PinWriteSingle(MCU_PIN_A_2,setValue);
}

static void BoardPortOutWrite_SLAVE_DC1_VAVLE_PC8(GPIO_PinState setValue)
{
    MCU_PinWriteSingle(MCU_PIN_C_8,setValue);
}

static void BoardPortOutWrite_SLAVE_DC2_RUBBISH_MOTOR1_PG9(GPIO_PinState setValue)
{
    MCU_PinWriteSingle(MCU_PIN_G_9,setValue);
}

static void BoardPortOutWrite_SLAVE_DC3_RUBBISH_MOTOR2_PD6(GPIO_PinState setValue)
{
    MCU_PinWriteSingle(MCU_PIN_D_6,setValue);
}

static void BoardPortOutWrite_SLAVE_DC4_RUBBISH_MOTOR3_PI3(GPIO_PinState setValue)
{
    MCU_PinWriteSingle(MCU_PIN_I_3,setValue);
}

static void BoardPortOutWrite_SLAVE_DC5_RUBBISH_MOTOR4_PI2(GPIO_PinState setValue)
{
    MCU_PinWriteSingle(MCU_PIN_I_2,setValue);
}

static void BoardPortOutWrite_YM1_ARM3_PB4(GPIO_PinState setValue)
{
    MCU_PinWriteSingle(MCU_PIN_B_4,setValue);
}

static void BoardPortOutWrite_YM1_ARM4_PG15(GPIO_PinState setValue)
{
    MCU_PinWriteSingle(MCU_PIN_G_15,setValue);
}

//函数指针数组
static const BoardPortOutInitFuncPtr BoardPortOutInitFuncPtrArray[BOARD_PORT_OUT_COUNT] = {
    BoardPortOutInit_CPLD_CDC1,
    BoardPortOutInit_CPLD_CDC2,
    BoardPortOutInit_CPLD_CDC3,
    BoardPortOutInit_CPLD_CDC4,
    BoardPortOutInit_CPLD_CDC5,
    BoardPortOutInit_CPLD_CDC6,
    BoardPortOutInit_CPLD_CDC7,
    BoardPortOutInit_CPLD_CDC8,
    BoardPortOutInit_CPLD_CDC9,
    BoardPortOutInit_CPLD_CDC10,
    BoardPortOutInit_CPLD_CDC11,
    BoardPortOutInit_CPLD_CDC12,
    BoardPortOutInit_CPLD_CDC13,
    BoardPortOutInit_CPLD_CDC14,
    BoardPortOutInit_CPLD_CDC15,
    BoardPortOutInit_CPLD_CDC16,
    BoardPortOutInit_CPLD_CDC17,
    BoardPortOutInit_CPLD_CDC18,
    BoardPortOutInit_CPLD_CDC19,
    BoardPortOutInit_CPLD_CDC20,
    BoardPortOutInit_CPLD_CDC21,
    BoardPortOutInit_CPLD_CDC22,
    BoardPortOutInit_CPLD_CDC23,
    BoardPortOutInit_CPLD_CDC24,
    BoardPortOutInit_CPLD_CDC25,
    BoardPortOutInit_CPLD_CDC26,
    BoardPortOutInit_CPLD_CDC27,
    BoardPortOutInit_CPLD_CDC28,
    BoardPortOutInit_CPLD_CDC29,
    BoardPortOutInit_CPLD_CDC30,
    BoardPortOutInit_CPLD_CDC31,
    BoardPortOutInit_CPLD_CDC32,
    BoardPortOutInit_CPLD_CDC33,
    BoardPortOutInit_CPLD_CDC34,
    BoardPortOutInit_CPLD_CDC35,
    BoardPortOutInit_CPLD_CDC36,
    BoardPortOutInit_SLAVE_SEN17_PC4,
    BoardPortOutInit_SLAVE_SEN18_PA7,
    BoardPortOutInit_SLAVE_SEN19_PA6,
    BoardPortOutInit_SLAVE_SEN20_PA5,
    BoardPortOutInit_SLAVE_SEN21_PA4,
    BoardPortOutInit_SLAVE_SEN22_PA3,
    BoardPortOutInit_SLAVE_SEN23_PF8,
    BoardPortOutInit_SLAVE_SEN24_PF9,
    BoardPortOutInit_SLAVE_SEN25_PF10,
    BoardPortOutInit_SLAVE_SEN26_PH3,
    BoardPortOutInit_SLAVE_SEN27_PH2,
    BoardPortOutInit_SLAVE_SEN28_PA2,
    BoardPortOutInit_SLAVE_DC1_VAVLE_PC8,
    BoardPortOutInit_SLAVE_DC2_RUBBISH_MOTOR1_PG9,
    BoardPortOutInit_SLAVE_DC3_RUBBISH_MOTOR2_PD6,
    BoardPortOutInit_SLAVE_DC4_RUBBISH_MOTOR3_PI3,
    BoardPortOutInit_SLAVE_DC5_RUBBISH_MOTOR4_PI2,
    BoardPortOutInit_YM1_ARM3_PB4,
    BoardPortOutInit_YM1_ARM4_PG15,
};

//函数指针数组
static const BoardPortOutWriteFuncPtr BoardPortOutWriteFuncPtrArray[BOARD_PORT_OUT_COUNT] = {
    BoardPortOutWrite_CPLD_CDC1,
    BoardPortOutWrite_CPLD_CDC2,
    BoardPortOutWrite_CPLD_CDC3,
    BoardPortOutWrite_CPLD_CDC4,
    BoardPortOutWrite_CPLD_CDC5,
    BoardPortOutWrite_CPLD_CDC6,
    BoardPortOutWrite_CPLD_CDC7,
    BoardPortOutWrite_CPLD_CDC8,
    BoardPortOutWrite_CPLD_CDC9,
    BoardPortOutWrite_CPLD_CDC10,
    BoardPortOutWrite_CPLD_CDC11,
    BoardPortOutWrite_CPLD_CDC12,
    BoardPortOutWrite_CPLD_CDC13,
    BoardPortOutWrite_CPLD_CDC14,
    BoardPortOutWrite_CPLD_CDC15,
    BoardPortOutWrite_CPLD_CDC16,
    BoardPortOutWrite_CPLD_CDC17,
    BoardPortOutWrite_CPLD_CDC18,
    BoardPortOutWrite_CPLD_CDC19,
    BoardPortOutWrite_CPLD_CDC20,
    BoardPortOutWrite_CPLD_CDC21,
    BoardPortOutWrite_CPLD_CDC22,
    BoardPortOutWrite_CPLD_CDC23,
    BoardPortOutWrite_CPLD_CDC24,
    BoardPortOutWrite_CPLD_CDC25,
    BoardPortOutWrite_CPLD_CDC26,
    BoardPortOutWrite_CPLD_CDC27,
    BoardPortOutWrite_CPLD_CDC28,
    BoardPortOutWrite_CPLD_CDC29,
    BoardPortOutWrite_CPLD_CDC30,
    BoardPortOutWrite_CPLD_CDC31,
    BoardPortOutWrite_CPLD_CDC32,
    BoardPortOutWrite_CPLD_CDC33,
    BoardPortOutWrite_CPLD_CDC34,
    BoardPortOutWrite_CPLD_CDC35,
    BoardPortOutWrite_CPLD_CDC36,
    BoardPortOutWrite_SLAVE_SEN17_PC4,
    BoardPortOutWrite_SLAVE_SEN18_PA7,
    BoardPortOutWrite_SLAVE_SEN19_PA6,
    BoardPortOutWrite_SLAVE_SEN20_PA5,
    BoardPortOutWrite_SLAVE_SEN21_PA4,
    BoardPortOutWrite_SLAVE_SEN22_PA3,
    BoardPortOutWrite_SLAVE_SEN23_PF8,
    BoardPortOutWrite_SLAVE_SEN24_PF9,
    BoardPortOutWrite_SLAVE_SEN25_PF10,
    BoardPortOutWrite_SLAVE_SEN26_PH3,
    BoardPortOutWrite_SLAVE_SEN27_PH2,
    BoardPortOutWrite_SLAVE_SEN28_PA2,
    BoardPortOutWrite_SLAVE_DC1_VAVLE_PC8,
    BoardPortOutWrite_SLAVE_DC2_RUBBISH_MOTOR1_PG9,
    BoardPortOutWrite_SLAVE_DC3_RUBBISH_MOTOR2_PD6,
    BoardPortOutWrite_SLAVE_DC4_RUBBISH_MOTOR3_PI3,
    BoardPortOutWrite_SLAVE_DC5_RUBBISH_MOTOR4_PI2,
    BoardPortOutWrite_YM1_ARM3_PB4,
    BoardPortOutWrite_YM1_ARM4_PG15,
};


//输出端口初始化全部
void BoardPortOutInitAll(void)
{
    uint16_t indexUtil = 0; 
    for(indexUtil = 0; indexUtil < BOARD_PORT_OUT_COUNT; indexUtil++)
    {
        BoardPortOutInitSingle((BOARD_PORT_OUT)indexUtil);
    }
}

//输出端口初始化单个
void BoardPortOutInitSingle(BOARD_PORT_OUT pinIndex)
{
    if(pinIndex >= BOARD_PORT_OUT_COUNT)
    {
        return;
    }
    BoardPortOutInitFuncPtrArray[pinIndex]();
}

//输出端口写入单个端口状态
void BoardPortOutWriteSingleState(BOARD_PORT_OUT pinIndex,GPIO_PinState value)
{
    if(pinIndex >= BOARD_PORT_OUT_COUNT)
    {
        return;
    }
    BoardPortOutWriteFuncPtrArray[pinIndex](value);
}





