/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-07-27 10:53:33 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-08-24 10:00:39 +0800
************************************************************************************************/ 
#include "BoardStepMotor.h"

//端口配置,方向IO和使能IO
//映射IO口
#define BOARD_STEP_MOTOR_1_ENABLE_PIN                   MCU_PIN_E_10   
#define BOARD_STEP_MOTOR_1_DIR_PIN                      MCU_PIN_E_8  
//上下拉模式
#define BOARD_STEP_MOTOR_1_ENABLE_PULL_MODE             MCU_PIN_PULL_PULLDOWN
#define BOARD_STEP_MOTOR_1_DIR_PULL_MODE                MCU_PIN_PULL_PULLUP 
//速度
#define BOARD_STEP_MOTOR_1_ENABLE_SPEED                 MCU_PIN_SPEED_FREQ_LOW
#define BOARD_STEP_MOTOR_1_DIR_SPEED                    MCU_PIN_SPEED_FREQ_LOW
//有效值
#define BOARD_STEP_MOTOR_1_ENABLE_VALID_STATE           GPIO_PIN_SET
#define BOARD_STEP_MOTOR_1_DIR_VALID_STATE              GPIO_PIN_SET

/*******************************步进电机1相关API********************************/
//步进电机初始化
void BoardStepMotor1Init(void* callBackFunc)
{
    //方向IO初始化
    MCU_PinInit(BOARD_STEP_MOTOR_1_DIR_PIN, MCU_PIN_MODE_OUTPUT_PP, BOARD_STEP_MOTOR_1_DIR_PULL_MODE, BOARD_STEP_MOTOR_1_DIR_SPEED,0);
    //使能引脚初始化
    MCU_PinInit(BOARD_STEP_MOTOR_1_ENABLE_PIN, MCU_PIN_MODE_OUTPUT_PP, BOARD_STEP_MOTOR_1_ENABLE_PULL_MODE, BOARD_STEP_MOTOR_1_ENABLE_SPEED,0);
    //电机PWM输出初始化
    MCU_Timer1Ch1_Init((MCU_TimerBaseCallBack)callBackFunc);
    //初始化设置为正向
    BoardStepMotor1SetDirCW();
    //步进电机失能
    BoardStepMotor1Disable();
    //初始化停止
    BoardStepMotor1Stop();
}

//设置步进电机方向,反向
void BoardStepMotor1SetDirCCW(void)
{
    MCU_PinWriteSingle(BOARD_STEP_MOTOR_1_DIR_PIN, PIN_INVALID_LEVEL(BOARD_STEP_MOTOR_1_DIR_VALID_STATE));
}

//设置步进电机方向,正向
void BoardStepMotor1SetDirCW(void)
{
    MCU_PinWriteSingle(BOARD_STEP_MOTOR_1_DIR_PIN, BOARD_STEP_MOTOR_1_DIR_VALID_STATE);
}

//步进电机启动
void BoardStepMotor1Start(float rpm)
{
    //写入频率
    MCU_Timer1Ch1_SetRpm(rpm);
    //启动
    MCU_Timer1Ch1_StartOutput();
}

//步进电机停止
void BoardStepMotor1Stop(void)
{
    MCU_Timer1Ch1_StopOutput();
}

//步进电机使能
void BoardStepMotor1Enable(void)
{
    MCU_PinWriteSingle(BOARD_STEP_MOTOR_1_ENABLE_PIN,BOARD_STEP_MOTOR_1_ENABLE_VALID_STATE);
}

//步进电机失能
void BoardStepMotor1Disable(void)
{
    MCU_PinWriteSingle(BOARD_STEP_MOTOR_1_ENABLE_PIN,PIN_INVALID_LEVEL(BOARD_STEP_MOTOR_1_ENABLE_VALID_STATE));
}

/*******************************步进电机2相关API********************************/
//端口配置,方向IO和使能IO
//映射IO口
#define BOARD_STEP_MOTOR_2_ENABLE_PIN                   MCU_PIN_E_6   
#define BOARD_STEP_MOTOR_2_DIR_PIN                      MCU_PIN_E_4  
//上下拉模式
#define BOARD_STEP_MOTOR_2_ENABLE_PULL_MODE             MCU_PIN_PULL_PULLDOWN
#define BOARD_STEP_MOTOR_2_DIR_PULL_MODE                MCU_PIN_PULL_PULLUP 
//速度
#define BOARD_STEP_MOTOR_2_ENABLE_SPEED                 MCU_PIN_SPEED_FREQ_LOW
#define BOARD_STEP_MOTOR_2_DIR_SPEED                    MCU_PIN_SPEED_FREQ_LOW
//有效值
#define BOARD_STEP_MOTOR_2_ENABLE_VALID_STATE           GPIO_PIN_SET
#define BOARD_STEP_MOTOR_2_DIR_VALID_STATE              GPIO_PIN_SET

//步进电机初始化
void BoardStepMotor2Init(void* callBackFunc)
{
    //方向IO初始化
    MCU_PinInit(BOARD_STEP_MOTOR_2_DIR_PIN, MCU_PIN_MODE_OUTPUT_PP, BOARD_STEP_MOTOR_2_DIR_PULL_MODE, BOARD_STEP_MOTOR_2_DIR_SPEED, 0);
    //使能引脚初始化
    MCU_PinInit(BOARD_STEP_MOTOR_2_ENABLE_PIN, MCU_PIN_MODE_OUTPUT_PP, BOARD_STEP_MOTOR_2_ENABLE_PULL_MODE, BOARD_STEP_MOTOR_2_ENABLE_SPEED, 0);
    //电机PWM输出初始化
    MCU_Timer9Ch1_Init((MCU_TimerBaseCallBack)callBackFunc);
    //初始化设置为正向
    BoardStepMotor2SetDirCW();
    //步进电机失能
    BoardStepMotor2Disable();
    //初始化停止
    BoardStepMotor2Stop();
}

//设置步进电机方向,反向
void BoardStepMotor2SetDirCCW(void)
{
    MCU_PinWriteSingle(BOARD_STEP_MOTOR_2_DIR_PIN, PIN_INVALID_LEVEL(BOARD_STEP_MOTOR_2_DIR_VALID_STATE));
}

//设置步进电机方向,正向
void BoardStepMotor2SetDirCW(void)
{
    MCU_PinWriteSingle(BOARD_STEP_MOTOR_2_DIR_PIN, BOARD_STEP_MOTOR_2_DIR_VALID_STATE);
}

//步进电机启动
void BoardStepMotor2Start(float rpm)
{
    //写入频率
    MCU_Timer9Ch1_SetRpm(rpm);
    //启动
    MCU_Timer9Ch1_StartOutput();
}

//步进电机停止
void BoardStepMotor2Stop(void)
{
    MCU_Timer9Ch1_StopOutput();
}

//步进电机使能
void BoardStepMotor2Enable(void)
{
    MCU_PinWriteSingle(BOARD_STEP_MOTOR_2_ENABLE_PIN,BOARD_STEP_MOTOR_2_ENABLE_VALID_STATE);
}

//步进电机失能
void BoardStepMotor2Disable(void)
{
    MCU_PinWriteSingle(BOARD_STEP_MOTOR_2_ENABLE_PIN,PIN_INVALID_LEVEL(BOARD_STEP_MOTOR_2_ENABLE_VALID_STATE));
}

