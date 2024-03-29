/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-02-24 17:29:31 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-12-08 09:44:33 +0800
************************************************************************************************/ 
#ifndef __BOARD_LIQUID_DETECT_H_
#define __BOARD_LIQUID_DETECT_H_
#include "BoardLiquidDetectConfig.h"

//初始化端口
void BoardLiquidDetectPortInit(BoardLiquidDetectAutoUploadPackProc funcPtr);

//检测设备是否存在
ERROR_SUB BoardLiquidDetectConfigDefault(void);

//获取液面探测的指定参数信息
ERROR_SUB BoardLiquidDetectGetParam(uint8_t paramNo,BOARD_LIQUID_DETECT_PARAM* paramPtr);

//获取液面探测配置讯息
ERROR_SUB BoardLiquidDetectGetConfig(BOARD_LIQUID_DETECT_CONFIG** configPtrPtr);

//获取探液板当前状态
ERROR_SUB BoardLiquidDetectGetState(BOARD_LIQUID_DETECT_STATE* statePtr);

//获取探液板触发讯息
ERROR_SUB BoardLiquidDetectTrigMsg(BOARD_LIQUID_DETECT_TRIG_MSG* trigMsgPtr);

//读取基准电阻值
ERROR_SUB BoardLiquidDetectReadBaseRes(uint16_t* resValPtr);

//写入基准电阻值
ERROR_SUB BoardLiquidDetectWriteBaseRes(uint16_t resVal);

//关闭液面探测
ERROR_SUB BoardLiquidDetectStop(void);

//打开液面检测
ERROR_SUB BoardLiquidDetectStart(BOARD_LIQUID_DETECT_FUNC enableMode,uint8_t* funcResultPtr);

//液面探测自校准
ERROR_SUB BoardLiquidDetectSelfAdjust(uint32_t timeoutMs);

//开关液面探测数据上传
ERROR_SUB BoardLiquidDetectSetDataUpload(FunctionalState setState);

//停止液面探测数据接收
void BoardLiquidDetectStopRecv(void);


#endif




