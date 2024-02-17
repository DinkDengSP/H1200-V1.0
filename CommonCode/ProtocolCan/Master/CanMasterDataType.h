/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-10-20 18:20:58 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-11-23 13:36:48 +0800
************************************************************************************************/ 
#ifndef __CAN_MASTER_DATA_TYPE_H_
#define __CAN_MASTER_DATA_TYPE_H_
#include "CanMasterMacro.h"
#ifdef STM32F407xx
#include "StepMotorDataType.h"
#endif

//步进电机复位
typedef struct CAN_MASTER_SM_CMD_RESET
{
    int32_t resetCorrectPos;            //复位修正
    SM_CURVE_SELECT resetCorrectCurve;  //复位修正曲线
    uint32_t timeOutMs;                 //超时时间
}CAN_MASTER_SM_CMD_RESET;

//步进电机走步数
typedef struct CAN_MASTER_SM_CMD_RUN_STEP
{
    int32_t totalSteps;             //总步数
    SM_CURVE_SELECT motorRunCurve;   //运行曲线
    SPEED_RATIO_MODE speedRatioMode;//速度比例模式
    uint16_t speedRatioUint16;  //速度比例
    uint32_t dirSwitchCorrect;  //换向修正
    FunctionalState emStopEnable;       //急停使能
    FunctionalState utilStop1Enable;    //辅助急停1使能
    FunctionalState utilStop2Enable;    //辅助急停2使能
    uint32_t timeOutMs;         //超时时间
}CAN_MASTER_SM_CMD_RUN_STEP;

//步进电机走坐标
typedef struct CAN_MASTER_SM_CMD_RUN_COORDINATE
{
    int32_t targetCoordinate;       //目标坐标
    SM_CURVE_SELECT motorRunCurve;   //运行曲线
    SPEED_RATIO_MODE speedRatioMode;//速度比例模式
    uint16_t speedRatioUint16;  //速度比例
    uint32_t dirSwitchCorrect;  //换向修正
    FunctionalState emStopEnable;       //急停使能
    FunctionalState utilStop1Enable;    //辅助急停1使能
    FunctionalState utilStop2Enable;    //辅助急停2使能
    uint32_t timeOutMs;         //超时时间
}CAN_MASTER_SM_CMD_RUN_COORDINATE;

//步进电机回零
typedef struct CAN_MASTER_SM_CMD_RETURN_ZERO
{
    int32_t returnZeroCorrectPos;   //回零后修正坐标
    SM_CURVE_SELECT motorRunCurve;   //运行曲线
    SPEED_RATIO_MODE speedRatioMode;//速度比例模式
    uint16_t speedRatioUint16;      //速度比例
    uint32_t dirSwitchCorrect;      //换向修正
    FunctionalState emStopEnable;           //急停使能
    FunctionalState utilStop1Enable;        //辅助急停1使能
    FunctionalState utilStop2Enable;        //辅助急停2使能
    FunctionalState needReset;              //回零是否复位
    uint32_t timeOutMs;             //超时时间
}CAN_MASTER_SM_CMD_RETURN_ZERO;

//步进电机持续运转
typedef struct CAN_MASTER_SM_CMD_RUN_ALWAY
{
    STEP_MOTOR_DIR runAlwaysDir;    //运行方向
    SM_CURVE_SELECT motorRunCurve;   //复位修正曲线
    SPEED_RATIO_MODE speedRatioMode;//速度比例模式
    uint16_t speedRatioUint16;      //速度比例
}CAN_MASTER_SM_CMD_RUN_ALWAY;

//步进电机减速停止
typedef struct CAN_MASTER_SM_CMD_STOP_SLOW
{
    uint8_t slowDownMode;           //减速模式
    uint32_t slowDownSteps;         //减速步数
    uint32_t timeOutMs;             //超时时间
}CAN_MASTER_SM_CMD_STOP_SLOW;

//步进电机状态合集
typedef struct CAN_MASTER_SM_STATE
{
    int32_t motorPosCurrent;        //当前电机坐标
    int32_t encoderPosCurrent;      //当前电机编码器坐标
    PIN_STATE originSensorState;    //原点传感器状态
    PIN_STATE emStopSensorState;    //急停传感器状态
    PIN_STATE utilStopSensorState; //辅助急停1传感器状态
    PIN_STATE utilStop2SensorState; //辅助急停2传感器状态
}CAN_MASTER_SM_STATE;

//步进电机坐标合集
typedef struct CAN_MASTER_SM_POS
{
    int32_t motorPosCurrent;    //当前电机坐标
    int32_t encoderPosCurrent;  //当前电机编码器坐标
}CAN_MASTER_SM_POS;

//步进电机传感器合集
typedef struct CAN_MASTER_SM_SENSOR
{
    PIN_STATE originSensorState;    //原点传感器状态
    PIN_STATE emStopSensorState;    //急停传感器状态
    PIN_STATE utilStopSensorState; //辅助急停1传感器状态
    PIN_STATE utilStop2SensorState; //辅助急停2传感器状态
}CAN_MASTER_SM_SENSOR;


#endif
































