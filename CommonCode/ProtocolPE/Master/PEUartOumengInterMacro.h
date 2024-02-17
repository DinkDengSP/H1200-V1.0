/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-09-04 11:57:26 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-09-05 19:41:59 +0800
************************************************************************************************/ 
#ifndef __PE_UART_OUMENG_INTER_MACRO_H_
#define __PE_UART_OUMENG_INTER_MACRO_H_
#include "ProtocolPEUartBase.h"

typedef enum MACRO_PE_UART_CMD
{
    MACRO_PE_UART_CMD_PE_DEVICE_STATE_REPORT        = 0X0080,//PE设备上报设备状态
    MACRO_PE_UART_CMD_QUERY_OUMENG_INTER_STATE      = 0X0081,//PE设备查询欧蒙接口设备状态
    MACRO_PE_UART_CMD_OUMENG_DEVICE_STATE_REPORT    = 0X0082,//欧蒙接口上报设备状态
    MACRO_PE_UART_CMD_QUERY_PE_DEVICE_STATE         = 0X0083,//欧蒙接口查询PE接口状态
    MACRO_PE_UART_CMD_QUERY_EMPTY_RANK_IS_READY     = 0X0084,//PE接口查询欧蒙接口空架子是否准备好
    MACRO_PE_UART_CMD_RESP_QUERY_RANK_READY         = 0X0085,//欧蒙接口报告PE接口空架子准备讯息
    MACRO_PE_UART_CMD_REPORT_TUBE_PUT_READY         = 0X0086,//PE接口上报欧蒙接口试管已经到位
    MACRO_PE_UART_CMD_QUERY_TUBE_RETURN_PE_PERMIT   = 0X0087,//欧蒙接口查询试管是否可以回到PE接口
    MACRO_PE_UART_CMD_RESP_TUBE_RETURN_PE_PERMIT    = 0X0088,//PE接口报告欧蒙接口试管是否可以回到PE
    MACRO_PE_UART_CMD_NOTIFY_PE_RANK_CATCH_POS      = 0X0089,//欧蒙接口告知PE接口抓试管的位置
    MACRO_PE_UART_CMD_PE_REPORT_START_CATCH_TUBE    = 0X008A,//PE接口上报开始抓试管
    MACRO_PE_UART_CMD_PE_REPORT_END_CATCH_TUBE      = 0X008B,//PE接口上报抓试管已经完成
}MACRO_PE_UART_CMD;


#endif









