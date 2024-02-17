/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-08-23 09:44:56 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-10-31 18:59:00 +0800
************************************************************************************************/ 
#ifndef __BOARD_PARAM_CONFIG_H_
#define __BOARD_PARAM_CONFIG_H_
#include "DriverHeaderMCU.h"

/*--------------------------板卡参数统一四个字节,存放在外部存储芯片中----------------------------*/
//BOOT版本号       
#define ADDR_BOARD_PARAM_BOOT_VERSION                   0X00000000
//SRV版本号        
#define ADDR_BOARD_PARAM_SRV_VERSION                    0X00000010
//APP版本号        
#define ADDR_BOARD_PARAM_APP_VERSION                    0X00000020

//AT24CXX自检标记
#define ADDR_BOARD_PARAM_AT24CXX_CHECK                  0x00000030

//FRAM FLASH低格标记
#define ADDR_BOARD_PARAM_FRAM_FORMAT                    0X00000040
//FRAM 自检标记      
#define ADDR_BOARD_PARAM_FRAM_CHECK                     0X00000044

//外部存储器 FLASH低格标记
#define ADDR_BOARD_PARAM_EX_FLASH_FORMAT                0X00000050
//外部存储器 自检标记      
#define ADDR_BOARD_PARAM_EX_FLASH_CHECK                 0X00000054

//外部RAM自检标记      
#define ADDR_BOARD_PARAM_EXRAM_CHECK                    0X00000060

//PHY1 IP自检标记
#define ADDR_BOARD_PARAM_NET_PHY1_IP_CHECK              0X00000070
//PHY1 IP存储
#define ADDR_BOARD_PARAM_NET_PHY1_IP_ADDR               0X00000074

//PHY2 IP自检标记
#define ADDR_BOARD_PARAM_NET_PHY2_IP_CHECK              0X00000080
//PHY2 IP存储
#define ADDR_BOARD_PARAM_NET_PHY2_IP_ADDR               0X00000084

//IAP相关标记的起始地址
#define ADDR_BOARD_IAP_PARAM_START                      0X000000C0

//电机配置的起始地址,每个电机8K存储空间,12个电机,占用96K
#define STEP_MOTOR_PARAM1_ADDR                          0X00002000
//电机配置的起始地址 8K                 
#define STEP_MOTOR_PARAM2_ADDR                          0X00004000
//电机配置的起始地址 8K                 
#define STEP_MOTOR_PARAM3_ADDR                          0X00006000
//电机配置的起始地址 8K                 
#define STEP_MOTOR_PARAM4_ADDR                          0X00008000
//电机配置的起始地址 8K                 
#define STEP_MOTOR_PARAM5_ADDR                          0X0000A000
//电机配置的起始地址 8K                 
#define STEP_MOTOR_PARAM6_ADDR                          0X0000C000
//电机配置的起始地址 8K                 
#define STEP_MOTOR_PARAM7_ADDR                          0X0000E000
//电机配置的起始地址 8K                 
#define STEP_MOTOR_PARAM8_ADDR                          0X00010000
//电机配置的起始地址 8K                 
#define STEP_MOTOR_PARAM9_ADDR                          0X00012000
//电机配置的起始地址 8K                 
#define STEP_MOTOR_PARAM10_ADDR                         0X00014000
//电机配置的起始地址 8K  
#define STEP_MOTOR_PARAM11_ADDR                         0X00016000
//电机配置的起始地址 8K                 
#define STEP_MOTOR_PARAM12_ADDR                         0X00018000
//步进电机配置尾段 8K               
#define STEP_MOTOR_PARAM_END_ADDR                       0X0001A000

//服务层参数的起始地址
#define ADDR_BOARD_SYS_PARAM_START                      STEP_MOTOR_PARAM_END_ADDR
//服务层参数的最大长度,64KB
#define LENGTH_MAX_BOARD_SYS_PARAM                      0X00010000

//应用层参数的起始地址
#define ADDR_BOARD_APP_PARAM_START                      (ADDR_BOARD_SYS_PARAM_START + LENGTH_MAX_BOARD_SYS_PARAM)
//应用层参数的最大长度,64KB
#define LENGTH_MAX_BOARD_APP_PARAM                      0X00010000

/*-------------------------------默认数据标记-----------------------------------------*/
//默认IP
#define DEFAULT_VALUE_BOARD_PARAM_NET_PHY1_IP           0X0A1FA8C0
//默认IP
#define DEFAULT_VALUE_BOARD_PARAM_NET_PHY2_IP           0X0B1FA8C0

/*------------------------------------数据标记--------------------------------------------------*/
//AT24CXX自检OK标记
#define BOARD_AT24CXX_CHECK_OK_FLAG                     0XAA55AA55
//FRAM低格标记             
#define BOARD_FRAM_FORMAT_OK_FLAG                       0XAA55AA5A
//FRAM自检标记               
#define BOARD_FRAM_CHECK_OK_FLAG                        0XAA55AAA5
//外部存储器低格标记             
#define BOARD_EX_FLASH_FORMAT_OK_FLAG                   0XAA55A555
//外部存储器自检标记               
#define BOARD_EX_FLASH_CHECK_OK_FLAG                    0XAA555A55
//EXRAM自检OK标记               
#define BOARD_EXRAM_CHECK_OK_FLAG                       0XAA5AAA55
//IP设置标记
#define BOARD_NET_PHY1_IP_CHECK_OK_FLAG                 0XAAA5AA55
//IP设置标记
#define BOARD_NET_PHY2_IP_CHECK_OK_FLAG                 0XA555AA55

#endif










