/************************************************************************ 
 ** Author       : 邓小俊
 ** Date         : 2024-01-16 11:35:28
 ** Description  : When I Has Free Time ,I Will Write Description Below.
 ** description  : 
 ** LastEditors  : 邓小俊
 ** LastEditTime : 2024-01-16 11:37:35
 ** Email: xiaojun.deng@aliyun.com
 ** Copyright (c) 2024 by 公司名称, All Rights Reserved. 
 ************************************************************************/
#include "MCU_CAN1.h"
#include "CanProtPort.h"


/*-------------------------------------------------------CAN1标准帧过滤器-----------------------------------------------------*/
#if(MCU_CAN1_STD_FILTER_NBR_EXIST == CONFIG_FUNC_ENABLE)
//CAN1标准帧过滤器配置
const CAN_STD_FILTER can1StdFilterArray[] = {
    //过滤00
    {
        FDCAN_FILTER_MASK,
        FDCAN_FILTER_TO_RXFIFO0,
        0X0000000F,//过滤ID
        0X0000000F,//匹配掩码
    },
    //过滤01
    {
        FDCAN_FILTER_MASK,
        FDCAN_FILTER_TO_RXFIFO1,
        0X000000F0,//过滤ID
        0X000000F0,//匹配掩码
    },
};
#define LENGTH_CAN1_STD_FILTER_ARRAY        DIM_ARRAY_ELEMENT_COUNT(can1StdFilterArray)
//供给外部调用的,过滤元素表长度
uint16_t MCU_Can1GetStdFilterElementCount(void)
{
    return LENGTH_CAN1_STD_FILTER_ARRAY;
}
#else
#define LENGTH_CAN1_STD_FILTER_ARRAY        0
#endif

/*-------------------------------------------------------CAN1扩展帧过滤器-----------------------------------------------------*/
#if(MCU_CAN1_EXT_FILTER_NBR_EXIST == CONFIG_FUNC_ENABLE)         
//CAN1扩展帧过滤器
const CAN_EXTEND_FILTER can1ExtendFilterArray[] = {
    //过滤,PC
    {
        FDCAN_FILTER_MASK,
        FDCAN_FILTER_TO_RXFIFO1,
        CAN_PROT_FILTER_ID(SYSTEM_MODULE_NO_PC,SYSTEM_BOARD_ID_PC),//过滤ID
        CAN_PROT_FILTER_MASK,//匹配掩码
    },
};
#define LENGTH_CAN1_EXTENR_FILTER_ARRAY        DIM_ARRAY_ELEMENT_COUNT(can1ExtendFilterArray)
//供给外部调用的,过滤元素表长度
uint16_t MCU_Can1GetExtendFilterElementCount(void)
{
    return LENGTH_CAN1_EXTENR_FILTER_ARRAY;
}
#else 
#define LENGTH_CAN1_EXTENR_FILTER_ARRAY         0
#endif

/*-------------------------------------------------------CAN2标准帧过滤器-----------------------------------------------------*/
#if(MCU_CAN2_STD_FILTER_NBR_EXIST == CONFIG_FUNC_ENABLE)
//CAN2标准帧过滤器配置
const CAN_STD_FILTER can2StdFilterArray[] = {
    //过滤00
    {
        FDCAN_FILTER_MASK,
        FDCAN_FILTER_TO_RXFIFO0,
        0X0000000F,
        0X0000000F,
    },
    //过滤01
    {
        FDCAN_FILTER_MASK,
        FDCAN_FILTER_TO_RXFIFO1,
        0X000000F0,
        0X000000F0,
    },
};
#define LENGTH_CAN2_STD_FILTER_ARRAY        DIM_ARRAY_ELEMENT_COUNT(can2StdFilterArray)
//供给外部调用的,过滤元素表长度
uint16_t MCU_Can2GetStdFilterElementCount(void)
{
    return LENGTH_CAN2_STD_FILTER_ARRAY;
}
#else
//没有标准帧过滤器
#define LENGTH_CAN2_STD_FILTER_ARRAY        0
#endif













