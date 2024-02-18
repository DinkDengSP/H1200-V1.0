#ifndef _CAN_RAIL_MULTI_DATA_TYPE_H_
#define _CAN_RAIL_MULTI_DATA_TYPE_H_

#include "CanRailMultiMacro.h"

//返回条码最大长度
#define PER_BAR_CODE_MAX_LENGTH         32
//无效架子ID
#define INVALID_RACK_ID                 0XFFFF
//样本托盘最大数量
#define MAX_SAMPLE_TRAY_COUNT           10
//每个样本托盘最大样本架数量
#define MAX_SAMPLE_RACK_COUNT_PER_TRAY  8
//样本仓最大数量
#define MAX_STORE_SAMPLE_RACK_COUNT     MAX_SAMPLE_TRAY_COUNT*MAX_SAMPLE_RACK_COUNT_PER_TRAY
//每个架子试管数量最大
#define RACK_TUBE_NUM_MAX               5


typedef enum SENSOR_STATE{
    SENSOR_STATE_INVALID   = 0X00,//传感器有效
    SENSOR_STATE_VALID     = 0X01,//传感器无效
}SENSOR_STATE;

//旋转方向
typedef enum ROTATE_SCAN_DIR
{
    ROTATE_SCAN_DIR_SAMPLE_STORE    = 0X00,//样本仓方向
    ROTATE_SCAN_DIR_RAIL            = 0X01,//轨道方向
}ROTATE_SCAN_DIR;

//试管架标签方向
typedef enum RACK_BAR_DIR
{
    RACK_BAR_DIR_LEFT    = 0X00,//试管架标签左侧
    RACK_BAR_DIR_RIGHT   = 0X01,//试管架标签右侧
}RACK_BAR_DIR;

//试管架是否存在
typedef enum SAMPLE_RACK_STATE
{
    SAMPLE_RACK_STATE_NONE  = 0X00,//试管架不存在
    SAMPLE_RACK_STATE_EXIST = 0X01,//试管架存在
}SAMPLE_RACK_STATE;

/* 条码索引 */
typedef enum BARSCAN_INDEX
{
    BARSCAN_INDEX_TUBE1 = 0X00,
    BARSCAN_INDEX_TUBE2 = 0X01,
    BARSCAN_INDEX_TUBE3 = 0X02,
    BARSCAN_INDEX_TUBE4 = 0X03,
    BARSCAN_INDEX_TUBE5 = 0X04,
    BARSCAN_INDEX_MAX   = 0X05,
    BARSCAN_INDEX_INVALID = 0XFF,
}BARSCAN_INDEX;


//变轨输入位置
typedef enum ENTRY_CHANGE_POS
{
    ENTRY_CHANGE_POS_RECEIVE_RACK  = 0X00,//接收转运试管架位置
    ENTRY_CHANGE_POS_PUSH_RACK     = 0X01,//推送试管架到转运
    ENTRY_CHANGE_POS_NORMAL_RAIL   = 0X02,//常规轨道
    ENTRY_CHANGE_POS_EM_RAIL       = 0X03,//急诊轨道
    ENTRY_CHANGE_POS_BACK_RAIL     = 0X04,//回收轨道
    ENTRY_CHANGE_POS_NONE          = 0XFF,//未知
}ENTRY_CHANGE_POS;

//轨道功能类型
typedef enum RAIL_FUNC_TYPE{
    RAIL_FUNC_TYPE_NORMAL_RAIL        = 0X00,//常规轨道
    RAIL_FUNC_TYPE_EM_RAIL            = 0X01,//急诊轨道
    RAIL_FUNC_TYPE_RECYCLE_RAIL       = 0X02,//回收轨道    
}RAIL_FUNC_TYPE;

//样本机械手位置
typedef enum SAMPLE_HAND_POS
{
   SAMPLE_HAND_POS_CATCH_PUT     = 0X00,//样本仓取放
   SAMPLE_HAND_POS_SCAN          = 0X01,//样本仓扫描
   SAMPLE_HAND_POS_PUT_RAIL      = 0X02,//变轨放入
   SAMPLE_HAND_POS_CATCH_RAIL    = 0X03,//变轨取出
   SAMPLE_HAND_POS_ROTATE_SCAN   = 0X04,//样本架旋转
   SAMPLE_HAND_POS_CATCH_RBU     = 0X05,//联机取架位
   SAMPLE_HAND_POS_PUT_RBU       = 0X06,//联机出架位
   SAMPLE_HAND_POS_NONE          = 0XFF,//未知         
}SAMPLE_HAND_POS;

//位置主索引
typedef enum POS_MAIN_INDEX
{
   POS_MAIN_INDEX_CATCH_PUT     = 0X00,//样本仓取放
   POS_MAIN_INDEX_SCAN          = 0X01,//样本仓扫描
   POS_MAIN_INDEX_PUT_RAIL      = 0X02,//变轨放入
   POS_MAIN_INDEX_CATCH_RAIL    = 0X03,//变轨取出
   POS_MAIN_INDEX_ROTATE_SCAN   = 0X04,//样本架旋转
   POS_MAIN_INDEX_CATCH_RBU     = 0X05,//联机取架位
   POS_MAIN_INDEX_PUT_RBU       = 0X06,//联机出架位
   POS_MAIN_INDEX_NONE          = 0XFF,//未知         
}POS_MAIN_INDEX;

// 吸样位置
typedef enum ABSORB_POS
{
    ABSORB_POS_TUBE1 = 0X00,
    ABSORB_POS_TUBE2 = 0X01,
    ABSORB_POS_TUBE3 = 0X02,
    ABSORB_POS_TUBE4 = 0X03,
    ABSORB_POS_TUBE5 = 0X04,
    ABSORB_POS_NONE  = 0XFF,
}ABSORB_POS;

//轨道位置
typedef enum RAIL_POS
{
    RAIL_POS_ENTRANCE       = 0X00,//
    RAIL_POS_BURRER         = 0X01,//
    RAIL_POS_WAIT_TEST      = 0X02,//
    RAIL_POS_TEST           = 0X03,//
    RAIL_POS_OUT            = 0X04,//
    RAIL_POS_NONE           = 0XFF,//
}RAIL_POS;

// 架子移动
typedef enum RACK_MOVE
{
    RACK_MOVE_ABSORB_TO_CHANGE  = 0X00,//
    RACK_MOVE_WAIT_TO_CHANGE    = 0X01,//
    RACK_MOVE_BUFFER_TO_CHANGE  = 0X02,//

    RACK_MOVE_WAIT_TO_ABOSRB    = 0X03,//
    RACK_MOVE_BUFFER_TO_ABOSRB  = 0X04,//

    RACK_MOVE_BUFFER_TO_WAIT    = 0X05,// 
    RACK_MOVE_ENTRANCE_TO_BUFFER = 0X06,//
      
}RACK_MOVE;

// 变轨位置
typedef enum RAIL_CHANGE_POS
{
    RAIL_CHANGE_POS_EM      = 0X00,
    RAIL_CHANGE_POS_NORMAL  = 0X01,
    RAIL_CHANGE_POS_BACK    = 0X02,
    RAIL_CHANGE_POS_NONE    = 0XFF,
}RAIL_CHANGE_POS;

//变轨放行
typedef enum RAIL_CHANGE_GATE
{
    RAIL_CHANGE_GATE_CLOSE   = 0X00,//阻挡
    RAIL_CHANGE_GATE_OPEN    = 0X01,//放行
}RAIL_CHANGE_GATE;

//联机进样
typedef enum ONLINE_IN_GATE{
    ONLINE_IN_GATE_CLOSE    = 0X00,//
    ONLINE_IN_GATE_OPEN     = 0X01,
}ONLINE_IN_GATE;

//轨道模块
typedef enum RAIL_MODULE_NO{
    RAIL_MODULE_NO_1 = 0,//轨道模块1
    RAIL_MODULE_NO_2 = 1,//轨道模块2
    RAIL_MODULE_NO_3 = 2,//轨道模块3
    RAIL_MODULE_NO_4 = 3,//轨道模块4
    RAIL_MODULE_NO_MAX,
}RAIL_MODULE_NO;

//电机动作结果
typedef struct MOD_SM_CAN_RESULT
{
    uint8_t motorIndex;
    int32_t motorPosCurrent;
}MOD_SM_CAN_RESULT;


#endif // _CAN_RAIL_MULIL_DATA_TYPE_H_
