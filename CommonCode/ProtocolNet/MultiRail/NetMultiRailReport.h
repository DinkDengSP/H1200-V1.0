#ifndef _NET_MULTI_RAIL_REPORT_H_
#define _NET_MULTI_RAIL_REPORT_H_
#include "NetMultiRailUtil.h"
#include "SysStateDataType.h"
//系统状态上传
void NetMultiRailReportSystemState(MODULE_SYSTEM_STATE *sysReportUnitPtr);

//条码数据上传
void NetMultiRailReportTubeRackState(uint32_t rackID,uint8_t *tubeReportBuffer,uint16_t buferLength);

//试管架进入测试轨道上报
void NetMultiRailReportRackEntryTargetRail(uint32_t rackID,uint8_t railIndex);

//试管架离开测试轨道上报
void NetMultiRailReportRackLeaveTargetRail(uint32_t rackID,uint8_t railIndex);

//试管架回到样本仓
void NetMultiRailReportRackBackSampleStore(uint32_t rackID,uint8_t trayIndex,uint8_t rackIndex);


#endif // _NET_MULTI_RAIL_REPORT_H_
