/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-10-17 15:04:23 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-10-17 15:11:03 +0800
************************************************************************************************/ 
#ifndef __NET_PROT_MACRO_H_
#define __NET_PROT_MACRO_H_
#include "DriverHeaderBoard.h"

//协议帧头
#define NET_PROT_FRAME_HEADER                       0XA55A
#define NET_PROT_FRAME_HEAD_LEADER_A                0XA5
#define NET_PROT_FRAME_HEAD_LEADER_B                0X5A
//有效数据的开始位置    
#define NET_PROT_FRAME_OFFSET_DATA_BUFFER           18
//协议帧头长度  
#define NET_PROT_FRAME_LENGTH_HEADER                NET_PROT_FRAME_OFFSET_DATA_BUFFER
//数据单包长度最大值    
#define NET_PROT_SINGLE_PACK_DATA_MAX               4096
//数据单包有效数据最大值
#define NET_PROT_SINGLE_PACK_DATA_BODY_MAX          (NET_PROT_SINGLE_PACK_DATA_MAX-NET_PROT_FRAME_OFFSET_DATA_BUFFER)

//网口数据接收缓存状态,主要用于数据接收至少是否接受一帧完成
typedef enum NET_PROT_RECV_STA
{
    NET_PROT_RECV_STA_NONE     = 0X00,//网口数据包接收未启动
    NET_PROT_RECV_STA_ERROR    = 0X01,//网口数据包接收错误
    NET_PROT_RECV_STA_COMPLETE = 0X02,//网口数据包一帧接收完成
}NET_PROT_RECV_STA;

//转发包类型
typedef enum NET_PROT_FWD_TYPE
{
    NET_PROT_FWD_TYPE_ACK_CMD     = 0,
    NET_PROT_FWD_TYPE_ACK_RESULT  = 1,
    NET_PROT_FWD_TYPE_ACK_UPLOAD  = 2,
    NET_PROT_FWD_TYPE_LOG         = 3,
    NET_PROT_FWD_TYPE_CMD         = 4,
    NET_PROT_FWD_TYPE_RESULT      = 5,
    NET_PROT_FWD_TYPE_UPLOAD      = 6,
}NET_PROT_FWD_TYPE;

//NET ACK类型
typedef enum NET_PROT_ACK_TYPE
{
    NET_PROT_ACK_CMD    = 0X00,
    NET_PROT_ACK_RESULT = 0X01,
    NET_PROT_ACK_UPLOAD = 0X02,
}NET_PROT_ACK_TYPE;


#endif










