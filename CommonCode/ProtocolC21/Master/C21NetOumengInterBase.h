/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-09-04 11:57:26 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-09-21 12:10:14 +0800
************************************************************************************************/ 
#ifndef __C21_NET_OUMENG_INTER_BASE_H_
#define __C21_NET_OUMENG_INTER_BASE_H_
#include "ProtocolC21NetBase.h"
#include "ProtocolC21NetMain.h"
#include "C21NetOumengInterDataType.h"

//C21向欧蒙接口申请试管架
ERROR_SUB C21NetSendCmdC21ApplyRankIn(uint32_t rankCode);

//欧蒙接口响应C21的申请试管
ERROR_SUB C21NetRespCmdC21ApplyRankIn(uint32_t errorCode,uint32_t rankCode);

//欧蒙接口通知C21进架结果
ERROR_SUB C21NetZetaNotifyC21RankInResult(uint32_t errorCode,uint32_t rankCode,C21_NET_RANK_MSG* rankMsgPtr);

//C21回复欧蒙接口进入的架子的取架结果
ERROR_SUB C21NetRespC21RankTakeAwayResultToZeta(uint32_t errorCode);

//C21告知出架子
ERROR_SUB C21NetNotifyZetaRackOut(uint32_t rankCode,C21_NET_RANK_OUT_TYPE rankType,C21_NET_TUBE_STATE_SET* tubeStateSetPtr);

//欧蒙接口告知是否可以出架子
ERROR_SUB C21NetRespZetaRackOut(uint32_t errorCode,uint32_t rankCode);

//欧蒙接口告知出架结果
ERROR_SUB C21NetReportZetaRackOutResult(uint32_t errorCode,uint32_t rankCode);

//C21设备上报设备状态
ERROR_SUB C21NetReportC21DeviceState(C21_NET_DEVICE_STATE deviceState);

//欧蒙接口上报设备状态
ERROR_SUB C21NetReportZetaDeviceState(C21_NET_DEVICE_STATE deviceState);

//欧蒙接口上报心跳
ERROR_SUB C21NetReportZetaHeart(void);

//C21控制欧蒙接口启动校准
ERROR_SUB C21NetSendCmdZetaStartPosAdjust(void);

//欧蒙接口反馈启动校准执行结果
ERROR_SUB C21NetRespCmdZetaStartPosAdjustResult(uint32_t errorCode);

//C21控制欧蒙接口执行到指定校准位置
ERROR_SUB C21NetSendCmdZetaPosAdjustNext(int32_t targetCoordinate);

//欧蒙接口反馈校准位置执行结果
ERROR_SUB C21NetRespCmdZetaPosAdjustNext(uint32_t errorCode);

//C21控制欧蒙接口完成校准并写入参数
ERROR_SUB C21NetSendCmdZetaPosAdjustOverWithParam(uint8_t pos,int32_t adjustPosOver);

//欧蒙接口反馈校准完成指令的执行结果
ERROR_SUB C21NetRespCmdZetaPosAdjustOverWithParam(uint32_t errorCode);

//C21控制欧蒙接口解除故障
ERROR_SUB C21NetSendCmdZetaTooubleShooting(uint8_t troubleLevel,uint32_t errorCode);

//欧蒙接口反馈解除故障执行结果
ERROR_SUB C21NetRespCmdZetaTooubleShooting(uint32_t errorCode);

//欧蒙接口主动故障上报
ERROR_SUB C21NetOumengInterReportErrorToC21(uint8_t troubleLevel,uint32_t errorCode);
//通用上报
ERROR_SUB C21NetOumengInterReport(uint16_t cmdCode,uint8_t* dataBody,uint16_t dataBodyLength);
#endif







