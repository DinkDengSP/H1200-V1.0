/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-09-04 11:57:26 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-09-21 11:53:26 +0800
************************************************************************************************/ 
#ifndef __C21_NET_OUMENG_INTER_MACRO_H_
#define __C21_NET_OUMENG_INTER_MACRO_H_
#include "ProtocolC21NetBase.h"

typedef enum MACRO_C21_NET_CMD
{
    MACRO_C21_NET_CMD_C21_APPLY_RANK_IN                         = 0X0080,//C21找欧蒙接口申请试管架
    MACRO_C21_NET_CMD_ZETA_REPORT_RANK_IN_RESULT                = 0X0082,//欧蒙接口告知进架结果
    MACRO_C21_NET_CMD_C21_NOTOFY_RANK_OUT                       = 0X0083,//C21 告知出架 0X0083
    MACRO_C21_NET_CMD_ZETA_REPORT_RANK_OUT_PROC_RESULT          = 0X0085,//欧蒙接口告知出架结果
    MACRO_C21_NET_CMD_C21_REPORT_DEVICE_STATE_CHANGE            = 0X0086,//C21仪器状态变化告知欧蒙接口
    MACRO_C21_NET_CMD_ZETA_REPORT_DEVICE_STATE_CHANGE           = 0X0087,//欧蒙接口状态变化告知C21
    MACRO_C21_NET_CMD_ZETA_REPORT_HEART                         = 0X0089,//欧蒙接口心跳上报
    MACRO_C21_NET_CMD_ZETA_POS_ADJUST_START                     = 0X009A,//欧蒙接口启动校准
    MACRO_C21_NET_CMD_ZETA_POS_ADJUST_NEXT                      = 0X009B,//欧蒙接口执行校准
    MACRO_C21_NET_CMD_ZETA_POS_ADJUST_OVER                      = 0X009C,//欧蒙接口校准完成
    MACRO_C21_NET_CMD_ZETA_TROUBLE_SHOOTING                     = 0X009D,//欧蒙接口故障解除    
    MACRO_C21_NET_CMD_ZETA_REPORT_ERROR                         = 0X009E,//欧蒙接口故障上报
}MACRO_C21_NET_CMD;

//出架子许可
typedef enum C21_NET_RANK_OUT
{
    C21_NET_RANK_OUT_ALLOW           = 0X00,//可以进架子
    C21_NET_RANK_OUT_NOT_ALLOW       = 0X01,//不可以进架子
}C21_NET_RANK_OUT;

//出架类型
typedef enum C21_NET_RANK_OUT_TYPE
{
    C21_NET_RANK_OUT_FREE       = 0x00,//架子可以释放掉
    C21_NET_RANK_OUT_CACHE      = 0x01,//架子需要缓存起来
}C21_NET_RANK_OUT_TYPE;

//试管测试状态
typedef enum C21_NET_TUBE_TEST_STATE
{
    C21_NET_TUBE_TEST_STATE_NOT_COMPLETE    = 0X00,//试管测试未完成
    C21_NET_TUBE_TEST_STATE_COMPLETE        = 0X01,//试管测试已完成
}C21_NET_TUBE_TEST_STATE;

//设备当前状态
typedef enum C21_NET_DEVICE_STATE
{
    C21_NET_DEVICE_STATE_FAULT   = 0X00,//PE设备故障
    C21_NET_DEVICE_STATE_NORMAL  = 0X01,//PE设备正常
}C21_NET_DEVICE_STATE;


#endif









