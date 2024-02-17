/************************************************************************ 
 ** Author       : 邓小俊
 ** Date         : 2024-01-31 16:34:18
 ** Description  : When I Has Free Time ,I Will Write Description Below.
 ** description  : 
 ** LastEditors  : 邓小俊
 ** LastEditTime : 2024-02-01 11:27:17
 ** Email: xiaojun.deng@aliyun.com
 ** Copyright (c) 2024 by 公司名称, All Rights Reserved. 
 ************************************************************************/
#ifndef __SRV_IMPL_BASE_START_H_
#define __SRV_IMPL_BASE_START_H_
#include "SrvImplBase.h"

//MCU ID显示
void SrvImplStartHeartShowChipInfo(void);

//RTC初始化
void SrvImplStartHeartRTC_Init(void);

//第三方组件初始化
void SrvImplStartHeartThirdComponentInit(void);

//MB85RS2MT初始化
void SrvImplStartHeartMB85RS2MT_Init(void);

//SDRAM初始化
void SrvImplStartHeartSDRAM_Init(void);

//创建日志任务
void SrvImplStartHeartCreateSysLogTask(void);

//AT24CXX初始化
void SrvImplStartAT24CXX_Init(void);

//写入Srv版本号初始化
void SrvImplStartHeartWriteSrvVersion(void);

//IAP管理
void SrvImplStartHeartIapInit(void);

//LAN8700网卡初始化
void SrvImplStartHeartLAN8700_Init(void);

//W5500初始化
void SrvImplStartHeartW5500_Init(void);

//初始化SDCARD
void SrvImplStartHeartSDCardInit(void);

//启动外部电源
void SrvImplStartHeartPowerOn(void);

#endif











