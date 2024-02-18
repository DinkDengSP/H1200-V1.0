/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-10-20 18:20:58 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-11-30 10:44:44 +0800
************************************************************************************************/ 
#ifndef __CAN_MASTER_MACRO_H_
#define __CAN_MASTER_MACRO_H_
#include "CanProtMain.h"
#include "CanProtUtil.h"
#include "CanProtBase.h"

//定义系统通道号
typedef enum CAN_MASTER_CH
{
    //公共通道
    CAN_MASTER_CH_ACT_UTIL                = 0X00,//辅助信道
    CAN_MASTER_CH_ACT_IAP                 = 0X01,//升级信道
    CAN_MASTER_CH_ACT_PORT_IN             = 0X02,//端口输入读取信道
    CAN_MASTER_CH_ACT_PORT_OUT            = 0X03,//端口输出写入信道
    CAN_MASTER_CH_ACT_PARAM_RW            = 0X04,//参数读写信道
    CAN_MASTER_CH_ACT_SM1                 = 0X05,//步进电机1控制信道
    CAN_MASTER_CH_ACT_SM2                 = 0X06,//步进电机2控制信道
    CAN_MASTER_CH_ACT_SM3                 = 0X07,//步进电机3控制信道
    CAN_MASTER_CH_ACT_SM4                 = 0X08,//步进电机4控制信道
    CAN_MASTER_CH_ACT_SM5                 = 0X09,//步进电机5控制信道
    CAN_MASTER_CH_ACT_SM6                 = 0X0A,//步进电机6控制信道
    CAN_MASTER_CH_ACT_SM7                 = 0X0B,//步进电机7控制信道
    CAN_MASTER_CH_ACT_SM8                 = 0X0C,//步进电机8控制信道
    CAN_MASTER_CH_ACT_SM9                 = 0X0D,//步进电机1控制信道
    CAN_MASTER_CH_ACT_SM10                = 0X0E,//步进电机2控制信道
    CAN_MASTER_CH_ACT_SM11                = 0X0F,//步进电机3控制信道
    CAN_MASTER_CH_ACT_SM12                = 0X10,//步进电机4控制信道
    CAN_MASTER_CH_ACT_SM13                = 0X11,//步进电机5控制信道
    CAN_MASTER_CH_ACT_SM14                = 0X12,//步进电机6控制信道
    CAN_MASTER_CH_ACT_SM15                = 0X13,//步进电机7控制信道
    CAN_MASTER_CH_ACT_SM16                = 0X14,//步进电机8控制信道
    //各个下位机模块信道   
    //立禾云
    CAN_MASTER_CH_CLD_BASE                = 0X15,//立禾云设备基础控制
    CAN_MASTER_CH_CLD_SOCKET              = 0X16,//立禾云网络通讯控制
}CAN_MASTER_CH;


//定义系统指令
typedef enum CAN_MASTER_CMD
{
    //辅助信道
    CAN_MASTER_CMD_UTIL_ONLINE                        = 0X00000000,//联机
    CAN_MASTER_CMD_UTIL_OFFLINE                       = 0X00000001,//脱机
    CAN_MASTER_CMD_UTIL_UTC_SYNC                      = 0X00000002,//UTC时间同步
    CAN_MASTER_CMD_UTIL_HEART_GET                     = 0X00000003,//心跳获取
    CAN_MASTER_CMD_UTIL_EM_STOP                       = 0X00000004,//急停
    CAN_MASTER_CMD_UTIL_GET_VERSION                   = 0X00000005,//版本号获取
    CAN_MASTER_CMD_UTIL_SET_RUNNING_MODE              = 0X00000006,//设置系统运行模式
    //在线升级信道
    CAN_MASTER_CMD_IAP_PREPARE                        = 0X00000000,//在线升级准备
    CAN_MASTER_CMD_IAP_START_MSG                      = 0X00000001,//在线升级启动信息
    CAN_MASTER_CMD_IAP_DATA_TRANS                     = 0X00000002,//在线升级数据传输
    CAN_MASTER_CMD_IAP_DATA_CHECK                     = 0X00000003,//在线升级数据校验
    CAN_MASTER_CMD_IAP_FORCE_JMP                      = 0X00000004,//在线升级强制跳转
    //IO输入信道
    CAN_MASTER_CMD_PORT_IN_READ_SINGLE                = 0X00000000,//输入单个读取
    CAN_MASTER_CMD_PORT_IN_READ_RANGE                 = 0X00000001,//输入区间读取
    CAN_MASTER_CMD_PORT_IN_READ_ANY                   = 0X00000002,//输入批量读取
    //IO输出信道
    CAN_MASTER_CMD_PORT_OUT_WRITE_SINGLE              = 0X00000000,//输出单个写入
    CAN_MASTER_CMD_PORT_OUT_WRITE_ANY                 = 0X00000001,//输出批量写入
    //参数读写信道
    CAN_MASTER_CMD_PARAM_RW_READ_SINGLE               = 0X00000000,//参数单个读取
    CAN_MASTER_CMD_PARAM_RW_READ_ANY                  = 0X00000001,//参数批量读取
    CAN_MASTER_CMD_PARAM_RW_WRITE_SINGLE              = 0X00000002,//参数单个写入
    CAN_MASTER_CMD_PARAM_RW_WRITE_ANY                 = 0X00000003,//参数批量写入
    //电机信道
    CAN_MASTER_CMD_SM_RESET                           = 0X00000000,//步进电机控制复位
    CAN_MASTER_CMD_SM_RUN_STEPS                       = 0X00000001,//步进电机控制走步数
    CAN_MASTER_CMD_SM_RUN_COORDINATE                  = 0X00000002,//步进电机控制走坐标
    CAN_MASTER_CMD_SM_RETURN_ZERO                     = 0X00000003,//步进电机控制回零
    CAN_MASTER_CMD_SM_RUN_ALWAYS                      = 0X00000004,//步进电机控制持续运转
    CAN_MASTER_CMD_SM_STOP_NOW                        = 0X00000005,//步进电机控制立即停止
    CAN_MASTER_CMD_SM_STOP_SLOW_DOWN                  = 0X00000006,//步进电机控制减速运转
    CAN_MASTER_CMD_SM_GET_COORDINATE                  = 0X00000007,//步进电机控制获取当前坐标
    CAN_MASTER_CMD_SM_READ_ORIGIN_SENSOR              = 0X00000008,//步进电机控制获取当前原点传感器状态
    CAN_MASTER_CMD_SM_DISABLE                         = 0X00000009,//电机失能
    CAN_MASTER_CMD_SM_ENTER_TEACH_MODE                = 0X0000000A,//进入学习模式
    CAN_MASTER_CMD_SM_FORCE_TO_DEFAULT                = 0X0000000B,//电机强行进入默认状态(异常之后无法复位的情况)
    //立禾云基础设备控制
    CAN_MASTER_CMD_CLD_BASE_PING_BAIDU                = 0X00000000,//PING 服务器测试 
    CAN_MASTER_CMD_CLD_BASE_GET_DESP_MANUFACTURE      = 0X00000001,//获取模块厂商信息
    CAN_MASTER_CMD_CLD_BASE_GET_DESP_MODULE_TYPE      = 0X00000002,//获取模块类型信息 
    CAN_MASTER_CMD_CLD_BASE_GET_DESP_SOFT_VERSION     = 0X00000003,//获取模块软件版本信息 
    CAN_MASTER_CMD_CLD_BASE_GET_DESP_IMEI             = 0X00000004,//获取模块入网许可
    CAN_MASTER_CMD_CLD_BASE_GET_DESP_ISMI             = 0X00000005,//获取SIM卡IMSI串号
    CAN_MASTER_CMD_CLD_BASE_GET_DESP_ICCID            = 0X00000006,//获取SIM卡ICCID识别号
    CAN_MASTER_CMD_CLD_BASE_GET_DESP_NET_NAME_LONG    = 0X00000007,//获取当前模块连接网络全名
    CAN_MASTER_CMD_CLD_BASE_GET_DESP_NET_NAME_SHORT   = 0X00000008,//获取当前模块连接网络短名
    CAN_MASTER_CMD_CLD_BASE_GET_DESP_SRV_PROVID_NAME  = 0X00000009,//获取网络服务商名称
    CAN_MASTER_CMD_CLD_BASE_GET_DESP_NET_TYPE         = 0X0000000A,//获取当前网络制式
    CAN_MASTER_CMD_CLD_BASE_GET_DESP_NET_BAUD         = 0X0000000B,//获取当前网络波段
    CAN_MASTER_CMD_CLD_BASE_GET_DESP_NET_LAC          = 0X0000000C,//获取LAC基站区域码
    CAN_MASTER_CMD_CLD_BASE_GET_DESP_NET_CID          = 0X0000000D,//获取CID基站编号
    CAN_MASTER_CMD_CLD_BASE_GET_DESP_STRENGTH_ERR     = 0X0000000E,//获取当前信号强度与误码率
    CAN_MASTER_CMD_CLD_BASE_CHECK_NET_STATE           = 0X0000000F,//查看当前模块网络是否正常
    CAN_MASTER_CMD_CLD_BASE_DEVICE_REBOOT             = 0X00000010,//通讯设备重启
    //立禾云SOCKET控制
    CAN_MASTER_CMD_CLD_SOCKET_CONNECT                 = 0X00000000,//设备连接网络
    CAN_MASTER_CMD_CLD_SOCKET_DISCONNECT              = 0X00000001,//设备断开网络连接
    CAN_MASTER_CMD_CLD_SOCKET_SEND_PACK_ONLY          = 0X00000002,//设备发送一包数据
    CAN_MASTER_CMD_CLD_SOCKET_RECV_PACK_ONLY          = 0X00000003,//设备读取一次数据
    CAN_MASTER_CMD_CLD_SOCKET_SEND_RECV_PACK          = 0X00000004,//设备集成发送并接收数据
    //其他下位机指令
    CAN_MASTER_CMD_PLACE_HOLDER                             = 0X00FFFFFF,//占位符
}CAN_MASTER_CMD;

//上报ID信息定义
typedef enum CAN_MASTER_REPORT_ID
{
    CAN_MASTER_REPORT_ID_ERR                            = 0X000000FF,//错误上报
    //H360温度上报
    CAN_MASTER_REPORT_H360_BOARD_TEMP                   = 0X00000100,//板卡温度上报
    CAN_MASTER_REPORT_H360_ENVIR_TEMP_CONTROL           = 0X00000101,//环境温度上报
    CAN_MASTER_REPORT_H360_PLATE_REACTION_TEMP_CONTROL  = 0X00000102,//反应盘温度上报
    CAN_MASTER_REPORT_H360_PLATE_WASH_TEMP_CONTROL      = 0X00000103,//清洗盘温度上报
    CAN_MASTER_REPORT_H360_PLATE_REAGENT_TEMP_CONTROL   = 0X00000104,//试剂盘温度上报
    //上报占位ID
    CAN_MASTER_REPORT_ID_HOLD_PLACE                     = 0XFFFFFFFF,//系统上报ID占位符
}CAN_MASTER_REPORT_ID;

#endif

































