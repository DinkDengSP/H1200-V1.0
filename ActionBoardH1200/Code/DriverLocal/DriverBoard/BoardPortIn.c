/************************************************************************ 
 ** Author       : 邓小俊
 ** Date         : 2024-01-16 11:35:28
 ** Description  : When I Has Free Time ,I Will Write Description Below.
 ** description  : 
 ** LastEditors  : 邓小俊
 ** LastEditTime : 2024-01-16 13:15:06
 ** Email: xiaojun.deng@aliyun.com
 ** Copyright (c) 2024 by 公司名称, All Rights Reserved. 
 ************************************************************************/
#include "BoardPortIn.h"
#include "BoardExtendPort.h"
#include "MCU_LogShow.h"

//外部导入初始化配置
extern const MCU_PIN_PULL boardPortInInitPullConfigArray[];
//导入外部使能配置
extern const FunctionalState boardPortInInitEnableConfigArray[];

//输入初始化函数指针
typedef void (*BoardPortInInitFuncPtr)(void);
//读取输入值状态
typedef GPIO_PinState (*BoardPortInReadValueFuncPtr)(void);

/***************************************输入端口初始化*******************************************/
static void BoardPortInInit_ARM_PF10_SEN_1(void)
{
    if(boardPortInInitEnableConfigArray[BOARD_PORT_IN_ARM_PF10_SEN_1] == DISABLE)
    {
        //不使用该IO
        return;
    }
    MCU_PinInit(MCU_PIN_F_10, MCU_PIN_MODE_INPUT,boardPortInInitPullConfigArray[BOARD_PORT_IN_ARM_PF10_SEN_1],
                    MCU_PIN_SPEED_FREQ_MEDIUM,0);
}

static void BoardPortInInit_ARM_PI11_SEN_2(void)
{
    if(boardPortInInitEnableConfigArray[BOARD_PORT_IN_ARM_PI11_SEN_2] == DISABLE)
    {
        //不使用该IO
        return;
    }
    MCU_PinInit(MCU_PIN_I_11, MCU_PIN_MODE_INPUT,boardPortInInitPullConfigArray[BOARD_PORT_IN_ARM_PI11_SEN_2],
                    MCU_PIN_SPEED_FREQ_MEDIUM,0);
}

static void BoardPortInInit_ARM_PI10_SEN_3(void)
{
    if(boardPortInInitEnableConfigArray[BOARD_PORT_IN_ARM_PI10_SEN_3] == DISABLE)
    {
        //不使用该IO
        return;
    }
    MCU_PinInit(MCU_PIN_I_10, MCU_PIN_MODE_INPUT,boardPortInInitPullConfigArray[BOARD_PORT_IN_ARM_PI10_SEN_3],
                    MCU_PIN_SPEED_FREQ_MEDIUM,0);
}

static void BoardPortInInit_ARM_PI9_SEN_4(void)
{
    if(boardPortInInitEnableConfigArray[BOARD_PORT_IN_ARM_PI9_SEN_4] == DISABLE)
    {
        //不使用该IO
        return;
    }
    MCU_PinInit(MCU_PIN_I_9, MCU_PIN_MODE_INPUT,boardPortInInitPullConfigArray[BOARD_PORT_IN_ARM_PI9_SEN_4],
                    MCU_PIN_SPEED_FREQ_MEDIUM,0);
}

static void BoardPortInInit_ARM_PC13_SEN_5(void)
{
    if(boardPortInInitEnableConfigArray[BOARD_PORT_IN_ARM_PC13_SEN_5] == DISABLE)
    {
        //不使用该IO
        return;
    }
    MCU_PinInit(MCU_PIN_C_13, MCU_PIN_MODE_INPUT,boardPortInInitPullConfigArray[BOARD_PORT_IN_ARM_PC13_SEN_5],
                    MCU_PIN_SPEED_FREQ_MEDIUM,0);
}

static void BoardPortInInit_ARM_PI8_SEN_6(void)
{
    if(boardPortInInitEnableConfigArray[BOARD_PORT_IN_ARM_PI8_SEN_6] == DISABLE)
    {
        //不使用该IO
        return;
    }
    MCU_PinInit(MCU_PIN_I_8, MCU_PIN_MODE_INPUT,boardPortInInitPullConfigArray[BOARD_PORT_IN_ARM_PI8_SEN_6],
                    MCU_PIN_SPEED_FREQ_MEDIUM,0);
}

static void BoardPortInInit_ARM_PE3_SEN_7(void)
{
    if(boardPortInInitEnableConfigArray[BOARD_PORT_IN_ARM_PE3_SEN_7] == DISABLE)
    {
        //不使用该IO
        return;
    }
    MCU_PinInit(MCU_PIN_E_3, MCU_PIN_MODE_INPUT,boardPortInInitPullConfigArray[BOARD_PORT_IN_ARM_PE3_SEN_7],
                    MCU_PIN_SPEED_FREQ_MEDIUM,0);
}

static void BoardPortInInit_ARM_PI7_SEN_8(void)
{
    if(boardPortInInitEnableConfigArray[BOARD_PORT_IN_ARM_PI7_SEN_8] == DISABLE)
    {
        //不使用该IO
        return;
    }
    MCU_PinInit(MCU_PIN_I_7, MCU_PIN_MODE_INPUT,boardPortInInitPullConfigArray[BOARD_PORT_IN_ARM_PI7_SEN_8],
                    MCU_PIN_SPEED_FREQ_MEDIUM,0);
}

static void BoardPortInInit_ARM_PI6_SEN_9(void)
{
    if(boardPortInInitEnableConfigArray[BOARD_PORT_IN_ARM_PI6_SEN_9] == DISABLE)
    {
        //不使用该IO
        return;
    }
    MCU_PinInit(MCU_PIN_I_6, MCU_PIN_MODE_INPUT,boardPortInInitPullConfigArray[BOARD_PORT_IN_ARM_PI6_SEN_9],
                    MCU_PIN_SPEED_FREQ_MEDIUM,0);
}

static void BoardPortInInit_ARM_PI5_SEN_10(void)
{
    if(boardPortInInitEnableConfigArray[BOARD_PORT_IN_ARM_PI5_SEN_10] == DISABLE)
    {
        //不使用该IO
        return;
    }
    MCU_PinInit(MCU_PIN_I_5, MCU_PIN_MODE_INPUT,boardPortInInitPullConfigArray[BOARD_PORT_IN_ARM_PI5_SEN_10],
                    MCU_PIN_SPEED_FREQ_MEDIUM,0);
}

static void BoardPortInInit_ARM_PI4_SEN_11(void)
{
    if(boardPortInInitEnableConfigArray[BOARD_PORT_IN_ARM_PI4_SEN_11] == DISABLE)
    {
        //不使用该IO
        return;
    }
    MCU_PinInit(MCU_PIN_I_4, MCU_PIN_MODE_INPUT,boardPortInInitPullConfigArray[BOARD_PORT_IN_ARM_PI4_SEN_11],
                    MCU_PIN_SPEED_FREQ_MEDIUM,0);
}

static void BoardPortInInit_ARM_PB9_SEN_12(void)
{
    if(boardPortInInitEnableConfigArray[BOARD_PORT_IN_ARM_PB9_SEN_12] == DISABLE)
    {
        //不使用该IO
        return;
    }
    MCU_PinInit(MCU_PIN_B_9, MCU_PIN_MODE_INPUT,boardPortInInitPullConfigArray[BOARD_PORT_IN_ARM_PB9_SEN_12],
                    MCU_PIN_SPEED_FREQ_MEDIUM,0);
}

static void BoardPortInInit_ARM_PB3_SEN_13(void)
{
    if(boardPortInInitEnableConfigArray[BOARD_PORT_IN_ARM_PB3_SEN_13] == DISABLE)
    {
        //不使用该IO
        return;
    }
    MCU_PinInit(MCU_PIN_B_3, MCU_PIN_MODE_INPUT,boardPortInInitPullConfigArray[BOARD_PORT_IN_ARM_PB3_SEN_13],
                    MCU_PIN_SPEED_FREQ_MEDIUM,0);
}

static void BoardPortInInit_ARM_PG9_SEN_14(void)
{
    if(boardPortInInitEnableConfigArray[BOARD_PORT_IN_ARM_PG9_SEN_14] == DISABLE)
    {
        //不使用该IO
        return;
    }
    MCU_PinInit(MCU_PIN_G_9, MCU_PIN_MODE_INPUT,boardPortInInitPullConfigArray[BOARD_PORT_IN_ARM_PG9_SEN_14],
                    MCU_PIN_SPEED_FREQ_MEDIUM,0);
}

static void BoardPortInInit_ARM_PC12_SEN_16(void)
{
    if(boardPortInInitEnableConfigArray[BOARD_PORT_IN_ARM_PC12_SEN_16] == DISABLE)
    {
        //不使用该IO
        return;
    }
    MCU_PinInit(MCU_PIN_C_12, MCU_PIN_MODE_INPUT,boardPortInInitPullConfigArray[BOARD_PORT_IN_ARM_PC12_SEN_16],
                    MCU_PIN_SPEED_FREQ_MEDIUM,0);
}

static void BoardPortInInit_ARM_PA15_SEN_17(void)
{
    if(boardPortInInitEnableConfigArray[BOARD_PORT_IN_ARM_PA15_SEN_17] == DISABLE)
    {
        //不使用该IO
        return;
    }
    MCU_PinInit(MCU_PIN_A_15, MCU_PIN_MODE_INPUT,boardPortInInitPullConfigArray[BOARD_PORT_IN_ARM_PA15_SEN_17],
                    MCU_PIN_SPEED_FREQ_MEDIUM,0);
}

static void BoardPortInInit_SMBUS_IN16_SEN_18(void)
{
    //扩展端口,配置使用不使用无效
    asm("nop");
}

static void BoardPortInInit_SMBUS_IN15_SEN_19(void)
{
    //扩展端口,配置使用不使用无效
    asm("nop");
}

static void BoardPortInInit_SMBUS_IN14_SEN_20(void)
{
    //扩展端口,配置使用不使用无效
    asm("nop");
}

static void BoardPortInInit_SMBUS_IN13_SEN_21(void)
{
    //扩展端口,配置使用不使用无效
    asm("nop");
}

static void BoardPortInInit_SMBUS_IN12(void)
{
    //扩展端口,配置使用不使用无效
    asm("nop");
}

static void BoardPortInInit_SMBUS_IN11(void)
{
    //扩展端口,配置使用不使用无效
    asm("nop");
}

static void BoardPortInInit_SMBUS_IN10(void)
{
    //扩展端口,配置使用不使用无效
    asm("nop");
}

static void BoardPortInInit_SMBUS_IN9(void)
{
    //扩展端口,配置使用不使用无效
    asm("nop");
}

static void BoardPortInInit_SMBUS_IN8(void)
{
    //扩展端口,配置使用不使用无效
    asm("nop");
}

static void BoardPortInInit_SMBUS_IN7(void)
{
    //扩展端口,配置使用不使用无效
    asm("nop");
}

static void BoardPortInInit_SMBUS_IN6(void)
{
    //扩展端口,配置使用不使用无效
    asm("nop");
}

static void BoardPortInInit_SMBUS_IN5(void)
{
    //扩展端口,配置使用不使用无效
    asm("nop");
}

static void BoardPortInInit_SMBUS_IN4(void)
{
    //扩展端口,配置使用不使用无效
    asm("nop");
}

static void BoardPortInInit_SMBUS_IN3(void)
{
    //扩展端口,配置使用不使用无效
    asm("nop");
}

static void BoardPortInInit_SMBUS_IN2(void)
{
    //扩展端口,配置使用不使用无效
    asm("nop");
}

static void BoardPortInInit_SMBUS_IN1(void)
{
    //扩展端口,配置使用不使用无效
    asm("nop");
}

/***************************************读取状态*******************************************/
static GPIO_PinState BoardPortInReadValue_ARM_PF10_SEN_1(void)
{
    if(boardPortInInitEnableConfigArray[BOARD_PORT_IN_ARM_PF10_SEN_1] == DISABLE)
    {
        MCU_LogPrintf("%s,Not Config Port In!!\r\n",__func__);
        //不使用该IO
        return GPIO_PIN_RESET;
    }
    //实际读取
    return MCU_PinReadSingle(MCU_PIN_F_10);
}

static GPIO_PinState BoardPortInReadValue_ARM_PI11_SEN_2(void)
{
    if(boardPortInInitEnableConfigArray[BOARD_PORT_IN_ARM_PI11_SEN_2] == DISABLE)
    {
        MCU_LogPrintf("%s,Not Config Port In!!\r\n",__func__);
        //不使用该IO
        return GPIO_PIN_RESET;
    }
    //实际读取
    return MCU_PinReadSingle(MCU_PIN_I_11);
}

static GPIO_PinState BoardPortInReadValue_ARM_PI10_SEN_3(void)
{
    if(boardPortInInitEnableConfigArray[BOARD_PORT_IN_ARM_PI10_SEN_3] == DISABLE)
    {
        MCU_LogPrintf("%s,Not Config Port In!!\r\n",__func__);
        //不使用该IO
        return GPIO_PIN_RESET;
    }
    //实际读取
    return MCU_PinReadSingle(MCU_PIN_I_10);
}

static GPIO_PinState BoardPortInReadValue_ARM_PI9_SEN_4(void)
{
    if(boardPortInInitEnableConfigArray[BOARD_PORT_IN_ARM_PI9_SEN_4] == DISABLE)
    {
        MCU_LogPrintf("%s,Not Config Port In!!\r\n",__func__);
        //不使用该IO
        return GPIO_PIN_RESET;
    }
    //实际读取
    return MCU_PinReadSingle(MCU_PIN_I_9);
}

static GPIO_PinState BoardPortInReadValue_ARM_PC13_SEN_5(void)
{
    if(boardPortInInitEnableConfigArray[BOARD_PORT_IN_ARM_PC13_SEN_5] == DISABLE)
    {
        MCU_LogPrintf("%s,Not Config Port In!!\r\n",__func__);
        //不使用该IO
        return GPIO_PIN_RESET;
    }
    //实际读取
    return MCU_PinReadSingle(MCU_PIN_C_13);
}

static GPIO_PinState BoardPortInReadValue_ARM_PI8_SEN_6(void)
{
    if(boardPortInInitEnableConfigArray[BOARD_PORT_IN_ARM_PI8_SEN_6] == DISABLE)
    {
        MCU_LogPrintf("%s,Not Config Port In!!\r\n",__func__);
        //不使用该IO
        return GPIO_PIN_RESET;
    }
    //实际读取
    return MCU_PinReadSingle(MCU_PIN_I_8);
}

static GPIO_PinState BoardPortInReadValue_ARM_PE3_SEN_7(void)
{
    if(boardPortInInitEnableConfigArray[BOARD_PORT_IN_ARM_PE3_SEN_7] == DISABLE)
    {
        MCU_LogPrintf("%s,Not Config Port In!!\r\n",__func__);
        //不使用该IO
        return GPIO_PIN_RESET;
    }
    //实际读取
    return MCU_PinReadSingle(MCU_PIN_E_3);
}

static GPIO_PinState BoardPortInReadValue_ARM_PI7_SEN_8(void)
{
    if(boardPortInInitEnableConfigArray[BOARD_PORT_IN_ARM_PI7_SEN_8] == DISABLE)
    {
        MCU_LogPrintf("%s,Not Config Port In!!\r\n",__func__);
        //不使用该IO
        return GPIO_PIN_RESET;
    }
    //实际读取
    return MCU_PinReadSingle(MCU_PIN_I_7);
}

static GPIO_PinState BoardPortInReadValue_ARM_PI6_SEN_9(void)
{
    if(boardPortInInitEnableConfigArray[BOARD_PORT_IN_ARM_PI6_SEN_9] == DISABLE)
    {
        MCU_LogPrintf("%s,Not Config Port In!!\r\n",__func__);
        //不使用该IO
        return GPIO_PIN_RESET;
    }
    //实际读取
    return MCU_PinReadSingle(MCU_PIN_I_6);
}

static GPIO_PinState BoardPortInReadValue_ARM_PI5_SEN_10(void)
{
    if(boardPortInInitEnableConfigArray[BOARD_PORT_IN_ARM_PI5_SEN_10] == DISABLE)
    {
        MCU_LogPrintf("%s,Not Config Port In!!\r\n",__func__);
        //不使用该IO
        return GPIO_PIN_RESET;
    }
    //实际读取
    return MCU_PinReadSingle(MCU_PIN_I_5);
}

static GPIO_PinState BoardPortInReadValue_ARM_PI4_SEN_11(void)
{
    if(boardPortInInitEnableConfigArray[BOARD_PORT_IN_ARM_PI4_SEN_11] == DISABLE)
    {
        MCU_LogPrintf("%s,Not Config Port In!!\r\n",__func__);
        //不使用该IO
        return GPIO_PIN_RESET;
    }
    //实际读取
    return MCU_PinReadSingle(MCU_PIN_I_4);
}

static GPIO_PinState BoardPortInReadValue_ARM_PB9_SEN_12(void)
{
    if(boardPortInInitEnableConfigArray[BOARD_PORT_IN_ARM_PB9_SEN_12] == DISABLE)
    {
        MCU_LogPrintf("%s,Not Config Port In!!\r\n",__func__);
        //不使用该IO
        return GPIO_PIN_RESET;
    }
    //实际读取
    return MCU_PinReadSingle(MCU_PIN_B_9);
}

static GPIO_PinState BoardPortInReadValue_ARM_PB3_SEN_13(void)
{
    if(boardPortInInitEnableConfigArray[BOARD_PORT_IN_ARM_PB3_SEN_13] == DISABLE)
    {
        MCU_LogPrintf("%s,Not Config Port In!!\r\n",__func__);
        //不使用该IO
        return GPIO_PIN_RESET;
    }
    //实际读取
    return MCU_PinReadSingle(MCU_PIN_B_3);
}

static GPIO_PinState BoardPortInReadValue_ARM_PG9_SEN_14(void)
{
    if(boardPortInInitEnableConfigArray[BOARD_PORT_IN_ARM_PG9_SEN_14] == DISABLE)
    {
        MCU_LogPrintf("%s,Not Config Port In!!\r\n",__func__);
        //不使用该IO
        return GPIO_PIN_RESET;
    }
    //实际读取
    return MCU_PinReadSingle(MCU_PIN_G_9);
}

static GPIO_PinState BoardPortInReadValue_ARM_PC12_SEN_16(void)
{   
    if(boardPortInInitEnableConfigArray[BOARD_PORT_IN_ARM_PC12_SEN_16] == DISABLE)
    {
        MCU_LogPrintf("%s,Not Config Port In!!\r\n",__func__);
        //不使用该IO
        return GPIO_PIN_RESET;
    }
    //实际读取
    return MCU_PinReadSingle(MCU_PIN_C_12);
}

static GPIO_PinState BoardPortInReadValue_ARM_PA15_SEN_17(void)
{
    if(boardPortInInitEnableConfigArray[BOARD_PORT_IN_ARM_PA15_SEN_17] == DISABLE)
    {
        MCU_LogPrintf("%s,Not Config Port In!!\r\n",__func__);
        //不使用该IO
        return GPIO_PIN_RESET;
    }
    //实际读取
    return MCU_PinReadSingle(MCU_PIN_A_15);
}

static GPIO_PinState BoardPortInReadValue_SMBUS_IN16_SEN_18(void)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    //扩展IO使能配置无效
    GPIO_PinState pinState = GPIO_PIN_RESET;
    errorSub = BoardExtendPortInSingle(BOARD_EXTEND_PORT_IN_PIN_16,&pinState);
    if(errorSub != ERROR_SUB_OK)
    {
        MCU_LogPrintf("%s,Read Error: 0X%08X!!\r\n",__func__,errorSub);
        //不使用该IO
        return pinState;
    }
    //正常完成
    return pinState;
}

static GPIO_PinState BoardPortInReadValue_SMBUS_IN15_SEN_19(void)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    //扩展IO使能配置无效
    GPIO_PinState pinState = GPIO_PIN_RESET;
    errorSub = BoardExtendPortInSingle(BOARD_EXTEND_PORT_IN_PIN_15,&pinState);
    if(errorSub != ERROR_SUB_OK)
    {
        MCU_LogPrintf("%s,Read Error: 0X%08X!!\r\n",__func__,errorSub);
        //不使用该IO
        return pinState;
    }
    //正常完成
    return pinState;
}

static GPIO_PinState BoardPortInReadValue_SMBUS_IN14_SEN_20(void)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    //扩展IO使能配置无效
    GPIO_PinState pinState = GPIO_PIN_RESET;
    errorSub = BoardExtendPortInSingle(BOARD_EXTEND_PORT_IN_PIN_14,&pinState);
    if(errorSub != ERROR_SUB_OK)
    {
        MCU_LogPrintf("%s,Read Error: 0X%08X!!\r\n",__func__,errorSub);
        //不使用该IO
        return pinState;
    }
    //正常完成
    return pinState;
}

static GPIO_PinState BoardPortInReadValue_SMBUS_IN13_SEN_21(void)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    //扩展IO使能配置无效
    GPIO_PinState pinState = GPIO_PIN_RESET;
    errorSub = BoardExtendPortInSingle(BOARD_EXTEND_PORT_IN_PIN_13,&pinState);
    if(errorSub != ERROR_SUB_OK)
    {
        MCU_LogPrintf("%s,Read Error: 0X%08X!!\r\n",__func__,errorSub);
        //不使用该IO
        return pinState;
    }
    //正常完成
    return pinState;
}

static GPIO_PinState BoardPortInReadValue_SMBUS_IN12(void)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    //扩展IO使能配置无效
    GPIO_PinState pinState = GPIO_PIN_RESET;
    errorSub = BoardExtendPortInSingle(BOARD_EXTEND_PORT_IN_PIN_12,&pinState);
    if(errorSub != ERROR_SUB_OK)
    {
        MCU_LogPrintf("%s,Read Error: 0X%08X!!\r\n",__func__,errorSub);
        //不使用该IO
        return pinState;
    }
    //正常完成
    return pinState;
}

static GPIO_PinState BoardPortInReadValue_SMBUS_IN11(void)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    //扩展IO使能配置无效
    GPIO_PinState pinState = GPIO_PIN_RESET;
    errorSub = BoardExtendPortInSingle(BOARD_EXTEND_PORT_IN_PIN_11,&pinState);
    if(errorSub != ERROR_SUB_OK)
    {
        MCU_LogPrintf("%s,Read Error: 0X%08X!!\r\n",__func__,errorSub);
        //不使用该IO
        return pinState;
    }
    //正常完成
    return pinState;
}

static GPIO_PinState BoardPortInReadValue_SMBUS_IN10(void)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    //扩展IO使能配置无效
    GPIO_PinState pinState = GPIO_PIN_RESET;
    errorSub = BoardExtendPortInSingle(BOARD_EXTEND_PORT_IN_PIN_10,&pinState);
    if(errorSub != ERROR_SUB_OK)
    {
        MCU_LogPrintf("%s,Read Error: 0X%08X!!\r\n",__func__,errorSub);
        //不使用该IO
        return pinState;
    }
    //正常完成
    return pinState;
}

static GPIO_PinState BoardPortInReadValue_SMBUS_IN9(void)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    //扩展IO使能配置无效
    GPIO_PinState pinState = GPIO_PIN_RESET;
    errorSub = BoardExtendPortInSingle(BOARD_EXTEND_PORT_IN_PIN_9,&pinState);
    if(errorSub != ERROR_SUB_OK)
    {
        MCU_LogPrintf("%s,Read Error: 0X%08X!!\r\n",__func__,errorSub);
        //不使用该IO
        return pinState;
    }
    //正常完成
    return pinState;
}

static GPIO_PinState BoardPortInReadValue_SMBUS_IN8(void)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    //扩展IO使能配置无效
    GPIO_PinState pinState = GPIO_PIN_RESET;
    errorSub = BoardExtendPortInSingle(BOARD_EXTEND_PORT_IN_PIN_8,&pinState);
    if(errorSub != ERROR_SUB_OK)
    {
        MCU_LogPrintf("%s,Read Error: 0X%08X!!\r\n",__func__,errorSub);
        //不使用该IO
        return pinState;
    }
    //正常完成
    return pinState;
}

static GPIO_PinState BoardPortInReadValue_SMBUS_IN7(void)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    //扩展IO使能配置无效
    GPIO_PinState pinState = GPIO_PIN_RESET;
    errorSub = BoardExtendPortInSingle(BOARD_EXTEND_PORT_IN_PIN_7,&pinState);
    if(errorSub != ERROR_SUB_OK)
    {
        MCU_LogPrintf("%s,Read Error: 0X%08X!!\r\n",__func__,errorSub);
        //不使用该IO
        return pinState;
    }
    //正常完成
    return pinState;
}

static GPIO_PinState BoardPortInReadValue_SMBUS_IN6(void)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    //扩展IO使能配置无效
    GPIO_PinState pinState = GPIO_PIN_RESET;
    errorSub = BoardExtendPortInSingle(BOARD_EXTEND_PORT_IN_PIN_6,&pinState);
    if(errorSub != ERROR_SUB_OK)
    {
        MCU_LogPrintf("%s,Read Error: 0X%08X!!\r\n",__func__,errorSub);
        //不使用该IO
        return pinState;
    }
    //正常完成
    return pinState;
}

static GPIO_PinState BoardPortInReadValue_SMBUS_IN5(void)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    //扩展IO使能配置无效
    GPIO_PinState pinState = GPIO_PIN_RESET;
    errorSub = BoardExtendPortInSingle(BOARD_EXTEND_PORT_IN_PIN_5,&pinState);
    if(errorSub != ERROR_SUB_OK)
    {
        MCU_LogPrintf("%s,Read Error: 0X%08X!!\r\n",__func__,errorSub);
        //不使用该IO
        return pinState;
    }
    //正常完成
    return pinState;
}

static GPIO_PinState BoardPortInReadValue_SMBUS_IN4(void)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    //扩展IO使能配置无效
    GPIO_PinState pinState = GPIO_PIN_RESET;
    errorSub = BoardExtendPortInSingle(BOARD_EXTEND_PORT_IN_PIN_4,&pinState);
    if(errorSub != ERROR_SUB_OK)
    {
        MCU_LogPrintf("%s,Read Error: 0X%08X!!\r\n",__func__,errorSub);
        //不使用该IO
        return pinState;
    }
    //正常完成
    return pinState;
}

static GPIO_PinState BoardPortInReadValue_SMBUS_IN3(void)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    //扩展IO使能配置无效
    GPIO_PinState pinState = GPIO_PIN_RESET;
    errorSub = BoardExtendPortInSingle(BOARD_EXTEND_PORT_IN_PIN_3,&pinState);
    if(errorSub != ERROR_SUB_OK)
    {
        MCU_LogPrintf("%s,Read Error: 0X%08X!!\r\n",__func__,errorSub);
        //不使用该IO
        return pinState;
    }
    //正常完成
    return pinState;
}

static GPIO_PinState BoardPortInReadValue_SMBUS_IN2(void)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    //扩展IO使能配置无效
    GPIO_PinState pinState = GPIO_PIN_RESET;
    errorSub = BoardExtendPortInSingle(BOARD_EXTEND_PORT_IN_PIN_2,&pinState);
    if(errorSub != ERROR_SUB_OK)
    {
        MCU_LogPrintf("%s,Read Error: 0X%08X!!\r\n",__func__,errorSub);
        //不使用该IO
        return pinState;
    }
    //正常完成
    return pinState;
}

static GPIO_PinState BoardPortInReadValue_SMBUS_IN1(void)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    //扩展IO使能配置无效
    GPIO_PinState pinState = GPIO_PIN_RESET;
    errorSub = BoardExtendPortInSingle(BOARD_EXTEND_PORT_IN_PIN_1,&pinState);
    if(errorSub != ERROR_SUB_OK)
    {
        MCU_LogPrintf("%s,Read Error: 0X%08X!!\r\n",__func__,errorSub);
        //不使用该IO
        return pinState;
    }
    //正常完成
    return pinState;
}

/**********************************函数指针数组*************************************/
static const BoardPortInInitFuncPtr BoardPortInInitFuncPtrArray[BOARD_PORT_IN_COUNT] = {
    BoardPortInInit_ARM_PF10_SEN_1,
    BoardPortInInit_ARM_PI11_SEN_2,
    BoardPortInInit_ARM_PI10_SEN_3,
    BoardPortInInit_ARM_PI9_SEN_4,
    BoardPortInInit_ARM_PC13_SEN_5,
    BoardPortInInit_ARM_PI8_SEN_6,
    BoardPortInInit_ARM_PE3_SEN_7,
    BoardPortInInit_ARM_PI7_SEN_8,
    BoardPortInInit_ARM_PI6_SEN_9,
    BoardPortInInit_ARM_PI5_SEN_10,
    BoardPortInInit_ARM_PI4_SEN_11,
    BoardPortInInit_ARM_PB9_SEN_12,
    BoardPortInInit_ARM_PB3_SEN_13,
    BoardPortInInit_ARM_PG9_SEN_14,
    BoardPortInInit_ARM_PC12_SEN_16,
    BoardPortInInit_ARM_PA15_SEN_17,
    BoardPortInInit_SMBUS_IN16_SEN_18,
    BoardPortInInit_SMBUS_IN15_SEN_19,
    BoardPortInInit_SMBUS_IN14_SEN_20,
    BoardPortInInit_SMBUS_IN13_SEN_21,
    BoardPortInInit_SMBUS_IN12,
    BoardPortInInit_SMBUS_IN11,
    BoardPortInInit_SMBUS_IN10,
    BoardPortInInit_SMBUS_IN9,
    BoardPortInInit_SMBUS_IN8,
    BoardPortInInit_SMBUS_IN7,
    BoardPortInInit_SMBUS_IN6,
    BoardPortInInit_SMBUS_IN5,
    BoardPortInInit_SMBUS_IN4,
    BoardPortInInit_SMBUS_IN3,
    BoardPortInInit_SMBUS_IN2,
    BoardPortInInit_SMBUS_IN1,
};

//函数指针数组
static const BoardPortInReadValueFuncPtr BoardPortInReadValueFuncPtrArray[BOARD_PORT_IN_COUNT] = {
    BoardPortInReadValue_ARM_PF10_SEN_1,
    BoardPortInReadValue_ARM_PI11_SEN_2,
    BoardPortInReadValue_ARM_PI10_SEN_3,
    BoardPortInReadValue_ARM_PI9_SEN_4,
    BoardPortInReadValue_ARM_PC13_SEN_5,
    BoardPortInReadValue_ARM_PI8_SEN_6,
    BoardPortInReadValue_ARM_PE3_SEN_7,
    BoardPortInReadValue_ARM_PI7_SEN_8,
    BoardPortInReadValue_ARM_PI6_SEN_9,
    BoardPortInReadValue_ARM_PI5_SEN_10,
    BoardPortInReadValue_ARM_PI4_SEN_11,
    BoardPortInReadValue_ARM_PB9_SEN_12,
    BoardPortInReadValue_ARM_PB3_SEN_13,
    BoardPortInReadValue_ARM_PG9_SEN_14,
    BoardPortInReadValue_ARM_PC12_SEN_16,
    BoardPortInReadValue_ARM_PA15_SEN_17,
    BoardPortInReadValue_SMBUS_IN16_SEN_18,
    BoardPortInReadValue_SMBUS_IN15_SEN_19,
    BoardPortInReadValue_SMBUS_IN14_SEN_20,
    BoardPortInReadValue_SMBUS_IN13_SEN_21,
    BoardPortInReadValue_SMBUS_IN12,
    BoardPortInReadValue_SMBUS_IN11,
    BoardPortInReadValue_SMBUS_IN10,
    BoardPortInReadValue_SMBUS_IN9,
    BoardPortInReadValue_SMBUS_IN8,
    BoardPortInReadValue_SMBUS_IN7,
    BoardPortInReadValue_SMBUS_IN6,
    BoardPortInReadValue_SMBUS_IN5,
    BoardPortInReadValue_SMBUS_IN4,
    BoardPortInReadValue_SMBUS_IN3,
    BoardPortInReadValue_SMBUS_IN2,
    BoardPortInReadValue_SMBUS_IN1,
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

//输入端口一次性读取全部信息
void BoardPortInAll(BOARD_PORT_IN_SET* portInSetPtr)
{
    int32_t indexUtil = 0; 
    GPIO_PinState pinStateLocal = GPIO_PIN_RESET;
    portInSetPtr->portInSetVal8Byte = 0XFFFFFFFFFFFFFFFF;
    for(indexUtil = (BOARD_PORT_IN_COUNT -1); indexUtil >= 0; indexUtil--)
    {
        //右移
        portInSetPtr->portInSetVal8Byte <<= 1;
        //读取
        pinStateLocal = BoardPortInReadState((BOARD_PORT_IN)indexUtil);
        //赋值
        portInSetPtr->portInSetVal8Byte += (uint8_t)(pinStateLocal);
    }
}


