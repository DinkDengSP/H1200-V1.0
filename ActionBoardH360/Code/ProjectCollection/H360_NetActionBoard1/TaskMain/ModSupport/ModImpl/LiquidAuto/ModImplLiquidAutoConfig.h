/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-11-24 15:09:16 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-12-04 14:50:31 +0800
************************************************************************************************/ 
#ifndef __MOD_IMPL_LIQUID_AUTO_CONFIG_H_
#define __MOD_IMPL_LIQUID_AUTO_CONFIG_H_
#include "ModImplBase.h"

/*************************************************废液相关信号********************************************************/
//废液箱上浮球
#define INPUT_WASTE_TANK_LEVEL_HIGH                             H360_SYS_PORT_IN_BOARD1_SLAVE_ARM_SEN1_PH12
/*废液桶上液位传感器有效电平,上液位满的时候,浮子浮起来,浮子触发传感器,下液位浮子和上液位浮子的传感器有差异 */
#define VALID_LEVEL_INPUT_WASTE_TANK_LEVEL_HIGH                 GPIO_PIN_SET

//V307废液箱废液排放阀
#define OUTPUT_LIQUID_VALVE_V307                                H360_SYS_PORT_OUT_BOARD1_SLAVE_SEN26_PH3
//打开两通阀的电平
#define VALID_LEVEL_OUTPUT_LIQUID_VALVE_V307                    GPIO_PIN_RESET

//DP1废液箱废液排放泵
#define OUTPUT_LIQUID_PUMP_DP1                                  H360_SYS_PORT_OUT_BOARD1_SLAVE_SEN25_PF10
//打开DP1的电平
#define VALID_LEVEL_OUTPUT_LIQUID_PUMP_DP1                      GPIO_PIN_RESET

/********************************************液路自动化稀释*****************************************/
//DP101 配置瓶到目标瓶抽液泵
#define OUTPUT_LIQUID_PUMP_DP101                                H360_SYS_PORT_OUT_BOARD1_CPLD_CDC1
//打开DP101的电平       
#define VALID_LEVEL_OUTPUT_LIQUID_PUMP_DP101                    GPIO_PIN_RESET

//DP304 浓缩液抽液到配置瓶泵        
#define OUTPUT_LIQUID_PUMP_DP304                                H360_SYS_PORT_OUT_BOARD1_CPLD_CDC24
//打开DP304的电平       
#define VALID_LEVEL_OUTPUT_LIQUID_PUMP_DP304                    GPIO_PIN_RESET

//DP103 纯水抽液到配置瓶抽液泵      
#define OUTPUT_LIQUID_PUMP_DP103                                H360_SYS_PORT_OUT_BOARD1_CPLD_CDC3
//打开DP103的电平       
#define VALID_LEVEL_OUTPUT_LIQUID_PUMP_DP103                    GPIO_PIN_RESET

//V235 浓缩液纯水选通阀,关闭选通纯水,打开选通浓缩液     
#define OUTPUT_LIQUID_VALVE_V235                                H360_SYS_PORT_OUT_BOARD1_CPLD_CDC8
//打开三通阀的电平      
#define VALID_LEVEL_OUTPUT_LIQUID_VALVE_V235                    GPIO_PIN_RESET
//三通阀切换到浓缩液的电平      
#define CONCENTRATE_LEVEL_OUTPUT_LIQUID_VALVE_V235              GPIO_PIN_RESET
//三通阀切换到纯水的电平        
#define PURE_WATER_LEVEL_OUTPUT_LIQUID_VALVE_V235               GPIO_PIN_SET

//V240 浓缩液选瓶阀,关闭选通瓶1,打开选通瓶2     
#define OUTPUT_LIQUID_VALVE_V240                                H360_SYS_PORT_OUT_BOARD1_CPLD_CDC17
//打开三通阀的电平      
#define VALID_LEVEL_OUTPUT_LIQUID_VALVE_V240                    GPIO_PIN_RESET
//三通阀切换到瓶1的电平     
#define BOTTLE_1_LEVEL_OUTPUT_LIQUID_VALVE_V240                 GPIO_PIN_SET
//三通阀切换到瓶2的电平     
#define BOTTLE_2_LEVEL_OUTPUT_LIQUID_VALVE_V240                 GPIO_PIN_RESET

//纯水水箱导通阀        
#define OUTPUT_LIQUID_VALVE_V237                                H360_SYS_PORT_OUT_BOARD1_CPLD_CDC14
//打开两通阀的电平      
#define VALID_LEVEL_OUTPUT_LIQUID_VALVE_V237                    GPIO_PIN_RESET

/********************************************液路自动化稀释*****************************************/
//当流量计传感器触发的时候,需要去关闭的IO口
#define PUMP_AUTO_CLOSE_WHILE_SENSOR_TRIG                       BOARD_PORT_OUT_CPLD_CDC3
//关闭状态的值
#define LEVEL_CLOSE_PUMP_AUTO_WHILE_SENSOR_TRIG                 GPIO_PIN_SET
//流量计端口
#define LIQUID_AUTO_SENSOR_PORT                                 BOARD_PORT_IN_ARM_SEN12_PI5


#endif



