/************************************************************************ 
 ** Author       : 邓小俊
 ** Date         : 2024-01-16 11:35:28
 ** Description  : When I Has Free Time ,I Will Write Description Below.
 ** description  : 
 ** LastEditors  : 邓小俊
 ** LastEditTime : 2024-01-16 11:37:35
 ** Email: xiaojun.deng@aliyun.com
 ** Copyright (c) 2024 by 公司名称, All Rights Reserved. 
 ************************************************************************/
#ifndef __SYSTEM_STATE_DATA_TYPE_H_
#define __SYSTEM_STATE_DATA_TYPE_H_
#include "DriverHeaderSystem.h"


//应用配置的标志位,初始化检测该标志位,如果不对,需要设置为默认参数
#define SYS_STATE_INIT_FLAG              0XA55A5AA5

//步进电机是否忙状态
typedef struct SYS_STATE_STEP_MOTOR
{
    ENTITY_STA stepMotor1EntityState;
    ENTITY_STA stepMotor2EntityState;
    ENTITY_STA stepMotor3EntityState;
    ENTITY_STA stepMotor4EntityState;
    ENTITY_STA stepMotor5EntityState;
    ENTITY_STA stepMotor6EntityState;
    ENTITY_STA stepMotor7EntityState;
    ENTITY_STA stepMotor8EntityState;
    ENTITY_STA stepMotor9EntityState;
    ENTITY_STA stepMotor10EntityState;
    ENTITY_STA stepMotor11EntityState;
    ENTITY_STA stepMotor12EntityState;
    ENTITY_STA stepMotor13EntityState;
    ENTITY_STA stepMotor14EntityState;
    ENTITY_STA stepMotor15EntityState;
    ENTITY_STA stepMotor16EntityState;
}SYS_STATE_STEP_MOTOR;
#define LENGTH_SYS_STATE_STEP_MOTOR                 DIM_STRUCT_LENGTH_BYTE(SYS_STATE_STEP_MOTOR)

//主控板的模块号和板号
typedef struct SYS_STATE_MAIN_CONTROL_MSG
{
    uint8_t moduleNo;//主控模块号
    uint8_t boardID;//主控板号
    FlagStatus onlineFlag;//联机标记
    PROT_CONNECT_FLAG protConnectFlag;//联机模式
    SYS_RUNNING_MODE sysRunningMode;//系统运行模式
}SYS_STATE_MAIN_CONTROL_MSG;
#define LENGTH_SYS_STATE_MAIN_CONTROL_MSG           DIM_STRUCT_LENGTH_BYTE(SYS_STATE_MAIN_CONTROL_MSG)

/*---------------------------------------------------------------------------------------------------------------*/
//总的系统参数
typedef struct SYS_STATE
{
    uint32_t stateInitFlag;//状态初始化标识
    SYS_STATE_STEP_MOTOR stepMotorState;//步进电机是否忙状态
    SYS_STATE_MAIN_CONTROL_MSG mainControlMsg;//主控信息
}SYS_STATE;
//参数长度
#define LENGTH_SYS_STATE     DIM_STRUCT_LENGTH_BYTE(SYS_STATE)

#endif







