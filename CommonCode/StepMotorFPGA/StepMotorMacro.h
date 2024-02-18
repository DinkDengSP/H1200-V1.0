/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-05-10 10:12:27 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-05-18 20:22:02 +0800
************************************************************************************************/ 
#ifndef __STEP_MOTOR_MACRO_H_
#define __STEP_MOTOR_MACRO_H_
#include "DriverHeaderBoard.h"
#include "DriverConfigBoard.h"
#include "StepMotorConfig.h"

//步进电机加减速曲线最大长度
#define STEP_MOTOR_MAIN_CURVE_TAB_LEN_MAX               BOARD_FPGA_STEP_MOTOR_TAB_LENGTH
//步进电机加减速曲线辅助曲线的最大长度
#define STEP_MOTOR_UTIL_CURVE_TAB_LEN_MAX               (STEP_MOTOR_MAIN_CURVE_TAB_LEN_MAX/2)

//电机支持最大数量
#define STEP_MOTOR_NUM_COUNT                            BOARD_FPGA_STEP_MOTOR_NUM_MAX
//步进电机单个最大支持曲线数量
#define SM_CURVE_SET_MAX                                21

//步进电机复位逻辑
typedef enum STEP_MOTOR_RESET_MODE
{
    STEP_MOTOR_RESET_MODE_FIX_FAST      = 0,//固定速度复位,快速
    STEP_MOTOR_RESET_MODE_FIX_SLOW      = 1,//固定速度复位,慢速
    STEP_MOTOR_RESET_MODE_CURVE         = 2,//曲线复位
}STEP_MOTOR_RESET_MODE;

//步进电机编码器修正逻辑
typedef enum STEP_MOTOR_ENCODER_FIXES
{
    STEP_MOTOR_ENCODER_FIXES_NONE     = 0X00,//无修正,只检测
    STEP_MOTOR_ENCODER_FIXES_RUNTIME  = 0X01,//运行时实时监测修正
    STEP_MOTOR_ENCODER_FIXES_STOP     = 0X02,//停止后修正
}STEP_MOTOR_ENCODER_FIXES;

//步进电机方向切换时的步数修正模式
typedef enum STEP_MOTOR_DIR_SWITCH_FIX_MODE
{
    STEP_MOTOR_DIR_SWITCH_FIX_MODE_NONE         = 0X00,//不修正
    STEP_MOTOR_DIR_SWITCH_FIX_MODE_CW_TO_CCW    = 0X01,//顺时针切换逆时针修正
    STEP_MOTOR_DIR_SWITCH_FIX_MODE_CCW_TO_CW    = 0X02,//逆时针切换顺时针修正
    STEP_MOTOR_DIR_SWITCH_FIX_MODE_ALL          = 0X03,//只要切换就修正
}STEP_MOTOR_DIR_SWITCH_FIX_MODE;

//电机是否处于学习状态
typedef enum STEP_MOTOR_MODE_TEACH
{
    STEP_MOTOR_MODE_TEACH_DISABLE   = 0X00,//当前不处于学习模式
    STEP_MOTOR_MODE_TEACH_ENABLE    = 0X01,//当前处于学习模式
}STEP_MOTOR_MODE_TEACH;

//步进电机主状态
typedef enum STEP_MOTOR_STATE_MAIN
{
    STEP_MOTOR_STATE_MAIN_IDLE            = 0X00,//电机空闲状态
    STEP_MOTOR_STATE_MAIN_STOP            = 0X01,//电机停止状态
    STEP_MOTOR_STATE_MAIN_RESET           = 0X02,//电机复位状态
    STEP_MOTOR_STATE_MAIN_RUN_STEPS       = 0X03,//电机走步数状态
    STEP_MOTOR_STATE_MAIN_RUN_COORDINATE  = 0X04,//电机走坐标状态
    STEP_MOTOR_STATE_MAIN_RUN_ALWAYS      = 0X05,//电机持续运转状态
    STEP_MOTOR_STATE_MAIN_RETURN_ZERO     = 0X06,//电机回零状态
}STEP_MOTOR_STATE_MAIN;

//步进电机子状态
typedef enum STEP_MOTOR_STATE_SUB
{
    STEP_MOTOR_STATE_SUB_NONE                           = 0X0000,//子状态空闲
    STEP_MOTOR_STATE_SUB_STOP_UNKNOW_STATE_INT          = 0X0100,//电机未知状态下收到了电机中断
    STEP_MOTOR_STATE_SUB_STOP_IDLE_STATE_INT            = 0X0101,//电机空闲状态下收到了电机中断
    STEP_MOTOR_STATE_SUB_STOP_STOP_STATE_INT            = 0X0102,//电机空停止状态下收到了电机中断
    STEP_MOTOR_STATE_SUB_STOP_EM_STOP_CMD               = 0X0103,//电机运转过程中收到了急停
    STEP_MOTOR_STATE_SUB_STOP_EM_STOP_SINGLE            = 0X0104,//急停停止
    STEP_MOTOR_STATE_SUB_STOP_UTIL_STOP_SINGLE          = 0X0105,//辅助急停停止
    STEP_MOTOR_STATE_SUB_STOP_PLUSE_OVER_STOP           = 0X0107,//脉冲发送完成停止
    STEP_MOTOR_STATE_SUB_STOP_RESET_VALID               = 0X0108,//复位传感器有效导致停止
    STEP_MOTOR_STATE_SUB_STOP_RESET_INVALID             = 0X0109,//复位传感器无效导致停止
    STEP_MOTOR_STATE_SUB_STOP_FIX_SPD_PARAM             = 0X010A,//定频运转参数异常导致停止
    STEP_MOTOR_STATE_SUB_STOP_CURVE_INDEX               = 0X010B,//定频运转曲线区间设置异常导致停止
    STEP_MOTOR_STATE_SUB_STOP_CURVE_PARAM               = 0X010C,//定频运转表参数异常导致停止
    STEP_MOTOR_STATE_SUB_STOP_NOT_SUPPORT_STATE         = 0X010D,//不支持的子状态停止
    STEP_MOTOR_STATE_SUB_STOP_UNEXCEPT_STOP             = 0X010E,//运行时不期望的中断导致停止
    STEP_MOTOR_STATE_SUB_STOP_STATE_ERR                 = 0X010F,//状态机混乱导致异常
    STEP_MOTOR_STATE_SUB_STOP_STATE_FPGA_STATE          = 0X0110,//FPGA状态不匹配导致异常
    STEP_MOTOR_STATE_SUB_STOP_TIME_OUT                  = 0X0111,//电机运行超时
    STEP_MOTOR_STATE_SUB_STOP_SINGAL_UNEXCEPT_EM_STOP   = 0X0112,//不期望的信号急停导致的停止
    STEP_MOTOR_STATE_SUB_STOP_SINGAL_UNEXCEPT_UTIL_STOP = 0X0113,//不期望的信号辅助急停导致的停止
    STEP_MOTOR_STATE_SUB_STOP_FPFA_CONTROL_CMD_STOP_INT = 0X0114,//电机中断中调用电机停止指令失败导致停止
    STEP_MOTOR_STATE_SUB_STOP_FPFA_CONTROL_START_FIX_INT= 0X0115,//电机中断中调用电机定频启动指令失败导致停止
    STEP_MOTOR_STATE_SUB_STOP_FPFA_CONTROL_START_TAB_INT= 0X0116,//电机中断中调用电机表启动指令失败导致停止
    STEP_MOTOR_STATE_SUB_STOP_FPFA_CONTROL_TO_IDLE      = 0X0117,//控制FPGA电机进入空闲失败导致电机停止
    STEP_MOTOR_STATE_SUB_STOP_FPFA_CONTROL_START_FIX    = 0X0118,//调用电机定频启动指令失败导致停止
    STEP_MOTOR_STATE_SUB_STOP_FPFA_CONTROL_START_TAB    = 0X0119,//调用电机表启动指令失败导致停止
    STEP_MOTOR_STATE_SUB_STOP_FPFA_CONTROL_CMD_STOP     = 0X011A,//调用电机停止指令失败导致停止
    STEP_MOTOR_STATE_SUB_STOP_FPFA_CONTROL_CMD_DISABLE  = 0X011B,//调用电机禁止指令失败导致停止
    STEP_MOTOR_STATE_SUB_STOP_RESET_SENSOR_DUMMY_TRIG   = 0X011C,//复位传感器伪触发
    STEP_MOTOR_STATE_SUB_STOP_PLUSE_OVER_NOT_FIND_ORIGIN= 0X011D,//脉冲发送完成没找到原点传感器
    STEP_MOTOR_STATE_SUB_STOP_RESET_SENSOR_DUMMY_DISAPP = 0X011E,//复位传感器伪消失
    STEP_MOTOR_STATE_SUB_STOP_PLUSE_OVER_NO_LEAVE_ORIGIN= 0X011F,//脉冲发送完成没离开原点传感器
    STEP_MOTOR_STATE_SUB_STOP_ORIGIN_DEC_LOSS           = 0X0120,//撞到原点后减速停止过程中,原点消失
    STEP_MOTOR_STATE_SUB_STOP_ORIGIN_BACK_LOSS          = 0X0121,//撞到原点后减速停止后回到撞击点,原点消失
    STEP_MOTOR_STATE_SUB_STOP_ORIGIN_DEC_TRIG           = 0X0122,//脱离原点过程中,减速时传感器又触发了有效
    STEP_MOTOR_STATE_SUB_RUN_ALWAYS_ACC                 = 0X0201,//持续运转加速状态
    STEP_MOTOR_STATE_SUB_RUN_ALWAYS_DEC                 = 0X0202,//持续运转减速状态
    STEP_MOTOR_STATE_SUB_RUN_STEPS_NORMAL               = 0X0301,//走步数常规运行
    STEP_MOTOR_STATE_SUB_RUN_STEPS_DEC_EM_STOP          = 0X0302,//走步数遇到急停减速
    STEP_MOTOR_STATE_SUB_RUN_STEPS_DEC_UTIL_STOP        = 0X0303,//走步数遇到辅助急停减速
    STEP_MOTOR_STATE_SUB_RUN_COORDINATE_NORMAL          = 0X0401,//走坐标常规运行
    STEP_MOTOR_STATE_SUB_RUN_COORDINATE_DEC_EM_STOP     = 0X0402,//走坐标遇到急停减速
    STEP_MOTOR_STATE_SUB_RUN_COORDINATE_DEC_UTIL_STOP   = 0X0403,//走坐标遇到辅助急停减速
    STEP_MOTOR_STATE_SUB_RETURN_ZERO_TO_ORIGIN_FIRST    = 0X0501,//回零向原点运动,第一次快速运动
    STEP_MOTOR_STATE_SUB_RETURN_ZERO_TO_ORIGIN_SECOND   = 0X0502,//回零运动过程中没找到原点,慢速找原点
    STEP_MOTOR_STATE_SUB_RETURN_ZERO_TO_ORIGIN_DEC      = 0X0503,//回零运动过程中撞击到原点,切换慢速停下来
    STEP_MOTOR_STATE_SUB_RETURN_ZERO_TO_ORIGIN_BACK     = 0X0504,//回零运动过程中撞击到原点,停下来之后离开原点
    STEP_MOTOR_STATE_SUB_RETURN_ZERO_TO_ORIGIN_RETURN   = 0X0505,//回零运动过程中撞击到原点,停下来之后离开原点然后再次回到原点
    STEP_MOTOR_STATE_SUB_RETURN_ZERO_LEAVE_ORIGIN_FIRST = 0X0506,//回零离开原点运动,第一次快速离开远点
    STEP_MOTOR_STATE_SUB_RETURN_ZERO_LEAVE_ORIGIN_SECOND= 0X0507,//回零离开原点运动,回零过程中没有完全离开原点,再次尝试找原点
    STEP_MOTOR_STATE_SUB_RETURN_ZERO_LEAVE_ORIGIN_DEC   = 0X0508,//回零离开原点运动中撞击到原点,切换减速停止
    STEP_MOTOR_STATE_SUB_RETURN_ZERO_LEAVE_ORIGIN_RETURN= 0X0509,//回零离开原点减速停止之后再次靠近原点
    STEP_MOTOR_STATE_SUB_RETURN_ZERO_DEC_EM_STOP        = 0X050A,//回零触发急停减速
    STEP_MOTOR_STATE_SUB_RETURN_ZERO_DEC_UTIL_STOP      = 0X050B,//回零触发辅助急停减速
    STEP_MOTOR_STATE_SUB_RESET_TO_ORIGIN                = 0X0601,//复位运动过程找原点
    STEP_MOTOR_STATE_SUB_RESET_TO_ORIGIN_DEC            = 0X0602,//复位运动过程中撞击到原点,切换慢速停下来
    STEP_MOTOR_STATE_SUB_RESET_TO_ORIGIN_BACK           = 0X0603,//复位运动过程中撞击到原点,停下来之后离开原点
    STEP_MOTOR_STATE_SUB_RESET_TO_ORIGIN_RETURN         = 0X0604,//复位运动过程中撞击到原点,停下来之后离开原点然后再次回到原点
    STEP_MOTOR_STATE_SUB_RESET_LEAVE_ORIGIN             = 0X0605,//复位离开原点运动
    STEP_MOTOR_STATE_SUB_RESET_LEAVE_ORIGIN_DEC         = 0X0606,//复位离开原点运动中撞击到原点,切换减速停止
    STEP_MOTOR_STATE_SUB_RESET_LEAVE_ORIGIN_RETURN      = 0X0607,//复位离开原点减速停止之后再次靠近原点
    STEP_MOTOR_STATE_SUB_RESET_DEC_EM_STOP              = 0X0608,//回零触发急停减速
}STEP_MOTOR_STATE_SUB;





#endif



