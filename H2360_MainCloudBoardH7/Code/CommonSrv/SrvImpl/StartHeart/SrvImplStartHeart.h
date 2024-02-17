/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-11-03 16:12:17 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-11-06 08:54:17 +0800
************************************************************************************************/ 
#ifndef __SRV_IMPL_START_HEART_H_
#define __SRV_IMPL_START_HEART_H_
#include "CommonSrvImplBase.h"

/*--------------------------系统启动初始化相关函数------------------------------*/
//系统设备初始化
void SrvImplStartHeartDeviceInit(void);

//公共服务层数据初始化
void SrvImplStartHeartCreateCommonSrvData(void);

//公共服务层任务初始化
void SrvImplStartHeartCreateCommonSrvTask(void);

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
















