/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-09-04 11:57:26 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-09-05 19:23:49 +0800
************************************************************************************************/ 
#include "PEUartOumengInterBase.h"


//欧蒙接口查询PE接口设备的状态
ERROR_SUB PeUartOumengInterQueryPeDeviceState(void)
{
    return ProtocolPEUartSocketSendCmdDetail(MACRO_PE_UART_CMD_QUERY_PE_DEVICE_STATE,NULL,0);
}

//PE接口向欧蒙接口上报设备状态
ERROR_SUB PeUartOumengInterReportPeState(PE_UART_DEVICE_STATE deviceState)
{
    return ProtocolPEUartSocketSendCmdDetail(MACRO_PE_UART_CMD_PE_DEVICE_STATE_REPORT,(uint8_t*)(&deviceState),1);
}

//PE接口查询欧蒙接口设备的状态
ERROR_SUB PeUartOumengInterQueryZetaDeviceState(void)
{
    return ProtocolPEUartSocketSendCmdDetail(MACRO_PE_UART_CMD_QUERY_OUMENG_INTER_STATE,NULL,0);
}

//向PE接口上报欧蒙接口的设备状态
ERROR_SUB PeUartOumengInterReportZetaState(PE_UART_DEVICE_STATE deviceState)
{
    return ProtocolPEUartSocketSendCmdDetail(MACRO_PE_UART_CMD_OUMENG_DEVICE_STATE_REPORT,(uint8_t*)(&deviceState),1);
}

//PE接口查询欧蒙接口空试管架是否准备好
ERROR_SUB PeUartOumengInterQueryZetaEmptyRankState(void)
{
    return ProtocolPEUartSocketSendCmdDetail(MACRO_PE_UART_CMD_QUERY_EMPTY_RANK_IS_READY,NULL,0);
}

//欧蒙接口上报空架子是否准备好
ERROR_SUB PeUartOumengInterReportEmptyRankState(PE_UART_EMPTY_RANK_STATE emptyRankState)
{
    return ProtocolPEUartSocketSendCmdDetail(MACRO_PE_UART_CMD_RESP_QUERY_RANK_READY,(uint8_t*)(&emptyRankState),1);
}

//PE接口上报试管已经放到位
ERROR_SUB PeUartOumengInterReportTubeReady(PE_UART_TUBE_POS tubePos,uint8_t* tubeBarCodePtr,uint16_t barCodeLength)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    uint8_t* sendBufferLocalPtr = NULL;
    if(barCodeLength == 0)
    {
        return ProtocolPEUartSocketSendCmdDetail(MACRO_PE_UART_CMD_REPORT_TUBE_PUT_READY,(uint8_t*)(&tubePos),1);
    }
    else
    {
        sendBufferLocalPtr = ProtocolPEUartSocketMemMallocWhileSuccess(1+barCodeLength);
        UserMemSet(sendBufferLocalPtr,0,1+barCodeLength);
        sendBufferLocalPtr[0] = (uint8_t)(tubePos);
        UserMemCopy(sendBufferLocalPtr+1,tubeBarCodePtr,barCodeLength);
        errorSub = ProtocolPEUartSocketSendCmdDetail(MACRO_PE_UART_CMD_REPORT_TUBE_PUT_READY,sendBufferLocalPtr,1+barCodeLength);
        //释放内存
        ProtocolPEUartSocketMemFree(sendBufferLocalPtr);
        return errorSub;
    }
}

//欧蒙接口查询试管能不能返回PE接口
ERROR_SUB PeUartOumengInterQueryTubeReturnPermit(void)
{
    return ProtocolPEUartSocketSendCmdDetail(MACRO_PE_UART_CMD_QUERY_TUBE_RETURN_PE_PERMIT,NULL,0);
}

//PE接口返回试管能不能回到PE接口
ERROR_SUB PeUartOumengInterReportTubeReturnPermit(PE_UART_TUBE_RETURN permit)
{
    return ProtocolPEUartSocketSendCmdDetail(MACRO_PE_UART_CMD_RESP_TUBE_RETURN_PE_PERMIT,(uint8_t*)(&permit),1);
}

//欧蒙接口上报需要抓走的试管的位置
ERROR_SUB PeUartOumengInterReportNeedCatchOutTubeState(PE_UART_TUBE_STATE tube1State,PE_UART_TUBE_STATE tube2State,PE_UART_TUBE_STATE tube3State,
                                                        PE_UART_TUBE_STATE tube4State,PE_UART_TUBE_STATE tube5State)
{
    uint8_t sendBufferLocal[5] = {0,0,0,0,0};
    sendBufferLocal[0] = (uint8_t)tube1State;
    sendBufferLocal[1] = (uint8_t)tube2State;
    sendBufferLocal[2] = (uint8_t)tube3State;
    sendBufferLocal[3] = (uint8_t)tube4State;
    sendBufferLocal[4] = (uint8_t)tube5State;
    return ProtocolPEUartSocketSendCmdDetail(MACRO_PE_UART_CMD_NOTIFY_PE_RANK_CATCH_POS,sendBufferLocal,5);
}

//PE接口上报开始抓取指定位置的试管
ERROR_SUB PeUartOumengInterReportStartCatchTube(PE_UART_TUBE_POS tubePos)
{
    return ProtocolPEUartSocketSendCmdDetail(MACRO_PE_UART_CMD_PE_REPORT_START_CATCH_TUBE,(uint8_t*)(&tubePos),1);
}

//PE接口上报抓取完成
ERROR_SUB PeUartOumengInterReportTubeCatchOver(PE_UART_TUBE_POS tubePos)
{
    return ProtocolPEUartSocketSendCmdDetail(MACRO_PE_UART_CMD_PE_REPORT_END_CATCH_TUBE,(uint8_t*)(&tubePos),1);
}






