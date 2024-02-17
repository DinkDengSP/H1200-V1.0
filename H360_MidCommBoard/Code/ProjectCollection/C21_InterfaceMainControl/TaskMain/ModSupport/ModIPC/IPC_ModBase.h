/************************************************************************ 
 ** Author       : 邓小俊
 ** Date         : 2024-01-31 16:34:18
 ** Description  : When I Has Free Time ,I Will Write Description Below.
 ** description  : 
 ** LastEditors  : 邓小俊
 ** LastEditTime : 2024-02-04 13:52:41
 ** Email: xiaojun.deng@aliyun.com
 ** Copyright (c) 2024 by 公司名称, All Rights Reserved. 
 ************************************************************************/
#ifndef __IPC_MOD_BASE_H_
#define __IPC_MOD_BASE_H_
#include "IPC_SrvHeader.h"

typedef enum IPC_CMD_MACRO_MACRO
{
    //RACK STORE
    IPC_CMD_MACRO_RACK_STORE_READ_SINGLE            = 0X00000000,//单个传感器读取
    IPC_CMD_MACRO_RACK_STORE_READ_ALL               = 0X00000001,//全部传感器读取
    //RACK MOVE
    IPC_CMD_MACRO_RACK_MOVE_MOTOR_RESET             = 0X00000000,//试管架移动单元电机复位
    IPC_CMD_MACRO_RACK_MOVE_MOTOR_RUN_STEP          = 0X00000001,//试管架移动单元电机走步数
    IPC_CMD_MACRO_RACK_MOVE_MOTOR_STATE_READ        = 0X00000002,//试管架移动单元电机状态读取
    IPC_CMD_MACRO_RACK_MOVE_SENSOR_READ_SINGLE      = 0X00000003,//试管架移动单元传感器信号单个读取
    IPC_CMD_MACRO_RACK_MOVE_SENSOR_READ_ALL         = 0X00000004,//试管架移动单元传感器信号批量读取
    IPC_CMD_MACRO_RACK_MOVE_GET_CUR_POS             = 0X00000005,//获取试管架移动单元当前位置
    IPC_CMD_MACRO_RACK_MOVE_MODULE_RESET            = 0X00000006,//试管架移动单元复位
    IPC_CMD_MACRO_RACK_MOVE_TO_IDLE                 = 0X00000007,//试管架移动单元到空闲位置
    IPC_CMD_MACRO_RACK_MOVE_LATERALLY_TO_STORE      = 0X00000008,//横移电机移动到指定试管架仓位置
    IPC_CMD_MACRO_RACK_MOVE_LATERALLY_TO_C21        = 0X00000009,//横移电机移动到C21接口位置
    IPC_CMD_MACRO_RACK_MOVE_TO_STORE_LOAD_POS       = 0X0000000A,//装载机构移动到装载试管架仓位置
    IPC_CMD_MACRO_RACK_MOVE_TO_STORE_UNLOAD_POS     = 0X0000000B,//装载机构移动到卸载试管架仓位置
    IPC_CMD_MACRO_RACK_MOVE_TO_C21_LOAD_POS         = 0X0000000C,//装载机构移动到装载C21位置
    IPC_CMD_MACRO_RACK_MOVE_TO_C21_UNLOAD_POS       = 0X0000000D,//装载机构移动到卸载C21位置
    IPC_CMD_MACRO_RACK_MOVE_LOAD_STORE_RACK_CUR     = 0X0000000E,//在当前位置执行试管架仓样本架装载
    IPC_CMD_MACRO_RACK_MOVE_UNLOAD_STORE_RACK_CUR   = 0X0000000F,//在当前位置执行试管架仓样本架放入
    IPC_CMD_MACRO_RACK_MOVE_LOAD_C21_RACK_CUR       = 0X00000010,//在当前位置执行C21样本架装载
    IPC_CMD_MACRO_RACK_MOVE_UNLOAD_C21_RACK_CUR     = 0X00000011,//在当前位置执行C21样本架放入
    IPC_CMD_MACRO_RACK_MOVE_LOAD_RACK_STORE_BY_POS  = 0X00000012,//集成指令,从试管架仓中装载一个试管架
    IPC_CMD_MACRO_RACK_MOVE_UNLOAD_RACK_STORE_BY_POS= 0X00000013,//集成指令,往试管架仓中放入一个试管架
    IPC_CMD_MACRO_RACK_MOVE_LOAD_RACK_C21           = 0X00000014,//集成指令,从C21接口装载一个试管架
    IPC_CMD_MACRO_RACK_MOVE_UNLOAD_RACK_C21         = 0X00000015,//集成指令,往C21接口放入一个试管架
    IPC_CMD_MACRO_RACK_MOVE_LOAD_RACK_STORE_TO_STORE= 0X00000016,//集成指令,从试管仓中取出一个试管架放入另一个试管仓中
    IPC_CMD_MACRO_RACK_MOVE_LOAD_RACK_STORE_TO_C21  = 0X00000017,//集成指令,从试管仓中取出一个试管架放入C21中
    IPC_CMD_MACRO_RACK_MOVE_LOAD_RACK_C21_TO_STORE  = 0X00000018,//从C21中取出一个试管架放入试管架仓中
}IPC_CMD_MACRO_MACRO;



#endif








