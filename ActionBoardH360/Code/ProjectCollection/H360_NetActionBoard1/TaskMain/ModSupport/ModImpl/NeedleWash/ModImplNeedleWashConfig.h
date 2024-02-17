/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-11-24 15:09:16 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-12-05 13:04:46 +0800
************************************************************************************************/ 
#ifndef __MOD_IMPL_NEEDLE_WASH_CONFIG_H_
#define __MOD_IMPL_NEEDLE_WASH_CONFIG_H_
#include "ModImplBase.h"

//本地电机ID
//清洗针升降运转电机编号
#define NEEDLE_WASH_STEP_MOTOR_UPDOWN_LOCAL_ID                  STEP_MOTOR_NO9
//清洗针吸废液电机编号      
#define NEEDLE_WASH_STEP_MOTOR_WASTE_PUMP_LOCAL_ID              STEP_MOTOR_NO6
//清洗针注射泵1电机编号     
#define NEEDLE_WASH_STEP_MOTOR_PUMP1_LOCAL_ID                   STEP_MOTOR_NO1
//清洗针注射泵2电机编号     
#define NEEDLE_WASH_STEP_MOTOR_PUMP2_LOCAL_ID                   STEP_MOTOR_NO2
//清洗针注射泵3电机编号     
#define NEEDLE_WASH_STEP_MOTOR_PUMP3_LOCAL_ID                   STEP_MOTOR_NO3
//清洗针注射泵4电机编号     
#define NEEDLE_WASH_STEP_MOTOR_PUMP4_LOCAL_ID                   STEP_MOTOR_NO4
//清洗针注射泵5电机编号     
#define NEEDLE_WASH_STEP_MOTOR_PUMP5_LOCAL_ID                   STEP_MOTOR_NO5

//系统电机ID
//清洗针升降运转电机编号
#define NEEDLE_WASH_STEP_MOTOR_UPDOWN_SYS_ID                    H360_SYS_BOARD1_SM9
//清洗针吸废液电机编号          
#define NEEDLE_WASH_STEP_MOTOR_WASTE_PUMP_SYS_ID                H360_SYS_BOARD1_SM6
//清洗针注射泵1电机编号         
#define NEEDLE_WASH_STEP_MOTOR_PUMP1_SYS_ID                     H360_SYS_BOARD1_SM1
//清洗针注射泵2电机编号         
#define NEEDLE_WASH_STEP_MOTOR_PUMP2_SYS_ID                     H360_SYS_BOARD1_SM2
//清洗针注射泵3电机编号         
#define NEEDLE_WASH_STEP_MOTOR_PUMP3_SYS_ID                     H360_SYS_BOARD1_SM3
//清洗针注射泵4电机编号         
#define NEEDLE_WASH_STEP_MOTOR_PUMP4_SYS_ID                     H360_SYS_BOARD1_SM4
//清洗针注射泵5电机编号         
#define NEEDLE_WASH_STEP_MOTOR_PUMP5_SYS_ID                     H360_SYS_BOARD1_SM5

//升降电机原点
#define INPUT_NEEDLE_WASH_UPDOWN_ORGIN                          H360_SYS_PORT_IN_BOARD1_ARM_SEN9_PE2
#define VALID_LEVEL_INPUT_NEEDLE_WASH_UPDOWN_ORGIN              GPIO_PIN_RESET

//清洗针注射泵1原点
#define INPUT_NEEDLE_WASH_PUMP1_ORGIN                           H360_SYS_PORT_IN_BOARD1_ARM_SEN1_PI10
#define VALID_LEVEL_INPUT_NEEDLE_WASH_PUMP1_ORGIN               GPIO_PIN_SET

//清洗针注射泵2原点
#define INPUT_NEEDLE_WASH_PUMP2_ORGIN                           H360_SYS_PORT_IN_BOARD1_ARM_SEN2_PI9
#define VALID_LEVEL_INPUT_NEEDLE_WASH_PUMP2_ORGIN               GPIO_PIN_SET

//清洗针注射泵3原点
#define INPUT_NEEDLE_WASH_PUMP3_ORGIN                           H360_SYS_PORT_IN_BOARD1_ARM_SEN3_PC13
#define VALID_LEVEL_INPUT_NEEDLE_WASH_PUMP3_ORGIN               GPIO_PIN_SET

//清洗针注射泵4原点
#define INPUT_NEEDLE_WASH_PUMP4_ORGIN                           H360_SYS_PORT_IN_BOARD1_ARM_SEN4_PI8
#define VALID_LEVEL_INPUT_NEEDLE_WASH_PUMP4_ORGIN               GPIO_PIN_SET

//清洗针注射泵5原点
#define INPUT_NEEDLE_WASH_PUMP5_ORGIN                           H360_SYS_PORT_IN_BOARD1_ARM_SEN5_PE6
#define VALID_LEVEL_INPUT_NEEDLE_WASH_PUMP5_ORGIN               GPIO_PIN_SET

/****************************************电机运行曲线配置***************************************************/
/*清洗针升降运行曲线 */
#define S_CURVE_NEEDLE_WASH_UPDOWN_DEBUG                            SM_CURVE_0//清洗针升降调试曲线
#define S_CURVE_NEEDLE_WASH_UPDOWN_TOP_TO_CUP_TOP_DOWN              SM_CURVE_1//清洗针升降下降顶部到杯口曲线
#define S_CURVE_NEEDLE_WASH_UPDOWN_CUP_TOP_TO_CUP_BOTTOM_DOWN       SM_CURVE_2//清洗针升降下降杯口到杯底曲线
#define S_CURVE_NEEDLE_WASH_UPDOWN_TOP_TO_PRIME_DOWN                SM_CURVE_3//清洗针升降下降顶部到灌注位曲线
#define S_CURVE_NEEDLE_WASH_UPDOWN_CUP_BOTTOM_TO_CUP_TOP_UP         SM_CURVE_4//清洗针升降上升杯底到杯口曲线
#define S_CURVE_NEEDLE_WASH_UPDOWN_CUP_TOP_TO_TOP_UP                SM_CURVE_5//清洗针升降上升杯口到顶部曲线
#define S_CURVE_NEEDLE_WASH_UPDOWN_PRIME_TO_TOP_UP                  SM_CURVE_6//清洗针升降上升灌注位到顶部曲线
//新清洗针三孔
#define S_CURVE_THREE_HOLE_NEEDLE_WASH_UPDOWN_CUP_TOP_TO_CUP_BOTTOM_DOWN       SM_CURVE_10//新三孔 清洗针升降下降杯口到杯底曲线
#define S_CURVE_THREE_HOLE_NEEDLE_WASH_UPDOWN_CUP_BOTTOM_TO_CUP_TOP_UP         SM_CURVE_11//新三孔 清洗针升降上升杯底到杯口曲线


/*清洗针注射泵运行曲线*/
#define S_CURVE_NEEDLE_WASH_PUMP_DEBUG                              SM_CURVE_0//清洗针注射泵调试曲线
#define S_CURVE_NEEDLE_WASH_PUMP_ABSORB_PRIME                       SM_CURVE_1//清洗针注射泵灌注吸液曲线
#define S_CURVE_NEEDLE_WASH_PUMP_INJECT_PRIME                       SM_CURVE_2//清洗针注射泵灌注注液曲线
#define S_CURVE_NEEDLE_WASH_PUMP_ABSORB_WASH                        SM_CURVE_3//清洗针注射泵清洗吸液曲线
#define S_CURVE_NEEDLE_WASH_PUMP_INJECT_WASH                        SM_CURVE_4//清洗针注射泵清洗注液曲线
//新清洗针三孔
#define S_CURVE_THREE_HOLE_NEEDLE_WASH_PUMP_INJECT_WASH             SM_CURVE_10//新三孔 清洗针注射泵清洗注液曲线


/*吸废液泵运行曲线,当注液不同的针的时候,速度可以不一样,因为管路压强可能不一样*/
//也可以设置成一样的
#define S_CURVE_NEEDLE_WASH_WASTE_PUMP_DEBUG                        SM_CURVE_0//吸废液泵调试曲线
#define S_CURVE_NEEDLE_WASH_WASTE_PUMP_ABSORB_WASTE_NEEDLE_1        SM_CURVE_1//吸废液泵注液1根针的吸液曲线
#define S_CURVE_NEEDLE_WASH_WASTE_PUMP_ABSORB_WASTE_NEEDLE_2        SM_CURVE_2//吸废液泵注液2根针的吸液曲线
#define S_CURVE_NEEDLE_WASH_WASTE_PUMP_ABSORB_WASTE_NEEDLE_3        SM_CURVE_3//吸废液泵注液3根针的吸液曲线
#define S_CURVE_NEEDLE_WASH_WASTE_PUMP_ABSORB_WASTE_NEEDLE_4        SM_CURVE_4//吸废液泵注液4根针的吸液曲线
#define S_CURVE_NEEDLE_WASH_WASTE_PUMP_ABSORB_WASTE_NEEDLE_5        SM_CURVE_5//吸废液泵注液5根针的吸液曲线

/*****************************其余配置***************************************/
//注射泵最大步数
#define MAX_STEP_PUMP_INJECT            16000
//注射泵最大容量
#define UL_MAX_PUMP_INJECT              1000 
//注射泵1UL代表的步数
#define STEP_PUMP_INJECT_ONE_UL         (MAX_STEP_PUMP_INJECT/UL_MAX_PUMP_INJECT)  

//-----------------------------------三针参数---------------------------------------
//三孔清洗针等待反应杯稳定延时
#define MS_THREE_HOLE_NEEDLE_WASH_WAIT_PLATE_CUP_STEADY         50

//清洗注射泵序号
typedef enum MOTOR_NO_NEEDLE_WASH_PUMP
{
    MOTOR_NO_NEEDLE_WASH_PUMP_1     = 0,
    MOTOR_NO_NEEDLE_WASH_PUMP_2     = 1,
    MOTOR_NO_NEEDLE_WASH_PUMP_3     = 2,
    MOTOR_NO_NEEDLE_WASH_PUMP_4     = 3,
    MOTOR_NO_NEEDLE_WASH_PUMP_5     = 4,
}MOTOR_NO_NEEDLE_WASH_PUMP;

#endif



