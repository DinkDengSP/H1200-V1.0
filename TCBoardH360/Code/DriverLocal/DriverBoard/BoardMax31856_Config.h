/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-11-13 11:21:37 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-11-13 15:08:27 +0800
************************************************************************************************/ 
#ifndef __BOARD_MAX_31856_CONFIG_H_
#define __BOARD_MAX_31856_CONFIG_H_
#include "DriverHeaderMCU.h"

//芯片的内部地址
typedef enum REG_ADDR_MAX31856
{
    REG_ADDR_MAX31856_CR0       = 0X00,//配置0
    REG_ADDR_MAX31856_CR1       = 0X01,//配置1
    REG_ADDR_MAX31856_MASK      = 0X02,//故障屏蔽
    REG_ADDR_MAX31856_CJHF      = 0X03,//冷端故障上限设置
    REG_ADDR_MAX31856_CJLF      = 0X04,//冷端故障下限设置
    REG_ADDR_MAX31856_LTHFTH    = 0X05,//热端温度故障上限高字节
    REG_ADDR_MAX31856_LTHFTL    = 0X06,//热端温度故障上限低字节
    REG_ADDR_MAX31856_LTLFTH    = 0X07,//热端温度故障下限高字节
    REG_ADDR_MAX31856_LTLFTL    = 0X08,//热端温度故障下限低字节
    REG_ADDR_MAX31856_CJTO      = 0X09,//冷端温度偏移校正
    REG_ADDR_MAX31856_CJTH      = 0X0A,//冷端温度高字节
    REG_ADDR_MAX31856_CJTL      = 0X0B,//冷端温度低字节
    REG_ADDR_MAX31856_LTCBH     = 0X0C,//热端温度字节H
    REG_ADDR_MAX31856_LTCBM     = 0X0D,//热端温度字节M
    REG_ADDR_MAX31856_LTCBL     = 0X0E,//热端温度字节L
    REG_ADDR_MAX31856_SR        = 0X0F,//故障状态
}REG_ADDR_MAX31856;

//芯片的配置
typedef struct CONFIG_MAX31856
{
    uint8_t configWithCR0;//配置0
    uint8_t configWithCR1;//配置1
    uint8_t faultMask;//故障屏蔽,也就是发生故障在故障引脚显示
    int8_t coolHighLimit;//冷端上限
    int8_t coolLowLimit; //冷端下限
    float hotHighLimit;//热端温度上限
    float hotLowLimit;//热端温度下限
}CONFIG_MAX31856;

//MAX31856通道
typedef enum CH_MAX31856
{
    CH_MAX31856_1   = 0X00,
    CH_MAX31856_2   = 0X01,
    CH_MAX31856_3   = 0X02,
    CH_MAX31856_4   = 0X03,
}CH_MAX31856;

//冷端转换分辨率计算系数,也就是芯片最小的那个计算单位代表的系数
#define MAX31856_COLD_JUNCTION_RESOLUTION           0.015625f
//热端转换分辨率计算系数,也就是芯片最小的那个计算单位代表的系数
#define MAX31856_TC_RESOLUTION                      0.0078125f 
//上下限分辨率转换
#define MAX31856_TC_LIMIT_RESOLUTION                16
//冷端温度分辨率
#define MAX31856_COOL_TEMP_RESOLUTION               64

//出错
#define FAULT_MAX31856_CJ_RANGE             (uint8_t)0x80//热电偶冷端温度超范围
#define FAULT_MAX31856_TC_RANGE             (uint8_t)0x40//热电偶热端温度超范围
#define FAULT_MAX31856_CJ_HIGH              (uint8_t)0x20//热电偶冷端到达温度上限
#define FAULT_MAX31856_CJ_LOW               (uint8_t)0x10//热电偶冷端到达温度下限
#define FAULT_MAX31856_TC_HIGH              (uint8_t)0x08//热电偶热端到达温度上限
#define FAULT_MAX31856_TC_LOW               (uint8_t)0x04//热电偶热端到达温度下限
#define FAULT_MAX31856_OV_UV                (uint8_t)0x02//热电偶过压或者欠压,一般是短路
#define FAULT_MAX31856_OPEN                 (uint8_t)0x01//热电偶开路,线路不通
#define FAULT_MAX31856_NO                   (uint8_t)0x00//无故障 


#endif



