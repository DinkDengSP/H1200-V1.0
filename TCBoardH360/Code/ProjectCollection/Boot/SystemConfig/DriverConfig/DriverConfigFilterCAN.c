/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-08-31 15:33:30 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-08-31 15:33:30 +0800
************************************************************************************************/ 
#include "DriverConfigMCU.h"
#include "DriverConfigCAN.h"
#include "DriverHeaderMCU.h"

/*--------------------------------------CAN1标准帧过滤器配置-----------------------------------*/
#if(MCU_CAN1_STD_FILTER_NBR_EXIST == CONFIG_FUNC_ENABLE)
const CAN_STD_FILTER can1StdFilterArray[] = {
    //过滤00
    {
        CAN_FILTER_FIFO0,//过滤器映射,数据是到FIFO0还是FIFO1
        0X00000000,//过滤ID,要匹配的值
        0X00000001,//过滤掩码,要匹配的哪些位
    },
    //过滤01
    {
        CAN_FILTER_FIFO1,//过滤器映射,数据是到FIFO0还是FIFO1
        0X00000001,//过滤ID,要匹配的值
        0X00000001,//过滤掩码,要匹配的哪些位
    },
};
#define LENGTH_CAN1_STD_FILTER_ARRAY        (sizeof(can1StdFilterArray)/sizeof(can1StdFilterArray[0]))
//供给外部调用的,过滤元素表长度
uint16_t MCU_Can1GetStdFilterElementCount(void)
{
    return LENGTH_CAN1_STD_FILTER_ARRAY;
}
#else
#define LENGTH_CAN1_STD_FILTER_ARRAY        0
#endif

/*--------------------------------------CAN1扩展帧过滤器配置-----------------------------------*/
#if(MCU_CAN1_EXT_FILTER_NBR_EXIST == CONFIG_FUNC_ENABLE)
//CAN1扩展帧过滤器
const CAN_EXTEND_FILTER can1ExtendFilterArray[] = {
    //过滤00
    {
        CAN_FILTER_FIFO0,
        0X00000000,
        0X00000001,
    },
    //过滤01
    {
        CAN_FILTER_FIFO1,
        0X00000001,
        0X00000001,
    },
};
#define LENGTH_CAN1_EXTENR_FILTER_ARRAY        (sizeof(can1ExtendFilterArray)/sizeof(can1ExtendFilterArray[0]))
//供给外部调用的,过滤元素表长度
uint16_t MCU_Can1GetExtendFilterElementCount(void)
{
    return LENGTH_CAN1_EXTENR_FILTER_ARRAY;
}
#else 
#define LENGTH_CAN1_EXTENR_FILTER_ARRAY         0
#endif




/*--------------------------------------CAN2标准帧过滤器配置-----------------------------------*/
#if(MCU_CAN2_STD_FILTER_NBR_EXIST == CONFIG_FUNC_ENABLE)
const CAN_STD_FILTER can2StdFilterArray[] = {
    //过滤00
    {
        CAN_FILTER_FIFO0,//过滤器映射,数据是到FIFO0还是FIFO1
        0X00000000,//过滤ID,要匹配的值
        0X00000001,//过滤掩码,要匹配的哪些位
    },
    //过滤01
    {
        CAN_FILTER_FIFO1,//过滤器映射,数据是到FIFO0还是FIFO1
        0X00000001,//过滤ID,要匹配的值
        0X00000001,//过滤掩码,要匹配的哪些位
    },
};
#define LENGTH_CAN2_STD_FILTER_ARRAY        (sizeof(can2StdFilterArray)/sizeof(can2StdFilterArray[0]))
//供给外部调用的,过滤元素表长度
uint16_t MCU_Can2GetStdFilterElementCount(void)
{
    return LENGTH_CAN2_STD_FILTER_ARRAY;
}
#else
#define LENGTH_CAN2_STD_FILTER_ARRAY        0
#endif

/*--------------------------------------CAN2扩展帧过滤器配置-----------------------------------*/
#if(MCU_CAN2_EXT_FILTER_NBR_EXIST == CONFIG_FUNC_ENABLE)
//CAN2扩展帧过滤器
const CAN_EXTEND_FILTER can2ExtendFilterArray[] = {
    //过滤00
    {
        CAN_FILTER_FIFO0,
        0X00000000,
        0X00000001,
    },
    //过滤01
    {
        CAN_FILTER_FIFO1,
        0X00000001,
        0X00000001,
    },
};
#define LENGTH_CAN2_EXTENR_FILTER_ARRAY        (sizeof(can2ExtendFilterArray)/sizeof(can2ExtendFilterArray[0]))
//供给外部调用的,过滤元素表长度
uint16_t MCU_Can2GetExtendFilterElementCount(void)
{
    return LENGTH_CAN2_EXTENR_FILTER_ARRAY;
}
#else 
#define LENGTH_CAN2_EXTENR_FILTER_ARRAY         0
#endif

