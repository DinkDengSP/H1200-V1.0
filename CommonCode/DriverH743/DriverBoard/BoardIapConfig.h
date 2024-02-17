/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-08-10 14:35:39 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-08-10 14:35:39 +0800
************************************************************************************************/ 
#ifndef __BOARD_IAP_CONFIG_H_
#define __BOARD_IAP_CONFIG_H_
#include "DriverHeaderMCU.h"
#include "BoardParam.h"

//地址偏移
#define OFFSET_BOARD_IAP_RUNNING_MODE           0X00000000
//BOOT IAP记录信息
#define OFFSET_BOARD_IAP_RECORD_BOOT            0X00000010
//SERVICE IAP记录信息
#define OFFSET_BOARD_IAP_RECORD_SERVICE         0X00000020
//APP IAP记录信息
#define OFFSET_BOARD_IAP_RECORD_APP             0X00000030


//IAP处于BOOT标记
#define FLAG_BOARD_IAP_CURRENT_RUN_MODE_BOOT    0XAAAA5555
//IAP处于APP标记
#define FLAG_BOARD_IAP_CURRENT_RUN_MODE_APP     0X5555AAAA



#endif




