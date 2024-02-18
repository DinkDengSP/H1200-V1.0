/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2021-03-31 11:12:45
 *LastEditors: DengXiaoJun
 *LastEditTime: 2021-06-08 16:49:33
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __STEP_MOTOR_DATA_TYPE_H_
#define __STEP_MOTOR_DATA_TYPE_H_
#include "StepMotorMacro.h"

//步进电机序号,系统最大可以带动八个电机
typedef enum STEP_MOTOR_NO
{
    STEP_MOTOR_NO1  = 0,
    STEP_MOTOR_NO2  = 1,
    STEP_MOTOR_NO3  = 2,
    STEP_MOTOR_NO4  = 3,
    STEP_MOTOR_NO5  = 4,
    STEP_MOTOR_NO6  = 5,
    STEP_MOTOR_NO7  = 6,
    STEP_MOTOR_NO8  = 7,
    STEP_MOTOR_NO_MAX = STEP_MOTOR_NO8,
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

//电机基础配置
typedef struct STEP_MOTOR_BASE_CONFIG
{
    
    BOARD_PORT_IN resetPinIndex;          //复位端口
    GPIO_PinState resetPinValidLevel;        //复位端口有效电平
    STEP_MOTOR_DIR dir;                  //电机运行正方向
    BOARD_PORT_IN emergencyStopPinIndex;  //急停端口
    GPIO_PinState emergencyStopPinValidLevel;//急停端口有效电平
    BOARD_PORT_IN utilStopPin1Index;      //辅助急停端口1
    GPIO_PinState utilStopPin1ValidLevel;    //辅助急停端口1有效电平
    BOARD_PORT_IN utilStopPin2Index;      //辅助急停端口2
    GPIO_PinState utilStopPin2ValidLevel;    //辅助急停端口有效电平
    BOARD_PORT_IN limitPointPinIndex;     //限位点引脚,适用于复位原点在中间,当检测到限位点后,切换方向为复位运行的反方向
    GPIO_PinState limitPointPinValidLevel;   //限位点有效电平
    BOARD_PORT_IN readyOkPinIndex;        //电机准备OK,用于故障检测
    GPIO_PinState readyOkPinValidLevel;      //电机准备有效电平
    uint32_t resetMaxSteps;              //电机复位最大行程
    int32_t reverseMaxSteps;             //电机运行负方向最大行程
    int32_t positiveMaxSteps;            //电机运行正方向最大行程
    uint32_t dacc2StopSteps;             //减速急停步数总和
    float resetSpeed;                    //复位速度
    uint32_t encoderLossLimit;           //编码器最大允许的丢步范围,超过改范围无法修正
    uint32_t encoderPassLimit;           //编码器许可的误差范围,在该范围内无需修正
    uint32_t encoderCorrectRetryMax;     //编码器尝试修正的重试次数
    uint32_t slowDown1StopSteps;         //到一号光耦后减速停止的步数
    uint32_t slowDown2StopSteps;         //到二号光耦后减速停止的步数
	uint32_t resetDaccSteps;             //到零点后减速停止的步数,不能大于10
    uint32_t resetOutSensorSteps;        //复位时脱离零点运行的步数
}STEP_MOTOR_BASE_CONFIG;

//步进电机初始化函数指针
typedef void (*StepMotorInitFuncPtr)(void* intCallBackFunc);
//步进电机设置逆向运转函数指针
typedef void (*StepMotorSetDirCCWFuncPtr)(void);
//步进电机设置正向运转函数指针
typedef void (*StepMotorSetDirCWFuncPtr)(void);
//步进电机启动函数指针
typedef void (*StepMotorStartFuncPtr)(float rpm);
//步进电机停止函数指针
typedef void (*StepMotorStopFuncPtr)(void);
//步进电机使能函数指针
typedef void (*StepMotorEnableFuncPtr)(void);
//步进电机禁用函数指针
typedef void (*StepMotorDisableFuncPtr)(void);
//编码器初始化函数
typedef void (*StepMotorInitEncoderFuncPtr)(void);
//编码器复位当前计数值
typedef void (*StepMotorResetEncoderCountFuncPtr)(void);
//编码器获取当前计数值
typedef int32_t (*StepMotorGetEncoderCountFuncPtr)(void);
//编码器反向设定
typedef enum ENCODER_DIR
{
    ENCODER_DIR_POSITIVE = 0X00,//编码器正向
    ENCODER_DIR_NEGATIVE = 0X01,//编码器反向
}ENCODER_DIR;
//电机外部回调方法配置结构体
typedef struct STEP_MOTOR_FUNC_CONFIG
{
    StepMotorInitFuncPtr stepMotorInit;//步进电机初始化函数
    StepMotorSetDirCCWFuncPtr stepMotorSetDirCCW;//步进电机设置逆向运转函数
    StepMotorSetDirCWFuncPtr stepMotorSetDirCW;//步进电机设置正向运转函数
    StepMotorStartFuncPtr stepMotorStart;//步进电机启动函数
    StepMotorStopFuncPtr stepMotorStop;//步进电机停止函数
    StepMotorEnableFuncPtr stepMotorEnable;//步进电机使能函数指针
    StepMotorDisableFuncPtr stepMotorDisable;//步进电机禁用函数指针
    StepMotorInitEncoderFuncPtr encoderInit;//步进电机附带的编码器初始化
    StepMotorResetEncoderCountFuncPtr encoderCountReset;//编码器当前位置复位为0
    StepMotorGetEncoderCountFuncPtr encoderCountGet;//编码器当前位置获取
    ENCODER_DIR encoderDir;//编码器方向与电机运行方向是否相反
    float ratioEncoderPosStepMotor;//编码器位置和电机一步之间的比例关系
}STEP_MOTOR_FUNC_CONFIG;

//电机速度曲线表单个表项
typedef struct SPEED_PARAM_TABLE_ITEM
{
    
    STEP_MODE stepMode; //步数配比模式
    uint32_t startStep; //启动步数
    float startSpeed;   //启动速度   
    float startAcc; //启动加速度
    float accOfAcc; //加加速
    float maxAcc;   //最大加速度  
    float maxSpeed; //最大速度       
}SPEED_PARAM_TABLE_ITEM;

//电机参数,也就是电机属性
typedef struct STEP_MOTOR_ATTRIBUTE
{   
    //基础配置
    STEP_MOTOR_BASE_CONFIG baseConfig;
    //操作函数配置
    STEP_MOTOR_FUNC_CONFIG callBackFuncConfig;
    //多个加减速曲线配置的集合
    SPEED_PARAM_TABLE_ITEM speedTableCollect[SM_SPEED_TABLE_COUNT_EACH_ONE];
}STEP_MOTOR_ATTRIBUTE;

//步进电机信号的状态,急停,减速端口当前状态
typedef enum STEP_MOTOR_SINGAL_STATE
{
    STEP_MOTOR_SINGAL_STATE_DISABLE_UNSUPPORT = 0x00,//信号端口未配置或者被禁用,反正就是不用管
    STEP_MOTOR_SINGAL_STATE_ENABLE_UNREGISTER = 0x01,//信号端口支持且使能了,但是没有注册成为中断
    STEP_MOTOR_SINGAL_STATE_ENABLE_REGISTER   = 0x02,//信号端口支持并且使能了,并且已经注册成为中断
}STEP_MOTOR_SINGAL_STATE;

//电机的主状态
typedef enum STEP_MOTOR_MAIN_STATE
{
    STEP_MOTOR_MAIN_STOP            = 0X00,//停止状态
    STEP_MOTOR_MAIN_RESET           = 0X01,//复位状态
    STEP_MOTOR_MAIN_RUN_STEPS       = 0X02,//走步数状态
    STEP_MOTOR_MAIN_RUN_COORDINATE  = 0X03,//走坐标状态
    STEP_MOTOR_MAIN_RUN_ALWAYS      = 0X04,//持续运行状态
    STEP_MOTOR_MAIN_DACC            = 0X05,//减速状态
}STEP_MOTOR_MAIN_STATE;

//电机的子状态
typedef enum STEP_MOTOR_SUB_STATE
{
    //停止状态的子状态
    STEP_MOTOR_STOP_SUB_NORMAL                  = 0X00,//正常停止
    STEP_MOTOR_STOP_SUB_TIMEOUT                 = 0X04,//运行超时停止
    STEP_MOTOR_STOP_SUB_RESET_STEP_LIMIT        = 0X06,//复位运行步数超过限制
    STEP_MOTOR_STOP_SUB_EMERGENCY_SIGNAL        = 0X08,//急停信号停止
    STEP_MOTOR_STOP_SUB_EMERGENCY_CMD           = 0X0B,//急停指令停止
    STEP_MOTOR_STOP_SUB_DACC1                   = 0X10,//辅助急停减速1停止
    STEP_MOTOR_STOP_SUB_DACC2                   = 0X14,//辅助急停减速2停止
    STEP_MOTOR_STOP_SUB_RESET_ENCODER_BLOCK     = 0X15,//复位编码器堵塞
    STEP_MOTOR_STOP_SUB_RESET_ENCODER_DIR       = 0X16,//编码器信号反向
    STEP_MOTOR_STOP_SUB_RESET_ENCODER_MORE      = 0X17,//编码器讯号传送比异常,编码器信号多于电机信号
    STEP_MOTOR_STOP_SUB_UNKNOW                  = 0X18,//未知异常
    STEP_MOTOR_STOP_SUB_RUN_STEP_BLOCK          = 0X19,//步进电机走步数运行阻塞
    STEP_MOTOR_STOP_SUB_RUN_COORDINATE_BLOCK    = 0X1A,//步进电机走坐标运行阻塞
    STEP_MOTOR_STOP_SUB_SLAVE_READY_FAULT       = 0X1B,//电机发生故障
    STEP_MOTOR_STOP_SUB_RESET_LEAVE_ZERO_LIMIT  = 0X1C,//离开远点超行程限制
    //复位状态的子状态
    STEP_MOTOR_RESET_SUB_FIND_ZERO_FIRST        = 0X00,//复位第一次找原点(正在靠近限位点或者复位原点)
    STEP_MOTOR_RESET_SUB_LEAVE_ZERO             = 0X04,//复位离开原点
    STEP_MOTOR_RESET_SUB_FIND_ZERO_SECOND       = 0X08,//复位第二次找原点
    STEP_MOTOR_RESET_SUB_FIND_ZERO_LOCATION     = 0X09,//复位从限位点处找原点
    STEP_MOTOR_RESET_SUB_CORRECTION             = 0X0B,//复位修正
    //走步数状态的子状态
    STEP_MOTOR_RUN_STEPS_SUB_NORMAL             = 0X00,
    //走坐标状态的子状态
    STEP_MOTOR_RUN_COORDINATE_SUB_NORMAL        = 0X00,
    STEP_MOTOR_RUN_COORDINATE_SUB_COMPENSATE    = 0X01,//补偿状态
    STEP_MOTOR_RUN_COORDINATE_SUB_WAIT          = 0X02,//过步补偿状态
    //持续运行状态的子状态
    STEP_MOTOR_RUN_ALWAYS_SUB_ACCELERATE        = 0X00,//加速过程
    STEP_MOTOR_RUN_ALWAYS_SUB_HOLDING           = 0X04,//保持
    STEP_MOTOR_RUN_ALWAYS_SUB_DECELERATE        = 0X08,//减速
    //减速状态的子状态
    STEP_MOTOR_DACC1_SUB_START                  = 0X00,//刚开始减速运行,还需要计算减速曲线
    STEP_MOTOR_DACC2_SUB_START                  = 0X01,
    STEP_MOTOR_DACC1_SUB_NORMAL                 = 0X04,//正常减速,正在减速
    STEP_MOTOR_DACC2_SUB_NORMAL                 = 0X05,
}STEP_MOTOR_SUB_STATE;


//电机的当前整体的状态,基础状态的整合
typedef struct StepMotorState
{
    STEP_MOTOR_MAIN_STATE motorMainState;//电机当前主要状态
    STEP_MOTOR_SUB_STATE motorSubState;  //电机当前子状态
    STEP_MOTOR_DIR currentDir;//电机当前方向
    int32_t currentCoordinate;//电机当前坐标
    int32_t currentEncoderCoordinate;//电机当前编码器坐标
    uint32_t stepMotorStartRunningTimeMs;//电机启动运行时间,用于计算超时
    uint32_t stepMotorRunningTimeMsCount;//电机运行时间总和,用于计算超时
    //每个电机的三个端口的状态
    STEP_MOTOR_SINGAL_STATE stepMotorSingalStateEmergency;//急停端口当前状态
    STEP_MOTOR_SINGAL_STATE stepMotorSingalStateUtilStop1;//辅助急停1当前状态
    STEP_MOTOR_SINGAL_STATE stepMotorSingalStateUtilStop2;//辅助急停2当前状态
    uint32_t resetCurrentSetOutPluse;//当前复位已经发出的脉冲数量
    uint32_t currentTableIndex;      //当前位于速度表的哪个位置
    uint32_t currentTableMaxLength;//当前步数速度表的最大长度
    float stepMotorSpeedTableDirect[SM_SPEED_TABLE_LENGTH_MAX];  //速度表
    uint32_t stepMotorStepTableDirect[SM_SPEED_TABLE_LENGTH_MAX];//步数表
    uint32_t currentDaccTableIndex;     //当前位于减速速度表的哪个位置
    uint32_t currentDaccTableMaxLength; //当前减速步数速度表的最大长度
    float stepMotorDaccSpeedTableDirect[SM_SPEED_TABLE_LENGTH_MAX/2];//减速速度表
    uint32_t stepMotorDaccStepTableDirect[SM_SPEED_TABLE_LENGTH_MAX/2];//减速步数表
    uint32_t targetCoordinate;//目标坐标
    uint32_t daccStepPluseOutCount;//原点检测后的减速步数计数
    uint32_t resetLimitSensorOverTrigResetSensorFlag;//从限位点找原点触发了原点的标记
}StepMotorState;


//步进电机相关的变量
typedef struct STEP_MOTOR_VAR
{
    //电机属性
    STEP_MOTOR_ATTRIBUTE* attriPtr;
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




