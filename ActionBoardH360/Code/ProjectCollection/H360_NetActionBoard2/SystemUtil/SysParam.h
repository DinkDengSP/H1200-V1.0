/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-08-29 14:07:30 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-11-22 15:13:40 +0800
************************************************************************************************/ 
#ifndef __SYS_PARAM_H_
#define __SYS_PARAM_H_
#include "SysParamDefault.h"

//主序号
typedef enum INDEX_MAIN_SYS_PARAM
{
    INDEX_MAIN_SYS_PARAM_PLATE_REAGENT      = 0X00,//试剂盘参数
    INDEX_MAIN_SYS_PARAM_CUP_TRAY_MANAGE    = 0X01,//杯栈管理参数
    INDEX_MAIN_SYS_PARAM_NEEDLE_BEAD        = 0X02,//磁珠针参数
    INDEX_MAIN_SYS_PARAM_LIQUID_DETECT      = 0X03,//探液板参数
    INDEX_MAIN_SYS_PARAM_OTHER              = 0X04,//辅助参数
}INDEX_MAIN_SYS_PARAM;


//试剂盘参数子序号,序号从0开始
typedef enum INDEX_SUB_PARAM_PLATE_REAGENT
{
    INDEX_SUB_PARAM_PLATE_REAGENT_ROTATE_RESET_CORRECT		                            = 0X00,//试剂盘零位修正
    INDEX_SUB_PARAM_PLATE_REAGENT_OFFSET_NEEDLE_REAGENT_WITH_R1                         = 0X01,//试剂盘相对试剂针到R1偏移
    INDEX_SUB_PARAM_PLATE_REAGENT_OFFSET_NEEDLE_REAGENT_WITH_R2                         = 0X02,//试剂盘相对试剂针到R2偏移
    INDEX_SUB_PARAM_PLATE_REAGENT_OFFSET_NEEDLE_BEAD_WITH_R1	                        = 0X03,//试剂盘相对磁珠针到R1偏移
    INDEX_SUB_PARAM_PLATE_REAGENT_OFFSET_NEEDLE_BEAD_WITH_R2	                        = 0X04,//试剂盘相对磁珠针到R2偏移
    INDEX_SUB_PARAM_PLATE_REAGENT_OFFSET_NEEDLE_BEAD_WITH_BEAD                          = 0X05,//试剂盘相对磁珠针到磁珠偏移
    INDEX_SUB_PARAM_PLATE_REAGENT_OFFSET_WITH_QR_SCAN			                        = 0X06,//试剂盘到扫码位偏移
}INDEX_SUB_PARAM_PLATE_REAGENT;

//杯栈管理参数子序号,序号从0开始
typedef enum INDEX_SUB_PARAM_CUP_TRAY_MANAGE
{
    INDEX_SUB_PARAM_CUP_TRAY_MANAGE_NEW_CUP_TRAY_RESET_CORRECT				            = 0X00,//新杯盘升降电机零位修正
    INDEX_SUB_PARAM_CUP_TRAY_MANAGE_EMPTY_CUP_TRAY_RESET_CORRECT		                = 0X01,//空杯盘升降电机零位修正
    INDEX_SUB_PARAM_CUP_TRAY_MANAGE_CLAMP_RESET_CORRECT						            = 0X02,//夹手电机零位修正
    INDEX_SUB_PARAM_CUP_TRAY_MANAGE_PUSH_RESET_CORRECT						            = 0X03,//推杆电机零位修正
    INDEX_SUB_PARAM_CUP_TRAY_MANAGE_POS_CLAMP_OPEN							            = 0X04,//夹手打开坐标
    INDEX_SUB_PARAM_CUP_TRAY_MANAGE_POS_CLAMP_CLOSE							            = 0X05,//夹手关闭坐标
    INDEX_SUB_PARAM_CUP_TRAY_MANAGE_POS_PUSH_TRAY_TO_TEST						        = 0X06,//推杆推新盘到测试位坐标
    INDEX_SUB_PARAM_CUP_TRAY_MANAGE_OFFSET_TOP_TO_SENSOR_WHEN_NEW_CUP_TRAY_FULL	        = 0X07,//新杯栈满时,顶部到检测位偏移
    INDEX_SUB_PARAM_CUP_TRAY_MANAGE_OFFSET_TOP_TO_PALTFORM_WHEN_NEW_CUP_TRAY_FULL       = 0X08,//新杯栈满时,顶部到平台偏移
    INDEX_SUB_PARAM_CUP_TRAY_MANAGE_SPACING_LAYER_NEW_CUP_TRAY					        = 0X09,//新杯栈上升一个杯盘的偏移
    INDEX_SUB_PARAM_CUP_TRAY_MANAGE_OFFSET_TOP_TO_SENSOR_WHEN_EMPTY_CUP_TRAY_FULL       = 0X0A,//空杯栈满时,顶部到检测位偏移
    INDEX_SUB_PARAM_CUP_TRAY_MANAGE_OFFSET_TOP_TO_PALTFORM_WHEN_EMPTY_CUP_TRAY_FULL     = 0X0B,//空杯栈满时,顶部到承接平台偏移
    INDEX_SUB_PARAM_CUP_TRAY_MANAGE_SPACING_LAYER_EMPTY_CUP_TRAY					    = 0X0C,//空杯栈上升一个杯盘的偏移
}INDEX_SUB_PARAM_CUP_TRAY_MANAGE;

//磁珠针参数子序号,序号从0开始
typedef enum INDEX_SUB_PARAM_NEEDLE_BEAD
{
    INDEX_SUB_PARAM_NEEDLE_BEAD_ROTATE_RESET_CORRECT					                = 0X00,//磁珠针旋转零位修正
    INDEX_SUB_PARAM_NEEDLE_BEAD_UPDOWN_RESET_CORRECT					                = 0X01,//磁珠针升降零位修正
    INDEX_SUB_PARAM_NEEDLE_BEAD_POS_ROTATE_TO_CLEAN						                = 0X02,//磁珠针旋转到清洗位坐标
    INDEX_SUB_PARAM_NEEDLE_BEAD_POS_UPDOWN_CLEAN						                = 0X03,//磁珠针升降到清洗位坐标
    INDEX_SUB_PARAM_NEEDLE_BEAD_POS_ROTATE_TO_R1						                = 0X04,//磁珠针旋转到R1坐标
    INDEX_SUB_PARAM_NEEDLE_BEAD_POS_UPDOWN_R1_LIQUID_DETECT_MAX_LIMIT	                = 0X05,//磁珠针升降R1吸样探液极限点
    INDEX_SUB_PARAM_NEEDLE_BEAD_POS_ROTATE_TO_R2						                = 0X06,//磁珠针旋转到R2坐标
    INDEX_SUB_PARAM_NEEDLE_BEAD_POS_UPDOWN_R2_LIQUID_DETECT_MAX_LIMIT	                = 0X07,//磁珠针升降R2吸样探液极限点
    INDEX_SUB_PARAM_NEEDLE_BEAD_POS_ROTATE_TO_BEAD						                = 0X08,//磁珠针旋转到磁珠位坐标
    INDEX_SUB_PARAM_NEEDLE_BEAD_POS_UPDOWN_BEAD_LIQUID_DETECT_MAX_LIMIT	                = 0X09,//磁珠针升降磁珠位吸样探液极限点
    INDEX_SUB_PARAM_NEEDLE_BEAD_POS_ROTATE_TO_PLATE_REACTION			                = 0X0A,//磁珠针旋转到反应盘排样坐标
    INDEX_SUB_PARAM_NEEDLE_BEAD_POS_UPDOWN_INJECT_AT_BOTTOM			                    = 0X0B,//磁珠针升降反应盘排样杯底高度
    INDEX_SUB_PARAM_NEEDLE_BEAD_POS_UPDOWN_INJECT_CORRECT				                = 0X0C,//磁珠针升降反应盘排样修正高度
    INDEX_SUB_PARAM_NEEDLE_BEAD_UL_ABSORB_AIR_AFTER_CLEAN				                = 0X0D,//磁珠针清洗完吸空气量
    INDEX_SUB_PARAM_NEEDLE_BEAD_UL_ABSORB_AIR_AFTER_LIQUID_ABSORB		                = 0X0E,//磁珠针吸样完吸空气量
    INDEX_SUB_PARAM_NEEDLE_BEAD_UL_RESERVE_AIR_AFTER_INJECT				                = 0X0F,//磁珠针注液保留空气量
    INDEX_SUB_PARAM_NEEDLE_BEAD_LIQUID_DETECT_REAL_TIME_COMM			                = 0X10,//磁珠针探液通讯使能
    INDEX_SUB_PARAM_NEEDLE_BEAD_UPDOWN_LITTLE_BEAD_MIX			                        = 0X11,//磁珠针小磁珠吸吐混匀高度
}INDEX_SUB_PARAM_NEEDLE_BEAD;

//探液板参数序号
typedef enum INDEX_SUB_PARAM_LIQUID_DETECT_BOARD
{
    INDEX_SUB_PARAM_LIQUID_DETECT_BOARD_CONFIG1_THRESHOLD                               = 0X00,//磁珠针探液板探液阈值1
    INDEX_SUB_PARAM_LIQUID_DETECT_BOARD_CONFIG1_SLOPE                                   = 0X01,//磁珠针探液板探液斜率1
    INDEX_SUB_PARAM_LIQUID_DETECT_BOARD_CONFIG2_THRESHOLD                               = 0X02,//磁珠针探液板探液阈值2
    INDEX_SUB_PARAM_LIQUID_DETECT_BOARD_CONFIG2_SLOPE                                   = 0X03,//磁珠针探液板探液斜率2
    INDEX_SUB_PARAM_LIQUID_DETECT_BOARD_CONFIG3_THRESHOLD                               = 0X04,//磁珠针探液板探液阈值3
    INDEX_SUB_PARAM_LIQUID_DETECT_BOARD_CONFIG3_SLOPE                                   = 0X05,//磁珠针探液板探液斜率3
    INDEX_SUB_PARAM_LIQUID_DETECT_BOARD_CONFIG4_THRESHOLD                               = 0X06,//磁珠针探液板探液阈值4
    INDEX_SUB_PARAM_LIQUID_DETECT_BOARD_CONFIG4_SLOPE                                   = 0X07,//磁珠针探液板探液斜率4
    INDEX_SUB_PARAM_LIQUID_DETECT_BOARD_CONFIG5_THRESHOLD                               = 0X08,//磁珠针探液板探液阈值5
    INDEX_SUB_PARAM_LIQUID_DETECT_BOARD_CONFIG5_SLOPE                                   = 0X09,//磁珠针探液板探液斜率5
    INDEX_SUB_PARAM_LIQUID_DETECT_BOARD_CONFIG6_THRESHOLD                               = 0X0A,//磁珠针探液板探液阈值6
    INDEX_SUB_PARAM_LIQUID_DETECT_BOARD_CONFIG6_SLOPE                                   = 0X0B,//磁珠针探液板探液斜率6
    INDEX_SUB_PARAM_LIQUID_DETECT_BOARD_CONFIG7_THRESHOLD                               = 0X0C,//磁珠针探液板探液阈值7
    INDEX_SUB_PARAM_LIQUID_DETECT_BOARD_CONFIG7_SLOPE                                   = 0X0D,//磁珠针探液板探液斜率7
    INDEX_SUB_PARAM_LIQUID_DETECT_BOARD_CONFIG8_THRESHOLD                               = 0X0E,//磁珠针探液板探液阈值8
    INDEX_SUB_PARAM_LIQUID_DETECT_BOARD_CONFIG8_SLOPE                                   = 0X0F,//磁珠针探液板探液斜率8
    INDEX_SUB_PARAM_LIQUID_DETECT_BOARD_CONFIG9_THRESHOLD                               = 0X10,//磁珠针探液板探液阈值9
    INDEX_SUB_PARAM_LIQUID_DETECT_BOARD_CONFIG9_SLOPE                                   = 0X11,//磁珠针探液板探液斜率9
    INDEX_SUB_PARAM_LIQUID_DETECT_BOARD_CONFIG10_THRESHOLD                              = 0X12,//磁珠针探液板探液阈值10
    INDEX_SUB_PARAM_LIQUID_DETECT_BOARD_CONFIG10_SLOPE                                  = 0X13,//磁珠针探液板探液斜率10
    INDEX_SUB_PARAM_LIQUID_DETECT_BOARD_VOL_REALTIME                                    = 0X14,//磁珠针探液板针尖电压
    INDEX_SUB_PARAM_LIQUID_DETECT_BOARD_BASE_RES                                        = 0X15,//磁珠针探液板基准电阻值
    INDEX_SUB_PARAM_LIQUID_DETECT_BOARD_VOL_BASE                                        = 0X16,//磁珠针针尖基准电压
}INDEX_SUB_PARAM_LIQUID_DETECT_BOARD;


//辅助参数参数子序号,序号从0开始
typedef enum INDEX_SUB_PARAM_OTHER
{
    INDEX_SUB_PARAM_OTHER_UTIL                                                          = 0X00,//辅助参数
}INDEX_SUB_PARAM_OTHER;


//初始化系统参数
void SysParamInit(void);

//写入参数
ERROR_SUB SysParamWriteSingle(INDEX_MAIN_SYS_PARAM mainIndex,uint8_t subIndex,int32_t paramWrite);

//读取参数
ERROR_SUB SysParamReadSingle(INDEX_MAIN_SYS_PARAM mainIndex,uint8_t subIndex,int32_t* paramRead);


#endif








