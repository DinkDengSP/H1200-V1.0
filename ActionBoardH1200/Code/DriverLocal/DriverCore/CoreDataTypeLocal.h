/************************************************************************ 
 ** Author       : 邓小俊
 ** Date         : 2024-01-16 11:35:28
 ** Description  : When I Has Free Time ,I Will Write Description Below.
 ** description  : 
 ** LastEditors  : 邓小俊
 ** LastEditTime : 2024-01-24 13:49:45
 ** Email: xiaojun.deng@aliyun.com
 ** Copyright (c) 2024 by 公司名称, All Rights Reserved. 
 ************************************************************************/
#ifndef __CORE_DATA_TYPE_LOCAL_H_
#define __CORE_DATA_TYPE_LOCAL_H_
#include "CoreDataType.h"

//步进电机序号,系统最大可以带动八个电机
typedef enum STEP_MOTOR_NO
{
    STEP_MOTOR_NO1      = 0X00,
    STEP_MOTOR_NO2      = 0X01,
    STEP_MOTOR_NO3      = 0X02,
    STEP_MOTOR_NO4      = 0X03,
    STEP_MOTOR_NO5      = 0X04,
    STEP_MOTOR_NO6      = 0X05,
    STEP_MOTOR_NO7      = 0X06,
    STEP_MOTOR_NO8      = 0X07,
    STEP_MOTOR_NO9      = 0X08,
    STEP_MOTOR_NO10     = 0X09,
    STEP_MOTOR_NO11     = 0X0A,
    STEP_MOTOR_NO12     = 0X0B,
    STEP_MOTOR_NO13     = 0X0C,
    STEP_MOTOR_NO14     = 0X0D,
    STEP_MOTOR_NO15     = 0X0E,
    STEP_MOTOR_NO16     = 0X0F,
}STEP_MOTOR_NO;

//步进电机方向
typedef enum STEP_MOTOR_DIR
{
    STEP_MOTOR_DIR_CW  = 0X00,//顺时针
    STEP_MOTOR_DIR_CCW = 0X01,//逆时针
}STEP_MOTOR_DIR;

//速度比例的生效模式
typedef enum SPEED_RATIO_MODE
{
    SPEED_RATIO_MODE_GLOBAL     = 0x00,//速度比例的生效模式,全局比例速度
    SPEED_RATIO_MODE_UP_START   = 0x01,//速度比例的生效模式,比例速度加上启动速度
}SPEED_RATIO_MODE;

//步数配比模式
typedef enum STEP_ALLOC_MODE
{
    STEP_ALLOC_MODE_FIXED           = 0x00,//固定步数
    STEP_ALLOC_MODE_SAME_TIME_FRAME = 0x01,//等时间帧
}STEP_ALLOC_MODE;

//电机细分设定
typedef enum STEP_MOTOR_DIV
{
    STEP_MOTOR_DIV_1        = 1,
    STEP_MOTOR_DIV_2        = 2,
    STEP_MOTOR_DIV_4        = 4,
    STEP_MOTOR_DIV_8        = 8,
    STEP_MOTOR_DIV_16       = 16,
    STEP_MOTOR_DIV_32       = 32,
    STEP_MOTOR_DIV_64       = 64,
    STEP_MOTOR_DIV_128      = 128,
    STEP_MOTOR_DIV_256      = 256,
    STEP_MOTOR_DIV_512      = 512,
    STEP_MOTOR_DIV_1024     = 1024,
}STEP_MOTOR_DIV;

//电机曲线定义
typedef enum SM_CURVE_SELECT
{
    SM_CURVE_1  = 0,
    SM_CURVE_2  = 1,
    SM_CURVE_3  = 2,
    SM_CURVE_4  = 3,
    SM_CURVE_5  = 4,
    SM_CURVE_6  = 5,
    SM_CURVE_7  = 6,
    SM_CURVE_8  = 7,
    SM_CURVE_9  = 8,
    SM_CURVE_10 = 9,
    SM_CURVE_11 = 10,
    SM_CURVE_12 = 11,
    SM_CURVE_13 = 12,
    SM_CURVE_14 = 13,
    SM_CURVE_15 = 14,
    SM_CURVE_16 = 15,
    SM_CURVE_17 = 16,
    SM_CURVE_18 = 17,
    SM_CURVE_19 = 18,
    SM_CURVE_20 = 19,
    SM_CURVE_21 = 20,
}SM_CURVE_SELECT;

//步进电机指令码
typedef enum STEP_MOTOR_CMD
{
    STEP_MOTOR_CMD_NONE     = 0X0000,//无指令码写入
    STEP_MOTOR_CMD_IDLE     = 0X0001,//步进电机切入空闲
    STEP_MOTOR_CMD_STOP     = 0X0002,//步进电机急停
    STEP_MOTOR_CMD_FIX_CW   = 0X0004,//定频正向运转
    STEP_MOTOR_CMD_FIX_CCW  = 0X0008,//定频反向运转
    STEP_MOTOR_CMD_TAB_CW   = 0X0010,//步数表正向运转
    STEP_MOTOR_CMD_TAB_CCW  = 0X0020,//步数表反向运转
    STEP_MOTOR_CMD_FULL     = 0XFFFF,//无意义,占位符
}STEP_MOTOR_CMD;

//步进电机当前状态
typedef enum STEP_MOTOR_STATE
{
    STEP_MOTOR_STATE_IDLE           = 0X0001,//电机空闲
    STEP_MOTOR_STATE_CW_FIX_SPD     = 0X0002,//定频正向脉冲发送模式
    STEP_MOTOR_STATE_CCW_FIX_SPD    = 0X0004,//定频反向脉冲发送模式
    STEP_MOTOR_STATE_CW_TAB_SPD     = 0X0008,//步数表正向脉冲发送模式
    STEP_MOTOR_STATE_CCW_TAB_SPD    = 0X0010,//步数表反向脉冲发送模式
    STEP_MOTOR_STATE_STOP_CONDITION = 0X0020,//电机条件停止
    STEP_MOTOR_STATE_STOP_CMD       = 0X0040,//电机指令停止
    STEP_MOTOR_STATE_FULL           = 0XFFFF,//占位符
}STEP_MOTOR_STATE;

//步进电机停止原因
typedef enum STEP_MOTOR_STOP_REASON
{
    STEP_MOTOR_STOP_REASON_NONE                                 = 0X0000,//电机运行中未停止或状态空闲
    STEP_MOTOR_STOP_REASON_CW_SPD_FIX_PARAM_SPD_ERR             = 0X0001,//正向定频,速度参数错误
    STEP_MOTOR_STOP_REASON_CW_SPD_FIX_PARAM_STP_ERR             = 0X0002,//正向定频,步数参数错误
    STEP_MOTOR_STOP_REASON_CW_SPD_FIX_CMD                       = 0X0003,//正向定频,指令急停
    STEP_MOTOR_STOP_REASON_CW_SPD_FIX_RESET_SENSOR_VALID        = 0X0004,//正向定频,复位信号有效导致停止
    STEP_MOTOR_STOP_REASON_CW_SPD_FIX_RESET_SENSOR_INVALID      = 0X0005,//正向定频,复位信号无效导致停止
    STEP_MOTOR_STOP_REASON_CW_SPD_FIX_EM_STOP_SENSOR_VALID      = 0X0006,//正向定频,急停信号有效导致停止
    STEP_MOTOR_STOP_REASON_CW_SPD_FIX_UTIL_STOP_SENSOR_VALID   = 0X0007,//正向定频,辅助急停有效导致停止
    //STEP_MOTOR_STOP_REASON_CW_SPD_FIX_UTIL_STOP2_SENSOR_VALID   = 0X0008,//正向定频,辅助急停2有效导致停止
    STEP_MOTOR_STOP_REASON_CW_SPD_FIX_PLUSE_OVER                = 0X0009,//正向定频,脉冲发送完成停止
    STEP_MOTOR_STOP_REASON_CCW_SPD_FIX_PARAM_SPD_ERR            = 0X0010,//反向定频,速度参数错误
    STEP_MOTOR_STOP_REASON_CCW_SPD_FIX_PARAM_STP_ERR            = 0X0020,//反向定频,步数参数错误
    STEP_MOTOR_STOP_REASON_CCW_SPD_FIX_CMD                      = 0X0030,//反向定频,指令急停
    STEP_MOTOR_STOP_REASON_CCW_SPD_FIX_RESET_SENSOR_VALID       = 0X0040,//反向定频,复位信号有效导致停止
    STEP_MOTOR_STOP_REASON_CCW_SPD_FIX_RESET_SENSOR_INVALID     = 0X0050,//反向定频,复位信号无效导致停止
    STEP_MOTOR_STOP_REASON_CCW_SPD_FIX_EM_STOP_SENSOR_VALID     = 0X0060,//反向定频,急停信号有效导致停止
    STEP_MOTOR_STOP_REASON_CCW_SPD_FIX_UTIL_STOP_SENSOR_VALID  = 0X0070,//反向定频,辅助急停有效导致停止
    //STEP_MOTOR_STOP_REASON_CCW_SPD_FIX_UTIL_STOP2_SENSOR_VALID  = 0X0080,//反向定频,辅助急停2有效导致停止
    STEP_MOTOR_STOP_REASON_CCW_SPD_FIX_PLUSE_OVER               = 0X0090,//反向定频,脉冲发送完成停止
    STEP_MOTOR_STOP_REASON_CW_SPD_TAB_PARAM_SPD_ERR             = 0X0100,//正向步数表,速度参数错误
    STEP_MOTOR_STOP_REASON_CW_SPD_TAB_PARAM_STP_ERR             = 0X0200,//正向步数表,步数参数错误
    STEP_MOTOR_STOP_REASON_CW_SPD_TAB_CMD                       = 0X0300,//正向步数表,指令急停
    STEP_MOTOR_STOP_REASON_CW_SPD_TAB_RESET_SENSOR_VALID        = 0X0400,//正向步数表,复位信号有效导致停止
    STEP_MOTOR_STOP_REASON_CW_SPD_TAB_RESET_SENSOR_INVALID      = 0X0500,//正向步数表,复位信号无效导致停止
    STEP_MOTOR_STOP_REASON_CW_SPD_TAB_EM_STOP_SENSOR_VALID      = 0X0600,//正向步数表,急停信号有效导致停止
    STEP_MOTOR_STOP_REASON_CW_SPD_TAB_UTIL_STOP_SENSOR_VALID   = 0X0700,//正向步数表,辅助急停有效导致停止
    //STEP_MOTOR_STOP_REASON_CW_SPD_TAB_UTIL_STOP2_SENSOR_VALID   = 0X0800,//正向步数表,辅助急停2有效导致停止
    STEP_MOTOR_STOP_REASON_CW_SPD_TAB_PLUSE_OVER                = 0X0900,//正向步数表,脉冲发送完成停止
    STEP_MOTOR_STOP_REASON_CW_SPD_TAB_START_INDEX               = 0X0A00,//正向步数表,启动步数表序号错误
    STEP_MOTOR_STOP_REASON_CW_SPD_TAB_STOP_INDEX                = 0X0B00,//正向步数表,停止步数表序号错误
    STEP_MOTOR_STOP_REASON_CW_SPD_TAB_INDEX_RANGE               = 0X0C00,//正向步数表,启动步数表序号区间错误
    STEP_MOTOR_STOP_REASON_CCW_SPD_TAB_PARAM_SPD_ERR            = 0X1000,//反向步数表,速度参数错误
    STEP_MOTOR_STOP_REASON_CCW_SPD_TAB_PARAM_STP_ERR            = 0X2000,//反向步数表,步数参数错误
    STEP_MOTOR_STOP_REASON_CCW_SPD_TAB_CMD                      = 0X3000,//反向步数表,指令急停
    STEP_MOTOR_STOP_REASON_CCW_SPD_TAB_RESET_SENSOR_VALID       = 0X4000,//反向步数表,复位信号有效导致停止
    STEP_MOTOR_STOP_REASON_CCW_SPD_TAB_RESET_SENSOR_INVALID     = 0X5000,//反向步数表,复位信号无效导致停止
    STEP_MOTOR_STOP_REASON_CCW_SPD_TAB_EM_STOP_SENSOR_VALID     = 0X6000,//反向步数表,急停信号有效导致停止
    STEP_MOTOR_STOP_REASON_CCW_SPD_TAB_UTIL_STOP_SENSOR_VALID  = 0X7000,//反向步数表,辅助急停有效导致停止
    //STEP_MOTOR_STOP_REASON_CCW_SPD_TAB_UTIL_STOP2_SENSOR_VALID  = 0X8000,//反向步数表,辅助急停2有效导致停止
    STEP_MOTOR_STOP_REASON_CCW_SPD_TAB_PLUSE_OVER               = 0X9000,//反向步数表,脉冲发送完成停止
    STEP_MOTOR_STOP_REASON_CCW_SPD_TAB_START_INDEX              = 0XA000,//反向步数表,启动步数表序号错误
    STEP_MOTOR_STOP_REASON_CCW_SPD_TAB_STOP_INDEX               = 0XB000,//反向步数表,停止步数表序号错误
    STEP_MOTOR_STOP_REASON_CCW_SPD_TAB_INDEX_RANGE              = 0XC000,//反向步数表,启动步数表序号区间错误
}STEP_MOTOR_STOP_REASON;

//步进电机复位参数结构体
typedef struct StepMotorResetCmd 
{
    //电机序号
    STEP_MOTOR_NO motorNo;
    //复位的修正坐标
    int32_t correctionCoordinate;
    //复位修正使用的速度曲线
    SM_CURVE_SELECT selectCruveCorrection;
    //超时时间
    uint32_t timeOutMsSet;
}StepMotorResetCmd;

//电机走位携带的参数结构体
typedef struct StepMotorRunStepsCmd
{
    //电机序号
    STEP_MOTOR_NO motorNo;
    //运行步数
    int32_t totalSteps;
    //选择的加速曲线
    SM_CURVE_SELECT selectCurveConfig;
    //速度比例模式
    SPEED_RATIO_MODE speedRatioMode;
    //速度比例
    float speedRatio;
    //换向回程误差修正步数
    uint32_t dirSwitchCorrectSteps;
    //急停是否使能
    FunctionalState emStopSensorSet;
    //辅助急停是否使能
    FunctionalState utilStopSensorSet;
    //超时时间
    uint32_t timeOutMsSet;
}StepMotorRunStepsCmd;

//电机走到指定坐标携带的参数结构体
typedef struct StepMotorRun2CoordationCmd
{
    //电机序号
    STEP_MOTOR_NO motorNo;
    //目标坐标
    int32_t targetCoordinate;
    //选择的加速曲线
    SM_CURVE_SELECT selectCurveConfig;
    //速度比例模式
    SPEED_RATIO_MODE speedRatioMode;
    //速度比例
    float speedRatio;
    //换向回程误差修正步数
    int32_t dirSwitchCorrectSteps;
    //急停是否使能
    FunctionalState emStopSensorSet;
    //辅助急停是否使能
    FunctionalState utilStopSensorSet;
    //超时时间
    uint32_t timeOutMsSet;
}StepMotorRun2CoordationCmd;

//电机持续运转携带的指令结构体
typedef struct StepMotorRunAlwaysCmd
{
    //电机序号
    STEP_MOTOR_NO motorNo;
    //方向
    STEP_MOTOR_DIR motorDir;
    //选择的加速曲线
    SM_CURVE_SELECT selectCurveConfig;
    //速度比例模式
    SPEED_RATIO_MODE speedRatioMode;
    //速度比例
    float speedRatio;
}StepMotorRunAlwaysCmd;

//电机回零携带的电机指令结构体
typedef struct StepMotorReturnZeroCmd
{
    //电机序号
    STEP_MOTOR_NO motorNo;
    //回零之后的修正坐标
    int32_t correctionCoordinate;
    //选择的加速曲线
    SM_CURVE_SELECT selectCurveConfig;
    //速度比例模式
    SPEED_RATIO_MODE speedRatioMode;
    //速度比例
    float speedRatio;
    //是否需要复位
    FunctionalState resetFlag;
    //换向回程误差修正步数
    int32_t dirSwitchCorrectSteps;
    //急停是否使能
    FunctionalState emStopSensorSet;
    //辅助急停是否使能
    FunctionalState utilStopSensorSet;
    //超时时间
    uint32_t timeOutMsSet;
}StepMotorReturnZeroCmd;


#endif





