/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-10-30 11:27:01 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-11-01 13:53:35 +0800
************************************************************************************************/ 
#ifndef __STEP_MOTOR_DATA_TYPE_H_
#define __STEP_MOTOR_DATA_TYPE_H_
#include "StepMotorMacro.h"
#include "DriverHeaderCore.h"

//步进电机序号,系统最大可以带动八个电机
typedef enum STEP_MOTOR_NO
{
    STEP_MOTOR_NO1      = 0,
    STEP_MOTOR_NO2      = 1,
    STEP_MOTOR_NO3      = 2,
    STEP_MOTOR_NO4      = 3,
    STEP_MOTOR_NO5      = 4,
    STEP_MOTOR_NO6      = 5,
    STEP_MOTOR_NO7      = 6,
    STEP_MOTOR_NO8      = 7,
    STEP_MOTOR_NO9      = 8,
    STEP_MOTOR_NO10     = 9,
    STEP_MOTOR_NO11     = 10,
    STEP_MOTOR_NO12     = 11,
}STEP_MOTOR_NO;

//步进电机方向
typedef enum STEP_MOTOR_DIR
{
    STEP_MOTOR_DIR_CCW = 0,//逆时针
    STEP_MOTOR_DIR_CW  = 1,//顺时针
}STEP_MOTOR_DIR;

//速度曲线计算比率
typedef enum SPEED_RATIO_MODE
{
    SPEED_RATIO_MODE_GLOBAL     = 0,//速度比例的生效模式,全局比例速度
    SPEED_RATIO_MODE_UP_START   = 1,//速度比例的生效模式,比例速度加上启动速度
}SPEED_RATIO_MODE;

//步数配比模式
typedef enum STEP_MODE
{
    FIXED           = 0,//固定步数
    SAME_TIME_FRAME = 1,//等时间帧,每一个加速点使用的时间是固定的
}STEP_MODE;

//电机速度曲线选择
typedef enum SM_CURVE_SELECT
{
    SM_CURVE_0      = 0X00,
    SM_CURVE_1      = 0X01,
    SM_CURVE_2      = 0X02,
    SM_CURVE_3      = 0X03,
    SM_CURVE_4      = 0X04,
    SM_CURVE_5      = 0X05,
    SM_CURVE_6      = 0X06,
    SM_CURVE_7      = 0X07,
    SM_CURVE_8      = 0X08,
    SM_CURVE_9      = 0X09,
    SM_CURVE_10     = 0X0A,
    SM_CURVE_11     = 0X0B,
    SM_CURVE_12     = 0X0C,
    SM_CURVE_13     = 0X0D,
    SM_CURVE_14     = 0X0E,
    SM_CURVE_15     = 0X0F,    
    SM_CURVE_16     = 0X10,
    SM_CURVE_17     = 0X11,
    SM_CURVE_18     = 0X12,
    SM_CURVE_19     = 0X13,
    SM_CURVE_20     = 0X14,
}SM_CURVE_SELECT;

//电机细分设定
typedef enum STEP_MOTOR_DIV
{
    STEP_MOTOR_DIV_1    = 1,
    STEP_MOTOR_DIV_2    = 2,
    STEP_MOTOR_DIV_4    = 4,
    STEP_MOTOR_DIV_8    = 8,
    STEP_MOTOR_DIV_16   = 16,
    STEP_MOTOR_DIV_32   = 32,
    STEP_MOTOR_DIV_64   = 64,
}STEP_MOTOR_DIV;


//电机的主状态
typedef enum STEP_MOTOR_MAIN_STATE
{
    STEP_MOTOR_MAIN_STATE_STOP              = 0X00,//停止状态
    STEP_MOTOR_MAIN_STATE_RESET             = 0X01,//复位状态
    STEP_MOTOR_MAIN_STATE_RUN_STEPS         = 0X02,//走步数状态
    STEP_MOTOR_MAIN_STATE_RUN_COORDINATE    = 0X03,//走坐标状态
    STEP_MOTOR_MAIN_STATE_RUN_ALWAYS        = 0X04,//持续运行状态
    STEP_MOTOR_MAIN_STATE_DACC              = 0X05,//减速状态
}STEP_MOTOR_MAIN_STATE;

//电机的子状态
typedef enum STEP_MOTOR_SUB_STATE
{
    //停止状态的子状态
    STEP_MOTOR_STOP_SUB_NORMAL                  = 0X00,//正常停止
    STEP_MOTOR_STOP_SUB_TIMEOUT                 = 0X02,//运行超时停止
    STEP_MOTOR_STOP_SUB_RESET_STEP_LIMIT        = 0X04,//复位运行步数超过限制
    STEP_MOTOR_STOP_SUB_EMERGENCY_SIGNAL        = 0X08,//急停信号停止
    STEP_MOTOR_STOP_SUB_EMERGENCY_CMD           = 0X10,//急停指令停止
    STEP_MOTOR_STOP_SUB_DACC1                   = 0X20,//辅助急停减速1停止
    STEP_MOTOR_STOP_SUB_DACC2                   = 0X40,//辅助急停减速2停止
    STEP_MOTOR_STOP_SUB_SLAVE_READY_FAULT       = 0X80,//电机发生故障
    STEP_MOTOR_STOP_SUB_RESET_LEAVE_ZERO_LIMIT  = 0X82,//离开远点超行程限制
    STEP_MOTOR_STOP_SUB_RESET_DACC_LOSS         = 0X84,//找原点减速停止之后,原点信号丢失
    STEP_MOTOR_STOP_SUB_UNKNOW                  = 0XFF,//未知异常
    //复位状态的子状态
    STEP_MOTOR_RESET_SUB_FIND_ZERO_FIRST        = 0X00,//复位第一次找原点
    STEP_MOTOR_RESET_SUB_LEAVE_ZERO             = 0X02,//复位离开原点
    STEP_MOTOR_RESET_SUB_FIND_ZERO_SECOND       = 0X04,//复位第二次找原点
    STEP_MOTOR_RESET_SUB_FIND_ZERO_LOCATION     = 0X08,//复位从限位点处找原点
    STEP_MOTOR_RESET_SUB_CORRECTION             = 0X80,//复位修正
    //走步数状态的子状态
    STEP_MOTOR_RUN_STEPS_SUB_NORMAL             = 0X00,//正在走步数
    //走坐标状态的子状态
    STEP_MOTOR_RUN_COORDINATE_SUB_NORMAL        = 0X00,//正在走坐标
    //持续运行状态的子状态
    STEP_MOTOR_RUN_ALWAYS_SUB_ACCELERATE        = 0X00,//加速过程
    STEP_MOTOR_RUN_ALWAYS_SUB_HOLDING           = 0X02,//保持
    STEP_MOTOR_RUN_ALWAYS_SUB_DECELERATE        = 0X04,//减速
    //减速状态的子状态
    STEP_MOTOR_DACC1_SUB_START                  = 0X00,//刚开始减速运行,还需要计算减速曲线
    STEP_MOTOR_DACC2_SUB_START                  = 0X02,//刚开始减速运行,还需要计算减速曲线
    STEP_MOTOR_DACC1_SUB_NORMAL                 = 0X04,//正常减速,正在减速
    STEP_MOTOR_DACC2_SUB_NORMAL                 = 0X08,//正常减速,正在减速
}STEP_MOTOR_SUB_STATE;

//步进电机信号的状态
typedef enum STEP_MOTOR_SINGAL_STATE
{
    STEP_MOTOR_SINGAL_STATE_DisableUnsupport = 0x00,//信号端口未配置或者被禁用,反正就是不用管
    STEP_MOTOR_SINGAL_STATE_EnableUnregister = 0x01,//信号端口支持且使能了,但是没有注册成为中断
    STEP_MOTOR_SINGAL_STATE_EnableRegister = 0x02,//信号端口支持并且使能了,并且已经注册成为中断
}STEP_MOTOR_SINGAL_STATE;

//电机的当前整体的状态,基础状态的整合
typedef struct StepMotorState
{
    STEP_MOTOR_MAIN_STATE motorMainState;//电机当前主要状态
    STEP_MOTOR_SUB_STATE motorSubState;//电机当前子状态
    STEP_MOTOR_DIR currentDir;//电机当前方向
    int32_t currentCoordinate;//电机当前坐标
    uint32_t stepMotorStartRunningTimeMs;//电机启动运行时间
    uint32_t stepMotorRunningTimeMsCount;//电机运行时间总和
    //每个电机的三个端口的状态
    STEP_MOTOR_SINGAL_STATE stepMotorSingalStateEmergency;//急停端口
    STEP_MOTOR_SINGAL_STATE stepMotorSingalStateUtilStop1;//辅助急停1
    STEP_MOTOR_SINGAL_STATE stepMotorSingalStateUtilStop2;//辅助急停2
    uint32_t resetCurrentSetOutPluse;//当前复位已经发出的脉冲数量
    uint32_t daccStepPluseOutCount;//原点检测后的减速步数计数
    uint32_t resetLimitSensorOverTrigResetSensorFlag;//从限位点找原点触发了原点的标记
    uint32_t lastSendOutPluse;//上一次发送的步数
    uint32_t currentTableIndex;//当前位于速度表的哪个位置
    uint32_t currentTableMaxLength;//当前步数速度表的最大长度
    float stepMotorSpeedTableDirect[SM_SPEED_TABLE_LENGTH_MAX];//速度表
    uint32_t stepMotorStepTableDirect[SM_SPEED_TABLE_LENGTH_MAX];//步数表
    uint32_t currentDaccTableIndex;//当前位于减速速度表的哪个位置
    uint32_t currentDaccTableMaxLength;//当前减速步数速度表的最大长度
    float stepMotorDaccSpeedTableDirect[SM_SPEED_TABLE_LENGTH_MAX/2];//减速速度表
    uint32_t stepMotorDaccStepTableDirect[SM_SPEED_TABLE_LENGTH_MAX/2];//减速步数表
}StepMotorState;

//自动计算的速度参数和可以进行手动标定的数据长度
typedef struct SPEED_PARAM_TABLE
{
    //步数配比模式
    STEP_MODE stepMode;
    //启动步数
    uint32_t startStep;
    //启动速度
    float startSpeed;   
    //启动加速度
    float startAcc;
    //加加速
    float accOfAcc;
    //最大加速度 
    float maxAcc;  
    //最大速度
    float maxSpeed;       
}SPEED_PARAM_TABLE;

//基础参数,占用空间 30
typedef struct STEP_MOTOR_BASE_CONFIG
{
    BOARD_PORT_IN resetPinIndex;        //复位端口
    GPIO_PinState resetPinValidLevel;   //复位端口有效电平
    STEP_MOTOR_DIR dir;                 //电机运行正方向
    STEP_MOTOR_DIV div;                 //电机细分
    BOARD_PORT_IN emStopPinIndex;       //急停端口
    GPIO_PinState emStopPinValidLevel;  //急停端口有效电平
    BOARD_PORT_IN utilStopPin1Index;    //辅助急停端口1
    GPIO_PinState utilStopPin1ValidLevel;//辅助急停端口1有效电平
    BOARD_PORT_IN utilStopPin2Index;    //辅助急停端口2
    GPIO_PinState utilStopPin2ValidLevel;//辅助急停端口有效电平
    BOARD_PORT_IN limitPointPinIndex;    //限位点引脚,适用于复位原点在中间,当检测到限位点后,切换方向为复位运行的反方向
    GPIO_PinState limitPointPinValidLevel;//限位点有效电平
    BOARD_PORT_IN readyOkPinIndex;        //电机准备OK,用于故障检测
    GPIO_PinState readyOkPinValidLevel;   //电机准备有效电平
    uint32_t resetMaxSteps;     //电机复位最大行程
    uint32_t resetDaccSteps;    //到零点后减速停止的步数,不能挡片遮挡原点传感器的间隙步数
    uint32_t resetOutSensorSteps;//复位时脱离零点运行的步数
    int32_t reverseMaxSteps;    //电机运行负方向最大行程
    int32_t positiveMaxSteps;   //电机运行正方向最大行程
    uint32_t dacc2StopSteps;    //减速急停步数总和
    float resetSpeedFast;       //复位速度,快速,主要用在找原点
    float resetSpeedSlow;       //复位速度,慢速,主要用在脱离原点和二次回到原点
    uint8_t singlePluseMax;     //最大单次发送脉冲数
}STEP_MOTOR_BASE_CONFIG;

//电机参数
typedef struct STEP_MOTOR_ATTRIBUTE
{   
    //电机配置标识
    uint32_t configFlag;
    //基础配置
    STEP_MOTOR_BASE_CONFIG baseConfig;
    //多个加减速曲线配置的集合
    SPEED_PARAM_TABLE speedTableCollect[SM_SPEED_TABLE_COUNT_EACH_ONE];
}STEP_MOTOR_ATTRIBUTE;

//电机参数占用内存空间数量
#define STEP_MOTOR_ATTRI_LENGTH     (sizeof(STEP_MOTOR_ATTRIBUTE)/sizeof(uint8_t))


//步进电机相关的变量
typedef struct STEP_MOTOR_VAR
{
    //电机属性
    STEP_MOTOR_ATTRIBUTE attri;
    //电机变量
    StepMotorState var;
}STEP_MOTOR_VAR;

//电机复位携带的参数结构体
typedef struct StepMotorResetCmd
{
    STEP_MOTOR_NO motorNo;
    //复位的修正坐标
    int32_t correctionCoordinate;
    //超时时间
    uint32_t timeOutSet;
}StepMotorResetCmd;

//电机走位携带的参数结构体
typedef struct StepMotorRunStepsCmd
{
    STEP_MOTOR_NO motorNo;
    //运行步数
    int32_t specialSteps;
    //选择的加速曲线
    uint8_t selectConfig;
    //速度比例模式
    SPEED_RATIO_MODE speedRatioMode;
    //速度比例
    uint8_t speedRatio;
    //辅助急停1是否使能
    uint8_t utilStop1Enable;
    //辅助急停2是否使能
    uint8_t utilStop2Enable;
    //超时时间
    uint32_t timeOutSet;
}StepMotorRunStepsCmd;

//电机走到指定坐标携带的电机结构体
typedef struct StepMotorRun2CoordationCmd
{
    STEP_MOTOR_NO motorNo;
    //目标坐标
    int32_t targetCoordinate;
    //选择的加速曲线
    uint8_t selectConfig;
    //速度比例模式
    SPEED_RATIO_MODE speedRatioMode;
    //速度比例
    uint8_t speedRatio;
    //辅助急停1是否使能
    uint8_t utilStop1Enable;
    //辅助急停2是否使能
    uint8_t utilStop2Enable;
    //超时时间
    uint32_t timeOutSet;
}StepMotorRun2CoordationCmd;

//电机回零携带的电机指令结构体
typedef struct StepMotorReturnZeroCmd
{
    STEP_MOTOR_NO motorNo;
    //回零之后的修正坐标
    int32_t correctionCoordinate;
    //选择的加速曲线
    uint8_t selectConfig;
    //速度比例模式
    SPEED_RATIO_MODE speedRatioMode;
    //速度比例
    uint8_t speedRatio;
    //辅助急停1是否使能
    uint8_t utilStop1Enable;
    //辅助急停2是否使能
    uint8_t utilStop2Enable;
    //超时时间
    uint32_t timeOutSet;
}StepMotorReturnZeroCmd;

//电机持续运转携带的指令结构体
typedef struct StepMotorRunAlwaysCmd
{
    STEP_MOTOR_NO motorNo;
    //选择的加速曲线
    uint8_t selectConfig;
    //速度比例模式
    SPEED_RATIO_MODE speedRatioMode;
    //速度比例
    uint8_t speedRatio;
    //方向
    STEP_MOTOR_DIR dir;
}StepMotorRunAlwaysCmd;

#endif



