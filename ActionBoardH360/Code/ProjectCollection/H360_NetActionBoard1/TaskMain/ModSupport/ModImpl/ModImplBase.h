/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-10-27 17:08:07 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-12-04 20:46:36 +0800
************************************************************************************************/ 
#ifndef __MOD_IMPL_BASE_H_
#define __MOD_IMPL_BASE_H_
#include "SrvImplHeader.h"
#include "ModTaskHeader.h"
#include "IPC_ModHeader.h"

//清洗针灌注配置
typedef enum NEEDLE_WASH_PRIME_OPT
{
    NEEDLE_WASH_PRIME_OPT_CLEAN         = 0X00,//清洗液灌注
    NEEDLE_WASH_PRIME_OPT_PURE_WATER    = 0X01,//纯水灌注
}NEEDLE_WASH_PRIME_OPT;

/************************************************通用延时*****************************************/
//泵操作等待稳定时间
#define TIME_MS_WAIT_STABLE_VALVE                           30
//阀操作等待稳定时间
#define TIME_MS_WAIT_STABLE_PUMP                            30
//开抽废液一次的时间,正常模式,100MS的倍数
#define TIMES_100MS_LIQUID_WASTE_OPEN_NORMAL                300//300表示30S
//关抽废液一次的时间,100MS的倍数
#define TIMES_100MS_LIQUID_WASTE_CLOSE                      600//600表示60S

/*************************************A B 清洗针公用***************************************************/
//清洗盘,A液 B液排放泵,DP2
#define OUTPUT_LIQUID_PUMP_DP2                              H360_SYS_PORT_OUT_BOARD1_SLAVE_SEN23_PF8
//打开DP2的电平
#define VALID_LEVEL_OUTPUT_LIQUID_PUMP_DP2                  GPIO_PIN_RESET

/***********************************************缓冲液注液*********************************************/
//缓冲液选瓶V201
#define OUTPUT_LIQUID_VALVE_V201                            H360_SYS_PORT_OUT_BOARD1_CPLD_CDC13
//打开三通阀的电平      
#define VALID_LEVEL_OUTPUT_LIQUID_VALVE_V201                GPIO_PIN_RESET
//三通阀切换到1瓶的电平     
#define BOTTLE_1_OUTPUT_LIQUID_VALVE_V201                   GPIO_PIN_SET
//三通阀切换到2瓶的电平     
#define BOTTLE_2_OUTPUT_LIQUID_VALVE_V201                   GPIO_PIN_RESET

//缓冲液选通V202
#define OUTPUT_LIQUID_VALVE_V202                            H360_SYS_PORT_OUT_BOARD1_CPLD_CDC12
//打开两通阀的电平      
#define VALID_LEVEL_OUTPUT_LIQUID_VALVE_V202                GPIO_PIN_RESET

//缓冲液注液选通V300
#define OUTPUT_LIQUID_VALVE_V300                            H360_SYS_PORT_OUT_BOARD1_CPLD_CDC23
//打开两通阀的电平      
#define VALID_LEVEL_OUTPUT_LIQUID_VALVE_V300                GPIO_PIN_RESET

/***********************************************激发液注液*********************************************/
//激发液选瓶V203
#define OUTPUT_LIQUID_VALVE_V203                            H360_SYS_PORT_OUT_BOARD1_CPLD_CDC11
//打开三通阀的电平      
#define VALID_LEVEL_OUTPUT_LIQUID_VALVE_V203                GPIO_PIN_RESET
//三通阀切换到1瓶的电平     
#define BOTTLE_1_OUTPUT_LIQUID_VALVE_V203                   GPIO_PIN_SET
//三通阀切换到2瓶的电平     
#define BOTTLE_2_OUTPUT_LIQUID_VALVE_V203                   GPIO_PIN_RESET

//激发液选通V204
#define OUTPUT_LIQUID_VALVE_V204                            H360_SYS_PORT_OUT_BOARD1_CPLD_CDC10
//打开两通阀的电平      
#define VALID_LEVEL_OUTPUT_LIQUID_VALVE_V204                GPIO_PIN_RESET

//激发液纯水维护选通纯水        
#define OUTPUT_LIQUID_VALVE_V308                            H360_SYS_PORT_OUT_BOARD1_CPLD_CDC16
//打开两通阀的电平      
#define VALID_LEVEL_OUTPUT_LIQUID_VALVE_V308                GPIO_PIN_RESET

//激发液注液选通        
#define OUTPUT_LIQUID_VALVE_V215                            H360_SYS_PORT_OUT_BOARD1_CPLD_CDC19
//打开两通阀的电平      
#define VALID_LEVEL_OUTPUT_LIQUID_VALVE_V215                GPIO_PIN_RESET

/*************************************************清洗针与三针共用泵阀*******************************/
//内外针清洗液泵
#define OUTPUT_LIQUID_PUMP_DP306                            H360_SYS_PORT_OUT_BOARD1_CPLD_CDC26
#define VALID_LEVEL_OUTPUT_LIQUID_PUMP_DP306                GPIO_PIN_RESET

//内外针清洗针纯水泵        
#define OUTPUT_LIQUID_PUMP_DP305                            H360_SYS_PORT_OUT_BOARD1_CPLD_CDC27
#define VALID_LEVEL_OUTPUT_LIQUID_PUMP_DP305                GPIO_PIN_RESET

//三针内针三通阀,关闭切换清洗液,打开切换纯水        
#define OUTPUT_LIQUID_VALVE_V309                            H360_SYS_PORT_OUT_BOARD1_CPLD_CDC18
//打开两通阀的电平      
#define VALID_LEVEL_OUTPUT_LIQUID_VALVE_V309                GPIO_PIN_RESET

//三针外针三通阀,关闭切换纯水,打开切换清洗液        
#define OUTPUT_LIQUID_VALVE_V232                            H360_SYS_PORT_OUT_BOARD1_CPLD_CDC5
//打开两通阀的电平      
#define VALID_LEVEL_OUTPUT_LIQUID_VALVE_V232                GPIO_PIN_RESET

/***************************************************清洗针*****************************************/
//清洗针清洗液注液针1,吸液两通阀 V301
#define OUTPUT_LIQUID_VALVE_V301                            H360_SYS_PORT_OUT_BOARD1_CPLD_CDC33
//打开两通阀的电平      
#define VALID_LEVEL_OUTPUT_LIQUID_VALVE_V301                GPIO_PIN_RESET

//清洗针清洗液注液针2,吸液两通阀 V302       
#define OUTPUT_LIQUID_VALVE_V302                            H360_SYS_PORT_OUT_BOARD1_CPLD_CDC32
//打开两通阀的电平      
#define VALID_LEVEL_OUTPUT_LIQUID_VALVE_V302                GPIO_PIN_RESET

//清洗针清洗液注液针3,吸液两通阀 V303       
#define OUTPUT_LIQUID_VALVE_V303                            H360_SYS_PORT_OUT_BOARD1_CPLD_CDC31
//打开两通阀的电平      
#define VALID_LEVEL_OUTPUT_LIQUID_VALVE_V303                GPIO_PIN_RESET

//清洗针清洗液注液针4,吸液两通阀 V304       
#define OUTPUT_LIQUID_VALVE_V304                            H360_SYS_PORT_OUT_BOARD1_CPLD_CDC30
//打开两通阀的电平      
#define VALID_LEVEL_OUTPUT_LIQUID_VALVE_V304                GPIO_PIN_RESET

//清洗针清洗液注液针5,吸液两通阀 V305       
#define OUTPUT_LIQUID_VALVE_V305                            H360_SYS_PORT_OUT_BOARD1_CPLD_CDC29
//打开两通阀的电平      
#define VALID_LEVEL_OUTPUT_LIQUID_VALVE_V305                GPIO_PIN_RESET 

//清洗针清洗液选通阀 V310,打开导通清洗液        
#define OUTPUT_LIQUID_VALVE_V310                            H360_SYS_PORT_OUT_BOARD1_CPLD_CDC28
//打开两通阀的电平      
#define VALID_LEVEL_OUTPUT_LIQUID_VALVE_V310                GPIO_PIN_RESET

//清洗针纯水电磁阀 V233,打开导通纯水        
#define OUTPUT_LIQUID_VALVE_V233                            H360_SYS_PORT_OUT_BOARD1_CPLD_CDC6
//打开两通阀的电平      
#define VALID_LEVEL_OUTPUT_LIQUID_VALVE_V233                GPIO_PIN_RESET

/***************************************************样本针*****************************************/
//样本针内针清洗阀
#define OUTPUT_LIQUID_VALVE_V221                            H360_SYS_PORT_OUT_BOARD1_CPLD_CDC34
//打开两通阀的电平  
#define VALID_LEVEL_OUTPUT_LIQUID_VALVE_V221                GPIO_PIN_RESET
    
//样本针外壁清洗阀  
#define OUTPUT_LIQUID_VALVE_V218                            H360_SYS_PORT_OUT_BOARD1_CPLD_CDC22
//打开两通阀的电平  
#define VALID_LEVEL_OUTPUT_LIQUID_VALVE_V218                GPIO_PIN_RESET

/***************************************************磁珠针*****************************************/
//磁珠针内壁清洗阀
#define OUTPUT_LIQUID_VALVE_V220                            H360_SYS_PORT_OUT_BOARD1_CPLD_CDC35
//打开两通阀的电平  
#define VALID_LEVEL_OUTPUT_LIQUID_VALVE_V220                GPIO_PIN_RESET

//磁珠针外壁清洗阀  
#define OUTPUT_LIQUID_VALVE_V217                            H360_SYS_PORT_OUT_BOARD1_CPLD_CDC21
//打开两通阀的电平  
#define VALID_LEVEL_OUTPUT_LIQUID_VALVE_V217                GPIO_PIN_RESET
/***************************************************试剂针*****************************************/
//试剂针内壁清洗阀
#define OUTPUT_LIQUID_VALVE_V219                            H360_SYS_PORT_OUT_BOARD1_CPLD_CDC36
//打开两通阀的电平  
#define VALID_LEVEL_OUTPUT_LIQUID_VALVE_V219                GPIO_PIN_RESET

//试剂针外壁清洗阀  
#define OUTPUT_LIQUID_VALVE_V216                            H360_SYS_PORT_OUT_BOARD1_CPLD_CDC20
//打开两通阀的电平  
#define VALID_LEVEL_OUTPUT_LIQUID_VALVE_V216                GPIO_PIN_RESET


//等待泵操作稳定
void ModLiquidUtilWaitPumpStable(void);
//等待阀操作稳定
void ModLiquidUtilWaitValveStable(void);


/***********************************A液 B液 清洗针公用*******************************/
//打开DP2,清洗盘,A液 B液排放泵,DP2
void ModLiquidUtilOpenPumpDP2(void);
//关闭DP2
void ModLiquidUtilClosePumpDP2(void);

/**********************************缓冲液使用************************************/
//关闭V201,缓冲液选瓶
void ModLiquidUtilCloseValveV201(void);
//选择V201,1瓶
void ModLiquidUtilValveV201SelectBottle1(void);
//选择V201,2瓶
void ModLiquidUtilValveV201SelectBottle2(void);

//关闭V202,A液关闭注液,防止回流
void ModLiquidUtilCloseValveV202(void);
//打开V202,A液准备注液
void ModLiquidUtilOpenValveV202(void);

//关闭V300,A液关闭注液
void ModLiquidUtilCloseValveV300(void);
//打开V300,A液准备注液
void ModLiquidUtilOpenValveV300(void);

/**********************************激发液使用************************************/
//打开V203,激发液选通
void ModLiquidUtilOpenValveV203(void);
//关闭V203
void ModLiquidUtilCloseValveV203(void);
//选择V203,1瓶
void ModLiquidUtilValveV203SelectBottle1(void);
//选择V203,2瓶
void ModLiquidUtilValveV203SelectBottle2(void);

//关闭V204,B液关闭注液,防止回流
void ModLiquidUtilCloseValveV204(void);
//打开V204,B液准备注液
void ModLiquidUtilOpenValveV204(void);

//打开V308,纯水维护三通
void ModLiquidUtilOpenValveV308(void);
//关闭V308
void ModLiquidUtilCloseValveV308(void);

//打开V215
void ModLiquidUtilOpenValveV215(void);
//关闭V215
void ModLiquidUtilCloseValveV215(void);

/**********************************************三针与清洗针公用***********************************************/
//打开DP305,内外针清洗针纯水泵
void ModLiquidUtilOpenPumpDP305(void);
//关闭DP305,内外针清洗针纯水泵
void ModLiquidUtilClosePumpDP305(void);

//打开DP306,内外针清洗液泵
void ModLiquidUtilOpenPumpDP306(void);
//关闭DP306,内外针清洗液泵
void ModLiquidUtilClosePumpDP306(void);

//关闭V309,三针内针三通阀
void ModLiquidUtilCloseValveV309(void);
//切换到正常运行状态,阀和清洗液导通
void ModLiquidUtilValveV309SwitchLiquidClean(void);
//切换到清洗维护状态,阀和纯水导通
void ModLiquidUtilValveV309SwitchLiquidPureWater(void);

//关闭V232,三针外针三通阀
void ModLiquidUtilCloseValveV232(void);
//切换到正常运行状态,阀和纯水导通
void ModLiquidUtilValveV232SwitchPureWater(void);
//切换到清洗维护状态,阀和清洗液导通
void ModLiquidUtilValveV232SwitchLiquidClean(void);

/*******************************清洗针使用*************************************/
//关闭V301,针准备吸液
void ModLiquidUtilCloseValveV301(void);
//打开V301,针准备排液
void ModLiquidUtilOpenValveV301(void);

//关闭V302,针准备吸液
void ModLiquidUtilCloseValveV302(void);
//打开V302,针准备排液
void ModLiquidUtilOpenValveV302(void);

//关闭V303,针准备吸液
void ModLiquidUtilCloseValveV303(void);
//打开V303,针准备排液
void ModLiquidUtilOpenValveV303(void);

//关闭V304,针准备吸液
void ModLiquidUtilCloseValveV304(void);
//打开V304,针准备排液
void ModLiquidUtilOpenValveV304(void);

//关闭V305,针准备吸液
void ModLiquidUtilCloseValveV305(void);
//打开V305,针准备排液
void ModLiquidUtilOpenValveV305(void);

//关闭V310,清洗针关闭清洗液导通
void ModLiquidUtilCloseValveV310(void);
//打开V310,清洗针导通清洗液
void ModLiquidUtilValveV310SwitchLiquidClean(void);

//关闭V233,清洗针关闭纯水导通
void ModLiquidUtilCloseValveV233(void);
//打开V233,清洗针导通纯水
void ModLiquidUtilValveV233SwitchPureWater(void);

/*****************************************样本针****************************/
//打开V218,样本针外壁清洗阀
void ModLiquidUtilOpenValveV218(void);
//关闭V218
void ModLiquidUtilCloseValveV218(void);

//打开V221,样本针内针清洗阀
void ModLiquidUtilOpenValveV221(void);
//关闭V221
void ModLiquidUtilCloseValveV221(void);


/***************************************************磁珠针*****************************************/
//打开V220,磁珠针内壁清洗阀
void ModLiquidUtilOpenValveV220(void);
//关闭V220
void ModLiquidUtilCloseValveV220(void);

//打开V217,磁珠针外壁清洗阀
void ModLiquidUtilOpenValveV217(void);
//关闭V217
void ModLiquidUtilCloseValveV217(void);

/***************************************************试剂针*****************************************/
//打开V219,磁珠针内壁清洗阀
void ModLiquidUtilOpenValveV219(void);
//关闭V219
void ModLiquidUtilCloseValveV219(void);

//打开V216,磁珠针外壁清洗阀
void ModLiquidUtilOpenValveV216(void);
//关闭V216
void ModLiquidUtilCloseValveV216(void);

#endif

































