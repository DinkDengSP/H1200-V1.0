#ifndef _CAN_RAIL_MULTI_MACRO_H_
#define _CAN_RAIL_MULTI_MACRO_H_

#include "CanProtMain.h"
#include "CanProtUtil.h"
#include "CanProtBase.h"

//多联机轨道定义
typedef enum CAN_RAIL_MUTIL_CH{
    //多联机轨道
    CAN_RAIL_MUTIL_CH_DEVICE_MAIN_TEST              = 0X15,//多联机轨道，设备主控通道
    //下位机板1
    CAN_RAIL_MUTIL_CH_SUB_BOARD1_SAMPLE_HAND        = 0X20,//多联机轨道，样本机械手
    CAN_RAIL_MUTIL_CH_SUB_BOARD1_ROTATE_SCAN        = 0X21,//多联机轨道，旋转扫码
    CAN_RAIL_MUTIL_CH_SUB_BOARD1_ENTRY_CHANGE       = 0X22,//多联机轨道，输入变轨  
    //下位机板2
    CAN_RAIL_MUTIL_CH_SUB_BOARD2_ONLINE_RACK_IN     = 0X20,//多联机轨道，联机进架
    CAN_RAIL_MUTIL_CH_SUB_BOARD2_ONLINE_RACK_OUT    = 0X21,//多联机轨道，联机出架
    //下位机轨道
    CAN_RAIL_MUTIL_CH_SUB_BOARD3_6_EMER_RAIL        = 0X20,//多联机轨道，下位机3~6，急诊轨道
    CAN_RAIL_MUTIL_CH_SUB_BOARD3_6_NORMAL_RAIL      = 0X21,//多联机轨道，下位机3~6，常规轨道
    CAN_RAIL_MUTIL_CH_SUB_BOARD3_6_RECYCLE_RAIL     = 0X22,//多联机轨道，下位机3~6，回收轨道
    CAN_RAIL_MUTIL_CH_SUB_BOARD3_6_RAIL_CHANGE      = 0X23,//多联机轨道，下位机3~6，轨道变轨
}CAN_RAIL_MUTIL_CH;


//多联机轨道定义
typedef enum CAN_RAIL_MUTIL_CMD{
   //转运机械手
    CAN_RAIL_MUTIL_CMD_BOARD1_SAMPLE_HAND_READ_INPUT                                  = 0X00000000,//转运机械手 输入读取
    CAN_RAIL_MUTIL_CMD_BOARD1_SAMPLE_HAND_WRITE_OUTPUT                                = 0X00000001,//转运机械手 输出写入
    CAN_RAIL_MUTIL_CMD_BOARD1_SAMPLE_HAND_MOTOR_RESET                                 = 0X00000002,//转运机械手 电机复位
    CAN_RAIL_MUTIL_CMD_BOARD1_SAMPLE_HAND_MOTOR_RUN_STEP                              = 0X00000003,//转运机械手 电机走步数
    CAN_RAIL_MUTIL_CMD_BOARD1_SAMPLE_HAND_RESET_ALL                                   = 0X00000004,//转运机械手 复位
    CAN_RAIL_MUTIL_CMD_BOARD1_SAMPLE_HAND_MOVE_TO_TARGET_POS                          = 0X00000005,//转运机械手 移动到指定位置
    CAN_RAIL_MUTIL_CMD_BOARD1_SAMPLE_HAND_RACK_EXIST_SCAN                             = 0X00000006,//转运机械手 样本仓扫描
    CAN_RAIL_MUTIL_CMD_BOARD1_SAMPLE_HAND_CATCH_TARGET_RACK                           = 0X00000007,//转运机械手 抓取指定位置样本架
    CAN_RAIL_MUTIL_CMD_BOARD1_SAMPLE_HAND_PUT_TARGET_RACK                             = 0X00000008,//转运机械手 样本架放回指定位置样本架
    CAN_RAIL_MUTIL_CMD_BOARD1_SAMPLE_HAND_CATCH_TRAY_PUT_TARGET_RACK                  = 0X00000009,//转运机械手 抓取样本仓样本架放到指定位置
    CAN_RAIL_MUTIL_CMD_BOARD1_SAMPLE_HAND_CATCH_TARGET_RACK_PUT_TRAY                  = 0X0000000A,//转运机械手 抓取指定位置样本架放到样本仓
    CAN_RAIL_MUTIL_CMD_BOARD1_SAMPLE_HAND_CATCH_TARGET_PUT_TARGET_RACK                = 0X0000000B,//转运机械手 抓取指定位置样本架放到指定位置，非样本仓
    CAN_RAIL_MUTIL_CMD_BOARD1_SAMPLE_HAND_CATCH_ROTATE_PUT_ENTRY_CHANGE_SCAN          = 0X0000000C,//转运机械手 抓取旋转扫码试管架放入输入变轨并扫码
    CAN_RAIL_MUTIL_CMD_BOARD1_SAMPLE_HAND_RACK_PUT_ENTRY_CHANGE_SCAN                  = 0X0000000D,//转运机械手 手上试管架放入输入变轨并扫码
    CAN_RAIL_MUTIL_CMD_BOARD1_SAMPLE_HAND_OPEN_OR_CLOSE_SCAN                          = 0X0000000E,//转运机械手 扫码枪控制

    //旋转扫码
    CAN_RAIL_MUTIL_CMD_BOARD1_ROTATE_SCAN_READ_INPUT                                 = 0X00000000,//旋转扫码 输入读取
    CAN_RAIL_MUTIL_CMD_BOARD1_ROTATE_SCAN_WRITE_OUTPUT                               = 0X00000001,//旋转扫码 输出写入
    CAN_RAIL_MUTIL_CMD_BOARD1_ROTATE_SCAN_MOTOR_RESET                                = 0X00000002,//旋转扫码 电机复位
    CAN_RAIL_MUTIL_CMD_BOARD1_ROTATE_SCAN_MOTOR_RUN_STEP                             = 0X00000003,//旋转扫码 电机走步数
    CAN_RAIL_MUTIL_CMD_BOARD1_ROTATE_SCAN_RESET_ALL                                  = 0X00000004,//旋转扫码 复位
    CAN_RAIL_MUTIL_CMD_BOARD1_ROTATE_SCAN_PREPARE_RECV_RACK                          = 0X00000005,//旋转扫码 准备接收试管架
    CAN_RAIL_MUTIL_CMD_BOARD1_ROTATE_SCAN_RACK_SCAN_ROTATE_TARGET_DIR                = 0X00000006,//旋转扫码 样本架扫码并旋转到指定方向
    CAN_RAIL_MUTIL_CMD_BOARD1_ROTATE_SCAN_RACK_ROTATE_AND_SCAN                       = 0X00000005,//旋转扫码 样本架扫码并旋转
    //输入变轨
    CAN_RAIL_MUTIL_CMD_BOARD1_ENTRY_CHANGE_READ_INPUT                                 = 0X00000000,//输入变轨 输入读取
    CAN_RAIL_MUTIL_CMD_BOARD1_ENTRY_CHANGE_WRITE_OUTPUT                               = 0X00000001,//输入变轨 输出写入
    CAN_RAIL_MUTIL_CMD_BOARD1_ENTRY_CHANGE_MOTOR_RESET                                = 0X00000002,//输入变轨 电机复位
    CAN_RAIL_MUTIL_CMD_BOARD1_ENTRY_CHANGE_MOTOR_RUN_STEP                             = 0X00000003,//输入变轨 电机走步数
    CAN_RAIL_MUTIL_CMD_BOARD1_ENTRY_CHANGE_RESET_ALL                                  = 0X00000004,//输入变轨 复位
    CAN_RAIL_MUTIL_CMD_BOARD1_ENTRY_CHANGE_MOVE_TO_TARGET_POS                         = 0X00000005,//输入变轨 移动到指定位置
    CAN_RAIL_MUTIL_CMD_BOARD1_ENTRY_CHANGE_PUSH_RACK_TO_NORMAL_RAIL                   = 0X00000006,//输入变轨 推送样本架到常规轨道
    CAN_RAIL_MUTIL_CMD_BOARD1_ENTRY_CHANGE_PUSH_RACK_TO_EM_RAIL                       = 0X00000007,//输入变轨 推送样本架到急诊轨道
    CAN_RAIL_MUTIL_CMD_BOARD1_ENTRY_CHANGE_PUSH_RACK_TO_SAMPLE_HAND                   = 0X00000008,//输入变轨 推送样本架到转运机械手
    CAN_RAIL_MUTIL_CMD_BOARD1_ENTRY_CHANGE_RECEIVE_SAMPLE_HAND_RACK                   = 0X00000009,//输入变轨 接收转运机械手架子
    CAN_RAIL_MUTIL_CMD_BOARD1_ENTRY_CHANGE_PREPARE_TO_RECEIVE_BACK_RAIL_RACK          = 0X0000000A,//输入变轨 准备接收回收轨道样本架
    CAN_RAIL_MUTIL_CMD_BOARD1_ENTRY_CHANGE_RECEIVE_BACK_RAIL_RACK                     = 0X0000000B,//输入变轨 接收回收轨道样本架
    CAN_RAIL_MUTIL_CMD_BOARD1_ENTRY_CHANGE_RELEASE_RACK                               = 0X0000000C,//输入变轨 释放架子
    CAN_RAIL_MUTIL_CMD_BOARD1_ENTRY_CHANGE_IS_EMPTY                                   = 0X0000000D,//输入变轨 有无试管架 

    //联机接口进样
    CAN_RAIL_MUTIL_CMD_BOARD2_ONLINE_RACK_IN_READ_INPUT                               = 0X00000000,//联机接口进样 输入读取
    CAN_RAIL_MUTIL_CMD_BOARD2_ONLINE_RACK_IN_WRITE_OUTPUT                             = 0X00000001,//联机接口进样 输出写入
    CAN_RAIL_MUTIL_CMD_BOARD2_ONLINE_RACK_IN_MOTOR_RESET                              = 0X00000002,//联机接口进样 电机复位
    CAN_RAIL_MUTIL_CMD_BOARD2_ONLINE_RACK_IN_MOTOR_RUN_STEP                           = 0X00000003,//联机接口进样 电机走步数
    CAN_RAIL_MUTIL_CMD_BOARD2_ONLINE_RACK_IN_RESET_ALL                                = 0X00000004,//联机接口进样 复位
    CAN_RAIL_MUTIL_CMD_BOARD2_ONLINE_RACK_IN_RELEASE_CONTROL                          = 0X00000005,//联机接口进样 放行控制
    //联机接口出样
    CAN_RAIL_MUTIL_CMD_BOARD2_ONLINE_RACK_OUT_READ_INPUT                              = 0X00000000,//联机接口出样 输入读取
    CAN_RAIL_MUTIL_CMD_BOARD2_ONLINE_RACK_OUT_WRITE_OUTPUT                            = 0X00000001,//联机接口出样 输出写入
    CAN_RAIL_MUTIL_CMD_BOARD2_ONLINE_RACK_OUT_MOTOR_RESET                             = 0X00000002,//联机接口出样 电机复位
    CAN_RAIL_MUTIL_CMD_BOARD2_ONLINE_RACK_OUT_MOTOR_RUN_STEP                          = 0X00000003,//联机接口出样 电机走步数
    CAN_RAIL_MUTIL_CMD_BOARD2_ONLINE_RACK_OUT_RESET_ALL                               = 0X00000004,//联机接口出样 复位
    CAN_RAIL_MUTIL_CMD_BOARD2_ONLINE_RACK_OUT_RELEASE                                 = 0X00000005,//联机接口出样 出样放行
    //轨道模块1~4 
   //急诊轨道
    CAN_RAIL_MUTIL_CMD_BOARD3_6_EMER_RAIL_READ_INPUT                             = 0X00000000,//急诊轨道 输入读取
    CAN_RAIL_MUTIL_CMD_BOARD3_6_EMER_RAIL_WRITE_OUTPUT                           = 0X00000001,//急诊轨道 输出写入
    CAN_RAIL_MUTIL_CMD_BOARD3_6_EMER_RAIL_MOTOR_RESET                            = 0X00000002,//急诊轨道 电机复位
    CAN_RAIL_MUTIL_CMD_BOARD3_6_EMER_RAIL_MOTOR_RUN_STEP                         = 0X00000003,//急诊轨道 电机走步数
    CAN_RAIL_MUTIL_CMD_BOARD3_6_EMER_RAIL_RESET_ALL                              = 0X00000004,//急诊轨道 复位
    CAN_RAIL_MUTIL_CMD_BOARD3_6_EMER_RAIL_FLUSH_SENSOR_STATE                     = 0X00000005,//急诊轨道 传感器状态刷新
    CAN_RAIL_MUTIL_CMD_BOARD3_6_EMER_RAIL_MOVE_TO_TARGET_ABSORB_POS              = 0X00000006,//急诊轨道 移动到指定吸样位
    CAN_RAIL_MUTIL_CMD_BOARD3_6_EMER_RAIL_RACK_MOVE_IN                           = 0X00000007,//急诊轨道 架子预进入
    CAN_RAIL_MUTIL_CMD_BOARD3_6_EMER_RAIL_RACK_MOVE_TO_TEST                      = 0X00000008,//急诊轨道 架子进入测试区
    CAN_RAIL_MUTIL_CMD_BOARD3_6_EMER_RAIL_RACK_MOVE                              = 0X00000009,//急诊轨道 架子放行
    CAN_RAIL_MUTIL_CMD_BOARD3_6_EMER_RAIL_PUSH_RACK_TO_RAIL_CHANGE               = 0X0000000A,//急诊轨道 放行一个架子到变轨
    //常规轨道
    CAN_RAIL_MUTIL_CMD_BOARD3_6_NORMAL_RAIL_READ_INPUT                           = 0X00000000,//常规轨道 输入读取
    CAN_RAIL_MUTIL_CMD_BOARD3_6_NORMAL_RAIL_WRITE_OUTPUT                         = 0X00000001,//常规轨道 输出写入
    CAN_RAIL_MUTIL_CMD_BOARD3_6_NORMAL_RAIL_MOTOR_RESET                          = 0X00000002,//常规轨道 电机复位
    CAN_RAIL_MUTIL_CMD_BOARD3_6_NORMAL_RAIL_MOTOR_RUN_STEP                       = 0X00000003,//常规轨道 电机走步数
    CAN_RAIL_MUTIL_CMD_BOARD3_6_NORMAL_RAIL_RESET_ALL                            = 0X00000004,//常规轨道 复位
    CAN_RAIL_MUTIL_CMD_BOARD3_6_NORMAL_RAIL_FLUSH_SENSOR_STATE                   = 0X00000005,//常规轨道 传感器状态刷新
    CAN_RAIL_MUTIL_CMD_BOARD3_6_NORMAL_RAIL_MOVE_TO_TARGET_ABSORB_POS            = 0X00000006,//常规轨道 移动到指定吸样位
    CAN_RAIL_MUTIL_CMD_BOARD3_6_NORMAL_RAIL_RACK_MOVE_IN                         = 0X00000007,//常规轨道 架子预进入
    CAN_RAIL_MUTIL_CMD_BOARD3_6_NORMAL_RAIL_RACK_MOVE_TO_BUFFER                  = 0X00000008,//常规轨道 架子移动到缓存
    CAN_RAIL_MUTIL_CMD_BOARD3_6_NORMAL_RAIL_RACK_MOVE                            = 0X00000009,//常规轨道 架子放行
    CAN_RAIL_MUTIL_CMD_BOARD3_6_NORMAL_RAIL_PUSH_RACK_TO_RAIL_CHANGE             = 0X0000000A,//急诊轨道 放行一个架子到变轨
    //回收轨道
    CAN_RAIL_MUTIL_CMD_BOARD3_6_RECYCLE_RAIL_READ_INPUT                          = 0X00000000,//回收轨道 输入读取
    CAN_RAIL_MUTIL_CMD_BOARD3_6_RECYCLE_RAIL_WRITE_OUTPUT                        = 0X00000001,//回收轨道 输出写入
    CAN_RAIL_MUTIL_CMD_BOARD3_6_RECYCLE_RAIL_MOTOR_RESET                         = 0X00000002,//回收轨道 电机复位
    CAN_RAIL_MUTIL_CMD_BOARD3_6_RECYCLE_RAIL_MOTOR_RUN_STEP                      = 0X00000003,//回收轨道 电机走步数
    CAN_RAIL_MUTIL_CMD_BOARD3_6_RECYCLE_RAIL_RESET_ALL                           = 0X00000004,//回收轨道 复位
    CAN_RAIL_MUTIL_CMD_BOARD3_6_RECYCLE_RAIL_MOVE_RACK_TO_EXIT                   = 0X00000005,//回收轨道 回收架子到出口
    CAN_RAIL_MUTIL_CMD_BOARD3_6_RECYCLE_RAIL_RECEIVE_RACK                        = 0X00000006,//回收轨道 接收架子
    CAN_RAIL_MUTIL_CMD_BOARD3_6_RECYCLE_RAIL_PUSH_RACK_TO_NEXT                   = 0X00000007,//回收轨道 推送架子到下一模块

    //轨道换轨
    CAN_RAIL_MUTIL_CMD_BOARD3_6_RAIL_CHANGE_READ_INPUT                            = 0X00000000,//轨道换轨 输入读取
    CAN_RAIL_MUTIL_CMD_BOARD3_6_RAIL_CHANGE_WRITE_OUTPUT                          = 0X00000001,//轨道换轨 输出写入
    CAN_RAIL_MUTIL_CMD_BOARD3_6_RAIL_CHANGE_MOTOR_RESET                           = 0X00000002,//轨道换轨 电机复位
    CAN_RAIL_MUTIL_CMD_BOARD3_6_RAIL_CHANGE_MOTOR_RUN_STEP                        = 0X00000003,//轨道换轨 电机走步数
    CAN_RAIL_MUTIL_CMD_BOARD3_6_RAIL_CHANGE_RESET_ALL                             = 0X00000004,//轨道换轨 复位
    CAN_RAIL_MUTIL_CMD_BOARD3_6_RAIL_CHANGE_MOVE_TO_TARGET_RAIL                   = 0X00000005,//轨道换轨 移动到指定轨道
    CAN_RAIL_MUTIL_CMD_BOARD3_6_RAIL_CHANGE_RELEASE_CONTROL                       = 0X00000006,//轨道换轨 放行控制
    CAN_RAIL_MUTIL_CMD_BOARD3_6_RAIL_CHANGE_PUSH_RACK_TO_TARGET_RAIL              = 0X00000007,//轨道换轨 推送样本架
    
    //轨道主控命令
    CAN_RAIL_MUTIL_CMD_DEVICE_MAIN_TUBE_IN_POS                                  = 0X00000001,//免疫主控 试管到位
    CAN_RAIL_MUTIL_CMD_DEVICE_MAIN_TUBE_MOV                                     = 0X00000002,//免疫主控 试管移动
    CAN_RAIL_MUTIL_CMD_DEVICE_MAIN_TUBE_MOV_ARRIVE                              = 0X00000003,//免疫主控 试管移动到位
    CAN_RAIL_MUTIL_CMD_DEVICE_MAIN_RAIL_TUBE_DUMP                               = 0X00000004,//免疫主控 轨道试管置废
    CAN_RAIL_MUTIL_CMD_DEVICE_MAIN_DEVICE_TUBE_DUMP                             = 0X00000005,//免疫主控 设备试管置废
    CAN_RAIL_MUTIL_CMD_DEVICE_MAIN_RAIL_STATE_INFORM                            = 0X00000006,//免疫主控 轨道状态通知   
    CAN_RAIL_MUTIL_CMD_DEVICE_MAIN_DEVICE_STATE_INFORM                          = 0X00000007,//免疫主控 设备状态通知   
    CAN_RAIL_MUTIL_CMD_DEVICE_MAIN_RAIL_CHANGE_LOCK                             = 0X00000008,//免疫主控 轨道变轨锁定
    CAN_RAIL_MUTIL_CMD_DEVICE_MAIN_RAIL_CHANGE_LOCK_RESULT                      = 0X00000009,//免疫主控 轨道变轨锁定结果
    CAN_RAIL_MUTIL_CMD_DEVICE_MAIN_INQUIRE_NEEDLE_POS                           = 0X0000000A,//免疫主控 样本针状态查询
}CAN_RAIL_MUTIL_CMD;

//多联机下位机
#define CAN_MODULE_NO_MULTI_RAIL_ACT1                       0X06
#define CAN_BOARD_ID_MULTI_RAIL_ACT1                        0x01

#define CAN_MODULE_NO_MULTI_RAIL_ACT2                       0X06
#define CAN_BOARD_ID_MULTI_RAIL_ACT2                        0x02

#define CAN_MODULE_NO_MULTI_RAIL_ACT3                       0X06
#define CAN_BOARD_ID_MULTI_RAIL_ACT3                        0x03

#define CAN_MODULE_NO_MULTI_RAIL_ACT4                       0X06
#define CAN_BOARD_ID_MULTI_RAIL_ACT4                        0x04

#define CAN_MODULE_NO_MULTI_RAIL_ACT5                       0X06
#define CAN_BOARD_ID_MULTI_RAIL_ACT5                        0x05

#define CAN_MODULE_NO_MULTI_RAIL_ACT6                       0X06
#define CAN_BOARD_ID_MULTI_RAIL_ACT6                        0x06

#endif // _CAN_RAIL_MULIL_MACRO_H_

