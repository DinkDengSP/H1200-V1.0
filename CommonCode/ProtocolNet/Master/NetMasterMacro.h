/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-10-19 10:08:51 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2024-01-04 12:58:00 +0800
************************************************************************************************/ 
#ifndef __NET_MASTER_MACRO_H_
#define __NET_MASTER_MACRO_H_
#include "NetProtMain.h"
#include "NetProtUtil.h"
#include "NetProtBase.h"

//指令码
typedef enum NET_MASTER_CMD
{
    //主控APP指令集
    NET_MASTER_CMD_MAIN_UTIL_ONLINE                                                 = 0X00000000,//联机
    NET_MASTER_CMD_MAIN_UTIL_OFFLINE                                                = 0X00000001,//脱机
    NET_MASTER_CMD_MAIN_UTIL_UTC_SYNC                                               = 0X00000002,//UTC时间同步
    NET_MASTER_CMD_MAIN_UTIL_HEART_GET                                              = 0X00000003,//心跳获取
    NET_MASTER_CMD_MAIN_UTIL_EM_STOP                                                = 0X00000004,//急停
    NET_MASTER_CMD_MAIN_UTIL_GET_VERSION                                            = 0X00000005,//版本号获取
    //在线升级                          
    NET_MASTER_CMD_MAIN_IAP_PREPARE                                                 = 0X00010000,//在线升级准备
    NET_MASTER_CMD_MAIN_IAP_START_MSG                                               = 0X00010001,//在线升级启动信息
    NET_MASTER_CMD_MAIN_IAP_DATA_TRANS                                              = 0X00010002,//在线升级数据传输
    NET_MASTER_CMD_MAIN_IAP_DATA_CHECK                                              = 0X00010003,//在线升级数据校验
    NET_MASTER_CMD_MAIN_IAP_FORCE_JMP                                               = 0X00010004,//在线升级强制跳转
    //调试辅助                          
    NET_MASTER_CMD_MAIN_PORT_IN_READ_SINGLE                                         = 0X00020000,//输入单个读取
    NET_MASTER_CMD_MAIN_PORT_IN_READ_RANGE                                          = 0X00020001,//输入区间读取
    NET_MASTER_CMD_MAIN_PORT_IN_READ_ANY                                            = 0X00020002,//输入批量读取
    NET_MASTER_CMD_MAIN_PORT_OUT_WRITE_SINGLE                                       = 0X00020003,//输出单个写入
    NET_MASTER_CMD_MAIN_PORT_OUT_WRITE_ANY                                          = 0X00020004,//输出批量写入
    NET_MASTER_CMD_MAIN_PARAM_RW_READ_SINGLE                                        = 0X00020005,//参数单个读取
    NET_MASTER_CMD_MAIN_PARAM_RW_READ_ANY                                           = 0X00020006,//参数批量读取
    NET_MASTER_CMD_MAIN_PARAM_RW_WRITE_SINGLE                                       = 0X00020007,//参数单个写入
    NET_MASTER_CMD_MAIN_PARAM_RW_WRITE_ANY                                          = 0X00020008,//参数批量写入
    /*-------------------------------------------------H360应用层指令--------------------------------------------------*/
    NET_MASTER_CMD_H360_FLOW_TEST_PARAM                                             = 0X01000000,// 测试参数
    NET_MASTER_CMD_H360_FLOW_READ_MODULE_INFO                                       = 0X01000001,// 读模块信息
    NET_MASTER_CMD_H360_FLOW_SYSTEM_RESET                                           = 0X01000002,// 系统复位
    NET_MASTER_CMD_H360_FLOW_EM_STOP                                                = 0X01000003,// 急停
    NET_MASTER_CMD_H360_FLOW_REAGENT_QUANTITY_POS_SET                               = 0X01000100,// 试剂余量扫描位置设置
    NET_MASTER_CMD_H360_FLOW_REAGENT_QUANTITY                                       = 0X01000101,// 试剂余量扫描
    NET_MASTER_CMD_H360_FLOW_START_TEST                                             = 0X01000102,// 开始测试
    NET_MASTER_CMD_H360_FLOW_M_BEAD_SEPARATE                                        = 0X01000103,// 磁分离
    NET_MASTER_CMD_H360_FLOW_EMPTY_CUP_TEST                                         = 0X01000104,// 空杯测试
    NET_MASTER_CMD_H360_FLOW_MANUAL_TEST                                            = 0X01000105,// 手动测试         
    NET_MASTER_CMD_H360_FLOW_DARK_NOISE_TEST                                        = 0X01000106,// 暗噪声测试
    NET_MASTER_CMD_H360_FLOW_NEEDLE_CLEAN                                           = 0X01000107,// 针清洗
    NET_MASTER_CMD_H360_FLOW_LIQUID_PRIME                                           = 0X01000108,// 液路灌注
    NET_MASTER_CMD_H360_FLOW_NEEDLE_ACCURACY                                        = 0X01000109,// 三针精度测试
    NET_MASTER_CMD_H360_FLOW_MACHINE_RESET                                          = 0X0100010A,// 机械复位
    NET_MASTER_CMD_H360_FLOW_WASH_ACCURACY                                          = 0X0100010B,// 清洗针精度
    NET_MASTER_CMD_H360_FLOW_NEEDLE_LIGHT_CHECK                                     = 0X0100010C,// 针光检
    NET_MASTER_CMD_H360_FLOW_BACK_TEST                                              = 0X0100010D,// 本底测试
    NET_MASTER_CMD_H360_FLOW_A_ACCURACY                                             = 0X0100010E,// A液针精度测试
    NET_MASTER_CMD_H360_FLOW_B_ACCURACY                                             = 0X0100010F,// B液针精度测试
    NET_MASTER_CMD_H360_FLOW_NEEDLE_MAINTAIN                                        = 0X01000110,// 针维护
    NET_MASTER_CMD_H360_FLOW_MOVE_CUP_TEST                                          = 0X01000111,// 走杯测试
    NET_MASTER_CMD_H360_FLOW_NEEDLE_CALIBRATE                                       = 0X01000112,// 三针校准
    NET_MASTER_CMD_H360_FLOW_CUP_STACK_SWITCH                                       = 0X01000113,// 切换杯盘测试
    NET_MASTER_CMD_H360_FLOW_MEASURE_MODULE_CUP_TEST                                = 0X01000114,// 测量室取放杯测试
    NET_MASTER_CMD_H360_FLOW_SAMPLE_DILUTE                                          = 0X01000115,// 样本稀释流程
    NET_MASTER_CMD_H360_FLOW_WASHNEEDLE_SOAK                                        = 0X01000116,// 清洗针浸泡
    NET_MASTER_CMD_H360_FLOW_ABANDON_CUP                                            = 0X01000117,// 清杯
    NET_MASTER_CMD_H360_FLOW_REAGENT_BARCODE_SCAN                                   = 0X01000118,// 试剂扫码
    NET_MASTER_CMD_H360_FLOW_TEST_RECOVER                                           = 0X01000119,// 测试恢复加样
    NET_MASTER_CMD_H360_FLOW_MULT_REAGENT                                           = 0X0100011A,// 试剂盘多试剂位信息
    NET_MASTER_CMD_H360_FLOW_SAMPLE_SOLUTION                                        = 0X0100011B,// 加样方案
    NET_MASTER_CMD_H360_FLOW_COMPENSATE_VALUE                                       = 0X0100011C,// 样本、试剂、磁珠补偿值
    NET_MASTER_CMD_H360_FLOW_REAGENT_RESERVE                                        = 0X0100011D,// 试剂预留次数
    NET_MASTER_CMD_H360_FLOW_SAMPLE_ENABLE                                          = 0X0100011E,// 加样继续
    NET_MASTER_CMD_H360_FLOW_SAMPLE_PAUSE                                           = 0X0100011F,// 加样暂停
    NET_MASTER_CMD_H360_FLOW_RESET_GARBAGE_COUNT                                    = 0X01000120,// 重置废杯桶计数
    NET_MASTER_CMD_H360_FLOW_STANDBY_UPDATE                                         = 0X01000121,// 更新待机时长
    NET_MASTER_CMD_H360_FLOW_PARAM_UPDATE_IN_TESTING                                = 0X01000122,// 测试中更新的参数
    NET_MASTER_CMD_H360_FLOW_TEST_ITEM                                              = 0X01000123,// 测试项目
    NET_MASTER_CMD_H360_FLOW_TEST_ITEM_SEND_OVER                                    = 0X01000124,// 测试项目下发完成     
    NET_MASTER_CMD_H360_FLOW_FORCE_GARBAGE_SWTICH                                   = 0X01000125,// 强制垃圾桶切换
    NET_MASTER_CMD_H360_FLOW_REAGENT_STATE                                          = 0X01000126,// 试剂状态
    NET_MASTER_CMD_H360_FLOW_NORMAL_RAIL_RECOVER                                    = 0X01000127,// 常规轨道恢复
    NET_MASTER_CMD_H360_FLOW_EM_RAIL_RECOVER                                        = 0X01000128,// 急诊轨道恢复
    NET_MASTER_CMD_H360_FLOW_SAMPLE_NEEDLE_RESET                                    = 0X01000129,// 样本针复位
    /*-------------------------------------------------H360调试层指令--------------------------------------------------*/
    //杯栈管理机构调试指令
    NET_MASTER_CMD_H360_DEBUG_CUP_STACK_MOTOR_RESET                                 = 0X01000200,//模块电机复位
    NET_MASTER_CMD_H360_DEBUG_CUP_STACK_MOTOR_RUN_STEP                              = 0X01000201,//模块电机运转  
    NET_MASTER_CMD_H360_DEBUG_CUP_STACK_INPUT_READ                                  = 0X01000202,//模块信号读取
    NET_MASTER_CMD_H360_DEBUG_CUP_STACK_OUTPUT_WRITE                                = 0X01000203,//模块输出写入
    NET_MASTER_CMD_H360_DEBUG_CUP_STACK_QUERY_STATE                                 = 0X01000204,//查询门状态
    NET_MASTER_CMD_H360_DEBUG_CUP_STACK_INIT_NEW_STACK                              = 0X01000205,//新杯盘堆栈初始化
    NET_MASTER_CMD_H360_DEBUG_CUP_STACK_INIT_EMPTY_STACK                            = 0X01000206,//空杯盘堆栈初始化
    NET_MASTER_CMD_H360_DEBUG_CUP_STACK_INIT_PUSH                                   = 0X01000207,//推手初始化
    NET_MASTER_CMD_H360_DEBUG_CUP_STACK_INIT_CLAMP                                  = 0X01000208,//夹手初始化
    NET_MASTER_CMD_H360_DEBUG_CUP_STACK_INIT_ALL                                    = 0X01000209,//全部初始化
    NET_MASTER_CMD_H360_DEBUG_CUP_STACK_CLAMP_OPEN                                  = 0X0100020A,//夹手打开
    NET_MASTER_CMD_H360_DEBUG_CUP_STACK_CLAMP_CLOSE                                 = 0X0100020B,//夹手关闭
    NET_MASTER_CMD_H360_DEBUG_CUP_STACK_UPLOAD_NEW_TRAY                             = 0X0100020C,//新杯盘堆栈上传一个新杯盘
    NET_MASTER_CMD_H360_DEBUG_CUP_STACK_RECOVERY_EMPTY_TRAY                         = 0X0100020D,//空杯盘堆栈回收一个空杯盘
    NET_MASTER_CMD_H360_DEBUG_CUP_STACK_PUSH_TRAY_2_TESTING                         = 0X0100020E,//推手推一个新杯盘到测试位
    NET_MASTER_CMD_H360_DEBUG_CUP_STACK_GARBAGE_LOCK_UP                             = 0X0100020F,//垃圾桶锁定,参数决定1还是2
    NET_MASTER_CMD_H360_DEBUG_CUP_STACK_GARBAGE_RELEASE_DOWN                        = 0X01000210,//垃圾桶解锁,参数决定1还是2
    NET_MASTER_CMD_H360_DEBUG_CUP_STACK_NEW_TRAY_UP_WHILE_OPENDOOR_INPLACE          = 0X01000211,//开门到位之后新杯盘堆栈上升指定层
    NET_MASTER_CMD_H360_DEBUG_CUP_STACK_NEW_TRAY_RETURN_ZERO                        = 0X01000212,//新杯盘堆栈回零
    NET_MASTER_CMD_H360_DEBUG_CUP_STACK_EMPTY_TRAY_RISEUP_WHILE_OPENDOOR_INPLACE    = 0X01000213,//开门到位后空杯盘上升指定层
    NET_MASTER_CMD_H360_DEBUG_CUP_STACK_EMPTY_TRAY_RETURN_ZERO                      = 0X01000214,//空杯盘堆栈回零
    //新杯机械手调试指令    
    NET_MASTER_CMD_H360_DEBUG_HAND_NEW_CUP_MOTOR_RESET                              = 0X01000300,//模块电机复位
    NET_MASTER_CMD_H360_DEBUG_HAND_NEW_CUP_MOTOR_RUN_STEP                           = 0X01000301,//模块电机运转
    NET_MASTER_CMD_H360_DEBUG_HAND_NEW_CUP_INPUT_READ                               = 0X01000302,//模块信号读取
    NET_MASTER_CMD_H360_DEBUG_HAND_NEW_CUP_OUTPUT_WRITE                             = 0X01000303,//模块输出写入
    NET_MASTER_CMD_H360_DEBUG_HAND_NEW_CUP_RESET_ALL                                = 0X01000304,//新杯机械手复位
    NET_MASTER_CMD_H360_DEBUG_HAND_NEW_CUP_MOVE_2_NEW_STACK                         = 0X01000305,//新杯机械手移动到新杯盘,行列号给出
    NET_MASTER_CMD_H360_DEBUG_HAND_NEW_CUP_MOVE_2_PLATE_REACTION                    = 0X01000306,//新杯机械手移动到反应盘
    NET_MASTER_CMD_H360_DEBUG_HAND_NEW_CUP_MOVE_2_GARBAGE                           = 0X01000307,//新杯机械手移动到垃圾桶,垃圾桶号给出
    NET_MASTER_CMD_H360_DEBUG_HAND_NEW_CUP_CATCH_NEW_STACK                          = 0X01000308,//新杯机械手从新杯盘抓杯
    NET_MASTER_CMD_H360_DEBUG_HAND_NEW_CUP_CATCH_PLATE_REACTION                     = 0X01000309,//新杯机械手从反应盘抓杯
    NET_MASTER_CMD_H360_DEBUG_HAND_NEW_CUP_PUT_PLATE_REACTION                       = 0X0100030A,//新杯机械手往反应盘放杯
    NET_MASTER_CMD_H360_DEBUG_HAND_NEW_CUP_PUT_GARBAGE                              = 0X0100030B,//新杯机械手往垃圾桶放杯
    //清洗机械手调试指令    
    NET_MASTER_CMD_H360_DEBUG_HAND_WASH_MOTOR_RESET                                 = 0X01000400,//模块电机复位
    NET_MASTER_CMD_H360_DEBUG_HAND_WASH_MOTOR_RUN_STEP                              = 0X01000401,//模块电机运转
    NET_MASTER_CMD_H360_DEBUG_HAND_WASH_INPUT_READ                                  = 0X01000402,//模块信号读取
    NET_MASTER_CMD_H360_DEBUG_HAND_WASH_OUTPUT_WRITE                                = 0X01000403,//模块输出写入
    NET_MASTER_CMD_H360_DEBUG_HAND_WASH_RESET_ALL                                   = 0X01000404,//清洗机械手复位
    NET_MASTER_CMD_H360_DEBUG_HAND_WASH_ROTATE_PLATE_WASH                           = 0X01000405,//清洗机械手旋转到清洗盘
    NET_MASTER_CMD_H360_DEBUG_HAND_WASH_ROTATE_PLATE_REACTION                       = 0X01000406,//清洗机械手旋转到反应盘
    NET_MASTER_CMD_H360_DEBUG_HAND_WASH_ROTATE_MEASURE_MODULE                       = 0X01000407,//清洗机械手旋转到测量模块
    NET_MASTER_CMD_H360_DEBUG_HAND_WASH_CATCH_PLATE_REACTION                        = 0X01000408,//清洗机械手从反应盘抓杯
    NET_MASTER_CMD_H360_DEBUG_HAND_WASH_PUT_PLATE_REACTION                          = 0X01000409,//清洗机械手往反应盘放杯
    NET_MASTER_CMD_H360_DEBUG_HAND_WASH_CATCH_PLATE_WASH                            = 0X0100040A,//清洗机械手从清洗盘抓杯
    NET_MASTER_CMD_H360_DEBUG_HAND_WASH_PUT_PLATE_WASH                              = 0X0100040B,//清洗机械手往清洗盘放杯
    NET_MASTER_CMD_H360_DEBUG_HAND_WASH_CATCH_MEASURE_MODULE                        = 0X0100040C,//清洗机械手从测量模块抓杯
    NET_MASTER_CMD_H360_DEBUG_HAND_WASH_PUT_MEASURE_MODULE                          = 0X0100040D,//清洗机械手往测量模块放杯
    //反应盘调试指令    
    NET_MASTER_CMD_H360_DEBUG_PLATE_REACTION_MOTOR_RESET                            = 0X01000500,//反应盘模块电机复位
    NET_MASTER_CMD_H360_DEBUG_PLATE_REACTION_MOTOR_RUN_STEP                         = 0X01000501,//反应盘模块电机运转
    NET_MASTER_CMD_H360_DEBUG_PLATE_REACTION_INPUT_READ                             = 0X01000502,//反应盘模块信号读取
    NET_MASTER_CMD_H360_DEBUG_PLATE_REACTION_OUTPUT_WRITE                           = 0X01000503,//反应盘模块输出写入
    NET_MASTER_CMD_H360_DEBUG_PLATE_REACTION_RESET_ALL                              = 0X01000504,//反应盘全部复位
    NET_MASTER_CMD_H360_DEBUG_PLATE_REACTION_RUN_ANY_CUP_POSITIVE                   = 0X01000505,//反应盘正向移动指定个杯位
    NET_MASTER_CMD_H360_DEBUG_PLATE_REACTION_RUN_TO_INDEX_POSITIVE                  = 0X01000506,//反应盘正向运行到特定序号杯位
    NET_MASTER_CMD_H360_DEBUG_PLATE_REACTION_RUN_ANY_CUP_NEGATIVE                   = 0X01000507,//反应盘反向运行指定个杯位
    NET_MASTER_CMD_H360_DEBUG_PLATE_REACTION_RUN_TO_INDEX_NEGATIVE                  = 0X01000508,//反应盘反向运行到特定序号杯位
    //试剂盘    
    NET_MASTER_CMD_H360_DEBUG_PLATE_REAGENT_MOTOR_RESET                             = 0X01000600,//试剂盘模块电机复位
    NET_MASTER_CMD_H360_DEBUG_PLATE_REAGENT_MOTOR_RUN_STEP                          = 0X01000601,//试剂盘模块电机运转
    NET_MASTER_CMD_H360_DEBUG_PLATE_REAGENT_INPUT_READ                              = 0X01000602,//试剂盘模块信号读取
    NET_MASTER_CMD_H360_DEBUG_PLATE_REAGENT_OUTPUT_WRITE                            = 0X01000603,//试剂盘模块输出写入
    NET_MASTER_CMD_H360_DEBUG_PLATE_REAGENT_RESET_ALL                               = 0X01000604,//试剂盘全部复位
    NET_MASTER_CMD_H360_DEBUG_PLATE_REAGENT_RUN_ANY_CUPS                            = 0X01000605,//试剂盘运行指定数量个杯位
    NET_MASTER_CMD_H360_DEBUG_PLATE_REAGENT_RUN_TO_INDEX_OFFSET                     = 0X01000606,//试剂盘运行到特定序号杯位并带偏移
    NET_MASTER_CMD_H360_DEBUG_PLATE_REAGENT_RUN_TO_INDEX_AND_SCAN                   = 0X01000607,//试剂盘运行到特定杯位扫码位并扫码
    NET_MASTER_CMD_H360_DEBUG_PLATE_REAGENT_SCAN                                    = 0X01000608,//试剂盘不运行但是扫码器执行一次
    NET_MASTER_CMD_H360_DEBUG_PLATE_REAGENT_CLEAR_LIQUID_DETECT_DATA                = 0X01000609,//试剂盘清除指定探液数据
    NET_MASTER_CMD_H360_DEBUG_PLATE_REAGENT_SET_IDLE                                = 0X0100060A,//试剂盘设置状态空闲
    //清洗盘    
    NET_MASTER_CMD_H360_DEBUG_PLATE_WASH_MOTOR_RESET                                = 0X01000700,//清洗盘模块电机复位
    NET_MASTER_CMD_H360_DEBUG_PLATE_WASH_MOTOR_RUN_STEP                             = 0X01000701,//清洗盘模块电机运转
    NET_MASTER_CMD_H360_DEBUG_PLATE_WASH_INPUT_READ                                 = 0X01000702,//清洗盘模块信号读取
    NET_MASTER_CMD_H360_DEBUG_PLATE_WASH_OUTPUT_WRITE                               = 0X01000703,//清洗盘模块输出写入
    NET_MASTER_CMD_H360_DEBUG_PLATE_WASH_RESET_ALL                                  = 0X01000704,//清洗盘复位
    NET_MASTER_CMD_H360_DEBUG_PLATE_WASH_MOVE_ANY_CUPS                              = 0X01000705,//清洗盘移动指定个杯位
    NET_MASTER_CMD_H360_DEBUG_PLATE_WASH_MOVE_TO_INDEX                              = 0X01000706,//清洗移动到指定序号杯位
    //磁珠混匀调试指令  
    NET_MASTER_CMD_H360_DEBUG_MIX_BEAD_MOTOR_RESET                                  = 0X01000800,//磁珠摇匀模块电机复位
    NET_MASTER_CMD_H360_DEBUG_MIX_BEAD_MOTOR_RUN_STEP                               = 0X01000801,//磁珠摇匀模块电机运转
    NET_MASTER_CMD_H360_DEBUG_MIX_BEAD_INPUT_READ                                   = 0X01000802,//磁珠摇匀模块信号读取
    NET_MASTER_CMD_H360_DEBUG_MIX_BEAD_OUTPUT_WRITE                                 = 0X01000803,//磁珠摇匀模块输出写入
    NET_MASTER_CMD_H360_DEBUG_MIX_BEAD_RESET_ALL                                    = 0X01000804,//磁珠摇匀复位
    NET_MASTER_CMD_H360_DEBUG_MIX_BEAD_UP                                           = 0X01000805,//磁珠摇匀升起来
    NET_MASTER_CMD_H360_DEBUG_MIX_BEAD_DOWN                                         = 0X01000806,//磁珠摇匀降下去
    NET_MASTER_CMD_H360_DEBUG_MIX_BEAD_START_ROTATE                                 = 0X01000807,//磁珠摇匀执行一次
    NET_MASTER_CMD_H360_DEBUG_MIX_BEAD_ROTATE_INTEGRATE                             = 0X01000808,//磁珠摇匀集成磁珠摇匀升起来,摇匀,降下去
    //试剂混匀调试指令  
    NET_MASTER_CMD_H360_DEBUG_MIX_REAGENT_MOTOR_RESET                               = 0X01000900,//试剂摇匀模块电机复位
    NET_MASTER_CMD_H360_DEBUG_MIX_REAGENT_MOTOR_RUN_STEP                            = 0X01000901,//试剂摇匀模块电机运转
    NET_MASTER_CMD_H360_DEBUG_MIX_REAGENT_INPUT_READ                                = 0X01000902,//试剂摇匀模块信号读取
    NET_MASTER_CMD_H360_DEBUG_MIX_REAGENT_OUTPUT_WRITE                              = 0X01000903,//试剂摇匀模块输出写入
    NET_MASTER_CMD_H360_DEBUG_MIX_REAGENT_RESET_ALL                                 = 0X01000904,//试剂摇匀复位
    NET_MASTER_CMD_H360_DEBUG_MIX_REAGENT_UP                                        = 0X01000905,//试剂摇匀升起来
    NET_MASTER_CMD_H360_DEBUG_MIX_REAGENT_DOWN                                      = 0X01000906,//试剂摇匀降下去
    NET_MASTER_CMD_H360_DEBUG_MIX_REAGENT_START_ROTATE                              = 0X01000907,//试剂摇匀执行一次
    NET_MASTER_CMD_H360_DEBUG_MIX_REAGENT_ROTATE_INTEGRATE                          = 0X01000908,//试剂摇匀集成试剂摇匀升起来,摇匀,降下去
    //清洗混匀调试指令  
    NET_MASTER_CMD_H360_DEBUG_MIX_WASH_MOTOR_RESET                                  = 0X01000A00,//清洗摇匀模块电机复位
    NET_MASTER_CMD_H360_DEBUG_MIX_WASH_MOTOR_RUN_STEP                               = 0X01000A01,//清洗摇匀模块电机运转
    NET_MASTER_CMD_H360_DEBUG_MIX_WASH_INPUT_READ                                   = 0X01000A02,//清洗摇匀模块信号读取
    NET_MASTER_CMD_H360_DEBUG_MIX_WASH_OUTPUT_WRITE                                 = 0X01000A03,//清洗摇匀模块输出写入
    NET_MASTER_CMD_H360_DEBUG_MIX_WASH_RESET_ALL                                    = 0X01000A04,//清洗摇匀复位
    NET_MASTER_CMD_H360_DEBUG_MIX_WASH_UP                                           = 0X01000A05,//清洗摇匀升起来
    NET_MASTER_CMD_H360_DEBUG_MIX_WASH_DOWN                                         = 0X01000A06,//清洗摇匀降下去
    NET_MASTER_CMD_H360_DEBUG_MIX_WASH_START_ROTATE                                 = 0X01000A07,//清洗摇匀执行一次
    NET_MASTER_CMD_H360_DEBUG_MIX_WASH_ROTATE_INTEGRATE                             = 0X01000A08,//清洗摇匀集成清洗摇匀升起来,摇匀,降下去  
    //样本针调试指令    
    NET_MASTER_CMD_H360_DEBUG_NEEDLE_SAMPLE_MOTOR_RESET                             = 0X01000B00,//样本针模块电机复位
    NET_MASTER_CMD_H360_DEBUG_NEEDLE_SAMPLE_MOTOR_RUN_STEP                          = 0X01000B01,//样本针模块电机运转
    NET_MASTER_CMD_H360_DEBUG_NEEDLE_SAMPLE_INPUT_READ                              = 0X01000B02,//样本针模块信号读取
    NET_MASTER_CMD_H360_DEBUG_NEEDLE_SAMPLE_OUTPUT_WRITE                            = 0X01000B03,//样本针模块输出写入
    NET_MASTER_CMD_H360_DEBUG_NEEDLE_SAMPLE_RESET_ALL                               = 0X01000B04,//样本针全部复位
    NET_MASTER_CMD_H360_DEBUG_NEEDLE_SAMPLE_ROTATE_POS                              = 0X01000B05,//样本针旋转到指定位置
    NET_MASTER_CMD_H360_DEBUG_NEEDLE_SAMPLE_CLEAN                                   = 0X01000B06,//样本针清洗
    NET_MASTER_CMD_H360_DEBUG_NEEDLE_SAMPLE_FORCE_CLEAN                             = 0X01000B07,//样本针强力清洗
    NET_MASTER_CMD_H360_DEBUG_NEEDLE_SAMPLE_PRIME                                   = 0X01000B08,//样本针灌注
    NET_MASTER_CMD_H360_DEBUG_NEEDLE_SAMPLE_FORCE_PRIME                             = 0X01000B09,//样本针强力灌注
    NET_MASTER_CMD_H360_DEBUG_NEEDLE_SAMPLE_SET_TUBE_TYPE                           = 0X01000B0A,//样本针设置样本管类型
    NET_MASTER_CMD_H360_DEBUG_NEEDLE_SAMPLE_ABSORB                                  = 0X01000B0B,//样本针试管架吸液
    NET_MASTER_CMD_H360_DEBUG_NEEDLE_SAMPLE_ABSORB_PLATE_REACTION                   = 0X01000B0C,//样本针反应盘吸液
    NET_MASTER_CMD_H360_DEBUG_NEEDLE_SAMPLE_INJECT                                  = 0X01000B0D,//样本针注液
    NET_MASTER_CMD_H360_DEBUG_NEEDLE_SAMPLE_INJECT_MIX                              = 0X01000B0E,//样本针注液混匀
    NET_MASTER_CMD_H360_DEBUG_NEEDLE_SAMPLE_MAINTAIN                                = 0X01000B0F,//样本针清洗维护
    NET_MASTER_CMD_H360_DEBUG_NEEDLE_SAMPLE_ADJUST                                  = 0X01000B10,//样本针探液校准
    //试剂针调试指令    
    NET_MASTER_CMD_H360_DEBUG_NEEDLE_REAGENT_MOTOR_RESET                            = 0X01000C00,//试剂针模块电机复位
    NET_MASTER_CMD_H360_DEBUG_NEEDLE_REAGENT_MOTOR_RUN_STEP                         = 0X01000C01,//试剂针模块电机运转
    NET_MASTER_CMD_H360_DEBUG_NEEDLE_REAGENT_INPUT_READ                             = 0X01000C02,//试剂针模块信号读取
    NET_MASTER_CMD_H360_DEBUG_NEEDLE_REAGENT_OUTPUT_WRITE                           = 0X01000C03,//试剂针模块输出写入
    NET_MASTER_CMD_H360_DEBUG_NEEDLE_REAGENT_RESET_ALL                              = 0X01000C04,//试剂针全部复位
    NET_MASTER_CMD_H360_DEBUG_NEEDLE_REAGENT_ROTATE_POS                             = 0X01000C05,//试剂针旋转到指定位置
    NET_MASTER_CMD_H360_DEBUG_NEEDLE_REAGENT_CLEAN                                  = 0X01000C06,//试剂针清洗
    NET_MASTER_CMD_H360_DEBUG_NEEDLE_REAGENT_PRIME                                  = 0X01000C07,//试剂针灌注
    NET_MASTER_CMD_H360_DEBUG_NEEDLE_REAGENT_ABSORB                                 = 0X01000C08,//试剂针吸液
    NET_MASTER_CMD_H360_DEBUG_NEEDLE_REAGENT_INJECT                                 = 0X01000C09,//试剂针注液
    NET_MASTER_CMD_H360_DEBUG_NEEDLE_REAGENT_INJECT_MIX                             = 0X01000C0A,//试剂针注液混匀
    NET_MASTER_CMD_H360_DEBUG_NEEDLE_REAGENT_MAINTAIN                               = 0X01000C0B,//试剂针清洗维护
    NET_MASTER_CMD_H360_DEBUG_NEEDLE_REAGENT_ADJUST                                 = 0X01000C0C,//试剂针探液校准
    //磁珠针调试指令    
    NET_MASTER_CMD_H360_DEBUG_NEEDLE_BEAD_MOTOR_RESET                               = 0X01000D00,//磁珠针模块电机复位
    NET_MASTER_CMD_H360_DEBUG_NEEDLE_BEAD_MOTOR_RUN_STEP                            = 0X01000D01,//磁珠针模块电机运转
    NET_MASTER_CMD_H360_DEBUG_NEEDLE_BEAD_INPUT_READ                                = 0X01000D02,//磁珠针模块信号读取
    NET_MASTER_CMD_H360_DEBUG_NEEDLE_BEAD_OUTPUT_WRITE                              = 0X01000D03,//磁珠针模块输出写入
    NET_MASTER_CMD_H360_DEBUG_NEEDLE_BEAD_RESET_ALL                                 = 0X01000D04,//磁珠针全部复位
    NET_MASTER_CMD_H360_DEBUG_NEEDLE_BEAD_ROTATE_POS                                = 0X01000D05,//磁珠针旋转到指定位置
    NET_MASTER_CMD_H360_DEBUG_NEEDLE_BEAD_CLEAN                                     = 0X01000D06,//磁珠针清洗
    NET_MASTER_CMD_H360_DEBUG_NEEDLE_BEAD_PRIME                                     = 0X01000D07,//磁珠针灌注
    NET_MASTER_CMD_H360_DEBUG_NEEDLE_BEAD_ABSORB                                    = 0X01000D08,//磁珠针吸液
    NET_MASTER_CMD_H360_DEBUG_NEEDLE_BEAD_INJECT                                    = 0X01000D09,//磁珠针注液
    NET_MASTER_CMD_H360_DEBUG_NEEDLE_BEAD_INJECT_MIX                                = 0X01000D0A,//磁珠针注液混匀
    NET_MASTER_CMD_H360_DEBUG_NEEDLE_BEAD_MAINTAIN                                  = 0X01000D0B,//磁珠针清洗维护
    NET_MASTER_CMD_H360_DEBUG_NEEDLE_BEAD_ADJUST                                    = 0X01000D0C,//磁珠针探液校准
    NET_MASTER_CMD_H360_DEBUG_NEEDLE_BEAD_READ_LIQUID_DETECT_VERSION                = 0X01000D0C,//磁珠针读探液板版本
    //清洗针调试指令    
    NET_MASTER_CMD_H360_DEBUG_NEEDLE_WASH_MOTOR_RESET                               = 0X01000E00,//模块电机复位
    NET_MASTER_CMD_H360_DEBUG_NEEDLE_WASH_MOTOR_RUN_STEP                            = 0X01000E01,//模块电机运转
    NET_MASTER_CMD_H360_DEBUG_NEEDLE_WASH_INPUT_READ                                = 0X01000E02,//模块信号读取
    NET_MASTER_CMD_H360_DEBUG_NEEDLE_WASH_OUTPUT_WRITE                              = 0X01000E03,//模块输出写入
    NET_MASTER_CMD_H360_DEBUG_NEEDLE_WASH_RESET_ALL                                 = 0X01000E04,//清洗针全部复位
    NET_MASTER_CMD_H360_DEBUG_NEEDLE_WASH_PRIME                                     = 0X01000E05,//清洗针灌注
    NET_MASTER_CMD_H360_DEBUG_NEEDLE_WASH_CLEAN                                     = 0X01000E06,//清洗针执行一次清洗
    NET_MASTER_CMD_H360_DEBUG_NEEDLE_WASH_MAINTAIN                                  = 0X01000E07,//清洗针维护
    NET_MASTER_CMD_H360_DEBUG_NEEDLE_WASH_UPDOWN_TO_SOAK                            = 0X01000E08,//清洗针升降到浸泡位
    //缓冲液注液调试指令    
    NET_MASTER_CMD_H360_DEBUG_LIQUID_A_MOTOR_RESET                                  = 0X01000F00,//模块电机复位
    NET_MASTER_CMD_H360_DEBUG_LIQUID_A_MOTOR_RUN_STEP                               = 0X01000F01,//模块电机运转
    NET_MASTER_CMD_H360_DEBUG_LIQUID_A_INPUT_READ                                   = 0X01000F02,//模块信号读取
    NET_MASTER_CMD_H360_DEBUG_LIQUID_A_OUTPUT_WRITE                                 = 0X01000F03,//模块输出写入
    NET_MASTER_CMD_H360_DEBUG_LIQUID_A_RESET                                        = 0X01000F04,//缓冲液注液复位
    NET_MASTER_CMD_H360_DEBUG_LIQUID_A_TRIG                                         = 0X01000F05,//缓冲液注液一次,1:A瓶,其他:B瓶
    NET_MASTER_CMD_H360_DEBUG_LIQUID_A_PRIME                                        = 0X01000F06,//缓冲液灌注一次,1:A瓶,其他:B瓶
    //测量模式调试指令  
    NET_MASTER_CMD_H360_DEBUG_MEASURE_MODULE_MOTOR_RESET                            = 0X01001000,//模块电机复位
    NET_MASTER_CMD_H360_DEBUG_MEASURE_MODULE_MOTOR_RUN_STEP                         = 0X01001001,//模块电机运转
    NET_MASTER_CMD_H360_DEBUG_MEASURE_MODULE_INPUT_READ                             = 0X01001002,//模块信号读取
    NET_MASTER_CMD_H360_DEBUG_MEASURE_MODULE_OUTPUT_WRITE                           = 0X01001003,//模块输出写入
    NET_MASTER_CMD_H360_DEBUG_MEASURE_MODULE_RESET_ALL                              = 0X01001004,//测量模块全部复位
    NET_MASTER_CMD_H360_DEBUG_MEASURE_MODULE_DOOR_CLOSE                             = 0X01001005,//测量模块上门关闭
    NET_MASTER_CMD_H360_DEBUG_MEASURE_MODULE_DOOR_OPEN_FULL                         = 0X01001006,//测量模块上门全开
    NET_MASTER_CMD_H360_DEBUG_MEASURE_MODULE_DOOR_OPEN_HALF                         = 0X01001007,//测量模块上门半开
    NET_MASTER_CMD_H360_DEBUG_MEASURE_MODULE_WINDOW_CLOSE                           = 0X01001008,//测量模块侧门关闭
    NET_MASTER_CMD_H360_DEBUG_MEASURE_MODULE_WINDOW_OPEN                            = 0X01001009,//测量模块侧门打开
    NET_MASTER_CMD_H360_DEBUG_MEASURE_MODULE_LIQUID_INJECT                          = 0X0100100A,//测量模块注液
    NET_MASTER_CMD_H360_DEBUG_MEASURE_MODULE_LIQUID_PRIME                           = 0X0100100B,//测量模块灌注
    NET_MASTER_CMD_H360_DEBUG_MEASURE_MODULE_READ_DARK                              = 0X0100100C,//测量模块读暗噪声
    NET_MASTER_CMD_H360_DEBUG_MEASURE_MODULE_READ_BACKGROUDN                        = 0X0100100D,//测量模块读本底
    NET_MASTER_CMD_H360_DEBUG_MEASURE_MODULE_INJECT_READ_LIGHT                      = 0X0100100E,//测量模块注液测光
    NET_MASTER_CMD_H360_DEBUG_MEASURE_MODULE_INJECT_READ_CHANGE_RATE                = 0X0100100F,//测量模块注液读变化率
    NET_MASTER_CMD_H360_DEBUG_MEASURE_MODULE_READ_RESULT_INTEGRATE                  = 0X01001010,//测量模块集成读本底,注液,测光
    NET_MASTER_CMD_H360_DEBUG_MEASURE_MODULE_MAINTAIN                               = 0X01001011,//测量模块清洗维护
    //液路自动化调试指令    
    NET_MASTER_CMD_H360_DEBUG_LIQUID_AUTO_MOTOR_RESET                               = 0X01001100,//模块电机复位
    NET_MASTER_CMD_H360_DEBUG_LIQUID_AUTO_MOTOR_RUN_STEP                            = 0X01001101,//模块电机运转
    NET_MASTER_CMD_H360_DEBUG_LIQUID_AUTO_INPUT_READ                                = 0X01001102,//模块信号读取
    NET_MASTER_CMD_H360_DEBUG_LIQUID_AUTO_OUTPUT_WRITE                              = 0X01001103,//模块输出写入
    NET_MASTER_CMD_H360_DEBUG_LIQUID_AUTO_ENFORCE_WASTE_DISCHARGE_ONCE              = 0X01001104,//液路自动化强制执行一次排废液
    NET_MASTER_CMD_H360_DEBUG_LIQUID_AUTO_ENFORCE_THREE_NEEDLE_CLEAN_DILUENT_ONCE   = 0X01001105,//液路自动化强制执行一次三针清洗液稀释
    NET_MASTER_CMD_H360_DEBUG_LIQUID_AUTO_ENFORCE_PURE_WATER_SUPPLEMENT_ONCE        = 0X01001106,//液路自动化强制执行一次纯水补水
    NET_MASTER_CMD_H360_DEBUG_LIQUID_AUTO_ENABLE_WASTE_DISCHARGE_AUTO               = 0X01001107,//液路自动化打开程序自动排废液
    NET_MASTER_CMD_H360_DEBUG_LIQUID_AUTO_ENABLE_THREE_NEEDLE_CLEAN_DILUENT_AUTO    = 0X01001108,//液路自动化打开程序自动三针清洗液稀释
    NET_MASTER_CMD_H360_DEBUG_LIQUID_AUTO_ENABLE_PURE_WATER_SUPPLEMENT_AUTO         = 0X01001109,//液路自动化打开程序自动纯水补水
    NET_MASTER_CMD_H360_DEBUG_LIQUID_AUTO_DISABLE_WASTE_DISCHARGE_AUTO              = 0X0100110A,//液路自动化关闭程序自动排废液
    NET_MASTER_CMD_H360_DEBUG_LIQUID_AUTO_DISABLE_THREE_NEEDLE_CLEAN_DILUENT_AUTO   = 0X0100110B,//液路自动化关闭程序自动三针清洗液稀释
    NET_MASTER_CMD_H360_DEBUG_LIQUID_AUTO_DISABLE_PURE_WATER_SUPPLEMENT_AUTO        = 0X0100110C,//液路自动化关闭程序自动纯水补水
    NET_MASTER_CMD_H360_DEBUG_LIQUID_AUTO_CONFIG_ADD_PURE_WATER_ONCE                = 0X0100110D,//液路自动化配置瓶注一次纯水
    NET_MASTER_CMD_H360_DEBUG_LIQUID_AUTO_CONFIG_ADD_CONCENTRATE_ONCE               = 0X0100110E,//液路自动化配置瓶注一次浓缩液
    NET_MASTER_CMD_H360_DEBUG_LIQUID_AUTO_CONFIG_TO_TARGET_ONCE                     = 0X0100110F,//液路自动化配置瓶向工作瓶抽一次
    //温控 
    NET_MASTER_CMD_H360_DEBUG_TEMPERATURE_CONTROL_MOTOR_RESET                       = 0X01001200,//模块电机复位
    NET_MASTER_CMD_H360_DEBUG_TEMPERATURE_CONTROL_MOTOR_RUN_STEP                    = 0X01001201,//模块电机运转 
    NET_MASTER_CMD_H360_DEBUG_TEMPERATURE_CONTROL_INPUT_READ                        = 0X01001202,//模块信号读取
    NET_MASTER_CMD_H360_DEBUG_TEMPERATURE_CONTROL_OUTPUT_WRITE                      = 0X01001203,//模块输出写入
    NET_MASTER_CMD_H360_DEBUG_TEMPERATURE_CONTROL_READ_CHANNEL_PLATE_REACTION       = 0X01001204,//温控模块读取反应盘温度
    NET_MASTER_CMD_H360_DEBUG_TEMPERATURE_CONTROL_READ_CHANNEL_PLATE_REAGENT        = 0X01001205,//温控读取试剂盘温度
    NET_MASTER_CMD_H360_DEBUG_TEMPERATURE_CONTROL_READ_CHANNEL_PLATE_WASH           = 0X01001206,//温控读取清洗盘温度
    NET_MASTER_CMD_H360_DEBUG_TEMPERATURE_CONTROL_READ_CHANNEL_ENVIR                = 0X01001207,//温控读取环境温度
    NET_MASTER_CMD_H360_DEBUG_TEMPERATURE_CONTROL_READ_CHANNEL_BOARD                = 0X01001208,//温控读取板上温度 
    NET_MASTER_CMD_H360_DEBUG_TEMPERATURE_CONTROL_READ_CURRENT                      = 0X01001209,//温控读取电流值
    NET_MASTER_CMD_H360_DEBUG_TEMPERATURE_CONTROL_SET_PLATE_REAGENT_ADD_FLUID_MODE  = 0X0100120A,//设置试剂盘加制冷液模式
    NET_MASTER_CMD_H360_DEBUG_TEMPERATURE_CONTROL_GET_FLOAT_SENSOR_STATE            = 0X0100120B,//获取试剂盘浮球状态
    //辅助
    NET_MASTER_CMD_H360_DEBUG_UTIL_MACHINE_INFO_READ                                = 0X01001300,//读取机器信息
    NET_MASTER_CMD_H360_DEBUG_UTIL_MACHINE_INFO_WRITE                               = 0X01001301,//写入机器信息
    /*-------------------------------------------------多联机轨道指令--------------------------------------------------*/
    //流程指令
    NET_MASTER_CMD_MULTI_RAIL_SYSTEM_PARAM_SET                                      = 0X03000000,//系统流程参数设置
    NET_MASTER_CMD_MULTI_RAIL_SYSTEM_MSG_READ                                       = 0X03000001,//设备信息读取
    NET_MASTER_CMD_MULTI_RAIL_SYSTEM_RESET                                          = 0X03000002,//设备系统复位
    NET_MASTER_CMD_MULTI_RAIL_FLOW_START                                            = 0X03000100,//流程启动
    NET_MASTER_CMD_MULTI_RAIL_FLOW_STOP                                             = 0X03000101,//流程停止
    NET_MASTER_CMD_MULTI_RAIL_RACK_TUBE_TEST_SET                                    = 0X03000102,//试管测试下发      
    NET_MASTER_CMD_MULTI_RAIL_RACK_CLEAN                                            = 0X03000103,//试管架清空
    NET_MASTER_CMD_MULTI_RAIL_DEVICE_FAULT_RECOVER                                  = 0X03000104,//设备异常恢复
    NET_MASTER_CMD_MULTI_RAIL_RACK_TEST_REVIEW                                      = 0X03000105,//试管架测试重试

    //样本存储区调试指令  
    NET_MASTER_CMD_MULTI_RAIL_SAMPLE_STORE_INPUT_READ                               = 0X03010000,//模块信号读取
    NET_MASTER_CMD_MULTI_RAIL_SAMPLE_STORE_OUTPUT_WRITE                             = 0X03010001,//模块输出写入
    NET_MASTER_CMD_MULTI_RAIL_SAMPLE_STORE_MOTOR_RESET                              = 0X03010002,//模块电机复位
    NET_MASTER_CMD_MULTI_RAIL_SAMPLE_STORE_MOTOR_RUN_STEP                           = 0X03010003,//模块电机运转
    /*-------------------------------------------------立禾云指令--------------------------------------------------*/
    NET_MASTER_CMD_CLD_BASE_PING_BAIDU                                              = 0X7F000000,//PING 服务器测试
    NET_MASTER_CMD_CLD_BASE_GET_DESP_MANUFACTURE                                    = 0X7F000001,//获取模块厂商信息
    NET_MASTER_CMD_CLD_BASE_GET_DESP_MODULE_TYPE                                    = 0X7F000002,//获取模块类型信息
    NET_MASTER_CMD_CLD_BASE_GET_DESP_SOFT_VERSION                                   = 0X7F000003,//获取模块软件版本信息
    NET_MASTER_CMD_CLD_BASE_GET_DESP_IMEI                                           = 0X7F000004,//获取模块入网许可
    NET_MASTER_CMD_CLD_BASE_GET_DESP_ISMI                                           = 0X7F000005,//获取SIM卡IMSI串号
    NET_MASTER_CMD_CLD_BASE_GET_DESP_ICCID                                          = 0X7F000006,//获取SIM卡ICCID识别号
    NET_MASTER_CMD_CLD_BASE_GET_DESP_NET_NAME_LONG                                  = 0X7F000007,//获取当前模块连接网络全名
    NET_MASTER_CMD_CLD_BASE_GET_DESP_NET_NAME_SHORT                                 = 0X7F000008,//获取当前模块连接网络短名
    NET_MASTER_CMD_CLD_BASE_GET_DESP_SRV_PROVID_NAME                                = 0X7F000009,//获取网络服务商名称
    NET_MASTER_CMD_CLD_BASE_GET_DESP_NET_TYPE                                       = 0X7F00000A,//获取当前网络制式
    NET_MASTER_CMD_CLD_BASE_GET_DESP_NET_BAUD                                       = 0X7F00000B,//获取当前网络波段
    NET_MASTER_CMD_CLD_BASE_GET_DESP_NET_LAC                                        = 0X7F00000C,//获取LAC基站区域码
    NET_MASTER_CMD_CLD_BASE_GET_DESP_NET_CID                                        = 0X7F00000D,//获取CID基站编号
    NET_MASTER_CMD_CLD_BASE_GET_DESP_STRENGTH_ERR                                   = 0X7F00000E,//获取当前信号强度与误码率
    NET_MASTER_CMD_CLD_BASE_CHECK_NET_STATE                                         = 0X7F00000F,//查看当前模块网络是否正常
    NET_MASTER_CMD_CLD_BASE_DEVICE_REBOOT                                           = 0X7F000010,//通讯设备重启
    NET_MASTER_CMD_CLD_SOCKET_CONNECT                                               = 0X7F000011,//设备连接网络
    NET_MASTER_CMD_CLD_SOCKET_DISCONNECT                                            = 0X7F000012,//设备断开网络连接
    NET_MASTER_CMD_CLD_SOCKET_SEND_PACK_ONLY                                        = 0X7F000013,//设备发送一包数据
    NET_MASTER_CMD_CLD_SOCKET_RECV_PACK_ONLY                                        = 0X7F000014,//设备读取一次数据
    NET_MASTER_CMD_CLD_SOCKET_SEND_RECV_PACK                                        = 0X7F000015,//设备集成发送并接收数据
    /***************************************下位机透传指令集***********************************************/
    //系统辅助
    NET_MASTER_CMD_ACT_UTIL_ONLINE                                                  = 0X80000000,//联机
    NET_MASTER_CMD_ACT_UTIL_OFFLINE                                                 = 0X80000001,//脱机
    NET_MASTER_CMD_ACT_UTIL_UTC_SYNC                                                = 0X80000002,//UTC时间同步
    NET_MASTER_CMD_ACT_UTIL_HEART_GET                                               = 0X80000003,//心跳获取
    NET_MASTER_CMD_ACT_UTIL_EM_STOP                                                 = 0X80000004,//急停
    NET_MASTER_CMD_ACT_UTIL_GET_VERSION                                             = 0X80000005,//版本号获取
    //在线升级                          
    NET_MASTER_CMD_ACT_IAP_PREPARE                                                  = 0X81000000,//在线升级准备
    NET_MASTER_CMD_ACT_IAP_START_MSG                                                = 0X81000001,//在线升级启动信息
    NET_MASTER_CMD_ACT_IAP_DARA_TRANS                                               = 0X81000002,//在线升级数据传输
    NET_MASTER_CMD_ACT_IAP_DATA_CHECK                                               = 0X81000003,//在线升级数据校验
    NET_MASTER_CMD_ACT_IAP_FORCE_JMP                                                = 0X81000004,//在线升级强制跳转
    //端口输入                          
    NET_MASTER_CMD_ACT_PORT_IN_READ_SINGLE                                          = 0X82000000,//输入单个读取
    NET_MASTER_CMD_ACT_PORT_IN_READ_RANGE                                           = 0X82000001,//输入区间读取
    NET_MASTER_CMD_ACT_PORT_IN_READ_ANY                                             = 0X82000002,//输入批量读取
    //端口输出                          
    NET_MASTER_CMD_ACT_PORT_OUT_WRITE_SINGLE                                        = 0X83000000,//输出单个写入
    NET_MASTER_CMD_ACT_PORT_OUT_WRITE_ANY                                           = 0X83000001,//输出批量写入
    //参数读写                          
    NET_MASTER_CMD_ACT_PARAM_RW_READ_SINGLE                                         = 0X84000000,//参数单个读取
    NET_MASTER_CMD_ACT_PARAM_RW_READ_ANY                                            = 0X84000001,//参数批量读取
    NET_MASTER_CMD_ACT_PARAM_RW_WRITE_SINGLE                                        = 0X84000002,//参数单个写入
    NET_MASTER_CMD_ACT_PARAM_RW_WRITE_ANY                                           = 0X84000003,//参数批量写入
    //步进电机1控制                         
    NET_MASTER_CMD_ACT_STEP_MOTOR1_RESET                                            = 0X85000000,//步进电机控制复位
    NET_MASTER_CMD_ACT_STEP_MOTOR1_RUN_STEPS                                        = 0X85000001,//步进电机控制走步数
    NET_MASTER_CMD_ACT_STEP_MOTOR1_RUN_COORDINATE                                   = 0X85000002,//步进电机控制走坐标
    NET_MASTER_CMD_ACT_STEP_MOTOR1_RETURN_ZERO                                      = 0X85000003,//步进电机控制回零
    NET_MASTER_CMD_ACT_STEP_MOTOR1_RUN_ALWAYS                                       = 0X85000004,//步进电机控制持续运转
    NET_MASTER_CMD_ACT_STEP_MOTOR1_STOP_NOW                                         = 0X85000005,//步进电机控制立即停止
    NET_MASTER_CMD_ACT_STEP_MOTOR1_STOP_SLOW_DOWN                                   = 0X85000006,//步进电机控制减速运转
    NET_MASTER_CMD_ACT_STEP_MOTOR1_GET_COORDINATE                                   = 0X85000007,//步进电机控制获取当前坐标
    NET_MASTER_CMD_ACT_STEP_MOTOR1_READ_ORIGIN_SENSOR                               = 0X85000008,//步进电机控制获取当前原点传感器状态
    //步进电机2控制                         
    NET_MASTER_CMD_ACT_STEP_MOTOR2_RESET                                            = 0X86000000,//步进电机控制复位
    NET_MASTER_CMD_ACT_STEP_MOTOR2_RUN_STEPS                                        = 0X86000001,//步进电机控制走步数
    NET_MASTER_CMD_ACT_STEP_MOTOR2_RUN_COORDINATE                                   = 0X86000002,//步进电机控制走坐标
    NET_MASTER_CMD_ACT_STEP_MOTOR2_RETURN_ZERO                                      = 0X86000003,//步进电机控制回零
    NET_MASTER_CMD_ACT_STEP_MOTOR2_RUN_ALWAYS                                       = 0X86000004,//步进电机控制持续运转
    NET_MASTER_CMD_ACT_STEP_MOTOR2_STOP_NOW                                         = 0X86000005,//步进电机控制立即停止
    NET_MASTER_CMD_ACT_STEP_MOTOR2_STOP_SLOW_DOWN                                   = 0X86000006,//步进电机控制减速运转
    NET_MASTER_CMD_ACT_STEP_MOTOR2_GET_COORDINATE                                   = 0X86000007,//步进电机控制获取当前坐标
    NET_MASTER_CMD_ACT_STEP_MOTOR2_READ_ORIGIN_SENSOR                               = 0X86000008,//步进电机控制获取当前原点传感器状态
    //步进电机3控制                         
    NET_MASTER_CMD_ACT_STEP_MOTOR3_RESET                                            = 0X87000000,//步进电机控制复位
    NET_MASTER_CMD_ACT_STEP_MOTOR3_RUN_STEPS                                        = 0X87000001,//步进电机控制走步数
    NET_MASTER_CMD_ACT_STEP_MOTOR3_RUN_COORDINATE                                   = 0X87000002,//步进电机控制走坐标
    NET_MASTER_CMD_ACT_STEP_MOTOR3_RETURN_ZERO                                      = 0X87000003,//步进电机控制回零
    NET_MASTER_CMD_ACT_STEP_MOTOR3_RUN_ALWAYS                                       = 0X87000004,//步进电机控制持续运转
    NET_MASTER_CMD_ACT_STEP_MOTOR3_STOP_NOW                                         = 0X87000005,//步进电机控制立即停止
    NET_MASTER_CMD_ACT_STEP_MOTOR3_STOP_SLOW_DOWN                                   = 0X87000006,//步进电机控制减速运转
    NET_MASTER_CMD_ACT_STEP_MOTOR3_GET_COORDINATE                                   = 0X87000007,//步进电机控制获取当前坐标
    NET_MASTER_CMD_ACT_STEP_MOTOR3_READ_ORIGIN_SENSOR                               = 0X87000008,//步进电机控制获取当前原点传感器状态
    //步进电机4控制                         
    NET_MASTER_CMD_ACT_STEP_MOTOR4_RESET                                            = 0X88000000,//步进电机控制复位
    NET_MASTER_CMD_ACT_STEP_MOTOR4_RUN_STEPS                                        = 0X88000001,//步进电机控制走步数
    NET_MASTER_CMD_ACT_STEP_MOTOR4_RUN_COORDINATE                                   = 0X88000002,//步进电机控制走坐标
    NET_MASTER_CMD_ACT_STEP_MOTOR4_RETURN_ZERO                                      = 0X88000003,//步进电机控制回零
    NET_MASTER_CMD_ACT_STEP_MOTOR4_RUN_ALWAYS                                       = 0X88000004,//步进电机控制持续运转
    NET_MASTER_CMD_ACT_STEP_MOTOR4_STOP_NOW                                         = 0X88000005,//步进电机控制立即停止
    NET_MASTER_CMD_ACT_STEP_MOTOR4_STOP_SLOW_DOWN                                   = 0X88000006,//步进电机控制减速运转
    NET_MASTER_CMD_ACT_STEP_MOTOR4_GET_COORDINATE                                   = 0X88000007,//步进电机控制获取当前坐标
    NET_MASTER_CMD_ACT_STEP_MOTOR4_READ_ORIGIN_SENSOR                               = 0X88000008,//步进电机控制获取当前原点传感器状态
    //步进电机5控制                         
    NET_MASTER_CMD_ACT_STEP_MOTOR5_RESET                                            = 0X89000000,//步进电机控制复位
    NET_MASTER_CMD_ACT_STEP_MOTOR5_RUN_STEPS                                        = 0X89000001,//步进电机控制走步数
    NET_MASTER_CMD_ACT_STEP_MOTOR5_RUN_COORDINATE                                   = 0X89000002,//步进电机控制走坐标
    NET_MASTER_CMD_ACT_STEP_MOTOR5_RETURN_ZERO                                      = 0X89000003,//步进电机控制回零
    NET_MASTER_CMD_ACT_STEP_MOTOR5_RUN_ALWAYS                                       = 0X89000004,//步进电机控制持续运转
    NET_MASTER_CMD_ACT_STEP_MOTOR5_STOP_NOW                                         = 0X89000005,//步进电机控制立即停止
    NET_MASTER_CMD_ACT_STEP_MOTOR5_STOP_SLOW_DOWN                                   = 0X89000006,//步进电机控制减速运转
    NET_MASTER_CMD_ACT_STEP_MOTOR5_GET_COORDINATE                                   = 0X89000007,//步进电机控制获取当前坐标
    NET_MASTER_CMD_ACT_STEP_MOTOR5_READ_ORIGIN_SENSOR                               = 0X89000008,//步进电机控制获取当前原点传感器状态
    //步进电机6控制                         
    NET_MASTER_CMD_ACT_STEP_MOTOR6_RESET                                            = 0X8A000000,//步进电机控制复位
    NET_MASTER_CMD_ACT_STEP_MOTOR6_RUN_STEPS                                        = 0X8A000001,//步进电机控制走步数
    NET_MASTER_CMD_ACT_STEP_MOTOR6_RUN_COORDINATE                                   = 0X8A000002,//步进电机控制走坐标
    NET_MASTER_CMD_ACT_STEP_MOTOR6_RETURN_ZERO                                      = 0X8A000003,//步进电机控制回零
    NET_MASTER_CMD_ACT_STEP_MOTOR6_RUN_ALWAYS                                       = 0X8A000004,//步进电机控制持续运转
    NET_MASTER_CMD_ACT_STEP_MOTOR6_STOP_NOW                                         = 0X8A000005,//步进电机控制立即停止
    NET_MASTER_CMD_ACT_STEP_MOTOR6_STOP_SLOW_DOWN                                   = 0X8A000006,//步进电机控制减速运转
    NET_MASTER_CMD_ACT_STEP_MOTOR6_GET_COORDINATE                                   = 0X8A000007,//步进电机控制获取当前坐标
    NET_MASTER_CMD_ACT_STEP_MOTOR6_READ_ORIGIN_SENSOR                               = 0X8A000008,//步进电机控制获取当前原点传感器状态
    //步进电机7控制                         
    NET_MASTER_CMD_ACT_STEP_MOTOR7_RESET                                            = 0X8B000000,//步进电机控制复位
    NET_MASTER_CMD_ACT_STEP_MOTOR7_RUN_STEPS                                        = 0X8B000001,//步进电机控制走步数
    NET_MASTER_CMD_ACT_STEP_MOTOR7_RUN_COORDINATE                                   = 0X8B000002,//步进电机控制走坐标
    NET_MASTER_CMD_ACT_STEP_MOTOR7_RETURN_ZERO                                      = 0X8B000003,//步进电机控制回零
    NET_MASTER_CMD_ACT_STEP_MOTOR7_RUN_ALWAYS                                       = 0X8B000004,//步进电机控制持续运转
    NET_MASTER_CMD_ACT_STEP_MOTOR7_STOP_NOW                                         = 0X8B000005,//步进电机控制立即停止
    NET_MASTER_CMD_ACT_STEP_MOTOR7_STOP_SLOW_DOWN                                   = 0X8B000006,//步进电机控制减速运转
    NET_MASTER_CMD_ACT_STEP_MOTOR7_GET_COORDINATE                                   = 0X8B000007,//步进电机控制获取当前坐标
    NET_MASTER_CMD_ACT_STEP_MOTOR7_READ_ORIGIN_SENSOR                               = 0X8B000008,//步进电机控制获取当前原点传感器状态
    //步进电机8控制                         
    NET_MASTER_CMD_ACT_STEP_MOTOR8_RESET                                            = 0X8C000000,//步进电机控制复位
    NET_MASTER_CMD_ACT_STEP_MOTOR8_RUN_STEPS                                        = 0X8C000001,//步进电机控制走步数
    NET_MASTER_CMD_ACT_STEP_MOTOR8_RUN_COORDINATE                                   = 0X8C000002,//步进电机控制走坐标
    NET_MASTER_CMD_ACT_STEP_MOTOR8_RETURN_ZERO                                      = 0X8C000003,//步进电机控制回零
    NET_MASTER_CMD_ACT_STEP_MOTOR8_RUN_ALWAYS                                       = 0X8C000004,//步进电机控制持续运转
    NET_MASTER_CMD_ACT_STEP_MOTOR8_STOP_NOW                                         = 0X8C000005,//步进电机控制立即停止
    NET_MASTER_CMD_ACT_STEP_MOTOR8_STOP_SLOW_DOWN                                   = 0X8C000006,//步进电机控制减速运转
    NET_MASTER_CMD_ACT_STEP_MOTOR8_GET_COORDINATE                                   = 0X8C000007,//步进电机控制获取当前坐标
    NET_MASTER_CMD_ACT_STEP_MOTOR8_READ_ORIGIN_SENSOR                               = 0X8C000008,//步进电机控制获取当前原点传感器状态
    //步进电机9控制                         
    NET_MASTER_CMD_ACT_STEP_MOTOR9_RESET                                            = 0X8D000000,//步进电机控制复位
    NET_MASTER_CMD_ACT_STEP_MOTOR9_RUN_STEPS                                        = 0X8D000001,//步进电机控制走步数
    NET_MASTER_CMD_ACT_STEP_MOTOR9_RUN_COORDINATE                                   = 0X8D000002,//步进电机控制走坐标
    NET_MASTER_CMD_ACT_STEP_MOTOR9_RETURN_ZERO                                      = 0X8D000003,//步进电机控制回零
    NET_MASTER_CMD_ACT_STEP_MOTOR9_RUN_ALWAYS                                       = 0X8D000004,//步进电机控制持续运转
    NET_MASTER_CMD_ACT_STEP_MOTOR9_STOP_NOW                                         = 0X8D000005,//步进电机控制立即停止
    NET_MASTER_CMD_ACT_STEP_MOTOR9_STOP_SLOW_DOWN                                   = 0X8D000006,//步进电机控制减速运转
    NET_MASTER_CMD_ACT_STEP_MOTOR9_GET_COORDINATE                                   = 0X8D000007,//步进电机控制获取当前坐标
    NET_MASTER_CMD_ACT_STEP_MOTOR9_READ_ORIGIN_SENSOR                               = 0X8D000008,//步进电机控制获取当前原点传感器状态
    //步进电机10控制                            
    NET_MASTER_CMD_ACT_STEP_MOTOR10_RESET                                           = 0X8E000000,//步进电机控制复位
    NET_MASTER_CMD_ACT_STEP_MOTOR10_RUN_STEPS                                       = 0X8E000001,//步进电机控制走步数
    NET_MASTER_CMD_ACT_STEP_MOTOR10_RUN_COORDINATE                                  = 0X8E000002,//步进电机控制走坐标
    NET_MASTER_CMD_ACT_STEP_MOTOR10_RETURN_ZERO                                     = 0X8E000003,//步进电机控制回零
    NET_MASTER_CMD_ACT_STEP_MOTOR10_RUN_ALWAYS                                      = 0X8E000004,//步进电机控制持续运转
    NET_MASTER_CMD_ACT_STEP_MOTOR10_STOP_NOW                                        = 0X8E000005,//步进电机控制立即停止
    NET_MASTER_CMD_ACT_STEP_MOTOR10_STOP_SLOW_DOWN                                  = 0X8E000006,//步进电机控制减速运转
    NET_MASTER_CMD_ACT_STEP_MOTOR10_GET_COORDINATE                                  = 0X8E000007,//步进电机控制获取当前坐标
    NET_MASTER_CMD_ACT_STEP_MOTOR10_READ_ORIGIN_SENSOR                              = 0X8E000008,//步进电机控制获取当前原点传感器状态
    //步进电机11控制                            
    NET_MASTER_CMD_ACT_STEP_MOTOR11_RESET                                           = 0X8F000000,//步进电机控制复位
    NET_MASTER_CMD_ACT_STEP_MOTOR11_RUN_STEPS                                       = 0X8F000001,//步进电机控制走步数
    NET_MASTER_CMD_ACT_STEP_MOTOR11_RUN_COORDINATE                                  = 0X8F000002,//步进电机控制走坐标
    NET_MASTER_CMD_ACT_STEP_MOTOR11_RETURN_ZERO                                     = 0X8F000003,//步进电机控制回零
    NET_MASTER_CMD_ACT_STEP_MOTOR11_RUN_ALWAYS                                      = 0X8F000004,//步进电机控制持续运转
    NET_MASTER_CMD_ACT_STEP_MOTOR11_STOP_NOW                                        = 0X8F000005,//步进电机控制立即停止
    NET_MASTER_CMD_ACT_STEP_MOTOR11_STOP_SLOW_DOWN                                  = 0X8F000006,//步进电机控制减速运转
    NET_MASTER_CMD_ACT_STEP_MOTOR11_GET_COORDINATE                                  = 0X8F000007,//步进电机控制获取当前坐标
    NET_MASTER_CMD_ACT_STEP_MOTOR11_READ_ORIGIN_SENSOR                              = 0X8F000008,//步进电机控制获取当前原点传感器状态
    //步进电机12控制                            
    NET_MASTER_CMD_ACT_STEP_MOTOR12_RESET                                           = 0X90000000,//步进电机控制复位
    NET_MASTER_CMD_ACT_STEP_MOTOR12_RUN_STEPS                                       = 0X90000001,//步进电机控制走步数
    NET_MASTER_CMD_ACT_STEP_MOTOR12_RUN_COORDINATE                                  = 0X90000002,//步进电机控制走坐标
    NET_MASTER_CMD_ACT_STEP_MOTOR12_RETURN_ZERO                                     = 0X90000003,//步进电机控制回零
    NET_MASTER_CMD_ACT_STEP_MOTOR12_RUN_ALWAYS                                      = 0X90000004,//步进电机控制持续运转
    NET_MASTER_CMD_ACT_STEP_MOTOR12_STOP_NOW                                        = 0X90000005,//步进电机控制立即停止
    NET_MASTER_CMD_ACT_STEP_MOTOR12_STOP_SLOW_DOWN                                  = 0X90000006,//步进电机控制减速运转
    NET_MASTER_CMD_ACT_STEP_MOTOR12_GET_COORDINATE                                  = 0X90000007,//步进电机控制获取当前坐标
    NET_MASTER_CMD_ACT_STEP_MOTOR12_READ_ORIGIN_SENSOR                              = 0X90000008,//步进电机控制获取当前原点传感器状态
    //步进电机13控制                            
    NET_MASTER_CMD_ACT_STEP_MOTOR13_RESET                                           = 0X91000000,//步进电机控制复位
    NET_MASTER_CMD_ACT_STEP_MOTOR13_RUN_STEPS                                       = 0X91000001,//步进电机控制走步数
    NET_MASTER_CMD_ACT_STEP_MOTOR13_RUN_COORDINATE                                  = 0X91000002,//步进电机控制走坐标
    NET_MASTER_CMD_ACT_STEP_MOTOR13_RETURN_ZERO                                     = 0X91000003,//步进电机控制回零
    NET_MASTER_CMD_ACT_STEP_MOTOR13_RUN_ALWAYS                                      = 0X91000004,//步进电机控制持续运转
    NET_MASTER_CMD_ACT_STEP_MOTOR13_STOP_NOW                                        = 0X91000005,//步进电机控制立即停止
    NET_MASTER_CMD_ACT_STEP_MOTOR13_STOP_SLOW_DOWN                                  = 0X91000006,//步进电机控制减速运转
    NET_MASTER_CMD_ACT_STEP_MOTOR13_GET_COORDINATE                                  = 0X91000007,//步进电机控制获取当前坐标
    NET_MASTER_CMD_ACT_STEP_MOTOR13_READ_ORIGIN_SENSOR                              = 0X91000008,//步进电机控制获取当前原点传感器状态
    //步进电机14控制                            
    NET_MASTER_CMD_ACT_STEP_MOTOR14_RESET                                           = 0X92000000,//步进电机控制复位
    NET_MASTER_CMD_ACT_STEP_MOTOR14_RUN_STEPS                                       = 0X92000001,//步进电机控制走步数
    NET_MASTER_CMD_ACT_STEP_MOTOR14_RUN_COORDINATE                                  = 0X92000002,//步进电机控制走坐标
    NET_MASTER_CMD_ACT_STEP_MOTOR14_RETURN_ZERO                                     = 0X92000003,//步进电机控制回零
    NET_MASTER_CMD_ACT_STEP_MOTOR14_RUN_ALWAYS                                      = 0X92000004,//步进电机控制持续运转
    NET_MASTER_CMD_ACT_STEP_MOTOR14_STOP_NOW                                        = 0X92000005,//步进电机控制立即停止
    NET_MASTER_CMD_ACT_STEP_MOTOR14_STOP_SLOW_DOWN                                  = 0X92000006,//步进电机控制减速运转
    NET_MASTER_CMD_ACT_STEP_MOTOR14_GET_COORDINATE                                  = 0X92000007,//步进电机控制获取当前坐标
    NET_MASTER_CMD_ACT_STEP_MOTOR14_READ_ORIGIN_SENSOR                              = 0X92000008,//步进电机控制获取当前原点传感器状态
    //步进电机15控制                            
    NET_MASTER_CMD_ACT_STEP_MOTOR15_RESET                                           = 0X93000000,//步进电机控制复位
    NET_MASTER_CMD_ACT_STEP_MOTOR15_RUN_STEPS                                       = 0X93000001,//步进电机控制走步数
    NET_MASTER_CMD_ACT_STEP_MOTOR15_RUN_COORDINATE                                  = 0X93000002,//步进电机控制走坐标
    NET_MASTER_CMD_ACT_STEP_MOTOR15_RETURN_ZERO                                     = 0X93000003,//步进电机控制回零
    NET_MASTER_CMD_ACT_STEP_MOTOR15_RUN_ALWAYS                                      = 0X93000004,//步进电机控制持续运转
    NET_MASTER_CMD_ACT_STEP_MOTOR15_STOP_NOW                                        = 0X93000005,//步进电机控制立即停止
    NET_MASTER_CMD_ACT_STEP_MOTOR15_STOP_SLOW_DOWN                                  = 0X93000006,//步进电机控制减速运转
    NET_MASTER_CMD_ACT_STEP_MOTOR15_GET_COORDINATE                                  = 0X93000007,//步进电机控制获取当前坐标
    NET_MASTER_CMD_ACT_STEP_MOTOR15_READ_ORIGIN_SENSOR                              = 0X93000008,//步进电机控制获取当前原点传感器状态
    //步进电机16控制                            
    NET_MASTER_CMD_ACT_STEP_MOTOR16_RESET                                           = 0X94000000,//步进电机控制复位
    NET_MASTER_CMD_ACT_STEP_MOTOR16_RUN_STEPS                                       = 0X94000001,//步进电机控制走步数
    NET_MASTER_CMD_ACT_STEP_MOTOR16_RUN_COORDINATE                                  = 0X94000002,//步进电机控制走坐标
    NET_MASTER_CMD_ACT_STEP_MOTOR16_RETURN_ZERO                                     = 0X94000003,//步进电机控制回零
    NET_MASTER_CMD_ACT_STEP_MOTOR16_RUN_ALWAYS                                      = 0X94000004,//步进电机控制持续运转
    NET_MASTER_CMD_ACT_STEP_MOTOR16_STOP_NOW                                        = 0X94000005,//步进电机控制立即停止
    NET_MASTER_CMD_ACT_STEP_MOTOR16_STOP_SLOW_DOWN                                  = 0X94000006,//步进电机控制减速运转
    NET_MASTER_CMD_ACT_STEP_MOTOR16_GET_COORDINATE                                  = 0X94000007,//步进电机控制获取当前坐标
    NET_MASTER_CMD_ACT_STEP_MOTOR16_READ_ORIGIN_SENSOR                              = 0X94000008,//步进电机控制获取当前原点传感器状态
    //占位符                            
    NET_MASTER_CMD_HOLD_PLACE                                                       = 0XFFFFFFFF,//占位符
}NET_MASTER_CMD;

typedef enum NET_MASTER_REPORT
{
    NET_MASTER_REPORT_HEART_BEAT                                                    = 0X00000000,//心跳上报ID
    NET_MASTER_REPORT_ERROR                                                         = 0X000000FF,//错误上报ID
    /*------------------------------------------------------H360上报------------------------------------------------*/                         
    NET_MASTER_REPORT_H360_BASE_WARNING                                             = 0X000000FF,//基础报警
    NET_MASTER_REPORT_H360_STATUS                                                   = 0X00000201,//设备状态
    NET_MASTER_REPORT_H360_SPECIAL_WARNING                                          = 0X00000202,//特殊报警,0：样本针、1：试剂针、2：磁珠针、3:新杯失败、
                                                                                    // 4：试剂混匀失败、5:磁珠混匀失败、6:没有反应杯、7:样本架超时、8:其他
    NET_MASTER_REPORT_H360_NEEDLE_CALIBRATE                                         = 0X00000203,//三针校准状态
    NET_MASTER_REPORT_H360_ITEM_START_TEST                                          = 0X00000204,//项目测试开始
    NET_MASTER_REPORT_H360_ITEM_REAGENT_ID                                          = 0X00000205,//项目对应试剂ID
    NET_MASTER_REPORT_H360_ITEM_STATUS                                              = 0X00000206,//项目加样状态
    NET_MASTER_REPORT_H360_CONCENTRATE_BOTTLE                                       = 0X00000207,//浓缩液瓶号
    NET_MASTER_REPORT_H360_GARBAGE_CAN                                              = 0X00000208,//垃圾桶号
    NET_MASTER_REPORT_H360_CUP_TRAY_SWITCH                                          = 0X00000209,//杯盘切换
    NET_MASTER_REPORT_H360_CONSUME                                                  = 0X0000020A,//消耗品
    NET_MASTER_REPORT_H360_ITEM_RESULT                                              = 0X0000020B,//项目测试结果
    NET_MASTER_REPORT_H360_TEST_FINISH                                              = 0X0000020C,//测试结束
    NET_MASTER_REPORT_H360_FLOW_RESULT                                              = 0X0000020D,//组合动作结果
    NET_MASTER_REPORT_H360_REQUEST_ITEM_DATA                                        = 0X0000020E,//申请测试数据
    NET_MASTER_REPORT_H360_SAMPLE_NEEDLE_CRASH                                      = 0X0000020F,//样本针防撞
    NET_MASTER_REPORT_H360_SAMPLE_RESET                                             = 0X00000210,//样本针复位上报
    NET_MASTER_REPORT_H360_SYSTEM_RESET_RESULT                                      = 0X00000211,//系统复位结果上报
    NET_MASTER_REPORT_H360_REAGENT_BAR_CODE                                         = 0X00000212,//试剂扫码上报
    NET_MASTER_REPORT_H360_REAGENT_QUANTITY                                         = 0X00000213,//试剂余量上报
    NET_MASTER_REPORT_H360_TEST_FLOW_WARNING                                        = 0X00000214,//流程报警上报
    /*-----------------------------------------------多联机轨道-------------------------------------------------------*/
    NET_MASTER_REPORT_MULTI_RAIL_STATE                                              = 0X00000301,//状态上传
    NET_MASTER_REPORT_MULTI_RACK_TUBE_CODE                                          = 0X00000302,//试管架条码上传
    NET_MASTER_REPORT_MULTI_RACK_ENTRY_RAIL                                         = 0X00000303,//试管架进入轨道
    NET_MASTER_REPORT_MULTI_RACK_LEAVE_RAIL                                         = 0X00000304,//试管架离开轨道         
    NET_MASTER_REPORT_MULTI_RACK_BACK_SAMPLE_STORE                                  = 0X00000305,//试管架回到样本仓
    //占位符                            
    NET_MASTER_REPORT_HOLD_PLACE                                                    = 0XFFFFFFFF,//占位符
}NET_MASTER_REPORT;


#endif












