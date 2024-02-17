/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-10-20 08:40:07 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-10-23 17:37:35 +0800
************************************************************************************************/ 
#include "CanProtPort.h"

//CRC校验算法
uint16_t CanProtPortCalcCRC16(uint8_t* bufferPtr,uint16_t dataLength)
{
    return MCU_Calc2ByteCRC(bufferPtr,dataLength);
}

typedef struct CAN_PROT_PORT_CONFIG
{
    uint8_t moduleNo;
    uint8_t boardID;
    PORT_CAN portCan;
}CAN_PROT_PORT_CONFIG;

//配置数组
static const CAN_PROT_PORT_CONFIG canProtPortConfigArray[] = {
    {0X05           , 0X01          , PORT_CAN1},//action jq1
    {0X05           , 0X02          , PORT_CAN1},//action jq2
};

//通过配置计算SOCKET组
PORT_CAN  CanProtPortCalcPort(uint8_t moduleNo,uint8_t boardID)
{
    uint8_t indexUtil = 0;
    for(indexUtil = 0; indexUtil < DIM_ARRAY_ELEMENT_COUNT(canProtPortConfigArray);indexUtil++)
    {
        if((moduleNo == canProtPortConfigArray[indexUtil].moduleNo)&&
            (boardID == canProtPortConfigArray[indexUtil].boardID))
        {
            return canProtPortConfigArray[indexUtil].portCan;
        }
    }
    return PORT_CAN1;
}




