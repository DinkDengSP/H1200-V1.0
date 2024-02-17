/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-08-23 09:44:56 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-08-23 10:52:24 +0800
************************************************************************************************/ 
#ifndef __BOARD_IAP_CONFIG_H_
#define __BOARD_IAP_CONFIG_H_
#include "DriverHeaderMCU.h"
#include "BoardParam.h"

//CRC计算初始值
#define BOARD_IAP_CRC_CALC_INIT_VAL             0XFFFFFFFF
//地址偏移
#define OFFSET_BOARD_IAP_RUNNING_MODE           0X00000000
//BOOT IAP记录信息
#define OFFSET_BOARD_IAP_RECORD_BOOT            0X00000010
//SRV IAP记录信息
#define OFFSET_BOARD_IAP_RECORD_SRV             0X00000030
//APP IAP记录信息
#define OFFSET_BOARD_IAP_RECORD_APP             0X00000050


//IAP处于BOOT标记
#define FLAG_BOARD_IAP_CURRENT_RUN_MODE_BOOT    0XAAAA5555
//IAP处于APP标记
#define FLAG_BOARD_IAP_CURRENT_RUN_MODE_APP     0X5555AAAA

//BOOT文件长度
#define LENGTH_BOARD_IAP_BOOT_BIN_FILE          (256*1024)

//APP文件长度
#define LENGTH_BOARD_IAP_APP_BIN_FILE           (768*1024)
//SRV文件长度
#define LENGTH_BOARD_IAP_SRV_BIN_FILE           (768*1024)

#endif










