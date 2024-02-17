/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-10-19 10:08:51 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-11-08 16:11:43 +0800
************************************************************************************************/ 
#ifndef __UART_MASTER_MACRO_H_
#define __UART_MASTER_MACRO_H_
#include "UartProtMain.h"
#include "UartProtUtil.h"
#include "UartProtBase.h"

//指令码
typedef enum UART_MASTER_CMD
{
    //主控APP指令集
    UART_MASTER_CMD_MAIN_UTIL_ONLINE                     = 0X00000000,//联机
    UART_MASTER_CMD_MAIN_UTIL_OFFLINE                    = 0X00000001,//脱机
    UART_MASTER_CMD_MAIN_UTIL_UTC_SYNC                   = 0X00000002,//UTC时间同步
    UART_MASTER_CMD_MAIN_UTIL_HEART_GET                  = 0X00000003,//心跳获取
    UART_MASTER_CMD_MAIN_UTIL_EM_STOP                    = 0X00000004,//急停
    UART_MASTER_CMD_MAIN_UTIL_GET_VERSION                = 0X00000005,//版本号获取
    UART_MASTER_CMD_MAIN_UTIL_SET_RUNNING_MODE           = 0X00000006,//设置系统运行模式
    //在线升级
    UART_MASTER_CMD_MAIN_IAP_PREPARE                     = 0X00010000,//在线升级准备
    UART_MASTER_CMD_MAIN_IAP_START_MSG                   = 0X00010001,//在线升级启动信息
    UART_MASTER_CMD_MAIN_IAP_DATA_TRANS                  = 0X00010002,//在线升级数据传输
    UART_MASTER_CMD_MAIN_IAP_DATA_CHECK                  = 0X00010003,//在线升级数据校验
    UART_MASTER_CMD_MAIN_IAP_FORCE_JMP                   = 0X00010004,//在线升级强制跳转
    //调试辅助
    UART_MASTER_CMD_MAIN_PORT_IN_READ_SINGLE             = 0X00020000,//输入单个读取
    UART_MASTER_CMD_MAIN_PORT_IN_READ_RANGE              = 0X00020001,//输入区间读取
    UART_MASTER_CMD_MAIN_PORT_IN_READ_ANY                = 0X00020002,//输入批量读取
    UART_MASTER_CMD_MAIN_PORT_OUT_WRITE_SINGLE           = 0X00020003,//输出单个写入
    UART_MASTER_CMD_MAIN_PORT_OUT_WRITE_ANY              = 0X00020004,//输出批量写入
    UART_MASTER_CMD_MAIN_PARAM_RW_READ_SINGLE            = 0X00020005,//参数单个读取
    UART_MASTER_CMD_MAIN_PARAM_RW_READ_ANY               = 0X00020006,//参数批量读取
    UART_MASTER_CMD_MAIN_PARAM_RW_WRITE_SINGLE           = 0X00020007,//参数单个写入
    UART_MASTER_CMD_MAIN_PARAM_RW_WRITE_ANY              = 0X00020008,//参数批量写入
    //立禾云指令
    UART_MASTER_CMD_CLD_BASE_PING_BAIDU                  = 0X7F000000,//PING 服务器测试
    UART_MASTER_CMD_CLD_BASE_GET_DESP_MANUFACTURE        = 0X7F000001,//获取模块厂商信息
    UART_MASTER_CMD_CLD_BASE_GET_DESP_MODULE_TYPE        = 0X7F000002,//获取模块类型信息
    UART_MASTER_CMD_CLD_BASE_GET_DESP_SOFT_VERSION       = 0X7F000003,//获取模块软件版本信息
    UART_MASTER_CMD_CLD_BASE_GET_DESP_IMEI               = 0X7F000004,//获取模块入网许可
    UART_MASTER_CMD_CLD_BASE_GET_DESP_ISMI               = 0X7F000005,//获取SIM卡IMSI串号
    UART_MASTER_CMD_CLD_BASE_GET_DESP_ICCID              = 0X7F000006,//获取SIM卡ICCID识别号
    UART_MASTER_CMD_CLD_BASE_GET_DESP_UART_NAME_LONG      = 0X7F000007,//获取当前模块连接网络全名
    UART_MASTER_CMD_CLD_BASE_GET_DESP_UART_NAME_SHORT     = 0X7F000008,//获取当前模块连接网络短名
    UART_MASTER_CMD_CLD_BASE_GET_DESP_SRV_PROVID_NAME    = 0X7F000009,//获取网络服务商名称
    UART_MASTER_CMD_CLD_BASE_GET_DESP_UART_TYPE           = 0X7F00000A,//获取当前网络制式
    UART_MASTER_CMD_CLD_BASE_GET_DESP_UART_BAUD           = 0X7F00000B,//获取当前网络波段
    UART_MASTER_CMD_CLD_BASE_GET_DESP_UART_LAC            = 0X7F00000C,//获取LAC基站区域码
    UART_MASTER_CMD_CLD_BASE_GET_DESP_UART_CID            = 0X7F00000D,//获取CID基站编号
    UART_MASTER_CMD_CLD_BASE_GET_DESP_STRENGTH_ERR       = 0X7F00000E,//获取当前信号强度与误码率
    UART_MASTER_CMD_CLD_BASE_CHECK_UART_STATE             = 0X7F00000F,//查看当前模块网络是否正常
    UART_MASTER_CMD_CLD_BASE_DEVICE_REBOOT               = 0X7F000010,//通讯设备重启
    UART_MASTER_CMD_CLD_SOCKET_CONNECT                   = 0X7F000011,//设备连接网络
    UART_MASTER_CMD_CLD_SOCKET_DISCONNECT                = 0X7F000012,//设备断开网络连接
    UART_MASTER_CMD_CLD_SOCKET_SEND_PACK_ONLY            = 0X7F000013,//设备发送一包数据
    UART_MASTER_CMD_CLD_SOCKET_RECV_PACK_ONLY            = 0X7F000014,//设备读取一次数据
    UART_MASTER_CMD_CLD_SOCKET_SEND_RECV_PACK            = 0X7F000015,//设备集成发送并接收数据
    /***************************************下位机透传指令集***********************************************/
    //系统辅助
    UART_MASTER_CMD_ACT_UTIL_ONLINE                      = 0X80000000,//联机
    UART_MASTER_CMD_ACT_UTIL_OFFLINE                     = 0X80000001,//脱机
    UART_MASTER_CMD_ACT_UTIL_UTC_SYNC                    = 0X80000002,//UTC时间同步
    UART_MASTER_CMD_ACT_UTIL_HEART_GET                   = 0X80000003,//心跳获取
    UART_MASTER_CMD_ACT_UTIL_EM_STOP                     = 0X80000004,//急停
    UART_MASTER_CMD_ACT_UTIL_GET_VERSION                 = 0X80000005,//版本号获取
    //在线升级
    UART_MASTER_CMD_ACT_IAP_PREPARE                      = 0X81000000,//在线升级准备
    UART_MASTER_CMD_ACT_IAP_START_MSG                    = 0X81000001,//在线升级启动信息
    UART_MASTER_CMD_ACT_IAP_DARA_TRANS                   = 0X81000002,//在线升级数据传输
    UART_MASTER_CMD_ACT_IAP_DATA_CHECK                   = 0X81000003,//在线升级数据校验
    UART_MASTER_CMD_ACT_IAP_FORCE_JMP                    = 0X81000004,//在线升级强制跳转
    //端口输入
    UART_MASTER_CMD_ACT_PORT_IN_READ_SINGLE              = 0X82000000,//输入单个读取
    UART_MASTER_CMD_ACT_PORT_IN_READ_RANGE               = 0X82000001,//输入区间读取
    UART_MASTER_CMD_ACT_PORT_IN_READ_ANY                 = 0X82000002,//输入批量读取
    //端口输出
    UART_MASTER_CMD_ACT_PORT_OUT_WRITE_SINGLE            = 0X83000000,//输出单个写入
    UART_MASTER_CMD_ACT_PORT_OUT_WRITE_ANY               = 0X83000001,//输出批量写入
    //参数读写
    UART_MASTER_CMD_ACT_PARAM_RW_READ_SINGLE             = 0X84000000,//参数单个读取
    UART_MASTER_CMD_ACT_PARAM_RW_READ_ANY                = 0X84000001,//参数批量读取
    UART_MASTER_CMD_ACT_PARAM_RW_WRITE_SINGLE            = 0X84000002,//参数单个写入
    UART_MASTER_CMD_ACT_PARAM_RW_WRITE_ANY               = 0X84000003,//参数批量写入
    //步进电机1控制
    UART_MASTER_CMD_ACT_STEP_MOTOR1_RESET                = 0X85000000,//步进电机控制复位
    UART_MASTER_CMD_ACT_STEP_MOTOR1_RUN_STEPS            = 0X85000001,//步进电机控制走步数
    UART_MASTER_CMD_ACT_STEP_MOTOR1_RUN_COORDINATE       = 0X85000002,//步进电机控制走坐标
    UART_MASTER_CMD_ACT_STEP_MOTOR1_RETURN_ZERO          = 0X85000003,//步进电机控制回零
    UART_MASTER_CMD_ACT_STEP_MOTOR1_RUN_ALWAYS           = 0X85000004,//步进电机控制持续运转
    UART_MASTER_CMD_ACT_STEP_MOTOR1_STOP_NOW             = 0X85000005,//步进电机控制立即停止
    UART_MASTER_CMD_ACT_STEP_MOTOR1_STOP_SLOW_DOWN       = 0X85000006,//步进电机控制减速运转
    UART_MASTER_CMD_ACT_STEP_MOTOR1_GET_COORDINATE       = 0X85000007,//步进电机控制获取当前坐标
    UART_MASTER_CMD_ACT_STEP_MOTOR1_READ_ORIGIN_SENSOR   = 0X85000008,//步进电机控制获取当前原点传感器状态
    //步进电机2控制
    UART_MASTER_CMD_ACT_STEP_MOTOR2_RESET                = 0X86000000,//步进电机控制复位
    UART_MASTER_CMD_ACT_STEP_MOTOR2_RUN_STEPS            = 0X86000001,//步进电机控制走步数
    UART_MASTER_CMD_ACT_STEP_MOTOR2_RUN_COORDINATE       = 0X86000002,//步进电机控制走坐标
    UART_MASTER_CMD_ACT_STEP_MOTOR2_RETURN_ZERO          = 0X86000003,//步进电机控制回零
    UART_MASTER_CMD_ACT_STEP_MOTOR2_RUN_ALWAYS           = 0X86000004,//步进电机控制持续运转
    UART_MASTER_CMD_ACT_STEP_MOTOR2_STOP_NOW             = 0X86000005,//步进电机控制立即停止
    UART_MASTER_CMD_ACT_STEP_MOTOR2_STOP_SLOW_DOWN       = 0X86000006,//步进电机控制减速运转
    UART_MASTER_CMD_ACT_STEP_MOTOR2_GET_COORDINATE       = 0X86000007,//步进电机控制获取当前坐标
    UART_MASTER_CMD_ACT_STEP_MOTOR2_READ_ORIGIN_SENSOR   = 0X86000008,//步进电机控制获取当前原点传感器状态
    //步进电机3控制
    UART_MASTER_CMD_ACT_STEP_MOTOR3_RESET                = 0X87000000,//步进电机控制复位
    UART_MASTER_CMD_ACT_STEP_MOTOR3_RUN_STEPS            = 0X87000001,//步进电机控制走步数
    UART_MASTER_CMD_ACT_STEP_MOTOR3_RUN_COORDINATE       = 0X87000002,//步进电机控制走坐标
    UART_MASTER_CMD_ACT_STEP_MOTOR3_RETURN_ZERO          = 0X87000003,//步进电机控制回零
    UART_MASTER_CMD_ACT_STEP_MOTOR3_RUN_ALWAYS           = 0X87000004,//步进电机控制持续运转
    UART_MASTER_CMD_ACT_STEP_MOTOR3_STOP_NOW             = 0X87000005,//步进电机控制立即停止
    UART_MASTER_CMD_ACT_STEP_MOTOR3_STOP_SLOW_DOWN       = 0X87000006,//步进电机控制减速运转
    UART_MASTER_CMD_ACT_STEP_MOTOR3_GET_COORDINATE       = 0X87000007,//步进电机控制获取当前坐标
    UART_MASTER_CMD_ACT_STEP_MOTOR3_READ_ORIGIN_SENSOR   = 0X87000008,//步进电机控制获取当前原点传感器状态
    //步进电机4控制
    UART_MASTER_CMD_ACT_STEP_MOTOR4_RESET                = 0X88000000,//步进电机控制复位
    UART_MASTER_CMD_ACT_STEP_MOTOR4_RUN_STEPS            = 0X88000001,//步进电机控制走步数
    UART_MASTER_CMD_ACT_STEP_MOTOR4_RUN_COORDINATE       = 0X88000002,//步进电机控制走坐标
    UART_MASTER_CMD_ACT_STEP_MOTOR4_RETURN_ZERO          = 0X88000003,//步进电机控制回零
    UART_MASTER_CMD_ACT_STEP_MOTOR4_RUN_ALWAYS           = 0X88000004,//步进电机控制持续运转
    UART_MASTER_CMD_ACT_STEP_MOTOR4_STOP_NOW             = 0X88000005,//步进电机控制立即停止
    UART_MASTER_CMD_ACT_STEP_MOTOR4_STOP_SLOW_DOWN       = 0X88000006,//步进电机控制减速运转
    UART_MASTER_CMD_ACT_STEP_MOTOR4_GET_COORDINATE       = 0X88000007,//步进电机控制获取当前坐标
    UART_MASTER_CMD_ACT_STEP_MOTOR4_READ_ORIGIN_SENSOR   = 0X88000008,//步进电机控制获取当前原点传感器状态
    //步进电机5控制
    UART_MASTER_CMD_ACT_STEP_MOTOR5_RESET                = 0X89000000,//步进电机控制复位
    UART_MASTER_CMD_ACT_STEP_MOTOR5_RUN_STEPS            = 0X89000001,//步进电机控制走步数
    UART_MASTER_CMD_ACT_STEP_MOTOR5_RUN_COORDINATE       = 0X89000002,//步进电机控制走坐标
    UART_MASTER_CMD_ACT_STEP_MOTOR5_RETURN_ZERO          = 0X89000003,//步进电机控制回零
    UART_MASTER_CMD_ACT_STEP_MOTOR5_RUN_ALWAYS           = 0X89000004,//步进电机控制持续运转
    UART_MASTER_CMD_ACT_STEP_MOTOR5_STOP_NOW             = 0X89000005,//步进电机控制立即停止
    UART_MASTER_CMD_ACT_STEP_MOTOR5_STOP_SLOW_DOWN       = 0X89000006,//步进电机控制减速运转
    UART_MASTER_CMD_ACT_STEP_MOTOR5_GET_COORDINATE       = 0X89000007,//步进电机控制获取当前坐标
    UART_MASTER_CMD_ACT_STEP_MOTOR5_READ_ORIGIN_SENSOR   = 0X89000008,//步进电机控制获取当前原点传感器状态
    //步进电机6控制
    UART_MASTER_CMD_ACT_STEP_MOTOR6_RESET                = 0X8A000000,//步进电机控制复位
    UART_MASTER_CMD_ACT_STEP_MOTOR6_RUN_STEPS            = 0X8A000001,//步进电机控制走步数
    UART_MASTER_CMD_ACT_STEP_MOTOR6_RUN_COORDINATE       = 0X8A000002,//步进电机控制走坐标
    UART_MASTER_CMD_ACT_STEP_MOTOR6_RETURN_ZERO          = 0X8A000003,//步进电机控制回零
    UART_MASTER_CMD_ACT_STEP_MOTOR6_RUN_ALWAYS           = 0X8A000004,//步进电机控制持续运转
    UART_MASTER_CMD_ACT_STEP_MOTOR6_STOP_NOW             = 0X8A000005,//步进电机控制立即停止
    UART_MASTER_CMD_ACT_STEP_MOTOR6_STOP_SLOW_DOWN       = 0X8A000006,//步进电机控制减速运转
    UART_MASTER_CMD_ACT_STEP_MOTOR6_GET_COORDINATE       = 0X8A000007,//步进电机控制获取当前坐标
    UART_MASTER_CMD_ACT_STEP_MOTOR6_READ_ORIGIN_SENSOR   = 0X8A000008,//步进电机控制获取当前原点传感器状态
    //步进电机7控制
    UART_MASTER_CMD_ACT_STEP_MOTOR7_RESET                = 0X8B000000,//步进电机控制复位
    UART_MASTER_CMD_ACT_STEP_MOTOR7_RUN_STEPS            = 0X8B000001,//步进电机控制走步数
    UART_MASTER_CMD_ACT_STEP_MOTOR7_RUN_COORDINATE       = 0X8B000002,//步进电机控制走坐标
    UART_MASTER_CMD_ACT_STEP_MOTOR7_RETURN_ZERO          = 0X8B000003,//步进电机控制回零
    UART_MASTER_CMD_ACT_STEP_MOTOR7_RUN_ALWAYS           = 0X8B000004,//步进电机控制持续运转
    UART_MASTER_CMD_ACT_STEP_MOTOR7_STOP_NOW             = 0X8B000005,//步进电机控制立即停止
    UART_MASTER_CMD_ACT_STEP_MOTOR7_STOP_SLOW_DOWN       = 0X8B000006,//步进电机控制减速运转
    UART_MASTER_CMD_ACT_STEP_MOTOR7_GET_COORDINATE       = 0X8B000007,//步进电机控制获取当前坐标
    UART_MASTER_CMD_ACT_STEP_MOTOR7_READ_ORIGIN_SENSOR   = 0X8B000008,//步进电机控制获取当前原点传感器状态
    //步进电机8控制
    UART_MASTER_CMD_ACT_STEP_MOTOR8_RESET                = 0X8C000000,//步进电机控制复位
    UART_MASTER_CMD_ACT_STEP_MOTOR8_RUN_STEPS            = 0X8C000001,//步进电机控制走步数
    UART_MASTER_CMD_ACT_STEP_MOTOR8_RUN_COORDINATE       = 0X8C000002,//步进电机控制走坐标
    UART_MASTER_CMD_ACT_STEP_MOTOR8_RETURN_ZERO          = 0X8C000003,//步进电机控制回零
    UART_MASTER_CMD_ACT_STEP_MOTOR8_RUN_ALWAYS           = 0X8C000004,//步进电机控制持续运转
    UART_MASTER_CMD_ACT_STEP_MOTOR8_STOP_NOW             = 0X8C000005,//步进电机控制立即停止
    UART_MASTER_CMD_ACT_STEP_MOTOR8_STOP_SLOW_DOWN       = 0X8C000006,//步进电机控制减速运转
    UART_MASTER_CMD_ACT_STEP_MOTOR8_GET_COORDINATE       = 0X8C000007,//步进电机控制获取当前坐标
    UART_MASTER_CMD_ACT_STEP_MOTOR8_READ_ORIGIN_SENSOR   = 0X8C000008,//步进电机控制获取当前原点传感器状态
    //步进电机9控制
    UART_MASTER_CMD_ACT_STEP_MOTOR9_RESET                = 0X8D000000,//步进电机控制复位
    UART_MASTER_CMD_ACT_STEP_MOTOR9_RUN_STEPS            = 0X8D000001,//步进电机控制走步数
    UART_MASTER_CMD_ACT_STEP_MOTOR9_RUN_COORDINATE       = 0X8D000002,//步进电机控制走坐标
    UART_MASTER_CMD_ACT_STEP_MOTOR9_RETURN_ZERO          = 0X8D000003,//步进电机控制回零
    UART_MASTER_CMD_ACT_STEP_MOTOR9_RUN_ALWAYS           = 0X8D000004,//步进电机控制持续运转
    UART_MASTER_CMD_ACT_STEP_MOTOR9_STOP_NOW             = 0X8D000005,//步进电机控制立即停止
    UART_MASTER_CMD_ACT_STEP_MOTOR9_STOP_SLOW_DOWN       = 0X8D000006,//步进电机控制减速运转
    UART_MASTER_CMD_ACT_STEP_MOTOR9_GET_COORDINATE       = 0X8D000007,//步进电机控制获取当前坐标
    UART_MASTER_CMD_ACT_STEP_MOTOR9_READ_ORIGIN_SENSOR   = 0X8D000008,//步进电机控制获取当前原点传感器状态
    //步进电机10控制
    UART_MASTER_CMD_ACT_STEP_MOTOR10_RESET               = 0X8E000000,//步进电机控制复位
    UART_MASTER_CMD_ACT_STEP_MOTOR10_RUN_STEPS           = 0X8E000001,//步进电机控制走步数
    UART_MASTER_CMD_ACT_STEP_MOTOR10_RUN_COORDINATE      = 0X8E000002,//步进电机控制走坐标
    UART_MASTER_CMD_ACT_STEP_MOTOR10_RETURN_ZERO         = 0X8E000003,//步进电机控制回零
    UART_MASTER_CMD_ACT_STEP_MOTOR10_RUN_ALWAYS          = 0X8E000004,//步进电机控制持续运转
    UART_MASTER_CMD_ACT_STEP_MOTOR10_STOP_NOW            = 0X8E000005,//步进电机控制立即停止
    UART_MASTER_CMD_ACT_STEP_MOTOR10_STOP_SLOW_DOWN      = 0X8E000006,//步进电机控制减速运转
    UART_MASTER_CMD_ACT_STEP_MOTOR10_GET_COORDINATE      = 0X8E000007,//步进电机控制获取当前坐标
    UART_MASTER_CMD_ACT_STEP_MOTOR10_READ_ORIGIN_SENSOR  = 0X8E000008,//步进电机控制获取当前原点传感器状态
    //步进电机11控制
    UART_MASTER_CMD_ACT_STEP_MOTOR11_RESET               = 0X8F000000,//步进电机控制复位
    UART_MASTER_CMD_ACT_STEP_MOTOR11_RUN_STEPS           = 0X8F000001,//步进电机控制走步数
    UART_MASTER_CMD_ACT_STEP_MOTOR11_RUN_COORDINATE      = 0X8F000002,//步进电机控制走坐标
    UART_MASTER_CMD_ACT_STEP_MOTOR11_RETURN_ZERO         = 0X8F000003,//步进电机控制回零
    UART_MASTER_CMD_ACT_STEP_MOTOR11_RUN_ALWAYS          = 0X8F000004,//步进电机控制持续运转
    UART_MASTER_CMD_ACT_STEP_MOTOR11_STOP_NOW            = 0X8F000005,//步进电机控制立即停止
    UART_MASTER_CMD_ACT_STEP_MOTOR11_STOP_SLOW_DOWN      = 0X8F000006,//步进电机控制减速运转
    UART_MASTER_CMD_ACT_STEP_MOTOR11_GET_COORDINATE      = 0X8F000007,//步进电机控制获取当前坐标
    UART_MASTER_CMD_ACT_STEP_MOTOR11_READ_ORIGIN_SENSOR  = 0X8F000008,//步进电机控制获取当前原点传感器状态
    //步进电机12控制
    UART_MASTER_CMD_ACT_STEP_MOTOR12_RESET               = 0X90000000,//步进电机控制复位
    UART_MASTER_CMD_ACT_STEP_MOTOR12_RUN_STEPS           = 0X90000001,//步进电机控制走步数
    UART_MASTER_CMD_ACT_STEP_MOTOR12_RUN_COORDINATE      = 0X90000002,//步进电机控制走坐标
    UART_MASTER_CMD_ACT_STEP_MOTOR12_RETURN_ZERO         = 0X90000003,//步进电机控制回零
    UART_MASTER_CMD_ACT_STEP_MOTOR12_RUN_ALWAYS          = 0X90000004,//步进电机控制持续运转
    UART_MASTER_CMD_ACT_STEP_MOTOR12_STOP_NOW            = 0X90000005,//步进电机控制立即停止
    UART_MASTER_CMD_ACT_STEP_MOTOR12_STOP_SLOW_DOWN      = 0X90000006,//步进电机控制减速运转
    UART_MASTER_CMD_ACT_STEP_MOTOR12_GET_COORDINATE      = 0X90000007,//步进电机控制获取当前坐标
    UART_MASTER_CMD_ACT_STEP_MOTOR12_READ_ORIGIN_SENSOR  = 0X90000008,//步进电机控制获取当前原点传感器状态
    //步进电机13控制
    UART_MASTER_CMD_ACT_STEP_MOTOR13_RESET               = 0X91000000,//步进电机控制复位
    UART_MASTER_CMD_ACT_STEP_MOTOR13_RUN_STEPS           = 0X91000001,//步进电机控制走步数
    UART_MASTER_CMD_ACT_STEP_MOTOR13_RUN_COORDINATE      = 0X91000002,//步进电机控制走坐标
    UART_MASTER_CMD_ACT_STEP_MOTOR13_RETURN_ZERO         = 0X91000003,//步进电机控制回零
    UART_MASTER_CMD_ACT_STEP_MOTOR13_RUN_ALWAYS          = 0X91000004,//步进电机控制持续运转
    UART_MASTER_CMD_ACT_STEP_MOTOR13_STOP_NOW            = 0X91000005,//步进电机控制立即停止
    UART_MASTER_CMD_ACT_STEP_MOTOR13_STOP_SLOW_DOWN      = 0X91000006,//步进电机控制减速运转
    UART_MASTER_CMD_ACT_STEP_MOTOR13_GET_COORDINATE      = 0X91000007,//步进电机控制获取当前坐标
    UART_MASTER_CMD_ACT_STEP_MOTOR13_READ_ORIGIN_SENSOR  = 0X91000008,//步进电机控制获取当前原点传感器状态
    //步进电机14控制
    UART_MASTER_CMD_ACT_STEP_MOTOR14_RESET               = 0X92000000,//步进电机控制复位
    UART_MASTER_CMD_ACT_STEP_MOTOR14_RUN_STEPS           = 0X92000001,//步进电机控制走步数
    UART_MASTER_CMD_ACT_STEP_MOTOR14_RUN_COORDINATE      = 0X92000002,//步进电机控制走坐标
    UART_MASTER_CMD_ACT_STEP_MOTOR14_RETURN_ZERO         = 0X92000003,//步进电机控制回零
    UART_MASTER_CMD_ACT_STEP_MOTOR14_RUN_ALWAYS          = 0X92000004,//步进电机控制持续运转
    UART_MASTER_CMD_ACT_STEP_MOTOR14_STOP_NOW            = 0X92000005,//步进电机控制立即停止
    UART_MASTER_CMD_ACT_STEP_MOTOR14_STOP_SLOW_DOWN      = 0X92000006,//步进电机控制减速运转
    UART_MASTER_CMD_ACT_STEP_MOTOR14_GET_COORDINATE      = 0X92000007,//步进电机控制获取当前坐标
    UART_MASTER_CMD_ACT_STEP_MOTOR14_READ_ORIGIN_SENSOR  = 0X92000008,//步进电机控制获取当前原点传感器状态
    //步进电机15控制
    UART_MASTER_CMD_ACT_STEP_MOTOR15_RESET               = 0X93000000,//步进电机控制复位
    UART_MASTER_CMD_ACT_STEP_MOTOR15_RUN_STEPS           = 0X93000001,//步进电机控制走步数
    UART_MASTER_CMD_ACT_STEP_MOTOR15_RUN_COORDINATE      = 0X93000002,//步进电机控制走坐标
    UART_MASTER_CMD_ACT_STEP_MOTOR15_RETURN_ZERO         = 0X93000003,//步进电机控制回零
    UART_MASTER_CMD_ACT_STEP_MOTOR15_RUN_ALWAYS          = 0X93000004,//步进电机控制持续运转
    UART_MASTER_CMD_ACT_STEP_MOTOR15_STOP_NOW            = 0X93000005,//步进电机控制立即停止
    UART_MASTER_CMD_ACT_STEP_MOTOR15_STOP_SLOW_DOWN      = 0X93000006,//步进电机控制减速运转
    UART_MASTER_CMD_ACT_STEP_MOTOR15_GET_COORDINATE      = 0X93000007,//步进电机控制获取当前坐标
    UART_MASTER_CMD_ACT_STEP_MOTOR15_READ_ORIGIN_SENSOR  = 0X93000008,//步进电机控制获取当前原点传感器状态
    //步进电机16控制
    UART_MASTER_CMD_ACT_STEP_MOTOR16_RESET               = 0X94000000,//步进电机控制复位
    UART_MASTER_CMD_ACT_STEP_MOTOR16_RUN_STEPS           = 0X94000001,//步进电机控制走步数
    UART_MASTER_CMD_ACT_STEP_MOTOR16_RUN_COORDINATE      = 0X94000002,//步进电机控制走坐标
    UART_MASTER_CMD_ACT_STEP_MOTOR16_RETURN_ZERO         = 0X94000003,//步进电机控制回零
    UART_MASTER_CMD_ACT_STEP_MOTOR16_RUN_ALWAYS          = 0X94000004,//步进电机控制持续运转
    UART_MASTER_CMD_ACT_STEP_MOTOR16_STOP_NOW            = 0X94000005,//步进电机控制立即停止
    UART_MASTER_CMD_ACT_STEP_MOTOR16_STOP_SLOW_DOWN      = 0X94000006,//步进电机控制减速运转
    UART_MASTER_CMD_ACT_STEP_MOTOR16_GET_COORDINATE      = 0X94000007,//步进电机控制获取当前坐标
    UART_MASTER_CMD_ACT_STEP_MOTOR16_READ_ORIGIN_SENSOR  = 0X94000008,//步进电机控制获取当前原点传感器状态
    //占位符
    UART_MASTER_CMD_HOLD_PLACE                           = 0XFFFFFFFF,//占位符
}UART_MASTER_CMD;

typedef enum UART_MASTER_REPORT
{
    UART_MASTER_REPORT_HEART_BEAT                        = 0X00000000,//心跳上报ID
    UART_MASTER_REPORT_ERROR                             = 0X000000FF,//错误上报ID
    UART_MASTER_REPORT_HOLD_PLACE                        = 0XFFFFFFFF,//占位符
}UART_MASTER_REPORT;


#endif












