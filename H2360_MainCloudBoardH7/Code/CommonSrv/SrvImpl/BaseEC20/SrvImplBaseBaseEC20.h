/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-11-03 16:12:17 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-11-08 17:22:18 +0800
************************************************************************************************/ 
#ifndef __SRV_IMPL_BASE_BASE_EC20_H_
#define __SRV_IMPL_BASE_BASE_EC20_H_
#include "CommonSrvImplBase.h"

//PING 服务器测试
void SrvImplBaseBaseEC20Ping(SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);

//获取模块厂商信息
void SrvImplBaseBaseEC20QueryManufacturerName(SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);

//获取模块类型信息
void SrvImplBaseBaseEC20QueryModuleType(SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);

//获取模块软件版本信息
void SrvImplBaseBaseEC20QuerySoftwareVersion(SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);

//获取模块入网许可
void SrvImplBaseBaseEC20QueryModuleIMEI(SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);

//获取SIM卡IMSI串号
void SrvImplBaseBaseEC20QuerySimIMSI(SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);

//获取SIM卡ICCID识别号
void SrvImplBaseBaseEC20QuerySimICCID(SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);

//获取当前模块连接网络全名
void SrvImplBaseBaseEC20QueryNetFullName(SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);

//获取当前模块连接网络短名
void SrvImplBaseBaseEC20QueryNetShortName(SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);

//获取网络服务商名称
void SrvImplBaseBaseEC20QueryNetProvide(SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);

//获取当前网络制式
void SrvImplBaseBaseEC20QueryNetType(SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);

//获取当前网络波段
void SrvImplBaseBaseEC20QueryNetBaud(SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);

//获取LAC基站区域码
void SrvImplBaseBaseEC20QueryBaseStationLAC(SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);

//获取CID基站编号
void SrvImplBaseBaseEC20QueryBaseStationCID(SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);

//获取当前信号强度与误码率
void SrvImplBaseBaseEC20QuerySignalStrength(SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);

//查看当前模块网络是否正常
void SrvImplBaseBaseEC20QueryNetState(SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);

//通讯设备重启
void SrvImplBaseBaseEC20ModuleRestart(SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);



#endif

















