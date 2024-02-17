/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-09-04 11:57:26 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-09-06 09:07:03 +0800
************************************************************************************************/ 
#ifndef __PE_UART_OUMENG_INTER_BASE_H_
#define __PE_UART_OUMENG_INTER_BASE_H_
#include "ProtocolPEUartBase.h"
#include "ProtocolPEUartMain.h"
#include "PEUartOumengInterDataType.h"

//欧蒙接口查询PE接口设备的状态
ERROR_SUB PeUartOumengInterQueryPeDeviceState(void);

//PE接口向欧蒙接口上报设备状态
ERROR_SUB PeUartOumengInterReportPeState(PE_UART_DEVICE_STATE deviceState);

//PE接口查询欧蒙接口设备的状态
ERROR_SUB PeUartOumengInterQueryZetaDeviceState(void);

//向PE接口上报欧蒙接口的设备状态
ERROR_SUB PeUartOumengInterReportZetaState(PE_UART_DEVICE_STATE deviceState);

//PE接口查询欧蒙接口空试管架是否准备好
ERROR_SUB PeUartOumengInterQueryZetaEmptyRankState(void);

//欧蒙接口上报空架子是否准备好
ERROR_SUB PeUartOumengInterReportEmptyRankState(PE_UART_EMPTY_RANK_STATE emptyRankState);

//PE接口上报试管已经放到位
ERROR_SUB PeUartOumengInterReportTubeReady(PE_UART_TUBE_POS tubePos,uint8_t* tubeBarCodePtr,uint16_t barCodeLength);

//欧蒙接口查询试管能不能返回PE接口 
ERROR_SUB PeUartOumengInterQueryTubeReturnPermit(void);

//PE接口返回试管能不能回到PE接口
ERROR_SUB PeUartOumengInterReportTubeReturnPermit(PE_UART_TUBE_RETURN permit);

//欧蒙接口上报需要抓走的试管的位置
ERROR_SUB PeUartOumengInterReportNeedCatchOutTubeState(PE_UART_TUBE_STATE tube1State,PE_UART_TUBE_STATE tube2State,PE_UART_TUBE_STATE tube3State,
                                                        PE_UART_TUBE_STATE tube4State,PE_UART_TUBE_STATE tube5State);

//PE接口上报开始抓取指定位置的试管
ERROR_SUB PeUartOumengInterReportStartCatchTube(PE_UART_TUBE_POS tubePos);

//PE接口上报抓取完成
ERROR_SUB PeUartOumengInterReportTubeCatchOver(PE_UART_TUBE_POS tubePos);

#endif







