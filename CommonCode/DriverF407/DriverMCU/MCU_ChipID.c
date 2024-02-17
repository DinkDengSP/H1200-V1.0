/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-08-23 09:44:56 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-08-23 10:52:24 +0800
************************************************************************************************/ 
#include "MCU_ChipID.h"

//读取芯片的电子签名和Flash尺寸信息
void MCU_ChipID_Read(MCU_CHIP_ID* idPtr)
{
    //设备电子签名
    idPtr->idSn0 = *(__IO uint32_t*)(0X1FFF7A10);
    idPtr->idSn1 = *(__IO uint32_t*)(0X1FFF7A10 + 4);
    idPtr->idSn2 = *(__IO uint32_t*)(0X1FFF7A10 + 8);
    //Flash大小
    idPtr->flashSizeID = (uint16_t)(*(__IO uint32_t*)(0X1FFF7A22));
}








