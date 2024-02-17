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
#include "SystemStateDefault.h"


//步进电机是否繁忙默认状态
const SYS_STATE_STEP_MOTOR sysStepMotorStateDefault = {
    ENTITY_STA_IDLE,//StepMotor1
    ENTITY_STA_IDLE,//StepMotor2
    ENTITY_STA_IDLE,//StepMotor3
    ENTITY_STA_IDLE,//StepMotor4
    ENTITY_STA_IDLE,//StepMotor5
    ENTITY_STA_IDLE,//StepMotor6
    ENTITY_STA_IDLE,//StepMotor7
    ENTITY_STA_IDLE,//StepMotor8
    ENTITY_STA_IDLE,//StepMotor9
    ENTITY_STA_IDLE,//StepMotor10
    ENTITY_STA_IDLE,//StepMotor11
    ENTITY_STA_IDLE,//StepMotor12
    ENTITY_STA_IDLE,//StepMotor13
    ENTITY_STA_IDLE,//StepMotor14
    ENTITY_STA_IDLE,//StepMotor15
    ENTITY_STA_IDLE,//StepMotor16
};

//主控信息标记
const SYS_STATE_MAIN_CONTROL_MSG mainControlMsgDefault = {
    SYSTEM_MODULE_NO_DEVICE_A,//主控模块号
    SYSTEM_BOARD_ID_MAIN_CONTROL_DEVICE,//主控板号
    RESET,//联机标记
    PROT_CONNECT_FLAG_DEBUG,//联机模式
    SYS_RUNNING_MODE_DEBUG,//系统运行模式
};








