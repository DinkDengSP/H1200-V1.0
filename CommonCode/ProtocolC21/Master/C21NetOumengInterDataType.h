/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-09-04 11:57:26 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-09-04 13:20:15 +0800
************************************************************************************************/ 
#ifndef __C21_NET_OUMENG_INTER_DATA_TYPE_H_
#define __C21_NET_OUMENG_INTER_DATA_TYPE_H_
#include "C21NetOumengInterMacro.h"

//试管信息
typedef struct C21_NET_RANK_MSG
{
    uint8_t* tube1BarCodeBufPtr;
    uint8_t tube1BarCodeLength;
    uint8_t* tube2BarCodeBufPtr;
    uint8_t tube2BarCodeLength;
    uint8_t* tube3BarCodeBufPtr;
    uint8_t tube3BarCodeLength;
    uint8_t* tube4BarCodeBufPtr;
    uint8_t tube4BarCodeLength;
    uint8_t* tube5BarCodeBufPtr;
    uint8_t tube5BarCodeLength;
    uint8_t* tube6BarCodeBufPtr;
    uint8_t tube6BarCodeLength;
    uint8_t* tube7BarCodeBufPtr;
    uint8_t tube7BarCodeLength;
    uint8_t* tube8BarCodeBufPtr;
    uint8_t tube8BarCodeLength;
    uint8_t* tube9BarCodeBufPtr;
    uint8_t tube9BarCodeLength;
    uint8_t* tube10BarCodeBufPtr;
    uint8_t tube10BarCodeLength;
}C21_NET_RANK_MSG;

//试管状态
typedef struct C21_NET_TUBE_STATE_SET
{
    C21_NET_TUBE_TEST_STATE tube1State;
    C21_NET_TUBE_TEST_STATE tube2State;
    C21_NET_TUBE_TEST_STATE tube3State;
    C21_NET_TUBE_TEST_STATE tube4State;
    C21_NET_TUBE_TEST_STATE tube5State;
    C21_NET_TUBE_TEST_STATE tube6State;
    C21_NET_TUBE_TEST_STATE tube7State;
    C21_NET_TUBE_TEST_STATE tube8State;
    C21_NET_TUBE_TEST_STATE tube9State;
    C21_NET_TUBE_TEST_STATE tube10State;
}C21_NET_TUBE_STATE_SET;

#endif









