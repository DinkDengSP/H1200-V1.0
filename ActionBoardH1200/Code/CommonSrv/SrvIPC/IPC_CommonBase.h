/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-08-25 08:45:09 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-11-27 12:05:25 +0800
************************************************************************************************/ 
#ifndef __IPC_COMMON_BASE_H_
#define __IPC_COMMON_BASE_H_
#include "IPC_Base.h"
#include "StepMotorType.h"

typedef enum IPC_CMD_MACRO
{
    IPC_CMD_MACRO_COMM_TEST                         = 0xFFFFFFFF,//通讯测试
    IPC_CMD_MACRO_PORT_IN_READ_SINGLE               = 0X00000000,//单个读取
    IPC_CMD_MACRO_PORT_IN_READ_RANGE                = 0X00000001,//区间读取
    IPC_CMD_MACRO_PORT_IN_READ_ANY                  = 0X00000002,//批量读取
    IPC_CMD_MACRO_PORT_OUT_WRITE_SINGLE             = 0X00000000,//单个输出写入
    IPC_CMD_MACRO_PORT_OUT_WRITE_ANY                = 0X00000001,//批量输出写入
    IPC_CMD_MACRO_UTIL_ONLINE                       = 0X00000000,//联机
    IPC_CMD_MACRO_UTIL_OFFLINE                      = 0X00000001,//脱机
    IPC_CMD_MACRO_UTIL_UTC_SYNC                     = 0X00000002,//UTC同步
    IPC_CMD_MACRO_UTIL_HEART_GET                    = 0X00000003,//心跳获取
    IPC_CMD_MACRO_UTIL_EM_STOP                      = 0X00000004,//急停
    IPC_CMD_MACRO_UTIL_GET_VERSION                  = 0X00000005,//获取版本号
    IPC_CMD_MACRO_STEP_MOTOR_RESET                  = 0X00000000,//电机复位
    IPC_CMD_MACRO_STEP_MOTOR_RUN_STEPS              = 0X00000001,//电机走步数       
    IPC_CMD_MACRO_STEP_MOTOR_RUN_COORDINATE         = 0X00000002,//电机走坐标
    IPC_CMD_MACRO_STEP_MOTOR_RETURN_ZERO            = 0X00000003,//电机回零 
    IPC_CMD_MACRO_STEP_MOTOR_RUN_ALWAYS             = 0X00000004,//电机持续运转 
    IPC_CMD_MACRO_STEP_MOTOR_STOP_NOW               = 0X00000005,//电机立即停止 
    IPC_CMD_MACRO_STEP_MOTOR_STOP_SLOW_DOWN         = 0X00000006,//电机减速停止  
    IPC_CMD_MACRO_STEP_MOTOR_GET_COORDINATE         = 0X00000007,//获取电机当前坐标  
    IPC_CMD_MACRO_STEP_MOTOR_READ_ORIGIN_SENSOR     = 0X00000008,//获取当前原点传感器状态
    IPC_CMD_MACRO_STEP_MOTOR_DISABLE                = 0X00000009,//电机失能
    IPC_CMD_MACRO_STEP_MOTOR_ENTER_TEACH_MODE       = 0X0000000A,//电机进入教学模式
    IPC_CMD_MACRO_STEP_MOTOR_FORCE_TO_DEFAULT       = 0X0000000B,//电机强行进入默认状态
}IPC_CMD_MACRO;

//步进电机复位
typedef struct IPC_STEP_MOTOR_CMD_RESET
{
    int32_t resetCorrectPos;            //复位修正
    SM_CURVE_SELECT resetCorrectCurve;  //复位修正曲线
    uint32_t timeOutMs;                 //超时时间
}IPC_STEP_MOTOR_CMD_RESET;

//步进电机走步数
typedef struct IPC_STEP_MOTOR_CMD_RUN_STEP
{
    int32_t totalSteps;             //总步数
    SM_CURVE_SELECT runStepCurve;   //运行曲线
    SPEED_RATIO_MODE speedRatioMode;//速度比例模式
    uint16_t speedRatioUint16;  //速度比例
    uint32_t dirSwitchCorrect;  //换向修正
    FunctionalState emStopEnable;       //急停使能
    FunctionalState utilStopEnable;    //辅助急停使能
    uint32_t timeOutMs;         //超时时间
}IPC_STEP_MOTOR_CMD_RUN_STEP;

//步进电机走坐标
typedef struct IPC_STEP_MOTOR_CMD_RUN_COORDINATE
{
    int32_t targetCoordinate;       //目标坐标
    SM_CURVE_SELECT runStepCurve;   //运行曲线
    SPEED_RATIO_MODE speedRatioMode;//速度比例模式
    uint16_t speedRatioUint16;  //速度比例
    uint32_t dirSwitchCorrect;  //换向修正
    FunctionalState emStopEnable;       //急停使能
    FunctionalState utilStopEnable;    //辅助急停使能
    uint32_t timeOutMs;         //超时时间
}IPC_STEP_MOTOR_CMD_RUN_COORDINATE;

//步进电机回零
typedef struct IPC_STEP_MOTOR_CMD_RETURN_ZERO
{
    int32_t returnZeroCorrectPos;   //回零后修正坐标
    SM_CURVE_SELECT runStepCurve;   //运行曲线
    SPEED_RATIO_MODE speedRatioMode;//速度比例模式
    uint16_t speedRatioUint16;      //速度比例
    uint32_t dirSwitchCorrect;      //换向修正
    FunctionalState emStopEnable;           //急停使能
    FunctionalState utilStopEnable;        //辅助急停使能
    FunctionalState needReset;              //回零是否复位
    uint32_t timeOutMs;             //超时时间
}IPC_STEP_MOTOR_CMD_RETURN_ZERO;

//步进电机持续运转
typedef struct IPC_STEP_MOTOR_CMD_RUN_ALWAY
{
    STEP_MOTOR_DIR runAlwaysDir;    //运行方向
    SM_CURVE_SELECT runStepCurve;   //复位修正曲线
    SPEED_RATIO_MODE speedRatioMode;//速度比例模式
    uint16_t speedRatioUint16;      //速度比例
}IPC_STEP_MOTOR_CMD_RUN_ALWAY;

//步进电机减速停止
typedef struct IPC_STEP_MOTOR_CMD_STOP_SLOW
{
    uint8_t slowDownMode;           //减速模式,0为慢速减速,从当前速度减速,1为快速减速,指定减速步数
    uint32_t slowDownSteps;         //减速步数
    uint32_t timeOutMs;             //超时时间
}IPC_STEP_MOTOR_CMD_STOP_SLOW;

//步进电机状态合集
typedef struct IPC_STEP_MOTOR_STATE
{
    int32_t motorPosCurrent;        //当前电机坐标
    int32_t encoderPosCurrent;      //当前电机编码器坐标
    PIN_STATE originSensorState;    //原点传感器状态
    PIN_STATE emStopSensorState;    //急停传感器状态
    PIN_STATE utilStop1SensorState; //辅助急停1传感器状态
    PIN_STATE utilStop2SensorState; //辅助急停2传感器状态
}IPC_STEP_MOTOR_STATE;

//步进电机坐标合集
typedef struct IPC_STEP_MOTOR_POS
{
    int32_t motorPosCurrent;    //当前电机坐标
    int32_t encoderPosCurrent;  //当前电机编码器坐标
}IPC_STEP_MOTOR_POS;

//步进电机传感器合集
typedef struct IPC_STEP_MOTOR_SENSOR
{
    PIN_STATE originSensorState;    //原点传感器状态
    PIN_STATE emStopSensorState;    //急停传感器状态
    PIN_STATE utilStop1SensorState; //辅助急停1传感器状态
    PIN_STATE utilStop2SensorState; //辅助急停2传感器状态
}IPC_STEP_MOTOR_SENSOR;

//释放内存
void IPC_CommonFreeReturnData(IPC_RETURN_DATA* returnDataPtr);

//复位指令初始化
void IPC_StepMotorBaseSetResetCmdDefault(IPC_STEP_MOTOR_CMD_RESET* resetCmdPtr);

//走步数指令初始化
void IPC_StepMotorBaseSetRunStepCmdDefault(IPC_STEP_MOTOR_CMD_RUN_STEP* runStepCmdPtr);

//走坐标指令初始化
void IPC_StepMotorBaseSetRunCoordinateCmdDefault(IPC_STEP_MOTOR_CMD_RUN_COORDINATE* runCoordinateCmdPtr);

//回零指令初始化
void IPC_StepMotorBaseSetReturnZeroCmdDefault(IPC_STEP_MOTOR_CMD_RETURN_ZERO* returnZeroCmdPtr);

//持续运转指令初始化
void IPC_StepMotorBaseSetRunAlwaysCmdDefault(IPC_STEP_MOTOR_CMD_RUN_ALWAY* runAlwayCmdPtr);

//减速停止指令初始化
void IPC_StepMotorBaseSetStopSlowCmdDefault(IPC_STEP_MOTOR_CMD_STOP_SLOW* stopSlowCmdPtr);

//状态初始化
void IPC_StepMotorBaseSetStateDefault(IPC_STEP_MOTOR_STATE* motorStatePtr);

//坐标初始化
void IPC_StepMotorBaseSetPosDefault(IPC_STEP_MOTOR_POS* motorPosPtr);

//传感器初始化
void IPC_StepMotorBaseSetSensorDefault(IPC_STEP_MOTOR_SENSOR* motorSensorPtr);

//从结果中获取数据
void IPC_SrvStepMotorGetResultMotorState(IPC_CHANNEL_DATA* ipcChannelStepMotorPtr,IPC_STEP_MOTOR_STATE* motorStatePtr);

//从结果中获取数据
void IPC_SrvStepMotorGetResultMotorPos(IPC_CHANNEL_DATA* ipcChannelStepMotorPtr,IPC_STEP_MOTOR_POS* motorPosPtr);

//从结果中获取数据
void IPC_SrvStepMotorGetResultMotorSensor(IPC_CHANNEL_DATA* ipcChannelStepMotorPtr,IPC_STEP_MOTOR_SENSOR* motorSensorPtr);

//设置电机复位参数
void IPC_SrvStepMotorSetResetCmdParam(IPC_CMD_BASE_DATA* cmdBaseDataPtr,IPC_STEP_MOTOR_CMD_RESET* resetCmdPtr);

//设置电机走步数参数
void IPC_SrvStepMotorSetRunStepCmdParam(IPC_CMD_BASE_DATA* cmdBaseDataPtr,IPC_STEP_MOTOR_CMD_RUN_STEP* runStepCmdPtr);

//设置电机走坐标参数
void IPC_SrvStepMotorSetRunCoordinateCmdParam(IPC_CMD_BASE_DATA* cmdBaseDataPtr,IPC_STEP_MOTOR_CMD_RUN_COORDINATE* runCoordinateCmdPtr);

//设置电机回零参数
void IPC_SrvStepMotorSetReturnZeroCmdParam(IPC_CMD_BASE_DATA* cmdBaseDataPtr,IPC_STEP_MOTOR_CMD_RETURN_ZERO* returnZeroCmdPtr);

//设置电机持续运转参数
void IPC_SrvStepMotorSetRunAlwayCmdParam(IPC_CMD_BASE_DATA* cmdBaseDataPtr,IPC_STEP_MOTOR_CMD_RUN_ALWAY* runAlwayCmdPtr);

//设置电机减速停止参数
void IPC_SrvStepMotorSetSlowStopCmdParam(IPC_CMD_BASE_DATA* cmdBaseDataPtr,IPC_STEP_MOTOR_CMD_STOP_SLOW* stopSlowCmdPtr);

#endif















