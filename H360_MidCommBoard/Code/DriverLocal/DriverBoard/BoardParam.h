/************************************************************************ 
 ** Author       : 邓小俊
 ** Date         : 2024-01-29 11:51:45
 ** Description  : When I Has Free Time ,I Will Write Description Below.
 ** description  : 
 ** LastEditors  : 邓小俊
 ** LastEditTime : 2024-01-29 13:08:48
 ** Email: xiaojun.deng@aliyun.com
 ** Copyright (c) 2024 by 公司名称, All Rights Reserved. 
 ************************************************************************/
#ifndef __BOARD_PARAM_H_
#define __BOARD_PARAM_H_
#include "BoardParamConfig.h"

//参数单次写入
ERROR_SUB BoardParamWrireSingle(uint8_t addr,uint32_t val);
//参数单个读取
ERROR_SUB BoardParamReadSingle(uint8_t addr,uint32_t* valPtr);

//读取BOOT版本号
ERROR_SUB BoardParamReadBootVersion(BOARD_SOFT_VERSION* versionDatPtr);
//写入BOOT版本号
ERROR_SUB BoardParamWriteBootVersion(void);

//读取SERVICE版本号
ERROR_SUB BoardParamReadServiceVersion(BOARD_SOFT_VERSION* versionDatPtr);
//写入SERVICE版本号
ERROR_SUB BoardParamWriteServiceVersion(void);

//读取APP版本号
ERROR_SUB BoardParamReadAppVersion(BOARD_SOFT_VERSION* versionDatPtr);
//写入APP版本号
ERROR_SUB BoardParamWriteAppVersion(void);

//检查AT24CXX自检成功标记
ErrorStatus BoardParamCheckFlagAT24CXX(void);
//设置AT24CXX自检成功标记
ERROR_SUB BoardParamSetCheckFlagAT24CXX(ErrorStatus set);

//检查FRAM低级格式化标记
ErrorStatus BoardParamCheckFormatFlagFRAM(void);
//设置FRAM低级格式化标记
ERROR_SUB BoardParamSetCheckFormatFlagFRAM(ErrorStatus set);

//检查FRAM自检成功标记
ErrorStatus BoardParamCheckFlagFRAM(void);
//设置FRAM自检成功标记
ERROR_SUB BoardParamSetCheckFlagFRAM(ErrorStatus set);

//检查EX_FLASH低级格式化标记
ErrorStatus BoardParamCheckExFlashFormatFlag(void);
//设置EX_FLASH低级格式化标记
ERROR_SUB BoardParamSetCheckExFlashFormatFlag(ErrorStatus set);

//检查EX_FLASH自检成功标记
ErrorStatus BoardParamCheckExFlashFlag(void);
//设置EX_FLASH自检成功标记
ERROR_SUB BoardParamSetCheckExFlashFlag(ErrorStatus set);

//检查EXRAM自检成功标记
ErrorStatus BoardParamCheckExRamFlag(void);
//设置EXRAM自检成功标记
ERROR_SUB BoardParamSetCheckExRamFlag(ErrorStatus set);

//检查NET PHY1 IP检查标记
ErrorStatus BoardParamCheckNetPhy1IpFlag(void);
//设置NET PHY1 IP检查标记
ERROR_SUB BoardParamSetCheckNetPhy1IpFlag(ErrorStatus set);
//写入NET PHY1 IP值
ERROR_SUB BoardParamSetNetPhy1Ip(uint32_t ipAddr);
//写入NET_PHY1默认IP值
ERROR_SUB BoardParamSetNetPhy1DefaultIp(void);
//读取NET PHY1 IP值
ERROR_SUB BoardParamReadNetPhy1IpCurrent(uint32_t* ipValue);


//检查NET PHY2 IP检查标记
ErrorStatus BoardParamCheckNetPhy2IpFlag(void);
//设置NET PHY2 IP检查标记
ERROR_SUB BoardParamSetCheckNetPhy2IpFlag(ErrorStatus set);
//写入NET PHY2 IP值
ERROR_SUB BoardParamSetNetPhy2Ip(uint32_t ipAddr);
//写入NET_PHY2默认IP值
ERROR_SUB BoardParamSetNetPhy2DefaultIp(void);
//读取NET PHY2 IP值
ERROR_SUB BoardParamReadNetPhy2IpCurrent(uint32_t* ipValue);


#endif








