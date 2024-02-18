#ifndef _NET_MULTI_RAIL_UTIL_H_
#define _NET_MULTI_RAIL_UTIL_H_

#include "NetMasterReport.h"
#include "NetMasterDataType.h"

//上报错误到PC
#define NET_REPORT_ERROR_TO_PC(resultPackPtr)   NetMasterReportErrorToPC((resultPackPtr)->errorMain,(resultPackPtr)->errorLevel,(resultPackPtr)->errorSub)  

//多联机网口发送结果报
void NetMultiRailSendToPCResult(uint32_t cmdVal,SYS_RESULT_PACK* resultPackPtr,uint8_t* resultParamBuffer,uint16_t resultBufferLen);

#endif
