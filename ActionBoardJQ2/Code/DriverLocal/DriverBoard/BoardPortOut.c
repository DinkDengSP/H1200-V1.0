/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-08-24 11:26:41 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-08-24 13:20:31 +0800
************************************************************************************************/ 
#include "BoardPortOut.h"

//外部导入初始化配置
extern const GPIO_PinState boardPortOutInitLevelConfigArray[];
extern const MCU_PIN_PULL boardPortOutInitPullConfigArray[];

//输出初始化函数指针
typedef void (*BoardPortOutInitFuncPtr)(void);
//输出指定值
typedef void (*BoardPortOutWriteFuncPtr)(GPIO_PinState value);


/*********************************************输出端口*****************************************************/
static void BoardPortOutInit_MCU_PB1_RBF_READYR(void)
{
    MCU_PinInit(MCU_PIN_B_1, MCU_PIN_MODE_OUTPUT_PP, boardPortOutInitPullConfigArray[BOARD_OUT_MCU_PB1_RBF_READYR], MCU_PIN_SPEED_FREQ_MEDIUM,0);
    MCU_PinWriteSingle(MCU_PIN_B_1,boardPortOutInitLevelConfigArray[BOARD_OUT_MCU_PB1_RBF_READYR]);
}

static void BoardPortOutInit_MCU_PF12_RBF_ENABLER(void)
{
    MCU_PinInit(MCU_PIN_F_12, MCU_PIN_MODE_OUTPUT_PP, boardPortOutInitPullConfigArray[BOARD_OUT_MCU_PF12_RBF_ENABLER], MCU_PIN_SPEED_FREQ_MEDIUM,0);
    MCU_PinWriteSingle(MCU_PIN_F_12,boardPortOutInitLevelConfigArray[BOARD_OUT_MCU_PF12_RBF_ENABLER]);
}

static void BoardPortOutInit_MCU_PF11_RBF_EXISTR(void)
{
    MCU_PinInit(MCU_PIN_F_11, MCU_PIN_MODE_OUTPUT_PP, boardPortOutInitPullConfigArray[BOARD_OUT_MCU_PF11_RBF_EXISTR], MCU_PIN_SPEED_FREQ_MEDIUM,0);
    MCU_PinWriteSingle(MCU_PIN_F_11,boardPortOutInitLevelConfigArray[BOARD_OUT_MCU_PF11_RBF_EXISTR]);
}

static void BoardPortOutInit_MCU_PE11_RBF_REQUESTS(void)
{
    MCU_PinInit(MCU_PIN_E_11, MCU_PIN_MODE_OUTPUT_PP, boardPortOutInitPullConfigArray[BOARD_OUT_MCU_PE11_RBF_REQUESTS], MCU_PIN_SPEED_FREQ_MEDIUM,0);
    MCU_PinWriteSingle(MCU_PIN_E_11,boardPortOutInitLevelConfigArray[BOARD_OUT_MCU_PE11_RBF_REQUESTS]);
}

static void BoardPortOutInit_MCU_PE12_RBF_MOVES(void)
{
    MCU_PinInit(MCU_PIN_E_12, MCU_PIN_MODE_OUTPUT_PP, boardPortOutInitPullConfigArray[BOARD_OUT_MCU_PE12_RBF_MOVES], MCU_PIN_SPEED_FREQ_MEDIUM,0);
    MCU_PinWriteSingle(MCU_PIN_E_12,boardPortOutInitLevelConfigArray[BOARD_OUT_MCU_PE12_RBF_MOVES]);
}
static void BoardPortOutInit_MCU_PG11_RED_LED(void)
{
    MCU_PinInit(MCU_PIN_G_11, MCU_PIN_MODE_OUTPUT_PP, boardPortOutInitPullConfigArray[BOARD_OUT_MCU_PG11_RED_LED], MCU_PIN_SPEED_FREQ_MEDIUM,0);
    MCU_PinWriteSingle(MCU_PIN_G_11,boardPortOutInitLevelConfigArray[BOARD_OUT_MCU_PG11_RED_LED]);
}
static void BoardPortOutInit_MCU_PG12_GREEN_LED(void)
{
    MCU_PinInit(MCU_PIN_G_12, MCU_PIN_MODE_OUTPUT_PP, boardPortOutInitPullConfigArray[BOARD_OUT_MCU_PG12_GREEN_LED], MCU_PIN_SPEED_FREQ_MEDIUM,0);
    MCU_PinWriteSingle(MCU_PIN_G_12,boardPortOutInitLevelConfigArray[BOARD_OUT_MCU_PG12_GREEN_LED]);
}
static void BoardPortOutInit_MCU_PG13_YELLOW_LED(void)
{
    MCU_PinInit(MCU_PIN_G_13, MCU_PIN_MODE_OUTPUT_PP, boardPortOutInitPullConfigArray[BOARD_OUT_MCU_PG13_YELLOW_LED], MCU_PIN_SPEED_FREQ_MEDIUM,0);
    MCU_PinWriteSingle(MCU_PIN_G_13,boardPortOutInitLevelConfigArray[BOARD_OUT_MCU_PG13_YELLOW_LED]);
}
static void BoardPortOutInit_MCU_PG14_BUZZER(void)
{
    MCU_PinInit(MCU_PIN_G_14, MCU_PIN_MODE_OUTPUT_PP, boardPortOutInitPullConfigArray[BOARD_OUT_MCU_PG14_BUZZER], MCU_PIN_SPEED_FREQ_MEDIUM,0);
    MCU_PinWriteSingle(MCU_PIN_G_14,boardPortOutInitLevelConfigArray[BOARD_OUT_MCU_PG14_BUZZER]);
}
static void BoardPortOutInit_MCU_PC4_RBF_AU_LED(void)
{
    MCU_PinInit(MCU_PIN_C_4, MCU_PIN_MODE_OUTPUT_PP, boardPortOutInitPullConfigArray[BOARD_OUT_MCU_PC4_RBF_AU_LED], MCU_PIN_SPEED_FREQ_MEDIUM,0);
    MCU_PinWriteSingle(MCU_PIN_C_4,boardPortOutInitLevelConfigArray[BOARD_OUT_MCU_PC4_RBF_AU_LED]);
}

/********************************************设置电平状态*********************************/

static void BoardPortOutWrite_MCU_PB1_RBF_READYR(GPIO_PinState setValue)
{
    MCU_PinWriteSingle(MCU_PIN_B_1,setValue);
}

static void BoardPortOutWrite_MCU_PF12_RBF_ENABLER(GPIO_PinState setValue)
{
    MCU_PinWriteSingle(MCU_PIN_F_12,setValue);
}

static void BoardPortOutWrite_MCU_PF11_RBF_EXISTR(GPIO_PinState setValue)
{
    MCU_PinWriteSingle(MCU_PIN_F_11,setValue);
}

static void BoardPortOutWrite_MCU_PE11_RBF_REQUESTS(GPIO_PinState setValue)
{
    MCU_PinWriteSingle(MCU_PIN_E_11,setValue);
}

static void BoardPortOutWrite_MCU_PE12_RBF_MOVES(GPIO_PinState setValue)
{
    MCU_PinWriteSingle(MCU_PIN_E_12,setValue);
}
static void BoardPortOutWrite_MCU_PG11_RED_LED(GPIO_PinState setValue)
{
    MCU_PinWriteSingle(MCU_PIN_G_11,setValue);
}
static void BoardPortOutWrite_MCU_PG12_GREEN_LED(GPIO_PinState setValue)
{
    MCU_PinWriteSingle(MCU_PIN_G_12,setValue);
}
static void BoardPortOutWrite_MCU_PG13_YELLOW_LED(GPIO_PinState setValue)
{
    MCU_PinWriteSingle(MCU_PIN_G_13,setValue);
}
static void BoardPortOutWrite_MCU_PG14_BUZZER(GPIO_PinState setValue)
{
    MCU_PinWriteSingle(MCU_PIN_G_14,setValue);
}
static void BoardPortOutWrite_MCU_PC4_RBF_AU_LED(GPIO_PinState setValue)
{
    MCU_PinWriteSingle(MCU_PIN_C_4,setValue);
}
//函数指针数组
static const BoardPortOutInitFuncPtr BoardPortOutInitFuncPtrArray[BOARD_PORT_OUT_COUNT] = {
    BoardPortOutInit_MCU_PB1_RBF_READYR     ,
    BoardPortOutInit_MCU_PF12_RBF_ENABLER   ,
    BoardPortOutInit_MCU_PF11_RBF_EXISTR    ,
    BoardPortOutInit_MCU_PE11_RBF_REQUESTS  ,
    BoardPortOutInit_MCU_PE12_RBF_MOVES     ,
    BoardPortOutInit_MCU_PG11_RED_LED       ,
    BoardPortOutInit_MCU_PG12_GREEN_LED     ,
    BoardPortOutInit_MCU_PG13_YELLOW_LED    ,
    BoardPortOutInit_MCU_PG14_BUZZER        ,
    BoardPortOutInit_MCU_PC4_RBF_AU_LED     ,
};

//函数指针数组
static const BoardPortOutWriteFuncPtr BoardPortOutWriteFuncPtrArray[BOARD_PORT_OUT_COUNT] = {
    BoardPortOutWrite_MCU_PB1_RBF_READYR    ,
    BoardPortOutWrite_MCU_PF12_RBF_ENABLER  ,
    BoardPortOutWrite_MCU_PF11_RBF_EXISTR   ,
    BoardPortOutWrite_MCU_PE11_RBF_REQUESTS ,
    BoardPortOutWrite_MCU_PE12_RBF_MOVES    ,
    BoardPortOutWrite_MCU_PG11_RED_LED      ,    
    BoardPortOutWrite_MCU_PG12_GREEN_LED    ,   
    BoardPortOutWrite_MCU_PG13_YELLOW_LED   ,
    BoardPortOutWrite_MCU_PG14_BUZZER       ,
    BoardPortOutWrite_MCU_PC4_RBF_AU_LED    ,     
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