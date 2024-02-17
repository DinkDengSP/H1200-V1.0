/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-10-20 18:20:58 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-10-23 17:37:27 +0800
************************************************************************************************/ 
#ifndef __CAN_PROT_MACRO_H_
#define __CAN_PROT_MACRO_H_
#include "DriverHeaderBoard.h"

//CAN帧类型
typedef enum CAN_PROT_FRAME_TYPE
{
    CAN_PROT_FRAME_TYPE_SHORT        = 0x00,//短帧
    CAN_PROT_FRAME_TYPE_LONG_START   = 0x01,//长帧起始帧
    CAN_PROT_FRAME_TYPE_LONG_DATA    = 0x02,//长帧数据帧
    CAN_PROT_FRAME_TYPE_LONG_END     = 0x03,//长帧结果帧
}CAN_PROT_FRAME_TYPE;

//ACK类型
typedef enum CAN_PROT_ACK_TYPE
{
    CAN_PROT_ACK_CMD    = 0X00,
    CAN_PROT_ACK_RESULT = 0X01,
    CAN_PROT_ACK_UPLOAD = 0X02,
}CAN_PROT_ACK_TYPE;

//接收cache的接收状态
typedef enum CAN_PROT_LONG_PACK_STA
{
    CAN_PROT_LONG_PACK_STA_WAIT_DATA  = 0X00,//等待常规数据包
    CAN_PROT_LONG_PACK_STA_WAIT_END   = 0X01,//等待结束包
}CAN_PROT_LONG_PACK_STA;

//接收缓存区定义
typedef enum CAN_PROT_RECV_CACHE
{
    CAN_PROT_RECV_CACHE_LOG     = 0X00,//日志包
    CAN_PROT_RECV_CACHE_CMD     = 0X01,//指令包
    CAN_PROT_RECV_CACHE_RESULT  = 0X02,//结果包
    CAN_PROT_RECV_CACHE_UPLOAD  = 0X03,//上传包
}CAN_PROT_RECV_CACHE;

//转发包类型
typedef enum CAN_PROT_FWD_TYPE
{
    CAN_PROT_FWD_TYPE_ACK_CMD     = 0,
    CAN_PROT_FWD_TYPE_ACK_RESULT  = 1,
    CAN_PROT_FWD_TYPE_ACK_UPLOAD  = 2,
    CAN_PROT_FWD_TYPE_LOG         = 3,
    CAN_PROT_FWD_TYPE_CMD         = 4,
    CAN_PROT_FWD_TYPE_RESULT      = 5,
    CAN_PROT_FWD_TYPE_UPLOAD      = 6,
}CAN_PROT_FWD_TYPE;

//CAN PORT定义
typedef enum PORT_CAN
{
    PORT_CAN1 = 0X00,//CAN1
    PORT_CAN2 = 0X01,//CAN2
}PORT_CAN;


#endif
































