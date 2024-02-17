/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-11-22 09:01:17 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-12-19 15:39:03 +0800
************************************************************************************************/ 
#ifndef __MOD_IMPL_NEEDLE_SAMPLE_CONFIG_H_
#define __MOD_IMPL_NEEDLE_SAMPLE_CONFIG_H_
#include "ModImplBase.h"


/*********************************************电机配置*************************************************************/
//本机ID
//样本针升降电机编号
#define NEEDLE_SAMPLE_STEP_MOTOR_UPDOWN_LOCAL_ID                                        STEP_MOTOR_NO1
//样本针旋转电机编号                            
#define NEEDLE_SAMPLE_STEP_MOTOR_ROTATE_LOCAL_ID                                        STEP_MOTOR_NO2
//样本针注射泵电机编号,满行程无细分2000步                       
#define NEEDLE_SAMPLE_STEP_MOTOR_PUMP2000_LOCAL_ID                                      STEP_MOTOR_NO3
//样本针注射泵电机编号,满行程无细分4000步                           
#define NEEDLE_SAMPLE_STEP_MOTOR_PUMP4000_LOCAL_ID                                      STEP_MOTOR_NO7
            
//系统ID            
//样本针升降电机编号            
#define NEEDLE_SAMPLE_STEP_MOTOR_UPDOWN_SYS_ID                                          H360_SYS_BOARD5_SM1
//样本针旋转电机编号                            
#define NEEDLE_SAMPLE_STEP_MOTOR_ROTATE_SYS_ID                                          H360_SYS_BOARD5_SM2
//样本针注射泵电机编号                          
#define NEEDLE_SAMPLE_STEP_MOTOR_PUMP2000_SYS_ID                                        H360_SYS_BOARD5_SM3
//样本针注射泵电机编号                          
#define NEEDLE_SAMPLE_STEP_MOTOR_PUMP4000_SYS_ID                                        H360_SYS_BOARD5_SM7

//升降防撞的电机错误代码
#define ERROR_CODE_NEEDLE_SAMPLE_UPDOWN_EMSTOP_SINGAL                                   ERROR_SUB_SM_1_EMERGENCY_SIGNAL
//升降探液触发的电机错误代码
#define ERROR_CODE_NEEDLE_SAMPLE_UPDOWN_LIQUID_DETECT                                   ERROR_SUB_SM_1_UTIL_STOP1
/*********************************************系统输入*************************************************************/
//升降电机原点
#define INPUT_NEEDLE_SAMPLE_UPDOWN_ORGIN                                                H360_SYS_PORT_IN_BOARD5_ARM_SEN1_PI10
#define VALID_LEVEL_INPUT_NEEDLE_SAMPLE_UPDOWN_ORGIN                                    GPIO_PIN_SET

//旋转电机原点                          
#define INPUT_NEEDLE_SAMPLE_ROTATE_ORGIN                                                H360_SYS_PORT_IN_BOARD5_ARM_SEN2_PI9
#define VALID_LEVEL_INPUT_NEEDLE_SAMPLE_ROTATE_ORGIN                                    GPIO_PIN_SET

//注射泵2000步原点                            
#define INPUT_NEEDLE_SAMPLE_PUMP2000_ORGIN                                              H360_SYS_PORT_IN_BOARD5_ARM_SEN3_PC13
#define VALID_LEVEL_INPUT_NEEDLE_SAMPLE_PUMP2000_ORGIN                                  GPIO_PIN_SET

//注射泵4000步原点                            
#define INPUT_NEEDLE_SAMPLE_PUMP4000_ORGIN                                              H360_SYS_PORT_IN_BOARD5_ARM_SEN12_PI5
#define VALID_LEVEL_INPUT_NEEDLE_SAMPLE_PUMP4000_ORGIN                                  GPIO_PIN_SET

//样本针升降防撞                            
#define INPUT_NEEDLE_SAMPLE_EMERGENCY_STOP                                              H360_SYS_PORT_IN_BOARD5_YM1_ARM1_PB6
#define VALID_LEVEL_INPUT_NEEDLE_SAMPLE_EMERGENCY_STOP                                  GPIO_PIN_RESET

//样本针升降探液                            
#define INPUT_NEEDLE_SAMPLE_LIQUID_DETECT                                               H360_SYS_PORT_IN_BOARD5_YM1_ARM2_PB5
#define VALID_LEVEL_INPUT_NEEDLE_SAMPLE_LIQUID_DETECT                                   GPIO_PIN_RESET


/*********************************************电机运行速度曲线*******************************************************/
/*不同的目标位置,样本针旋转的运转曲线 */
#define S_CURVE_NEEDLE_SAMPLE_ROTATE_DEBUG                                              SM_CURVE_0//调试曲线
#define S_CURVE_NEEDLE_SAMPLE_ROTATE_CLEAN_TO_POWERFUL_CLEAN                            SM_CURVE_1//清洗到强洗位
#define S_CURVE_NEEDLE_SAMPLE_ROTATE_CLEAN_TO_SAMPLE_RACK                               SM_CURVE_2//清洗到样本管位
#define S_CURVE_NEEDLE_SAMPLE_ROTATE_CLEAN_TO_PLATE_REACTION_INJECT                     SM_CURVE_3//清洗到反应盘注液位
#define S_CURVE_NEEDLE_SAMPLE_ROTATE_CLEAN_TO_PLATE_REACTION_ABSORB                     SM_CURVE_4//清洗到反应盘吸液位
#define S_CURVE_NEEDLE_SAMPLE_ROTATE_SAMPLE_RACK_TO_POWERFUL_CLEAN                      SM_CURVE_5//样本管到强洗位
#define S_CURVE_NEEDLE_SAMPLE_ROTATE_SAMPLE_RACK_TO_TO_PLATE_REACTION_INJECT            SM_CURVE_6//样本管到反应盘注液位
#define S_CURVE_NEEDLE_SAMPLE_ROTATE_SAMPLE_RACK_TO_TO_PLATE_REACTION_ABSORB            SM_CURVE_7//样本管到反应盘吸样位
#define S_CURVE_NEEDLE_SAMPLE_ROTATE_POWERFUL_CLEAN_TO_PLATE_REACTION_INJECT            SM_CURVE_8//强洗到反应盘注液位
#define S_CURVE_NEEDLE_SAMPLE_ROTATE_POWERFUL_CLEAN_TO_PLATE_REACTION_ABSORB            SM_CURVE_9//强洗到反应盘吸液位
#define S_CURVE_NEEDLE_SAMPLE_ROTATE_PLATE_REACTION_INJECT_TO_PLATE_REACTION_ABSORB     SM_CURVE_10//反应盘注液位到吸液位
#define S_CURVE_NEEDLE_SAMPLE_ROTATE_PLATE_REACTION_INJECT_TO_CLEAN                     SM_CURVE_11//反应盘注液位到清洗位
#define S_CURVE_NEEDLE_SAMPLE_ROTATE_PLATE_REACTION_INJECT_TO_POWERFUL_CLEAN            SM_CURVE_12//反应盘注液位到强洗位
#define S_CURVE_NEEDLE_SAMPLE_ROTATE_SAMPLE_ABSORB_TO_PLATE_REACTION_INJECT             SM_CURVE_13//样本管到反应盘注液
#define S_CURVE_NEEDLE_SAMPLE_ROTATE_EMERGENCY_TO_CLEAN                                 SM_CURVE_14//清洗位到急诊吸样位
#define S_CURVE_NEEDLE_SAMPLE_ROTATE_EMERGENCY_TO_PLATE_REACTION_INJECT                 SM_CURVE_15//反应盘注液位到急诊吸样位

/*不同的目标位置,样本针升降的运转曲线 */
#define S_CURVE_NEEDLE_SAMPLE_UPDOWN_DEBUG                                              SM_CURVE_0//调试曲线
#define S_CURVE_NEEDLE_SAMPLE_UPDOWN_CLEAN_DONW                                         SM_CURVE_1//清洗位与强力清洗位下降
#define S_CURVE_NEEDLE_SAMPLE_UPDOWN_CLEAN_UP                                           SM_CURVE_2//清洗位与强力清洗位上升
#define S_CURVE_NEEDLE_SAMPLE_UPDOWN_SAMPLE_RACK_ABSORB_DONW_FAST                       SM_CURVE_3//试管位吸液快速下降
#define S_CURVE_NEEDLE_SAMPLE_UPDOWN_SAMPLE_RACK_ABSORB_DONW_SLOW_TUBE_TYPE1            SM_CURVE_4//试管位吸液慢速下降,针对试管类型1
#define S_CURVE_NEEDLE_SAMPLE_UPDOWN_SAMPLE_RACK_ABSORB_DONW_SLOW_TUBE_TYPE2            SM_CURVE_5//试管位吸液慢速下降,针对试管类型2
#define S_CURVE_NEEDLE_SAMPLE_UPDOWN_SAMPLE_RACK_ABSORB_DONW_SLOW_TUBE_TYPE3            SM_CURVE_6//试管位吸液慢速下降,针对试管类型3
#define S_CURVE_NEEDLE_SAMPLE_UPDOWN_SAMPLE_RACK_ABSORB_DONW_SLOW_TUBE_TYPE4            SM_CURVE_7//试管位吸液慢速下降,针对试管类型4
#define S_CURVE_NEEDLE_SAMPLE_UPDOWN_SAMPLE_RACK_ABSORB_DONW_SLOW_TUBE_TYPE5            SM_CURVE_8//试管位吸液慢速下降,针对试管类型5
#define S_CURVE_NEEDLE_SAMPLE_UPDOWN_SAMPLE_RACK_ABSORB_DONW_SLOW_TUBE_TYPE6            SM_CURVE_9//试管位吸液慢速下降,针对试管类型6
#define S_CURVE_NEEDLE_SAMPLE_UPDOWN_SAMPLE_RACK_ABSORB_DONW_SLOW_TUBE_TYPE7            SM_CURVE_10//试管位吸液慢速下降,针对试管类型7
#define S_CURVE_NEEDLE_SAMPLE_UPDOWN_SAMPLE_RACK_ABSORB_DONW_SLOW_TUBE_TYPE8            SM_CURVE_11//试管位吸液慢速下降,针对试管类型8
#define S_CURVE_NEEDLE_SAMPLE_UPDOWN_SAMPLE_RACK_ABSORB_UP_FAST                         SM_CURVE_12//试管位吸液快速上升
#define S_CURVE_NEEDLE_SAMPLE_UPDOWN_PLATE_REACTION_ABSORB_DONW_LEVEL_1_2_3             SM_CURVE_13//反应盘吸液下降速度,针对高度等级1 2 3
#define S_CURVE_NEEDLE_SAMPLE_UPDOWN_PLATE_REACTION_ABSORB_DONW_LEVEL_4_5_6             SM_CURVE_14//反应盘吸液下降速度,针对高度等级4 5 6
#define S_CURVE_NEEDLE_SAMPLE_UPDOWN_PLATE_REACTION_ABSORB_DONW_LEVEL_7_8               SM_CURVE_15//反应盘吸液下降速度,针对高度等级7 8
#define S_CURVE_NEEDLE_SAMPLE_UPDOWN_PLATE_REACTION_ABSORB_UP                           SM_CURVE_16//反应盘吸液升起来的速度
#define S_CURVE_NEEDLE_SAMPLE_UPDOWN_PLATE_REACTION_INJECT_DONW_LEVEL_1_2_3             SM_CURVE_17//反应盘注液下降速度,针对高度等级1 2 3
#define S_CURVE_NEEDLE_SAMPLE_UPDOWN_PLATE_REACTION_INJECT_DONW_LEVEL_4_5_6             SM_CURVE_18//反应盘注液下降速度,针对高度等级4 5 6
#define S_CURVE_NEEDLE_SAMPLE_UPDOWN_PLATE_REACTION_INJECT_DONW_LEVEL_7_8               SM_CURVE_19//反应盘注液下降速度,针对高度等级7 8
#define S_CURVE_NEEDLE_SAMPLE_UPDOWN_PLATE_REACTION_INJECT_UP                           SM_CURVE_20//反应盘注液升起来的速度


/*不同的目标位置,样本针注射泵的运转曲线 */
//样本针液量测试 5 10 25 50 75 100 125 150 200
#define S_CURVE_NEEDLE_SAMPLE_PUMP_DEBUG                                                SM_CURVE_0//调试曲线
#define S_CURVE_NEEDLE_SAMPLE_PUMP_PRIME_ABSORB_LIQUID                                  SM_CURVE_1//样本针注射泵灌注时吸液速度    
#define S_CURVE_NEEDLE_SAMPLE_PUMP_PRIME_INJECT_LIQUID                                  SM_CURVE_2//样本针注射泵灌注时排液速度    
#define S_CURVE_NEEDLE_SAMPLE_PUMP_ABSORB_AIR_AFTER_CLEAN                               SM_CURVE_3//样本针注射泵清洗完成后吸空气速度
#define S_CURVE_NEEDLE_SAMPLE_PUMP_ABSORB_AIR_AFTER_ABSORB                              SM_CURVE_4//样本针注射泵吸液完成后吸空气速度
#define S_CURVE_NEEDLE_SAMPLE_PUMP_ABSORB_LIQUID_1_8                                    SM_CURVE_6//样本针注射泵吸液1到8UL速度
#define S_CURVE_NEEDLE_SAMPLE_PUMP_ABSORB_LIQUID_9_15                                   SM_CURVE_7//样本针注射泵吸液9到15UL速度
#define S_CURVE_NEEDLE_SAMPLE_PUMP_ABSORB_LIQUID_16_35                                  SM_CURVE_8//样本针注射泵吸液16到35UL速度
#define S_CURVE_NEEDLE_SAMPLE_PUMP_ABSORB_LIQUID_36_65                                  SM_CURVE_9//样本针注射泵吸液36到65UL速度
#define S_CURVE_NEEDLE_SAMPLE_PUMP_ABSORB_LIQUID_66_90                                  SM_CURVE_10//样本针注射泵吸液66到90UL速度
#define S_CURVE_NEEDLE_SAMPLE_PUMP_ABSORB_LIQUID_91_120                                 SM_CURVE_11//样本针注射泵吸液91到120UL速度
#define S_CURVE_NEEDLE_SAMPLE_PUMP_ABSORB_LIQUID_121_140                                SM_CURVE_12//样本针注射泵吸液121到140UL速度
#define S_CURVE_NEEDLE_SAMPLE_PUMP_ABSORB_LIQUID_141_180                                SM_CURVE_13//样本针注射泵吸液141到180UL速度
#define S_CURVE_NEEDLE_SAMPLE_PUMP_ABSORB_LIQUID_181_210                                SM_CURVE_14//样本针注射泵吸液181到210UL速度
#define S_CURVE_NEEDLE_SAMPLE_PUMP_ABSORB_LIQUID_211_250                                SM_CURVE_15//样本针注射泵吸液211到250UL速度
#define S_CURVE_NEEDLE_SAMPLE_PUMP_INJECT_LIQUID_50_250                                 SM_CURVE_16//样本针注射泵注液速度,快速
#define S_CURVE_NEEDLE_SAMPLE_PUMP_INJECT_LIQUID_1_50                                   SM_CURVE_5//样本针注射泵注液速度,慢速   

/********************************************清洗灌注时间配置************************************************************************************/
//样本针清洗时间1,毫秒,长清洗
#define NEEDLE_SAMPLE_TIME_MS_CLEAN_1                                                   4000        
//样本针清洗时间2,毫秒,正常清洗                                     
#define NEEDLE_SAMPLE_TIME_MS_CLEAN_2                                                   1000        
//样本针清洗时间3,毫秒                                      
#define NEEDLE_SAMPLE_TIME_MS_CLEAN_3                                                   1000        
//样本针清洗时间4,毫秒                                      
#define NEEDLE_SAMPLE_TIME_MS_CLEAN_4                                                   1000        
//样本针清洗时间5,毫秒                                      
#define NEEDLE_SAMPLE_TIME_MS_CLEAN_5                                                   1000        
//样本针清洗灌注时间,毫秒                                       
#define NEEDLE_SAMPLE_TIME_MS_PRIME_CLEAN                                               5000  

/************************************************其他配置************************************************************************************/
//样本针注射泵最大步数
#define STEPS_NEEDLE_SAMPLE_PUMP2000_MAX                                                16000
//样本针注射泵1UL的步数                             
#define STEPS_NEEDLE_SAMPLE_PUMP2000_ONE_UL                                             64

//样本针注射泵最大步数
#define STEPS_NEEDLE_SAMPLE_PUMP4000_MAX                                                32000
//样本针注射泵1UL的步数                             
#define STEPS_NEEDLE_SAMPLE_PUMP4000_ONE_UL                                             128

//样本针注射泵最大液量          
#define UL_NEEDLE_SAMPLE_PUMP2000_MAX                                                   ((STEPS_NEEDLE_SAMPLE_PUMP2000_MAX)/(STEPS_NEEDLE_SAMPLE_PUMP2000_ONE_UL))
#define UL_NEEDLE_SAMPLE_PUMP4000_MAX                                                   ((STEPS_NEEDLE_SAMPLE_PUMP4000_MAX)/(STEPS_NEEDLE_SAMPLE_PUMP4000_ONE_UL))

//样本针一个吸液单元对应的微升数
#define UL_NEEDLE_SAMPLE_ONE_UNIT_PUMP2000                                              (0.125f)
#define UL_NEEDLE_SAMPLE_ONE_UNIT_PUMP4000                                              (0.0625f)

//通过泵内物质单元数量计算微升数
#define NEEDLE_SAMPLE_PUMP2000_LIQUID_UNIT_TO_UL(unit)                                 ((float)(unit*UL_NEEDLE_SAMPLE_ONE_UNIT_PUMP2000))
//通过微升数计算泵内物质单元数量
#define NEEDLE_SAMPLE_PUMP2000_LIQUID_UL_TO_UNIT(ulCount)                              ((uint32_t)(ulCount/UL_NEEDLE_SAMPLE_ONE_UNIT_PUMP2000))

//通过泵内物质单元数量计算微升数
#define NEEDLE_SAMPLE_PUMP4000_LIQUID_UNIT_TO_UL(unit)                                 ((float)(unit*UL_NEEDLE_SAMPLE_ONE_UNIT_PUMP4000))
//通过微升数计算泵内物质单元数量
#define NEEDLE_SAMPLE_PUMP4000_LIQUID_UL_TO_UNIT(ulCount)                              ((uint32_t)(ulCount/UL_NEEDLE_SAMPLE_ONE_UNIT_PUMP4000))

//试剂针最小单位对应的步数
#define STEPS_NEEDLE_SAMPLE_ONE_UNIT_PUMP2000                                           (STEPS_NEEDLE_SAMPLE_PUMP2000_ONE_UL*UL_NEEDLE_SAMPLE_ONE_UNIT_PUMP2000)
//试剂针注射泵泵最小单位数最大容量      
#define NEEDLE_SAMPLE_PUMP_MAX_UNIT_PUMP2000                                            ((STEPS_NEEDLE_SAMPLE_PUMP2000_MAX)/(STEPS_NEEDLE_SAMPLE_ONE_UNIT_PUMP2000))

//试剂针最小单位对应的步数
#define STEPS_NEEDLE_SAMPLE_ONE_UNIT_PUMP4000                                           (STEPS_NEEDLE_SAMPLE_PUMP4000_ONE_UL*UL_NEEDLE_SAMPLE_ONE_UNIT_PUMP4000)
//试剂针注射泵泵最小单位数最大容量      
#define NEEDLE_SAMPLE_PUMP_MAX_UNIT_PUMP4000                                            ((STEPS_NEEDLE_SAMPLE_PUMP4000_MAX)/(STEPS_NEEDLE_SAMPLE_ONE_UNIT_PUMP4000))

//样本泵混匀补偿液量
#define UL_UNIT_COMPENSATE_SAMPLE_PUMP2000_INJECT_MIX                                   ((100.0f/250.0f)*2000.0f)
#define UL_UNIT_COMPENSATE_SAMPLE_PUMP4000_INJECT_MIX                                   ((100.0f/250.0f)*4000.0f)

//样本针强洗吸吐液量            
#define UL_NEEDLE_SAMPLE_POWERFUL_CLEAN                                                 100
//样本针探液保护步数            
#define STEPS_NEEDLE_SAMPLE_LIQUID_DETECT_PROTECT                                       200

//样本针探液起始位置回退高度,轨道位置
#define STEPS_NEEDLE_SAMPLE_UPDOWN_LIQUID_DETECT_BACK_RACK_POS                          300

//吸液等待稳定时间
#define TIME_MS_PUMP_ABSORB_WAIT_STABLE                                                 20
//注液等待稳定时间                              
#define TIME_MS_PUMP_INJECT_WAIT_STABLE                                                 20

//样本针反应盘注液参数
//注液时根据分辨率确定注液下降速度曲线和混匀步数
#define STEPS_NEEDLE_SAMPLE_LIQUID_INJECT_SCALE                                         (130.0f)
//样本针液量高度计算平方系数                                
#define NEEDLE_SAMPLE_INJECT_COEFFICIENT_AXX                                            (-0.0004f)
//样本针液量高度计算一次系数                                
#define NEEDLE_SAMPLE_INJECT_COEFFICIENT_BX                                             (1.1945f)
//修正系数                              
#define NEEDLE_SAMPLE_INJECT_COEFFICIENT_C                                              (11.925f) 

//样本针反应盘吸液参数
//吸液时根据分辨率确定注液下降速度曲线和混匀步数
#define STEPS_NEEDLE_SAMPLE_LIQUID_ABSORB_SCALE                                         (130.0f)
//样本针液量高度计算平方系数                                
#define NEEDLE_SAMPLE_ABSORB_COEFFICIENT_AXX                                            (-0.0004f)
//样本针液量高度计算一次系数                                
#define NEEDLE_SAMPLE_ABSORB_COEFFICIENT_BX                                             (1.1945f)
//修正系数                              
#define NEEDLE_SAMPLE_ABSORB_COEFFICIENT_C                                              (11.925f)  

//从杯口到杯底步数
#define NEEDLE_SAMPLE_STEPS_FROM_CUP_TOP_TO_BOTTOM                                      (1048.0f)
//样本针吸吐混匀步数
#define STEPS_NEEDLE_SAMPLE_MIX_INJECT_LEVEL_1_SHALLOWEST                               50//注液1混匀升降步数
#define STEPS_NEEDLE_SAMPLE_MIX_INJECT_LEVEL_2                                          50//注液2混匀升降步数
#define STEPS_NEEDLE_SAMPLE_MIX_INJECT_LEVEL_3                                          50//注液3混匀升降步数
#define STEPS_NEEDLE_SAMPLE_MIX_INJECT_LEVEL_4                                          50//注液4混匀升降步数
#define STEPS_NEEDLE_SAMPLE_MIX_INJECT_LEVEL_5                                          50//注液5混匀升降步数
#define STEPS_NEEDLE_SAMPLE_MIX_INJECT_LEVEL_6                                          50//注液6混匀升降步数
#define STEPS_NEEDLE_SAMPLE_MIX_INJECT_LEVEL_7                                          50//注液7混匀升降步数
#define STEPS_NEEDLE_SAMPLE_MIX_INJECT_LEVEL_8_DEEPEST                                  50//注液8混匀升降步数

//不同试管类型的起始高度和试管架平面的高度差
#define STEPS_NEEDLE_SAMPLE_UP_DOWN_SAMPLE_RACK_FLAT_TO_TUBE_TYPE1_START                -1215//试管架平面到试管类型1起始位步数 5ml采血管
#define STEPS_NEEDLE_SAMPLE_UP_DOWN_SAMPLE_RACK_FLAT_TO_TUBE_TYPE1_MAX_LIMIT            1400//试管架平面到试管类型1极限位步数
#define STEPS_NEEDLE_SAMPLE_UP_DOWN_SAMPLE_RACK_FLAT_TO_TUBE_TYPE2_START                -525//试管架平面到试管类型2起始位步数 2ml采血管
#define STEPS_NEEDLE_SAMPLE_UP_DOWN_SAMPLE_RACK_FLAT_TO_TUBE_TYPE2_MAX_LIMIT            1420//试管架平面到试管类型2极限位步数
#define STEPS_NEEDLE_SAMPLE_UP_DOWN_SAMPLE_RACK_FLAT_TO_TUBE_TYPE3_START                -340//试管架平面到试管类型3起始位步数 校准品管   
#define STEPS_NEEDLE_SAMPLE_UP_DOWN_SAMPLE_RACK_FLAT_TO_TUBE_TYPE3_MAX_LIMIT            940//试管架平面到试管类型3极限位步数
#define STEPS_NEEDLE_SAMPLE_UP_DOWN_SAMPLE_RACK_FLAT_TO_TUBE_TYPE5_START                -270//试管架平面到试管类型5起始位步数 日立管
#define STEPS_NEEDLE_SAMPLE_UP_DOWN_SAMPLE_RACK_FLAT_TO_TUBE_TYPE5_MAX_LIMIT            640//试管架平面到试管类型5极限位步数
#define STEPS_NEEDLE_SAMPLE_UP_DOWN_SAMPLE_RACK_FLAT_TO_TUBE_TYPE6_START                -340//试管架平面到试管类型6起始位步数 冻融管
#define STEPS_NEEDLE_SAMPLE_UP_DOWN_SAMPLE_RACK_FLAT_TO_TUBE_TYPE6_MAX_LIMIT            1400//试管架平面到试管类型6极限位步数
#define STEPS_NEEDLE_SAMPLE_UP_DOWN_SAMPLE_RACK_FLAT_TO_TUBE_TYPE7_START                -1215//试管架平面到试管类型7起始位步数 
#define STEPS_NEEDLE_SAMPLE_UP_DOWN_SAMPLE_RACK_FLAT_TO_TUBE_TYPE7_MAX_LIMIT            1400//试管架平面到试管类型7极限位步数
#define STEPS_NEEDLE_SAMPLE_UP_DOWN_SAMPLE_RACK_FLAT_TO_TUBE_TYPE8_START                -1215//试管架平面到试管类型8起始位步数
#define STEPS_NEEDLE_SAMPLE_UP_DOWN_SAMPLE_RACK_FLAT_TO_TUBE_TYPE8_MAX_LIMIT            1400//试管架平面到试管类型8极限位步数

//流水线类型 试管高度步数
#define STEPS_NEEDLE_SAMPLE_UP_DOWN_ASSEMBLY_LINE_TUBE_TYPE1_HIGHT                      2615//联机轨道 试管类型1试管高度  5ml采血管
#define STEPS_NEEDLE_SAMPLE_UP_DOWN_ASSEMBLY_LINE_TUBE_TYPE2_HIGHT                      1945//联机轨道 试管类型2试管高度  2ml采血管
#define STEPS_NEEDLE_SAMPLE_UP_DOWN_ASSEMBLY_LINE_TUBE_TYPE3_HIGHT                      1190//联机轨道 试管类型3试管高度  锥形容器 顶部1050，底部2240

//极限高度抬高步数，死腔量设置为0
#define STEPS_NEEDLE_SAMPLE_UP_DOWN_TUBE_TYPE1_MAX_LIMIT_RAISE                          0//5ml采血管 极限抬高步数
#define STEPS_NEEDLE_SAMPLE_UP_DOWN_TUBE_TYPE2_MAX_LIMIT_RAISE                          0//2ml采血管 极限抬高步数
#define STEPS_NEEDLE_SAMPLE_UP_DOWN_TUBE_TYPE3_MAX_LIMIT_RAISE                          0//校准品管  极限抬高步数
#define STEPS_NEEDLE_SAMPLE_UP_DOWN_TUBE_TYPE4_MAX_LIMIT_RAISE                          0//子弹头极限抬高步数
#define STEPS_NEEDLE_SAMPLE_UP_DOWN_TUBE_TYPE5_MAX_LIMIT_RAISE                          0//日立管 极限抬高步数
#define STEPS_NEEDLE_SAMPLE_UP_DOWN_TUBE_TYPE6_MAX_LIMIT_RAISE                          0//冻融管 极限抬高步数
#define STEPS_NEEDLE_SAMPLE_UP_DOWN_TUBE_TYPE7_MAX_LIMIT_RAISE                          0//极限抬高步数
#define STEPS_NEEDLE_SAMPLE_UP_DOWN_TUBE_TYPE8_MAX_LIMIT_RAISE                          0//极限抬高步数

// 不同试管类型的移液针吸1unit下降步数 上半段
#define STEPS_NEEDLE_SAMPLE_UP_DOWN_HIGH_SEGMENT_ABSORB_ONE_UNIT_TUBE_TYPE_1            (0.0f)//移液针吸1unit下降步数 上半段 类型1 5ml采血管  100ul- 40步 (40/100)*0.125
#define STEPS_NEEDLE_SAMPLE_UP_DOWN_HIGH_SEGMENT_ABSORB_ONE_UNIT_TUBE_TYPE_2            (0.0f)//移液针吸1unit下降步数 上半段 类型2 2ml采血管
#define STEPS_NEEDLE_SAMPLE_UP_DOWN_HIGH_SEGMENT_ABSORB_ONE_UNIT_TUBE_TYPE_3            (0.0f)//移液针吸1unit下降步数 上半段 类型3 校准品管  
#define STEPS_NEEDLE_SAMPLE_UP_DOWN_HIGH_SEGMENT_ABSORB_ONE_UNIT_TUBE_TYPE_4            (0.1f)//移液针吸1unit下降步数 上半段 类型4 子弹头 按最低高度，100ul多下降50步
#define STEPS_NEEDLE_SAMPLE_UP_DOWN_HIGH_SEGMENT_ABSORB_ONE_UNIT_TUBE_TYPE_5            (0.0f)//移液针吸1unit下降步数 上半段 类型5  日立管
#define STEPS_NEEDLE_SAMPLE_UP_DOWN_HIGH_SEGMENT_ABSORB_ONE_UNIT_TUBE_TYPE_6            (0.0f)//移液针吸1unit下降步数 上半段 类型6 冻融管  
#define STEPS_NEEDLE_SAMPLE_UP_DOWN_HIGH_SEGMENT_ABSORB_ONE_UNIT_TUBE_TYPE_7            (0.0f)//移液针吸1unit下降步数 上半段 类型7 
#define STEPS_NEEDLE_SAMPLE_UP_DOWN_HIGH_SEGMENT_ABSORB_ONE_UNIT_TUBE_TYPE_8            (0.0f)//移液针吸1unit下降步数 上半段 类型8

//不同试管类型的移液针吸1unit下降步数 下半段   
#define STEPS_NEEDLE_SAMPLE_UP_DOWN_LOW_SEGMENT_ABSORB_ONE_UNIT_TUBE_TYPE_1             (0.0f)//移液针吸1unit下降步数 下半段 类型1 5ml采血管
#define STEPS_NEEDLE_SAMPLE_UP_DOWN_LOW_SEGMENT_ABSORB_ONE_UNIT_TUBE_TYPE_2             (0.0f)//移液针吸1unit下降步数 下半段 类型2 2ml采血管
#define STEPS_NEEDLE_SAMPLE_UP_DOWN_LOW_SEGMENT_ABSORB_ONE_UNIT_TUBE_TYPE_3             (0.0f)//移液针吸1unit下降步数 下半段 类型3 校准品管  100ul多下降50度
#define STEPS_NEEDLE_SAMPLE_UP_DOWN_LOW_SEGMENT_ABSORB_ONE_UNIT_TUBE_TYPE_4             (0.1f)//移液针吸1unit下降步数 下半段 类型4 子弹头
#define STEPS_NEEDLE_SAMPLE_UP_DOWN_LOW_SEGMENT_ABSORB_ONE_UNIT_TUBE_TYPE_5             (0.0f)//移液针吸1unit下降步数 下半段 类型5  日立管
#define STEPS_NEEDLE_SAMPLE_UP_DOWN_LOW_SEGMENT_ABSORB_ONE_UNIT_TUBE_TYPE_6             (0.0f)//移液针吸1unit下降步数 下半段 类型6 冻融管  
#define STEPS_NEEDLE_SAMPLE_UP_DOWN_LOW_SEGMENT_ABSORB_ONE_UNIT_TUBE_TYPE_7             (0.0f)//移液针吸1unit下降步数 下半段 类型7 
#define STEPS_NEEDLE_SAMPLE_UP_DOWN_LOW_SEGMENT_ABSORB_ONE_UNIT_TUBE_TYPE_8             (0.0f)//移液针吸1unit下降步数 下半段 类型8

//分段点到极限点的步数差
#define STEPS_NEEDLE_SAMPLE_UP_DOWN_SWITCH_SEGMENT_POS_TO_MAX_POS_TUBE_TYPE_1           65//极限点到分段点的步数差  类型1 5ml采血管        
#define STEPS_NEEDLE_SAMPLE_UP_DOWN_SWITCH_SEGMENT_POS_TO_MAX_POS_TUBE_TYPE_2           65//极限点到分段点的步数差  类型2 2ml采血管 
#define STEPS_NEEDLE_SAMPLE_UP_DOWN_SWITCH_SEGMENT_POS_TO_MAX_POS_TUBE_TYPE_3           175//极限点到分段点的步数差  类型3 校准品管   
#define STEPS_NEEDLE_SAMPLE_UP_DOWN_SWITCH_SEGMENT_POS_TO_MAX_POS_TUBE_TYPE_4           500//极限点到分段点的步数差  类型4 子弹头   
#define STEPS_NEEDLE_SAMPLE_UP_DOWN_SWITCH_SEGMENT_POS_TO_MAX_POS_TUBE_TYPE_5           170//极限点到分段点的步数差  类型5 日立管   
#define STEPS_NEEDLE_SAMPLE_UP_DOWN_SWITCH_SEGMENT_POS_TO_MAX_POS_TUBE_TYPE_6           0//极限点到分段点的步数差  类型6 冻融管   
#define STEPS_NEEDLE_SAMPLE_UP_DOWN_SWITCH_SEGMENT_POS_TO_MAX_POS_TUBE_TYPE_7           0//极限点到分段点的步数差  类型7  
#define STEPS_NEEDLE_SAMPLE_UP_DOWN_SWITCH_SEGMENT_POS_TO_MAX_POS_TUBE_TYPE_8           0//极限点到分段点的步数差  类型8     


   


#endif













