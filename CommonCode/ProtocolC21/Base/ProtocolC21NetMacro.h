/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-09-04 11:57:26 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-09-04 16:51:25 +0800
************************************************************************************************/ 
#ifndef __PROTOCOL_C21_NET_MACRO_H_
#define __PROTOCOL_C21_NET_MACRO_H_
#include "DriverHeaderBoard.h"

//协议帧头
#define PROTOCOL_C21_NET_FRAME_HEADER                       0XA55A
#define PROTOCOL_C21_NET_FRAME_HEAD_LEADER_A                0XA5
#define PROTOCOL_C21_NET_FRAME_HEAD_LEADER_B                0X5A
//有效数据的开始位置    
#define PROTOCOL_C21_NET_FRAME_OFFSET_DATA_BUFFER           12
//协议帧头长度  
#define PROTOCOL_C21_NET_FRAME_LENGTH_HEADER                PROTOCOL_C21_NET_FRAME_OFFSET_DATA_BUFFER
//数据单包长度最大值    
#define PROTOCOL_C21_NET_SINGLE_PACK_DATA_MAX               4096
//数据单包有效数据最大值
#define PROTOCOL_C21_NET_SOCKET_SINGLE_PACK_DATA_BODY_MAX   (PROTOCOL_C21_NET_SINGLE_PACK_DATA_MAX-PROTOCOL_C21_NET_FRAME_OFFSET_DATA_BUFFER)

//串口数据接收缓存状态,主要用于数据接收至少是否接受一帧完成
typedef enum PROTOCOL_C21_NET_RECV_STATE
{
    PROTOCOL_C21_NET_RECV_STATE_NONE     = 0X00,//串口数据包接收未启动
    PROTOCOL_C21_NET_RECV_STATE_ERROR    = 0X01,//串口数据包接收错误
    PROTOCOL_C21_NET_RECV_STATE_COMPLETE = 0X02,//串口数据包一帧接收完成
}PROTOCOL_C21_NET_RECV_STATE;

typedef enum C21_ACK_RESULT
{
    C21_ACK_RESULT_FAIL = 0X00,//异常指令ACK
    C21_ACK_RESULT_OK   = 0X01,//正常指令ACK
}C21_ACK_RESULT;

//ACK指令
#define PROTOCOL_C21_NET_ACK_CMD                            0X00FF

#endif










