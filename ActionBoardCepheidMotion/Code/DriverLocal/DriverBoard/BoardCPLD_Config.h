/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-10-30 10:08:12 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-10-31 18:13:48 +0800
************************************************************************************************/ 
#ifndef __BOARD_CPLD_CONFIG_H_
#define __BOARD_CPLD_CONFIG_H_
#include "DriverHeaderMCU.h"
#include "DriverConfigBoard.h"

//CPLD ID识别码     
#define BOARD_CPLD_ID_CODE                              0XCC55
//CPLD脉冲计数器最大值
#define BOARD_CPLD_MOTOR_FREQ_COUNT_MAX                 0X1FFF//8191
//CPLD脉冲计数器最小值  
#define BOARD_CPLD_MOTOR_FREQ_COUNT_MIN                 0X0001//1
//使能位偏移,左移 SM == STEPMOTOR
#define BOARD_CPLD_SM_ENABLE_BIT_SHIFT                  15
//方向位偏移,左 
#define BOARD_CPLD_SM_DIR_BIT_SHIFT                     14
//停止位偏移,左 
#define BOARD_CPLD_SM_STOP_BIT_SHIFT                    13
//频率位偏移,左 
#define BOARD_CPLD_SM_BIT_FREQ                          0

//CPLD寄存器写入地址
typedef enum BOARD_CPLD_REG_WRITE_ADDR
{
    // 电机中断清除
    BOARD_CPLD_REG_WRITE_MOTOR_INT_CLEAR_L = 0 + BOARD_CPLD_ADDR_BASE,
    BOARD_CPLD_REG_WRITE_MOTOR_INT_CLEAR_H = 1 + BOARD_CPLD_ADDR_BASE,
    // 通用寄存器端口,电机寄存器
    BOARD_CPLD_REG_WRITE_MOTOR_COMMON_REG_L = 2 + BOARD_CPLD_ADDR_BASE,
    // 电机1控制
    BOARD_CPLD_REG_WRITE_MOTOR_1_REG_H = 3 + BOARD_CPLD_ADDR_BASE,
    BOARD_CPLD_REG_WRITE_MOTOR_1_COUNT = 4 + BOARD_CPLD_ADDR_BASE,
    // 电机2控制
    BOARD_CPLD_REG_WRITE_MOTOR_2_REG_H = 5 + BOARD_CPLD_ADDR_BASE,
    BOARD_CPLD_REG_WRITE_MOTOR_2_COUNT = 6 + BOARD_CPLD_ADDR_BASE,
    // 电机3控制
    BOARD_CPLD_REG_WRITE_MOTOR_3_REG_H = 7 + BOARD_CPLD_ADDR_BASE,
    BOARD_CPLD_REG_WRITE_MOTOR_3_COUNT = 8 + BOARD_CPLD_ADDR_BASE,
    // 电机4控制
    BOARD_CPLD_REG_WRITE_MOTOR_4_REG_H = 9 + BOARD_CPLD_ADDR_BASE,
    BOARD_CPLD_REG_WRITE_MOTOR_4_COUNT = 10 + BOARD_CPLD_ADDR_BASE,
    // 电机5控制
    BOARD_CPLD_REG_WRITE_MOTOR_5_REG_H = 11 + BOARD_CPLD_ADDR_BASE,
    BOARD_CPLD_REG_WRITE_MOTOR_5_COUNT = 12 + BOARD_CPLD_ADDR_BASE,
    // 电机6控制
    BOARD_CPLD_REG_WRITE_MOTOR_6_REG_H = 13 + BOARD_CPLD_ADDR_BASE,
    BOARD_CPLD_REG_WRITE_MOTOR_6_COUNT = 14 + BOARD_CPLD_ADDR_BASE,
    // 电机7控制
    BOARD_CPLD_REG_WRITE_MOTOR_7_REG_H = 15 + BOARD_CPLD_ADDR_BASE,
    BOARD_CPLD_REG_WRITE_MOTOR_7_COUNT = 16 + BOARD_CPLD_ADDR_BASE,
    // 电机8控制
    BOARD_CPLD_REG_WRITE_MOTOR_8_REG_H = 17 + BOARD_CPLD_ADDR_BASE,
    BOARD_CPLD_REG_WRITE_MOTOR_8_COUNT = 18 + BOARD_CPLD_ADDR_BASE,
    // 电机9控制
    BOARD_CPLD_REG_WRITE_MOTOR_9_REG_H = 19 + BOARD_CPLD_ADDR_BASE,
    BOARD_CPLD_REG_WRITE_MOTOR_9_COUNT = 20 + BOARD_CPLD_ADDR_BASE,
    // 电机10控制
    BOARD_CPLD_REG_WRITE_MOTOR_10_REG_H = 21 + BOARD_CPLD_ADDR_BASE,
    BOARD_CPLD_REG_WRITE_MOTOR_10_COUNT = 22 + BOARD_CPLD_ADDR_BASE,
    // 电机11控制
    BOARD_CPLD_REG_WRITE_MOTOR_11_REG_H = 23 + BOARD_CPLD_ADDR_BASE,
    BOARD_CPLD_REG_WRITE_MOTOR_11_COUNT = 24 + BOARD_CPLD_ADDR_BASE,
    // 电机12控制
    BOARD_CPLD_REG_WRITE_MOTOR_12_REG_H = 25 + BOARD_CPLD_ADDR_BASE,
    BOARD_CPLD_REG_WRITE_MOTOR_12_COUNT = 26 + BOARD_CPLD_ADDR_BASE,
    // 五个输出端口
    BOARD_CPLD_REG_WRITE_PORT_OUT1 = 27 + BOARD_CPLD_ADDR_BASE,
    BOARD_CPLD_REG_WRITE_PORT_OUT2 = 28 + BOARD_CPLD_ADDR_BASE,
    BOARD_CPLD_REG_WRITE_PORT_OUT3 = 29 + BOARD_CPLD_ADDR_BASE,
    BOARD_CPLD_REG_WRITE_PORT_OUT4 = 30 + BOARD_CPLD_ADDR_BASE,
    BOARD_CPLD_REG_WRITE_PORT_OUT5 = 31 + BOARD_CPLD_ADDR_BASE,
} BOARD_CPLD_REG_WRITE_ADDR;

//CPLD寄存器读出地址
typedef enum BOARD_CPLD_REG_READ_ADDR
{
    //电机中断状态,电机1-8
    BOARD_CPLD_REG_READ_MOTOR_INT_L = 0 + BOARD_CPLD_ADDR_BASE, 
    //电机中断状态 电机9-12
    BOARD_CPLD_REG_READ_MOTOR_INT_H = 1 + BOARD_CPLD_ADDR_BASE,      
    //当前CPLD输入IO口状态                  
    BOARD_CPLD_REG_READ_PORT_IN_0   = 2 + BOARD_CPLD_ADDR_BASE,    
    //当前CPLD输入IO口状态                       
    BOARD_CPLD_REG_READ_PORT_IN_1   = 3 + BOARD_CPLD_ADDR_BASE, 
    //CPLD ID识别码
    BOARD_CPLD_REG_ID_CODE_55       = 4 + BOARD_CPLD_ADDR_BASE, 
    //CPLD ID识别码
    BOARD_CPLD_REG_ID_CODE_CC       = 5 + BOARD_CPLD_ADDR_BASE,                          
} BOARD_CPLD_REG_READ_ADDR;

//电机序号
typedef enum BOARD_CPLD_MOTOR
{
    BOARD_CPLD_MOTOR1   = 0,
    BOARD_CPLD_MOTOR2   = 1,
    BOARD_CPLD_MOTOR3   = 2,
    BOARD_CPLD_MOTOR4   = 3,
    BOARD_CPLD_MOTOR5   = 4,
    BOARD_CPLD_MOTOR6   = 5,
    BOARD_CPLD_MOTOR7   = 6,
    BOARD_CPLD_MOTOR8   = 7,
    BOARD_CPLD_MOTOR9   = 8,
    BOARD_CPLD_MOTOR10  = 9,
    BOARD_CPLD_MOTOR11  = 10,
    BOARD_CPLD_MOTOR12  = 11,
}BOARD_CPLD_MOTOR;

//低电平使能还是高电平使能
#if(BOARD_BOARD_CPLD_MOTOR_ENABLE_REVERSE == CONFIG_FUNC_ENABLE)
//使能引脚
typedef enum BOARD_CPLD_MOTOR_EN
{
    BOARD_CPLD_MOTOR_EN_DISABLE   = 1,
    BOARD_CPLD_MOTOR_EN_ENABLE    = 0,
}BOARD_CPLD_MOTOR_EN;
#else
//使能引脚
typedef enum BOARD_CPLD_MOTOR_EN
{
    BOARD_CPLD_MOTOR_EN_DISABLE   = 0,
    BOARD_CPLD_MOTOR_EN_ENABLE    = 1,
}BOARD_CPLD_MOTOR_EN;
#endif



//CPLD使能位宏定义
typedef enum BOARD_CPLD_MOTOR_STOP
{
    BOARD_CPLD_MOTOR_STOP_ON            = 0,//步进电机急停使能
    BOARD_CPLD_MOTOR_STOP_OFF           = 1,//步进电机急停关闭
}BOARD_CPLD_MOTOR_STOP;

//CPLD电机方向引脚
typedef enum BOARD_CPLD_MOTOR_DIR
{
    BOARD_CPLD_MOTOR_DIR_CCW            = 0,//逆时针
    BOARD_CPLD_MOTOR_DIR_CW             = 1,//顺时针
}BOARD_CPLD_MOTOR_DIR;

//CPLD输出
typedef enum BOARD_CPLD_OUT_PIN
{
    BOARD_CPLD_OUT_PIN_1            = 0,
    BOARD_CPLD_OUT_PIN_2            = 1,
    BOARD_CPLD_OUT_PIN_3            = 2,
    BOARD_CPLD_OUT_PIN_4            = 3,
    BOARD_CPLD_OUT_PIN_5            = 4,
    BOARD_CPLD_OUT_PIN_6            = 5,
    BOARD_CPLD_OUT_PIN_7            = 6,
    BOARD_CPLD_OUT_PIN_8            = 7,
    BOARD_CPLD_OUT_PIN_9            = 8,
    BOARD_CPLD_OUT_PIN_10           = 9,
    BOARD_CPLD_OUT_PIN_11           = 10,
    BOARD_CPLD_OUT_PIN_12           = 11,
    BOARD_CPLD_OUT_PIN_13           = 12,
    BOARD_CPLD_OUT_PIN_14           = 13,
    BOARD_CPLD_OUT_PIN_15           = 14,
    BOARD_CPLD_OUT_PIN_16           = 15,
    BOARD_CPLD_OUT_PIN_17           = 16,
    BOARD_CPLD_OUT_PIN_18           = 17,
    BOARD_CPLD_OUT_PIN_19           = 18,
    BOARD_CPLD_OUT_PIN_20           = 19,
    BOARD_CPLD_OUT_PIN_21           = 20,
    BOARD_CPLD_OUT_PIN_22           = 21,
    BOARD_CPLD_OUT_PIN_23           = 22,
    BOARD_CPLD_OUT_PIN_24           = 23,
    BOARD_CPLD_OUT_PIN_25           = 24,
    BOARD_CPLD_OUT_PIN_26           = 25,
    BOARD_CPLD_OUT_PIN_27           = 26,
    BOARD_CPLD_OUT_PIN_28           = 27,
    BOARD_CPLD_OUT_PIN_29           = 28,
    BOARD_CPLD_OUT_PIN_30           = 29,
    BOARD_CPLD_OUT_PIN_31           = 30,
    BOARD_CPLD_OUT_PIN_32           = 31,
    BOARD_CPLD_OUT_PIN_33           = 32,
    BOARD_CPLD_OUT_PIN_34           = 33,
    BOARD_CPLD_OUT_PIN_35           = 34,
    BOARD_CPLD_OUT_PIN_36           = 35,
}BOARD_CPLD_OUT_PIN;

//CPLD输入
typedef enum BOARD_CPLD_IN_PIN
{
    BOARD_CPLD_IN_PIN_1             = 0,
    BOARD_CPLD_IN_PIN_2             = 1,
    BOARD_CPLD_IN_PIN_3             = 2,
    BOARD_CPLD_IN_PIN_4             = 3,
    BOARD_CPLD_IN_PIN_5             = 4,
    BOARD_CPLD_IN_PIN_6             = 5,
    BOARD_CPLD_IN_PIN_7             = 6,
    BOARD_CPLD_IN_PIN_8             = 7,
    BOARD_CPLD_IN_PIN_9             = 8,
    BOARD_CPLD_IN_PIN_10            = 9,
    BOARD_CPLD_IN_PIN_11            = 10,
    BOARD_CPLD_IN_PIN_12            = 11,
    BOARD_CPLD_IN_PIN_13            = 12,
    BOARD_CPLD_IN_PIN_14            = 13,
}BOARD_CPLD_IN_PIN;

//步进电机缓冲数据类型
typedef struct BOARD_CPLD_MOTOR_TEMP
{
    uint16_t motorReg;
    uint8_t motorCount;
}BOARD_CPLD_MOTOR_TEMP;

//CPLD电机中断函数
typedef void(*BOARD_CPLD_MotorIntProc)(BOARD_CPLD_MOTOR motorNo);



#endif













