/************************************************************************ 
 ** Author       : 邓小俊
 ** Date         : 2024-01-16 11:35:28
 ** Description  : When I Has Free Time ,I Will Write Description Below.
 ** description  : 
 ** LastEditors  : 邓小俊
 ** LastEditTime : 2024-01-16 14:37:51
 ** Email: xiaojun.deng@aliyun.com
 ** Copyright (c) 2024 by 公司名称, All Rights Reserved. 
 ************************************************************************/
#include "BoardPortOut.h"
#include "BoardExtendPort.h"

//外部导入初始化配置
extern const GPIO_PinState boardPortOutInitLevelConfigArray[];
extern const MCU_PIN_PULL boardPortOutInitPullConfigArray[];

//输出初始化函数指针
typedef void (*BoardPortOutInitFuncPtr)(void);
//输出指定值
typedef void (*BoardPortOutWriteFuncPtr)(GPIO_PinState value);
//输出值反转
typedef void (*BoardPortOutToogleFuncPtr)(void);

/*********************************************输出端口初始化*****************************************************/
void BoardPortOutInit_SMBUS_OUT1_IO_OUT1(void)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    //不用初始化,直接设置为默认值
    errorSub = BoardExtendPortOutWriteSingle(BOARD_EXTEND_PORT_OUT_PIN_1,
                    boardPortOutInitLevelConfigArray[BOARD_PORT_OUT_SMBUS_OUT1_IO_OUT1]);
    if(errorSub != ERROR_SUB_OK)
    {
        MCU_LogShowErrorSubCode(errorSub);
    }
    return;
}

void BoardPortOutInit_SMBUS_OUT2_IO_OUT2(void)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    //不用初始化,直接设置为默认值
    errorSub = BoardExtendPortOutWriteSingle(BOARD_EXTEND_PORT_OUT_PIN_2,
                    boardPortOutInitLevelConfigArray[BOARD_PORT_OUT_SMBUS_OUT2_IO_OUT2]);
    if(errorSub != ERROR_SUB_OK)
    {
        MCU_LogShowErrorSubCode(errorSub);
    }
    return;
}

void BoardPortOutInit_SMBUS_OUT3_IO_OUT3(void)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    //不用初始化,直接设置为默认值
    errorSub = BoardExtendPortOutWriteSingle(BOARD_EXTEND_PORT_OUT_PIN_3,
                    boardPortOutInitLevelConfigArray[BOARD_PORT_OUT_SMBUS_OUT3_IO_OUT3]);
    if(errorSub != ERROR_SUB_OK)
    {
        MCU_LogShowErrorSubCode(errorSub);
    }
    return;
}

void BoardPortOutInit_SMBUS_OUT4_IO_OUT4(void)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    //不用初始化,直接设置为默认值
    errorSub = BoardExtendPortOutWriteSingle(BOARD_EXTEND_PORT_OUT_PIN_4,
                    boardPortOutInitLevelConfigArray[BOARD_PORT_OUT_SMBUS_OUT4_IO_OUT4]);
    if(errorSub != ERROR_SUB_OK)
    {
        MCU_LogShowErrorSubCode(errorSub);
    }
    return;
}

void BoardPortOutInit_SMBUS_OUT5_IO_OUT5(void)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    //不用初始化,直接设置为默认值
    errorSub = BoardExtendPortOutWriteSingle(BOARD_EXTEND_PORT_OUT_PIN_5,
                    boardPortOutInitLevelConfigArray[BOARD_PORT_OUT_SMBUS_OUT5_IO_OUT5]);
    if(errorSub != ERROR_SUB_OK)
    {
        MCU_LogShowErrorSubCode(errorSub);
    }
    return;
}

void BoardPortOutInit_SMBUS_OUT6_IO_OUT6(void)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    //不用初始化,直接设置为默认值
    errorSub = BoardExtendPortOutWriteSingle(BOARD_EXTEND_PORT_OUT_PIN_6,
                    boardPortOutInitLevelConfigArray[BOARD_PORT_OUT_SMBUS_OUT6_IO_OUT6]);
    if(errorSub != ERROR_SUB_OK)
    {
        MCU_LogShowErrorSubCode(errorSub);
    }
    return;
}

void BoardPortOutInit_SMBUS_OUT7_IO_OUT7(void)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    //不用初始化,直接设置为默认值
    errorSub = BoardExtendPortOutWriteSingle(BOARD_EXTEND_PORT_OUT_PIN_7,
                    boardPortOutInitLevelConfigArray[BOARD_PORT_OUT_SMBUS_OUT7_IO_OUT7]);
    if(errorSub != ERROR_SUB_OK)
    {
        MCU_LogShowErrorSubCode(errorSub);
    }
    return;
}

void BoardPortOutInit_SMBUS_OUT8_IO_OUT8(void)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    //不用初始化,直接设置为默认值
    errorSub = BoardExtendPortOutWriteSingle(BOARD_EXTEND_PORT_OUT_PIN_8,
                    boardPortOutInitLevelConfigArray[BOARD_PORT_OUT_SMBUS_OUT8_IO_OUT8]);
    if(errorSub != ERROR_SUB_OK)
    {
        MCU_LogShowErrorSubCode(errorSub);
    }
    return;
}

void BoardPortOutInit_SMBUS_OUT9_IO_OUT9(void)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    //不用初始化,直接设置为默认值
    errorSub = BoardExtendPortOutWriteSingle(BOARD_EXTEND_PORT_OUT_PIN_9,
                    boardPortOutInitLevelConfigArray[BOARD_PORT_OUT_SMBUS_OUT9_IO_OUT9]);
    if(errorSub != ERROR_SUB_OK)
    {
        MCU_LogShowErrorSubCode(errorSub);
    }
    return;
}

void BoardPortOutInit_SMBUS_OUT10_IO_OUT10(void)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    //不用初始化,直接设置为默认值
    errorSub = BoardExtendPortOutWriteSingle(BOARD_EXTEND_PORT_OUT_PIN_10,
                    boardPortOutInitLevelConfigArray[BOARD_PORT_OUT_SMBUS_OUT10_IO_OUT10]);
    if(errorSub != ERROR_SUB_OK)
    {
        MCU_LogShowErrorSubCode(errorSub);
    }
    return;
}

void BoardPortOutInit_SMBUS_OUT11_IO_OUT11(void)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    //不用初始化,直接设置为默认值
    errorSub = BoardExtendPortOutWriteSingle(BOARD_EXTEND_PORT_OUT_PIN_11,
                    boardPortOutInitLevelConfigArray[BOARD_PORT_OUT_SMBUS_OUT11_IO_OUT11]);
    if(errorSub != ERROR_SUB_OK)
    {
        MCU_LogShowErrorSubCode(errorSub);
    }
    return;
}

void BoardPortOutInit_SMBUS_OUT12_IO_OUT12(void)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    //不用初始化,直接设置为默认值
    errorSub = BoardExtendPortOutWriteSingle(BOARD_EXTEND_PORT_OUT_PIN_12,
                    boardPortOutInitLevelConfigArray[BOARD_PORT_OUT_SMBUS_OUT12_IO_OUT12]);
    if(errorSub != ERROR_SUB_OK)
    {
        MCU_LogShowErrorSubCode(errorSub);
    }
    return;
}

void BoardPortOutInit_SMBUS_OUT13_IO_OUT13(void)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    //不用初始化,直接设置为默认值
    errorSub = BoardExtendPortOutWriteSingle(BOARD_EXTEND_PORT_OUT_PIN_13,
                    boardPortOutInitLevelConfigArray[BOARD_PORT_OUT_SMBUS_OUT13_IO_OUT13]);
    if(errorSub != ERROR_SUB_OK)
    {
        MCU_LogShowErrorSubCode(errorSub);
    }
    return;
}

void BoardPortOutInit_SMBUS_OUT14_IO_OUT14(void)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    //不用初始化,直接设置为默认值
    errorSub = BoardExtendPortOutWriteSingle(BOARD_EXTEND_PORT_OUT_PIN_14,
                    boardPortOutInitLevelConfigArray[BOARD_PORT_OUT_SMBUS_OUT14_IO_OUT14]);
    if(errorSub != ERROR_SUB_OK)
    {
        MCU_LogShowErrorSubCode(errorSub);
    }
    return;
}

void BoardPortOutInit_SMBUS_OUT15_IO_OUT15(void)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    //不用初始化,直接设置为默认值
    errorSub = BoardExtendPortOutWriteSingle(BOARD_EXTEND_PORT_OUT_PIN_15,
                    boardPortOutInitLevelConfigArray[BOARD_PORT_OUT_SMBUS_OUT15_IO_OUT15]);
    if(errorSub != ERROR_SUB_OK)
    {
        MCU_LogShowErrorSubCode(errorSub);
    }
    return;
}

void BoardPortOutInit_SMBUS_OUT16_IO_OUT16(void)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    //不用初始化,直接设置为默认值
    errorSub = BoardExtendPortOutWriteSingle(BOARD_EXTEND_PORT_OUT_PIN_16,
                    boardPortOutInitLevelConfigArray[BOARD_PORT_OUT_SMBUS_OUT16_IO_OUT16]);
    if(errorSub != ERROR_SUB_OK)
    {
        MCU_LogShowErrorSubCode(errorSub);
    }
    return;
}


/********************************************设置电平状态*********************************/
void BoardPortOutWrite_SMBUS_OUT1_IO_OUT1(GPIO_PinState setValue)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    //不用初始化,直接设置为默认值
    errorSub = BoardExtendPortOutWriteSingle(BOARD_EXTEND_PORT_OUT_PIN_1,setValue);
    if(errorSub != ERROR_SUB_OK)
    {
        MCU_LogShowErrorSubCode(errorSub);
    }
    return;
}

void BoardPortOutWrite_SMBUS_OUT2_IO_OUT2(GPIO_PinState setValue)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    //不用初始化,直接设置为默认值
    errorSub = BoardExtendPortOutWriteSingle(BOARD_EXTEND_PORT_OUT_PIN_2,setValue);
    if(errorSub != ERROR_SUB_OK)
    {
        MCU_LogShowErrorSubCode(errorSub);
    }
    return;
}

void BoardPortOutWrite_SMBUS_OUT3_IO_OUT3(GPIO_PinState setValue)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    //不用初始化,直接设置为默认值
    errorSub = BoardExtendPortOutWriteSingle(BOARD_EXTEND_PORT_OUT_PIN_3,setValue);
    if(errorSub != ERROR_SUB_OK)
    {
        MCU_LogShowErrorSubCode(errorSub);
    }
    return;
}

void BoardPortOutWrite_SMBUS_OUT4_IO_OUT4(GPIO_PinState setValue)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    //不用初始化,直接设置为默认值
    errorSub = BoardExtendPortOutWriteSingle(BOARD_EXTEND_PORT_OUT_PIN_4,setValue);
    if(errorSub != ERROR_SUB_OK)
    {
        MCU_LogShowErrorSubCode(errorSub);
    }
    return;
}

void BoardPortOutWrite_SMBUS_OUT5_IO_OUT5(GPIO_PinState setValue)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    //不用初始化,直接设置为默认值
    errorSub = BoardExtendPortOutWriteSingle(BOARD_EXTEND_PORT_OUT_PIN_5,setValue);
    if(errorSub != ERROR_SUB_OK)
    {
        MCU_LogShowErrorSubCode(errorSub);
    }
    return;
}

void BoardPortOutWrite_SMBUS_OUT6_IO_OUT6(GPIO_PinState setValue)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    //不用初始化,直接设置为默认值
    errorSub = BoardExtendPortOutWriteSingle(BOARD_EXTEND_PORT_OUT_PIN_6,setValue);
    if(errorSub != ERROR_SUB_OK)
    {
        MCU_LogShowErrorSubCode(errorSub);
    }
    return;
}

void BoardPortOutWrite_SMBUS_OUT7_IO_OUT7(GPIO_PinState setValue)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    //不用初始化,直接设置为默认值
    errorSub = BoardExtendPortOutWriteSingle(BOARD_EXTEND_PORT_OUT_PIN_7,setValue);
    if(errorSub != ERROR_SUB_OK)
    {
        MCU_LogShowErrorSubCode(errorSub);
    }
    return;
}

void BoardPortOutWrite_SMBUS_OUT8_IO_OUT8(GPIO_PinState setValue)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    //不用初始化,直接设置为默认值
    errorSub = BoardExtendPortOutWriteSingle(BOARD_EXTEND_PORT_OUT_PIN_8,setValue);
    if(errorSub != ERROR_SUB_OK)
    {
        MCU_LogShowErrorSubCode(errorSub);
    }
    return;
}

void BoardPortOutWrite_SMBUS_OUT9_IO_OUT9(GPIO_PinState setValue)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    //不用初始化,直接设置为默认值
    errorSub = BoardExtendPortOutWriteSingle(BOARD_EXTEND_PORT_OUT_PIN_9,setValue);
    if(errorSub != ERROR_SUB_OK)
    {
        MCU_LogShowErrorSubCode(errorSub);
    }
    return;
}

void BoardPortOutWrite_SMBUS_OUT10_IO_OUT10(GPIO_PinState setValue)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    //不用初始化,直接设置为默认值
    errorSub = BoardExtendPortOutWriteSingle(BOARD_EXTEND_PORT_OUT_PIN_10,setValue);
    if(errorSub != ERROR_SUB_OK)
    {
        MCU_LogShowErrorSubCode(errorSub);
    }
    return;
}

void BoardPortOutWrite_SMBUS_OUT11_IO_OUT11(GPIO_PinState setValue)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    //不用初始化,直接设置为默认值
    errorSub = BoardExtendPortOutWriteSingle(BOARD_EXTEND_PORT_OUT_PIN_11,setValue);
    if(errorSub != ERROR_SUB_OK)
    {
        MCU_LogShowErrorSubCode(errorSub);
    }
    return;
}

void BoardPortOutWrite_SMBUS_OUT12_IO_OUT12(GPIO_PinState setValue)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    //不用初始化,直接设置为默认值
    errorSub = BoardExtendPortOutWriteSingle(BOARD_EXTEND_PORT_OUT_PIN_12,setValue);
    if(errorSub != ERROR_SUB_OK)
    {
        MCU_LogShowErrorSubCode(errorSub);
    }
    return;
}

void BoardPortOutWrite_SMBUS_OUT13_IO_OUT13(GPIO_PinState setValue)
{   
    ERROR_SUB errorSub = ERROR_SUB_OK;
    //不用初始化,直接设置为默认值
    errorSub = BoardExtendPortOutWriteSingle(BOARD_EXTEND_PORT_OUT_PIN_13,setValue);
    if(errorSub != ERROR_SUB_OK)
    {
        MCU_LogShowErrorSubCode(errorSub);
    }
    return;
}

void BoardPortOutWrite_SMBUS_OUT14_IO_OUT14(GPIO_PinState setValue)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    //不用初始化,直接设置为默认值
    errorSub = BoardExtendPortOutWriteSingle(BOARD_EXTEND_PORT_OUT_PIN_14,setValue);
    if(errorSub != ERROR_SUB_OK)
    {
        MCU_LogShowErrorSubCode(errorSub);
    }
    return;
}

void BoardPortOutWrite_SMBUS_OUT15_IO_OUT15(GPIO_PinState setValue)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    //不用初始化,直接设置为默认值
    errorSub = BoardExtendPortOutWriteSingle(BOARD_EXTEND_PORT_OUT_PIN_15,setValue);
    if(errorSub != ERROR_SUB_OK)
    {
        MCU_LogShowErrorSubCode(errorSub);
    }
    return;
}

void BoardPortOutWrite_SMBUS_OUT16_IO_OUT16(GPIO_PinState setValue)
{   
    ERROR_SUB errorSub = ERROR_SUB_OK;
    //不用初始化,直接设置为默认值
    errorSub = BoardExtendPortOutWriteSingle(BOARD_EXTEND_PORT_OUT_PIN_16,setValue);
    if(errorSub != ERROR_SUB_OK)
    {
        MCU_LogShowErrorSubCode(errorSub);
    }
    return;
}


/*********************************************输出端口输出值反转*****************************************************/
static void BoardPortOutToogle_SMBUS_OUT1_IO_OUT1(void)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    //反转状态
    errorSub = BoardExtendPortOutToogleSingle(BOARD_EXTEND_PORT_OUT_PIN_1);
    if(errorSub != ERROR_SUB_OK)
    {
        MCU_LogShowErrorSubCode(errorSub);
    }
    return;
}

static void BoardPortOutToogle_SMBUS_OUT2_IO_OUT2(void)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    //反转状态
    errorSub = BoardExtendPortOutToogleSingle(BOARD_EXTEND_PORT_OUT_PIN_2);
    if(errorSub != ERROR_SUB_OK)
    {
        MCU_LogShowErrorSubCode(errorSub);
    }
    return;
}

static void BoardPortOutToogle_SMBUS_OUT3_IO_OUT3(void)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    //反转状态
    errorSub = BoardExtendPortOutToogleSingle(BOARD_EXTEND_PORT_OUT_PIN_3);
    if(errorSub != ERROR_SUB_OK)
    {
        MCU_LogShowErrorSubCode(errorSub);
    }
    return;
}

static void BoardPortOutToogle_SMBUS_OUT4_IO_OUT4(void)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    //反转状态
    errorSub = BoardExtendPortOutToogleSingle(BOARD_EXTEND_PORT_OUT_PIN_4);
    if(errorSub != ERROR_SUB_OK)
    {
        MCU_LogShowErrorSubCode(errorSub);
    }
    return;
}

static void BoardPortOutToogle_SMBUS_OUT5_IO_OUT5(void)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    //反转状态
    errorSub = BoardExtendPortOutToogleSingle(BOARD_EXTEND_PORT_OUT_PIN_5);
    if(errorSub != ERROR_SUB_OK)
    {
        MCU_LogShowErrorSubCode(errorSub);
    }
    return;
}

static void BoardPortOutToogle_SMBUS_OUT6_IO_OUT6(void)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    //反转状态
    errorSub = BoardExtendPortOutToogleSingle(BOARD_EXTEND_PORT_OUT_PIN_6);
    if(errorSub != ERROR_SUB_OK)
    {
        MCU_LogShowErrorSubCode(errorSub);
    }
    return;
}

static void BoardPortOutToogle_SMBUS_OUT7_IO_OUT7(void)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    //反转状态
    errorSub = BoardExtendPortOutToogleSingle(BOARD_EXTEND_PORT_OUT_PIN_7);
    if(errorSub != ERROR_SUB_OK)
    {
        MCU_LogShowErrorSubCode(errorSub);
    }
    return;
}

static void BoardPortOutToogle_SMBUS_OUT8_IO_OUT8(void)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    //反转状态
    errorSub = BoardExtendPortOutToogleSingle(BOARD_EXTEND_PORT_OUT_PIN_8);
    if(errorSub != ERROR_SUB_OK)
    {
        MCU_LogShowErrorSubCode(errorSub);
    }
    return;
}

static void BoardPortOutToogle_SMBUS_OUT9_IO_OUT9(void)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    //反转状态
    errorSub = BoardExtendPortOutToogleSingle(BOARD_EXTEND_PORT_OUT_PIN_9);
    if(errorSub != ERROR_SUB_OK)
    {
        MCU_LogShowErrorSubCode(errorSub);
    }
    return;
}

static void BoardPortOutToogle_SMBUS_OUT10_IO_OUT10(void)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    //反转状态
    errorSub = BoardExtendPortOutToogleSingle(BOARD_EXTEND_PORT_OUT_PIN_10);
    if(errorSub != ERROR_SUB_OK)
    {
        MCU_LogShowErrorSubCode(errorSub);
    }
    return;
}

static void BoardPortOutToogle_SMBUS_OUT11_IO_OUT11(void)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    //反转状态
    errorSub = BoardExtendPortOutToogleSingle(BOARD_EXTEND_PORT_OUT_PIN_11);
    if(errorSub != ERROR_SUB_OK)
    {
        MCU_LogShowErrorSubCode(errorSub);
    }
    return;
}

static void BoardPortOutToogle_SMBUS_OUT12_IO_OUT12(void)
{   
    ERROR_SUB errorSub = ERROR_SUB_OK;
    //反转状态
    errorSub = BoardExtendPortOutToogleSingle(BOARD_EXTEND_PORT_OUT_PIN_12);
    if(errorSub != ERROR_SUB_OK)
    {
        MCU_LogShowErrorSubCode(errorSub);
    }
    return;
}

static void BoardPortOutToogle_SMBUS_OUT13_IO_OUT13(void)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    //反转状态
    errorSub = BoardExtendPortOutToogleSingle(BOARD_EXTEND_PORT_OUT_PIN_13);
    if(errorSub != ERROR_SUB_OK)
    {
        MCU_LogShowErrorSubCode(errorSub);
    }
    return;
}

static void BoardPortOutToogle_SMBUS_OUT14_IO_OUT14(void)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    //反转状态
    errorSub = BoardExtendPortOutToogleSingle(BOARD_EXTEND_PORT_OUT_PIN_14);
    if(errorSub != ERROR_SUB_OK)
    {
        MCU_LogShowErrorSubCode(errorSub);
    }
    return;
}

static void BoardPortOutToogle_SMBUS_OUT15_IO_OUT15(void)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    //反转状态
    errorSub = BoardExtendPortOutToogleSingle(BOARD_EXTEND_PORT_OUT_PIN_15);
    if(errorSub != ERROR_SUB_OK)
    {
        MCU_LogShowErrorSubCode(errorSub);
    }
    return;
}

static void BoardPortOutToogle_SMBUS_OUT16_IO_OUT16(void)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    //反转状态
    errorSub = BoardExtendPortOutToogleSingle(BOARD_EXTEND_PORT_OUT_PIN_16);
    if(errorSub != ERROR_SUB_OK)
    {
        MCU_LogShowErrorSubCode(errorSub);
    }
    return;
}

/*********************************************系统表驱动数组*****************************************************/
//函数指针数组
static const BoardPortOutInitFuncPtr BoardPortOutInitFuncPtrArray[BOARD_PORT_OUT_COUNT] = {
    BoardPortOutInit_SMBUS_OUT1_IO_OUT1       ,
    BoardPortOutInit_SMBUS_OUT2_IO_OUT2       ,
    BoardPortOutInit_SMBUS_OUT3_IO_OUT3       ,
    BoardPortOutInit_SMBUS_OUT4_IO_OUT4       ,
    BoardPortOutInit_SMBUS_OUT5_IO_OUT5       ,
    BoardPortOutInit_SMBUS_OUT6_IO_OUT6       ,
    BoardPortOutInit_SMBUS_OUT7_IO_OUT7       ,
    BoardPortOutInit_SMBUS_OUT8_IO_OUT8       ,
    BoardPortOutInit_SMBUS_OUT9_IO_OUT9       ,
    BoardPortOutInit_SMBUS_OUT10_IO_OUT10     ,
    BoardPortOutInit_SMBUS_OUT11_IO_OUT11     ,
    BoardPortOutInit_SMBUS_OUT12_IO_OUT12     ,
    BoardPortOutInit_SMBUS_OUT13_IO_OUT13     ,
    BoardPortOutInit_SMBUS_OUT14_IO_OUT14     ,
    BoardPortOutInit_SMBUS_OUT15_IO_OUT15     ,
    BoardPortOutInit_SMBUS_OUT16_IO_OUT16     ,
};

//函数指针数组
static const BoardPortOutWriteFuncPtr BoardPortOutWriteFuncPtrArray[BOARD_PORT_OUT_COUNT] = {
    BoardPortOutWrite_SMBUS_OUT1_IO_OUT1     ,
    BoardPortOutWrite_SMBUS_OUT2_IO_OUT2     ,
    BoardPortOutWrite_SMBUS_OUT3_IO_OUT3     ,
    BoardPortOutWrite_SMBUS_OUT4_IO_OUT4     ,
    BoardPortOutWrite_SMBUS_OUT5_IO_OUT5     ,
    BoardPortOutWrite_SMBUS_OUT6_IO_OUT6     ,    
    BoardPortOutWrite_SMBUS_OUT7_IO_OUT7     ,   
    BoardPortOutWrite_SMBUS_OUT8_IO_OUT8     ,
    BoardPortOutWrite_SMBUS_OUT9_IO_OUT9     ,
    BoardPortOutWrite_SMBUS_OUT10_IO_OUT10   ,
    BoardPortOutWrite_SMBUS_OUT11_IO_OUT11   ,
    BoardPortOutWrite_SMBUS_OUT12_IO_OUT12   ,
    BoardPortOutWrite_SMBUS_OUT13_IO_OUT13   ,
    BoardPortOutWrite_SMBUS_OUT14_IO_OUT14   ,
    BoardPortOutWrite_SMBUS_OUT15_IO_OUT15   ,
    BoardPortOutWrite_SMBUS_OUT16_IO_OUT16   ,       
};

//函数指针数组
static const BoardPortOutToogleFuncPtr BoardPortOutToogleFuncPtrArray[BOARD_PORT_OUT_COUNT] = {
    BoardPortOutToogle_SMBUS_OUT1_IO_OUT1     ,
    BoardPortOutToogle_SMBUS_OUT2_IO_OUT2     ,
    BoardPortOutToogle_SMBUS_OUT3_IO_OUT3     ,
    BoardPortOutToogle_SMBUS_OUT4_IO_OUT4     ,
    BoardPortOutToogle_SMBUS_OUT5_IO_OUT5     ,
    BoardPortOutToogle_SMBUS_OUT6_IO_OUT6     ,    
    BoardPortOutToogle_SMBUS_OUT7_IO_OUT7     ,   
    BoardPortOutToogle_SMBUS_OUT8_IO_OUT8     ,
    BoardPortOutToogle_SMBUS_OUT9_IO_OUT9     ,
    BoardPortOutToogle_SMBUS_OUT10_IO_OUT10   ,
    BoardPortOutToogle_SMBUS_OUT11_IO_OUT11   ,
    BoardPortOutToogle_SMBUS_OUT12_IO_OUT12   ,
    BoardPortOutToogle_SMBUS_OUT13_IO_OUT13   ,
    BoardPortOutToogle_SMBUS_OUT14_IO_OUT14   ,
    BoardPortOutToogle_SMBUS_OUT15_IO_OUT15   ,
    BoardPortOutToogle_SMBUS_OUT16_IO_OUT16   ,       
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

//端口反转指定输出状态
void BoardPortOutToogleSingleState(BOARD_PORT_OUT pinIndex)
{
    if(pinIndex >= BOARD_PORT_OUT_COUNT)
    {
        return;
    }
    BoardPortOutToogleFuncPtrArray[pinIndex]();
}


