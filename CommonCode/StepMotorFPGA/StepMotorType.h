/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-05-10 10:12:41 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-05-19 17:04:53 +0800
************************************************************************************************/ 
#ifndef __STEP_MOTOR_TYPE_H_
#define __STEP_MOTOR_TYPE_H_
#include "StepMotorMacro.h"

//电机基础配置,传感器
typedef struct STEP_MOTOR_ATTRI_BASE_SENSOR
{
    FunctionalState resetSensorSet;         //复位传感器是否安装
    GPIO_PinState resetSensorValidLevel;    //复位传感器有效电平
    FunctionalState emStopSensorSet;        //急停传感器是否安装
    GPIO_PinState emStopSensorValidLevel;   //急停传感器有效电平
    FunctionalState utilStopSensorSet;      //辅助急停传感器是否安装
    GPIO_PinState utilStopSensorValidLevel; //辅助急停传感器有效电平
    GPIO_PinState motorEnablePinValidLevel; //电机使能有效信号
    GPIO_PinState motorDirPinCwValidLevel;  //电机正方向有效信号
    GPIO_PinState motorPlusePinIdleLevel;   //电机脉冲信号空闲电平
}STEP_MOTOR_ATTRI_BASE_SENSOR;

//电机基础配置,编码器
typedef struct STEP_MOTOR_ATTRI_BASE_ENCODER
{
    FunctionalState encoderSet;         //编码器是否使能
    GPIO_PinState validLevelA;          //编码器A相有效信号电平
    GPIO_PinState validLevelB;          //编码器B相有效信号电平
    float ratioWithMotorPos;            //编码器坐标与电机坐标之间的比例关系
    uint32_t lossLimitRunning;          //编码器最大允许的丢步范围,超过该范围无法修正,运行过程
    uint32_t passLimit;                 //编码器许可的误差范围,在该范围内无需修正
    uint32_t lossLimitReset;            //编码器最大允许的丢步范围,超过该范围无法修正,复位过程
    uint32_t thresholdRealTimeCorrect;  //实时修正修正阈值,当运行时编码器差值阈值超过多少就要进行实时的修正,没超过就等到运行完成再修正
    uint32_t encoderCorrectRetryMax;    //编码器尝试修正的重试次数
    STEP_MOTOR_ENCODER_FIXES fixMode;   //编码器位置修正模式
}STEP_MOTOR_ATTRI_BASE_ENCODER;

//电机基础配置,运行时
typedef struct STEP_MOTOR_ATTRI_BASE_RUN
{
    FunctionalState motorExist;             //电机是否存在
    STEP_MOTOR_DIV motorDriverDiv;          //电机细分数
    int32_t positiveDirMaxSteps;            //电机正方向最大行程
    int32_t reverseDirMaxSteps;             //电机负方向最大行程
    STEP_MOTOR_RESET_MODE firstToOriginSensor;   //复位第一次靠近原点的模式,复位的时候原点无效,找原点的时候的模式
    STEP_MOTOR_RESET_MODE firstLeaveOriginSensor;//复位第一次脱离原点的模式,复位的时候原点有效,脱离原点的模式
    uint32_t resetMaxStepsToOriginSensor;   //电机复位找原点最大行程
    uint32_t resetMaxStepsLeaveOrigin;      //电机复位脱离远点最大行程
    uint32_t resetSpdRpmFast;               //电机复位速度,快速
    uint32_t resetSpdRpmSlow;               //电机复位速度,慢速
    uint32_t resetDecelerationSteps;        //电机复位减速步数
    uint32_t resetDirSwitchStepsCompensate; //复位换向补偿步数
    SM_CURVE_SELECT resetCurveSelect;   //复位使用曲线复位时,使用的复位曲线
    STEP_MOTOR_DIR_SWITCH_FIX_MODE dirSwitchFixMode;//方向切换的修正模式
    uint32_t decStepsEmStopTrig;            //急停被触发后的减速停止步数
    uint32_t decStepsUtilStopTrig;         //辅助急停被触发后的减速停止步数
}STEP_MOTOR_ATTRI_BASE_RUN;

//电机曲线配置
typedef struct STEP_MOTOR_ATTRI_CURVE
{
    float startRpm;         //启动速度
    float maxRpm;           //最大速度
    float accelerationStart;//启动时加速度
    float accelerationMax;  //最大加速度
    float accelerationGradient;//加速度增长梯度,也就是加加速
    uint32_t startSteps;    //启动步数
    STEP_ALLOC_MODE stepAllocMode;//步数分配模式
}STEP_MOTOR_ATTRI_CURVE;

//电机配置讯息
typedef struct STEP_MOTOR_ATTRI
{
    STEP_MOTOR_ATTRI_BASE_SENSOR motorSensorAttri;//电机传感器属性
    STEP_MOTOR_ATTRI_BASE_ENCODER motorEncoderAttri;//电机编码器属性
    STEP_MOTOR_ATTRI_BASE_RUN motorRunAttri;//电机运行时属性
    STEP_MOTOR_ATTRI_CURVE motorCurveAttriSet[SM_CURVE_SET_MAX];//电机加减速曲线属性集合
}STEP_MOTOR_ATTRI;

//全功能曲线计算数据
typedef struct STEP_MOTOR_MAIN_CURVE_TAB_DATA
{
    float spdTableArray[STEP_MOTOR_MAIN_CURVE_TAB_LEN_MAX];//存放速度曲线的数组指针
    uint32_t stpTableArray[STEP_MOTOR_MAIN_CURVE_TAB_LEN_MAX];//存放步数曲线的数组指针
    uint32_t tabLengthAfterCalc;//计算完成后数组有效长度
    uint32_t totalRunningSteps;//总运行步数
}STEP_MOTOR_MAIN_CURVE_TAB_DATA;

//辅助曲线计算数据,只有完整曲线的一半
typedef struct STEP_MOTOR_UTIL_CURVE_TAB_DATA
{
    float spdTableArray[STEP_MOTOR_UTIL_CURVE_TAB_LEN_MAX];//存放速度曲线的数组指针
    uint32_t stpTableArray[STEP_MOTOR_UTIL_CURVE_TAB_LEN_MAX];//存放步数曲线的数组指针
    uint32_t tabLengthAfterCalc;//计算完成后数组有效长度
    uint32_t totalRunningSteps;//总运行步数
}STEP_MOTOR_UTIL_CURVE_TAB_DATA;

//电机状态集合
typedef struct STEP_MOTOR_STATE_SET
{
    TX_MUTEX mutexAccess;//电机互斥访问量
    STEP_MOTOR_MODE_TEACH teachMode;//电机是否处于学习模式
    STEP_MOTOR_STATE_MAIN mainState;//电机主状态
    STEP_MOTOR_STATE_SUB subState;//电机子状态
    STEP_MOTOR_DIR currentDir;//电机当前方向
    int32_t currentMotorCoordinate;//电机当前坐标
    int32_t baseMotorCoordinate;//电机本次运行的基础坐标
    uint32_t startTimeTick;//电机启动运行时间,用于计算超时
    uint32_t runningTimeMsCount;//电机运行时间总和,用于计算超时
    int32_t targetCoordinate;//目标位置
    STEP_MOTOR_MAIN_CURVE_TAB_DATA* mainCurveTabDataPtr;//主曲线数据指针
    STEP_MOTOR_UTIL_CURVE_TAB_DATA* utilCurveTabDataPtr;//辅助曲线数据指针
    FPGA_STEP_MOTOR_REGS* fpgaRegSetPtr;//对应的FPGA寄存器状态
    //中断中保存的参数,用作系统调试
    ERROR_SUB errorSubCodeInt;
    STEP_MOTOR_STATE motorStateFpgaInt;
    STEP_MOTOR_STOP_REASON motorStopReasonFpgaInt;
    //保留运行指令,用作运行时状态切换检测灯一系列操作
    StepMotorResetCmd paramMotorReset;
    StepMotorRunStepsCmd paramMotorRunSteps;
    StepMotorRun2CoordationCmd paramMotorRunCoordinate;
    StepMotorRunAlwaysCmd paramMotorRunAlways;
    StepMotorReturnZeroCmd paramMotorReturnZero;
    //电机定频运转指令结构体
    FPGA_MOTOR_FIX_SPD_CMD cmdFixSpd;
    FPGA_MOTOR_TAB_SPD_CMD cmdTabSpd;
}STEP_MOTOR_STATE_SET;

//电机数据集合
typedef struct STEP_MOTOR_DATA_SET
{
    STEP_MOTOR_ATTRI* attriPtr;
    STEP_MOTOR_STATE_SET stateSet;
}STEP_MOTOR_DATA_SET;

//电机结果标记,是否触发异常停止
typedef struct MOTOR_SENSOR_FLAGS
{
    ITStatus sensorEmStopFlag;  //急停触发状态
    ITStatus sensorUtilStopFlag;//辅助急停状态
}MOTOR_SENSOR_FLAGS;

//电机传感器状态
typedef struct MOTOR_SENSOR_SET
{
    PIN_STATE resetSensorState;
    PIN_STATE emStopSensorState;
    PIN_STATE utilStopSensorState;
}MOTOR_SENSOR_SET;


#endif




