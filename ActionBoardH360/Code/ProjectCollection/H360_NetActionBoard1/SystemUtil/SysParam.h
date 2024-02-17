/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-08-29 14:07:30 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-12-04 17:57:39 +0800
************************************************************************************************/ 
#ifndef __SYS_PARAM_H_
#define __SYS_PARAM_H_
#include "SysParamDefault.h"

//主序号
typedef enum INDEX_MAIN_SYS_PARAM
{
    INDEX_MAIN_SYS_PARAM_PLATE_WASH                 = 0X00,//清洗盘
    INDEX_MAIN_SYS_PARAM_MIX_WASH                   = 0X01,//清洗混匀
    INDEX_MAIN_SYS_PARAM_NEEDLE_WASH                = 0X02,//清洗针
    INDEX_MAIN_SYS_PARAM_BUFFER_INJECT              = 0X03,//缓冲液注液
    INDEX_MAIN_SYS_PARAM_LIQUID_AUTO                = 0X04,//液路自动化   
    INDEX_MAIN_SYS_PARAM_OTHER                      = 0X05,//其他参数
}INDEX_MAIN_SYS_PARAM;

//清洗盘参数子序号
typedef enum INDEX_SUB_PARAM_PLATE_WASH
{
    INDEX_SUB_PARAM_PLATE_WASH_RESET_CORRECT    = 0X00,//清洗盘零位修正
}INDEX_SUB_PARAM_PLATE_WASH;

//清洗混匀参数子序号
typedef enum INDEX_SUB_PARAM_MIX_WASH
{
    INDEX_SUB_PARAM_MIX_WASH_RESET_CORRECT_UPDOWN   = 0X00,//清洗摇匀升降零位修正
    INDEX_SUB_PARAM_MIX_WASH_RESET_CORRECT_ROTATE   = 0X01,//清洗摇匀旋转零位修正
    INDEX_SUB_PARAM_MIX_WASH_UP_POS_MODE1           = 0X02,//清洗摇匀模式1升降高度
    INDEX_SUB_PARAM_MIX_WASH_ROTATE_TIME_MS_MODE1   = 0X03,//清洗摇匀模式1摇匀时间,毫秒
    INDEX_SUB_PARAM_MIX_WASH_UP_POS_MODE2           = 0X04,//清洗摇匀模式2升降高度
    INDEX_SUB_PARAM_MIX_WASH_ROTATE_TIME_MS_MODE2   = 0X05,//清洗摇匀模式2摇匀时间,毫秒
    INDEX_SUB_PARAM_MIX_WASH_UP_POS_MODE3           = 0X06,//清洗摇匀模式3升降高度
    INDEX_SUB_PARAM_MIX_WASH_ROTATE_TIME_MS_MODE3   = 0X07,//清洗摇匀模式3摇匀时间,毫秒
    INDEX_SUB_PARAM_MIX_WASH_UP_POS_MODE4           = 0X08,//清洗摇匀模式4升降高度
    INDEX_SUB_PARAM_MIX_WASH_ROTATE_TIME_MS_MODE4   = 0X09,//清洗摇匀模式4摇匀时间,毫秒
    INDEX_SUB_PARAM_MIX_WASH_UP_POS_MODE5           = 0X0A,//清洗摇匀模式5升降高度
    INDEX_SUB_PARAM_MIX_WASH_ROTATE_TIME_MS_MODE5   = 0X0B,//清洗摇匀模式5摇匀时间,毫秒
    INDEX_SUB_PARAM_MIX_WASH_UP_POS_MODE6           = 0X0C,//清洗摇匀模式6升降高度
    INDEX_SUB_PARAM_MIX_WASH_ROTATE_TIME_MS_MODE6   = 0X0D,//清洗摇匀模式6摇匀时间,毫秒
    INDEX_SUB_PARAM_MIX_WASH_UP_POS_MODE7           = 0X0E,//清洗摇匀模式7升降高度
    INDEX_SUB_PARAM_MIX_WASH_ROTATE_TIME_MS_MODE7   = 0X0F,//清洗摇匀模式7摇匀时间,毫秒
    INDEX_SUB_PARAM_MIX_WASH_UP_POS_MODE8           = 0X10,//清洗摇匀模式8升降高度
    INDEX_SUB_PARAM_MIX_WASH_ROTATE_TIME_MS_MODE8   = 0X11,//清洗摇匀模式8摇匀时间,毫秒
    INDEX_SUB_PARAM_MIX_WASH_UP_POS_MODE9           = 0X12,//清洗摇匀模式9升降高度
    INDEX_SUB_PARAM_MIX_WASH_ROTATE_TIME_MS_MODE9   = 0X13,//清洗摇匀模式9摇匀时间,毫秒
    INDEX_SUB_PARAM_MIX_WASH_UP_POS_MODE10          = 0X14,//清洗摇匀模式10升降高度
    INDEX_SUB_PARAM_MIX_WASH_ROTATE_TIME_MS_MODE10  = 0X15,//清洗摇匀模式10摇匀时间,毫秒
}INDEX_SUB_PARAM_MIX_WASH;

//清洗针参数子序号
typedef enum INDEX_SUB_PARAM_NEEDLE_WASH
{
    INDEX_SUB_PARAM_NEEDLE_WASH_RESET_CORRECT_UPDOWN                    = 0X00,//清洗针升降零位修正
    INDEX_SUB_PARAM_NEEDLE_WASH_POS_CUP_TOP_NEEDLE_UPDOWN               = 0X01,//清洗针升降到杯口坐标
    INDEX_SUB_PARAM_NEEDLE_WASH_POS_CUP_BOTTOM_NEEDLE_UPDOWN            = 0X02,//清洗针升降到杯底坐标
    INDEX_SUB_PARAM_NEEDLE_WASH_UL_ABSORB_WASH_PUMP1                    = 0X03,//清洗针注射泵1吸液量,微升
    INDEX_SUB_PARAM_NEEDLE_WASH_UL_ABSORB_WASH_PUMP2                    = 0X04,//清洗针注射泵2吸液量,微升
    INDEX_SUB_PARAM_NEEDLE_WASH_UL_ABSORB_WASH_PUMP3                    = 0X05,//清洗针注射泵3吸液量,微升
    INDEX_SUB_PARAM_NEEDLE_WASH_UL_ABSORB_WASH_PUMP4                    = 0X06,//清洗针注射泵4吸液量,微升
    INDEX_SUB_PARAM_NEEDLE_WASH_UL_ABSORB_WASH_PUMP5                    = 0X07,//清洗针注射泵5吸液量,微升
    INDEX_SUB_PARAM_NEEDLE_WASH_TIME_MS_WAIT_WASTE_LIQUID_AT_CUP_BOTTOM = 0X08,//清洗针在杯底等待抽废液时间,毫秒
    INDEX_SUB_PARAM_NEEDLE_WASH_TIME_MS_WAIT_INJECT_STABLE_AT_CUP_TOP   = 0X09,//清洗针在杯口等待注液稳定时间,毫秒
    INDEX_SUB_PARAM_NEEDLE_WASH_TIME_MS_WAIT_WASTE_LIQUID_AT_PRIME      = 0X0A,//清洗针灌注一次等抽废液完成时间,毫秒
    INDEX_SUB_PARAM_NEEDLE_WASH_POS_UPDOWN_TO_SOAK                      = 0X0B,//清洗针升降到浸泡位
    INDEX_SUB_PARAM_NEEDLA_WASH_TIME_MS_MAINTAIN_CLEAN                  = 0X0C,//清洗针维护清洗抽液等待时间
    INDEX_SUB_PARAM_NEEDLE_WASH_THREE_HOLE_NEEDLE_2_5_CUP_BUTTOM_INJECT_DELAY_TIME  = 0X0D,//清洗针三孔类型2-5针杯底注液延时时间
    INDEX_SUB_PARAM_NEEDLE_WASH_THREE_HOLE_NEEDLE_1_INJECT_WAIT_NEEDLE_UP_TIME  = 0X0E,//清洗针三孔类型1号针注液前等待针抬起时间    
    INDEX_SUB_PARAM_NEEDLE_WASH_BLOCK_UP_THREHOLD                       = 0X0F,//清洗针堵针阈值上限 
}INDEX_SUB_PARAM_NEEDLE_WASH;

//缓冲液注液参数子序号
typedef enum INDEX_SUB_PARAM_BUFFER_INJECT
{
    INDEX_SUB_PARAM_BUFFER_INJECT_RESET_CORRECT_PUMP    = 0X00,//缓冲液注射泵零位修正
}INDEX_SUB_PARAM_BUFFER_INJECT;

//液路自动化参数子序号
typedef enum INDEX_SUB_PARAM_LIQUID_AUTO
{
    INDEX_SUB_PARAM_LIQUID_AUTO_RESERVE                 = 0X00,//预留参数,现在没用
}INDEX_SUB_PARAM_LIQUID_AUTO;

//应用层辅助参数子序号
typedef enum INDEX_SUB_PARAM_OTHER
{
    INDEX_SUB_PARAM_OTHER_BASE                          = 0X00,//预留参数,现在没用
}INDEX_SUB_PARAM_OTHER;


//初始化系统参数
void SysParamInit(void);

//写入参数
ERROR_SUB SysParamWriteSingle(INDEX_MAIN_SYS_PARAM mainIndex,uint8_t subIndex,int32_t paramWrite);

//读取参数
ERROR_SUB SysParamReadSingle(INDEX_MAIN_SYS_PARAM mainIndex,uint8_t subIndex,int32_t* paramRead);


#endif








