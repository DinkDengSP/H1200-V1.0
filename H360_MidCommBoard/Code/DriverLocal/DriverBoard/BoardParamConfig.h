/************************************************************************ 
 ** Author       : 邓小俊
 ** Date         : 2024-01-29 11:51:45
 ** Description  : When I Has Free Time ,I Will Write Description Below.
 ** description  : 
 ** LastEditors  : 邓小俊
 ** LastEditTime : 2024-01-29 14:59:41
 ** Email: xiaojun.deng@aliyun.com
 ** Copyright (c) 2024 by 公司名称, All Rights Reserved. 
 ************************************************************************/
#ifndef __BOARD_PARAM_CONFIG_H_
#define __BOARD_PARAM_CONFIG_H_
#include "DriverHeaderMCU.h"

/*--------------------------板卡参数统一四个字节,存放在外部存储芯片中----------------------------*/
//BOOT版本号,开发版本       
#define ADDR_BOARD_PARAM_BOOT_VERSION                   0X00000000
//SERVICE版本号,开发版本        
#define ADDR_BOARD_PARAM_SERVICE_VERSION                0X00000010
//APP版本号,开发版本        
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

/*--------------------------板卡参数统一四个字节,存放在外部存储芯片中----------------------------*/
//服务层参数的起始地址
#define ADDR_BOARD_SRV_PARAM_START                      0X00010000
//服务层参数的最大长度,64KB
#define LENGTH_MAX_BOARD_SRV_PARAM                      0X00010000

//应用层参数的起始地址
#define ADDR_BOARD_APP_PARAM_START                      (ADDR_BOARD_SRV_PARAM_START + LENGTH_MAX_BOARD_SRV_PARAM)
//应用层参数的最大长度,64KB
#define LENGTH_MAX_BOARD_APP_PARAM                      0X00010000


#endif















