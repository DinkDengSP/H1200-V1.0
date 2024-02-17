/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-11-03 21:10:26 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-11-06 10:18:05 +0800
************************************************************************************************/ 
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















