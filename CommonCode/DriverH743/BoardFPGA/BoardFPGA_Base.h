/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-04-26 10:16:02 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-04-26 12:13:31 +0800
************************************************************************************************/ 
#ifndef __BOARD_FPGA_BASE_H_
#define __BOARD_FPGA_BASE_H_
#include "BoardFPGA_Type.h"

//FPGA读取寄存器
uint16_t BoardFPGA_ReadReg16(uint16_t regAddr);

//FPGA读取寄存器
uint32_t BoardFPGA_ReadReg32(uint16_t regAddr);

//写入FPGA指定地址指定数据
ERROR_SUB BoardFPGA_WriteDataByAddr(uint32_t addr,uint32_t data);

//写入FPGA指定地址指定数据并读取相应的地址查看是否成功写入
ERROR_SUB BoardFPGA_WriteDataByAddrWithCheck(uint32_t writeAddr,uint32_t data,uint16_t readCheckAddr);

//FPGA复位
void BoardFPGA_PortReset(void);

#endif



