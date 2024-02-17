/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-08-29 14:07:30 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-12-21 13:22:44 +0800
************************************************************************************************/ 
#ifndef __SYS_PARAM_H_
#define __SYS_PARAM_H_
#include "SysParamDefault.h"

//主序号
typedef enum INDEX_MAIN_SYS_PARAM
{
    INDEX_MAIN_SYS_PARAM_HAND_NEW_CUP       = 0X00,//新杯机械手
    INDEX_MAIN_SYS_PARAM_NEEDLE_SAMPLE      = 0X01,//样本针
    INDEX_MAIN_SYS_PARAM_LIQUID_DETECT      = 0X02,//探液板
    INDEX_MAIN_SYS_PARAM_RESERVE            = 0X03,//保留参数
}INDEX_MAIN_SYS_PARAM;

//新杯机械手 参数集合
typedef enum INDEX_SUB_PARAM_HAND_NEW_CUP
{
    INDEX_SUB_PARAM_HAND_NEW_CUP_RESET_CORRECT_UP_DOWN                      = 0X00,//新杯机械手升降零位修正
    INDEX_SUB_PARAM_HAND_NEW_CUP_RESET_CORRECT_LEFT_RIGHT                   = 0x01,//新杯机械手左右零位修正
    INDEX_SUB_PARAM_HAND_NEW_CUP_RESET_CORRECT_FRONT_BACK                   = 0x02,//新杯机械手前后零位修正
    INDEX_SUB_PARAM_HAND_NEW_CUP_POS_NEW_CUP_TRAY_FRONT_BACK_HOLE_NO_1      = 0x03,//新杯机械手前后新杯盘1号孔坐标
    INDEX_SUB_PARAM_HAND_NEW_CUP_POS_NEW_CUP_TRAY_LEFT_RIGHT_HOLE_NO_1      = 0x04,//新杯机械手左右新杯盘1号孔坐标
    INDEX_SUB_PARAM_HAND_NEW_CUP_POS_NEW_CUP_TRAY_FRONT_BACK_HOLE_NO_140    = 0x05,//新杯机械手前后新杯盘140号孔坐标
    INDEX_SUB_PARAM_HAND_NEW_CUP_POS_NEW_CUP_TRAY_LEFT_RIGHT_HOLE_NO_140    = 0x06,//新杯机械手左右新杯盘140号孔坐标
    INDEX_SUB_PARAM_HAND_NEW_CUP_POS_NEW_CUP_TRAY_UP_DOWN_CATCH_CUP         = 0x07,//新杯机械手升降新杯盘取杯坐标
    INDEX_SUB_PARAM_HAND_NEW_CUP_POS_FRONT_BACK_PLATE_REACTION              = 0x08,//新杯机械手前后反应盘坐标
    INDEX_SUB_PARAM_HAND_NEW_CUP_POS_LEFT_RIGHT_PLATE_REACTION              = 0x09,//新杯机械手左右反应盘坐标
    INDEX_SUB_PARAM_HAND_NEW_CUP_POS_PLATE_REACTION_UP_DOWN_CATCH_CUP       = 0x0A,//新杯机械手升降反应盘取杯坐标
    INDEX_SUB_PARAM_HAND_NEW_CUP_POS_PLATE_REACTION_UP_DOWN_PUT_CUP         = 0x0B,//新杯机械手升降反应盘放杯坐标
    INDEX_SUB_PARAM_HAND_NEW_CUP_POS_FRONT_BACK_GARBAGE_1                   = 0x0C,//新杯机械手前后垃圾桶1坐标
    INDEX_SUB_PARAM_HAND_NEW_CUP_POS_LEFT_RIGHT_GARBAGE_1                   = 0x0D,//新杯机械手左右垃圾桶1坐标
    INDEX_SUB_PARAM_HAND_NEW_CUP_POS_UP_DOWN_PUT_CUP_GARBAGE_1              = 0x0E,//新杯机械手升降垃圾桶1丢杯坐标
    INDEX_SUB_PARAM_HAND_NEW_CUP_POS_FRONT_BACK_GARBAGE_2                   = 0x0F,//新杯机械手前后垃圾桶2坐标
    INDEX_SUB_PARAM_HAND_NEW_CUP_POS_LEFT_RIGHT_GARBAGE_2                   = 0x10,//新杯机械手左右垃圾桶2坐标
    INDEX_SUB_PARAM_HAND_NEW_CUP_POS_UP_DOWN_PUT_CUP_GARBAGE_2              = 0x11,//新杯机械手升降垃圾桶2丢杯坐标
}INDEX_SUB_PARAM_HAND_NEW_CUP;

//样本针 参数集合
typedef enum INDEX_SUB_PARAM_NEEDLE_SAMPLE
{
    INDEX_SUB_PARAM_NEEDLE_SAMPLE_RESET_CORRECT_ROTATE                                          = 0X00,//样本针旋转零位修正
    INDEX_SUB_PARAM_NEEDLE_SAMPLE_RESET_CORRECT_UPDOWN                                          = 0x01,//样本针升降零位修正
    INDEX_SUB_PARAM_NEEDLE_SAMPLE_POS_ROTATE_POWERFUL_CLEAN                                     = 0x02,//样本针旋转到强洗位坐标
    INDEX_SUB_PARAM_NEEDLE_SAMPLE_POS_UP_DOWN_POWERFUL_CLEAN                                    = 0x03,//样本针升降到强洗位坐标
    INDEX_SUB_PARAM_NEEDLE_SAMPLE_TIME_MS_POWERFUL_CLEAN                                        = 0x04,//样本针强洗时间,置换清洗液 毫秒
    INDEX_SUB_PARAM_NEEDLE_SAMPLE_TIME_MS_PRIME_POWERFUL_CLEAN                                  = 0x05,//样本针强洗液灌注时间 毫秒
    INDEX_SUB_PARAM_NEEDLE_SAMPLE_POS_ROTATE_CLEAN                                              = 0x06,//样本针旋转到清洗位
    INDEX_SUB_PARAM_NEEDLE_SAMPLE_POS_UP_DOWN_CLEAN                                             = 0x07,//样本针升降到清洗位坐标
    INDEX_SUB_PARAM_NEEDLE_SAMPLE_POS_ROTATE_SAMPLE_RACK                                        = 0x08,//样本针旋转到试管架吸样位坐标
    INDEX_SUB_PARAM_NEEDLE_SAMPLE_POS_UP_DOWN_SAMPLE_RACK_FLAT                                  = 0x09,//样本针到试管架水平面高度
    INDEX_SUB_PARAM_NEEDLE_SAMPLE_POS_UP_DOWN_SAMPLE_RACK_LIQUID_DETECT_START_TUBE_TYPE_4       = 0x0A,//样本针吸样探液起始点,试管类型4 子弹头
    INDEX_SUB_PARAM_NEEDLE_SAMPLE_POS_UP_DOWN_SAMPLE_RACK_LIQUID_DETECT_MAX_LIMIT_TUBE_TYPE_4   = 0x0B,//样本针吸样探液极限点,试管类型4 子弹头
    INDEX_SUB_PARAM_NEEDLE_SAMPLE_POS_ROTATE_PLATE_REACTION_ABSORB                              = 0x0C,//样本针旋转到反应盘吸液坐标
    INDEX_SUB_PARAM_NEEDLE_SAMPLE_POS_UP_DOWN_PLATE_REACTION_ABSORB_AT_BOTTOM                   = 0x0D,//样本针升降反应盘吸液杯底高度
    INDEX_SUB_PARAM_NEEDLE_SAMPLE_POS_UP_DOWN_PLATE_REACTION_ABSORB_CORRECT                     = 0x0E,//样本针升降反应盘吸液修正高度
    INDEX_SUB_PARAM_NEEDLE_SAMPLE_POS_ROTATE_PLATE_REACTION_INJECT                              = 0x0F,//样本针旋转到反应盘注液坐标
    INDEX_SUB_PARAM_NEEDLE_SAMPLE_POS_UP_DOWN_PLATE_REACTION_INJECT_AT_BOTTOM                   = 0x10,//样本针升降到反应盘杯底高度
    INDEX_SUB_PARAM_NEEDLE_SAMPLE_POS_UP_DOWN_PLATE_REACTION_INJECT_CORRECT                     = 0x11,//样本针升降到反应盘注液修正高度
    INDEX_SUB_PARAM_NEEDLE_SAMPLE_UL_ABSORB_AIR_AFTER_CLEAN                                     = 0x12,//样本针清洗完吸空气量
    INDEX_SUB_PARAM_NEEDLE_SAMPLE_UL_ABSORB_AIR_AFTER_LIQUID_ABSORB                             = 0x13,//样本针吸样完吸空气量
    INDEX_SUB_PARAM_NEEDLE_SAMPLE_UL_RESERVE_AIR_AFTER_INJECT                                   = 0x14,//样本针注液保留空气量
    INDEX_SUB_PARAM_NEEDLE_SAMPLE_LIQUID_DETECT_REAL_TIME_COMM                                  = 0x15,//样本针探液通讯使能
    INDEX_SUB_PARAM_NEEDLE_SAMPLE_POS_ROTATE_EMERGENCY                                          = 0x16,//样本针旋转到急诊吸样位
    INDEX_SUB_PARAM_NEEDLE_SAMPLE_PUMP_VERSION                                                  = 0x17,//样本针泵版本，0：250ul2000步，1：250ul4000步
}INDEX_SUB_PARAM_NEEDLE_SAMPLE;

//探液板参数序号
typedef enum INDEX_SUB_PARAM_LIQUID_DETECT_BOARD
{
    INDEX_SUB_PARAM_LIQUID_DETECT_BOARD_CONFIG1_THRESHOLD   = 0X00,//样本针探液板探液阈值1
    INDEX_SUB_PARAM_LIQUID_DETECT_BOARD_CONFIG1_SLOPE       = 0X01,//样本针探液板探液斜率1
    INDEX_SUB_PARAM_LIQUID_DETECT_BOARD_CONFIG2_THRESHOLD   = 0X02,//样本针探液板探液阈值2
    INDEX_SUB_PARAM_LIQUID_DETECT_BOARD_CONFIG2_SLOPE       = 0X03,//样本针探液板探液斜率2
    INDEX_SUB_PARAM_LIQUID_DETECT_BOARD_CONFIG3_THRESHOLD   = 0X04,//样本针探液板探液阈值3
    INDEX_SUB_PARAM_LIQUID_DETECT_BOARD_CONFIG3_SLOPE       = 0X05,//样本针探液板探液斜率3
    INDEX_SUB_PARAM_LIQUID_DETECT_BOARD_CONFIG4_THRESHOLD   = 0X06,//样本针探液板探液阈值4
    INDEX_SUB_PARAM_LIQUID_DETECT_BOARD_CONFIG4_SLOPE       = 0X07,//样本针探液板探液斜率4
    INDEX_SUB_PARAM_LIQUID_DETECT_BOARD_CONFIG5_THRESHOLD   = 0X08,//样本针探液板探液阈值5
    INDEX_SUB_PARAM_LIQUID_DETECT_BOARD_CONFIG5_SLOPE       = 0X09,//样本针探液板探液斜率5
    INDEX_SUB_PARAM_LIQUID_DETECT_BOARD_CONFIG6_THRESHOLD   = 0X0A,//样本针探液板探液阈值6
    INDEX_SUB_PARAM_LIQUID_DETECT_BOARD_CONFIG6_SLOPE       = 0X0B,//样本针探液板探液斜率6
    INDEX_SUB_PARAM_LIQUID_DETECT_BOARD_CONFIG7_THRESHOLD   = 0X0C,//样本针探液板探液阈值7
    INDEX_SUB_PARAM_LIQUID_DETECT_BOARD_CONFIG7_SLOPE       = 0X0D,//样本针探液板探液斜率7
    INDEX_SUB_PARAM_LIQUID_DETECT_BOARD_CONFIG8_THRESHOLD   = 0X0E,//样本针探液板探液阈值8
    INDEX_SUB_PARAM_LIQUID_DETECT_BOARD_CONFIG8_SLOPE       = 0X0F,//样本针探液板探液斜率8
    INDEX_SUB_PARAM_LIQUID_DETECT_BOARD_CONFIG9_THRESHOLD   = 0X10,//样本针探液板探液阈值9
    INDEX_SUB_PARAM_LIQUID_DETECT_BOARD_CONFIG9_SLOPE       = 0X11,//样本针探液板探液斜率9
    INDEX_SUB_PARAM_LIQUID_DETECT_BOARD_CONFIG10_THRESHOLD  = 0X12,//样本针探液板探液阈值10
    INDEX_SUB_PARAM_LIQUID_DETECT_BOARD_CONFIG10_SLOPE      = 0X13,//样本针探液板探液斜率10
    INDEX_SUB_PARAM_LIQUID_DETECT_BOARD_VOL_REALTIME        = 0X14,//样本针探液板针尖电压
    INDEX_SUB_PARAM_LIQUID_DETECT_BOARD_BASE_RES            = 0X15,//样本针探液板基准电阻值
    INDEX_SUB_PARAM_LIQUID_DETECT_BOARD_VOL_BASE            = 0X16,//样本针针尖基准电压
}INDEX_SUB_PARAM_LIQUID_DETECT_BOARD;

//系统保留 参数集合
typedef enum INDEX_SUB_PARAM_RESERVE
{
    INDEX_SUB_PARAM_RESERVE_OTHER                           = 0X00,//辅助功能 
}INDEX_SUB_PARAM_RESERVE;


//初始化系统参数
void SysParamInit(void);

//写入参数
ERROR_SUB SysParamWriteSingle(INDEX_MAIN_SYS_PARAM mainIndex,uint8_t subIndex,int32_t paramWrite);

//读取参数
ERROR_SUB SysParamReadSingle(INDEX_MAIN_SYS_PARAM mainIndex,uint8_t subIndex,int32_t* paramRead);


#endif








