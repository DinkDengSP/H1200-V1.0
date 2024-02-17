/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-11-22 09:01:17 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-12-14 12:07:27 +0800
************************************************************************************************/ 
#ifndef __MOD_IMPL_MEASURE_MODULE_CONFIG_H_
#define __MOD_IMPL_MEASURE_MODULE_CONFIG_H_
#include "ModImplBase.h"

//上门当前位置
typedef enum MEASURE_UNIT_UPDOOR_POS
{
    MEASURE_UNIT_UPDOOR_POS_CLOSE       = 0X00,//上门关闭
    MEASURE_UNIT_UPDOOR_POS_OPEN_HALF   = 0X01,//上门半开
    MEASURE_UNIT_UPDOOR_POS_OPEN_FULL   = 0X02,//上门全开
}MEASURE_UNIT_UPDOOR_POS;

/**************************************************电机设置**********************************************************/
//本机ID
//测量模块上门电机
#define MEASURE_UNIT_STEP_MOTOR_UPDOOR_LOCAL_ID                     STEP_MOTOR_NO4
//测量模块侧门电机                      
#define MEASURE_UNIT_STEP_MOTOR_WINDOW_LOCAL_ID                     STEP_MOTOR_NO3
//B液泵电机                     
#define MEASURE_UNIT_STEP_MOTOR_PUMP_LOCAL_ID                       STEP_MOTOR_NO6

//系统ID
//测量模块上门电机
#define MEASURE_UNIT_STEP_MOTOR_UPDOOR_SYS_ID                       H360_SYS_BOARD3_SM4
//测量模块侧门电机                        
#define MEASURE_UNIT_STEP_MOTOR_WINDOW_SYS_ID                       H360_SYS_BOARD3_SM3
//B液泵电机                       
#define MEASURE_UNIT_STEP_MOTOR_PUMP_SYS_ID                         H360_SYS_BOARD3_SM6

/**************************************************输入信号**********************************************************/
//测量室上门原点
#define INPUT_MEASURE_UP_DOOR_ORGIN                                 H360_SYS_PORT_IN_BOARD3_ARM_SEN4_PI8
#define VALID_LEVEL_INPUT_MEASURE_UP_DOOR_ORGIN                     GPIO_PIN_RESET

//测量室侧门原点
#define INPUT_MEASURE_WINDOW_ORGIN                                  H360_SYS_PORT_IN_BOARD3_ARM_SEN3_PC13
#define VALID_LEVEL_INPUT_MEASURE_WINDOW_ORGIN                      GPIO_PIN_RESET

//上门全开检测光电
#define INPUT_MEASURE_UNIT_UPDOOR_FULL_OPEN                         H360_SYS_PORT_IN_BOARD3_ARM_SEN14_PB9
#define VALID_LEVEL_INPUT_MEASURE_UNIT_UPDOOR_FULL_OPEN             GPIO_PIN_RESET

//激发液注射泵原点
#define INPUT_MEASURE_INJECT_PUMP_ORGIN                             H360_SYS_PORT_IN_BOARD3_ARM_SEN7_PE4
#define VALID_LEVEL_INPUT_MEASURE_INJECT_PUMP_ORGIN                 GPIO_PIN_SET

/**************************************************电机速度曲线设置****************************************************/
//侧门速度曲线
#define S_CURVE_MEASURE_UNIT_WINDOW_DEBUG                          SM_CURVE_0//侧门调试参数
#define S_CURVE_MEASURE_UNIT_WINDOW_OPEN                           SM_CURVE_1//侧门打开速度曲线选取
#define S_CURVE_MEASURE_UNIT_WINDOW_CLOSE                          SM_CURVE_2//侧门关闭速度曲线选取

//上门速度曲线
#define S_CURVE_MEASURE_UNIT_UPDOOR_DEBUG                          SM_CURVE_0//上门调试曲线
#define S_CURVE_MEASURE_UNIT_UPDOOR_CLOSE_TO_OPEN_HALF             SM_CURVE_1//上门从关闭到半开
#define S_CURVE_MEASURE_UNIT_UPDOOR_CLOSE_TO_OPEN_FULL             SM_CURVE_2//上门从关闭到全开
#define S_CURVE_MEASURE_UNIT_UPDOOR_OPEN_HALF_TO_OPEN_FULL         SM_CURVE_3//上门从半开到全开
#define S_CURVE_MEASURE_UNIT_UPDOOR_OPEN_HALF_TO_CLOSE             SM_CURVE_4//上门从半开到关闭
#define S_CURVE_MEASURE_UNIT_UPDOOR_OPEN_FULL_TO_CLOSE             SM_CURVE_5//上门从全开到关闭

//注射泵速度曲线
#define S_CURVE_MEASURE_UNIT_PUMP_DEBUG                            SM_CURVE_11//注射泵调试曲线
#define S_CURVE_MEASURE_UNIT_PUMP_INJECT                           SM_CURVE_12//注射泵注液曲线
#define S_CURVE_MEASURE_UNIT_PUMP_ABSORB                           SM_CURVE_13//注射泵吸液曲线
#define S_CURVE_MEASURE_UNIT_PUMP_ABSORB_BACK                      SM_CURVE_14//注射泵吸液回吐曲线
#define S_CURVE_MEASURE_UNIT_PUMP_RETURN_ZERO                      SM_CURVE_15//注射泵回零曲线

//测量时间
//暗噪声时间
#define TIME_MUASURE_UNIT_READ_DARK_MS                              1000
//测暗计数时间                  
#define TIME_MEASURE_UNIT_READ_BACK_GROUND_MS                       200
//测光时间                  
#define TIME_MEASURE_UNIT_READ_LIGHT_MS                             3500


/**************************************************延迟时间**********************************************************/
//注射泵吸液平衡时间
#define TIME_MS_MEASURE_PUMP_ABSORB_STABLE                          50
//注射泵回吸空气压力平衡时间                    
#define TIME_MS_MEASURE_PUMP_ABSORB_BACK_STABLE                     50
//注射泵注射压力平衡时间                    
#define TIME_MS_MEASURE_PUMP_INJECT_STABLE                          50

/********************************************其余参数*********************************************************/
//激发液注射泵最大行程
#define STEPS_PUMP_LIQUID_MAX                                       16000
//激发液泵每UL对应步数
#define STEPS_PER_UL_LIQUID_B_PUMP_ABSORB                           16
//注射泵吸液最大UL数
#define UL_MAX_PUMP_LIQUID_MEASURE_MODULE                           (STEPS_PUMP_LIQUID_MAX/STEPS_PER_UL_LIQUID_B_PUMP_ABSORB)
//激发液每次吸液量
#define UL_PUMP_ABSORB_LIQUID_MEASURE_MODULE                        200
//激发液吸液补偿ul          
#define LIQUID_B_INJECT_ONCE_COMPENSATE_UL                          10
//注液吸空气UL
#define UL_ABSORB_AIR_BEFORE_INJECT                                 200
//测光变化率设定的时间分辨率                    
#define TIME_MS_LIGHT_MULTI_POINT_RESOLUTION                        50
//抽废液泵持续工作时间                  
#define TIME_MS_WASTE_PUMP_WORK_TIME                                20000



#endif












