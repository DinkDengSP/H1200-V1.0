/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-06-08 11:39:07 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-06-08 11:59:09 +0800
************************************************************************************************/ 
#ifndef __IPC_MACRO_H_
#define __IPC_MACRO_H_
#include "DriverHeaderBoard.h"
#include "tx_queue.h"

//保留指令,用于标记上一次指令为空
#define IPC_BASE_CMD_RESERVE    0X19900525

//IPC指令发送结果
typedef enum IPC_SEND_RESULT
{
    IPC_SEND_RESULT_FAIL       = 0X00,//发送失败
    IPC_SEND_RESULT_SUCCESS    = 0X01,//发送成功
} IPC_SEND_RESULT;

//IPC指令执行状态
typedef enum IPC_CMD_STAGE
{
    IPC_CMD_STAGE_WAIT          = 0X00,//指令等待执行
    IPC_CMD_STAGE_OVER          = 0X01,//指令执行完成
    IPC_CMD_STAGE_PROC_FUNC_NULL= 0XFD,//指令回调函数未设置
    IPC_CMD_STAGE_ERROR_SUPPORT = 0XFE,//指令不支持
    IPC_CMD_STAGE_CMD_PAR_LEN   = 0XFE,//指令参数长度不满足其预期
}IPC_CMD_STAGE;










#endif




