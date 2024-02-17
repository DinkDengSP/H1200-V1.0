/************************************************************************ 
 ** Author       : 邓小俊
 ** Date         : 2024-01-16 11:35:28
 ** Description  : When I Has Free Time ,I Will Write Description Below.
 ** description  : 
 ** LastEditors  : 邓小俊
 ** LastEditTime : 2024-01-16 11:37:35
 ** Email: xiaojun.deng@aliyun.com
 ** Copyright (c) 2024 by 公司名称, All Rights Reserved. 
 ************************************************************************/
#ifndef __SRV_IMPL_START_HEART_H_
#define __SRV_IMPL_START_HEART_H_
#include "SrvImplBase.h"

/*--------------------------系统启动初始化相关函数------------------------------*/
//系统设备初始化
void SrvImplStartHeartDeviceInit(void);

//服务层数据初始化
void SrvImplStartHeartCreateSrvData(void);

//服务层任务初始化
void SrvImplStartHeartCreateSrvTask(void);

//模块层数据初始化
void SrvImplStartHeartCreateModuleData(void);

//模块层任务初始化
void SrvImplStartHeartCreateModuleTask(void);

//应用层数据初始化
void SrvImplStartHeartCreateAppData(void);

//应用层任务初始化
void SrvImplStartHeartCreateAppTask(void);

/*--------------------------系统启动状态机相关函数------------------------------*/
//任务全局对象初始化
void SrvImplStartHeartVarInit(void);

//任务接收消息处理
void SrvImplStartHeartMsgProcess(void* taskMsgPtr);

//任务状态机初始化
void SrvImplStartHeartFSM_Init(void);

//任务状态机周期运行
void SrvImplStartHeartFSM_RunPeriod(void);

#endif







