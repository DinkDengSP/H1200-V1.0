/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-04-26 10:17:19 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-04-26 10:17:19 +0800
************************************************************************************************/ 
#ifndef __BOARD_FPGA_PORT_IN_H_
#define __BOARD_FPGA_PORT_IN_H_
#include "BoardFPGA_Base.h"

/*******************************************定义端口输入操作函数类型*****************************************************/
//读取输入端口的引脚的配置有效电平
typedef FPGA_PORT_VALID_LEVEL (*BoardFPGA_PortInReadValidLevelFunc)(void);
//单次设置输入端口的全部引脚的配置有效电平
typedef ERROR_SUB (*BoardFPGA_PortInWriteValidLevelFunc)(FPGA_PORT_VALID_LEVEL portValidLevelSet);
//读取指定端口当前全部状态
typedef FPGA_PORT_IN_STATE (*BoardFPGA_PortInReadStateFunc)(void);

/**************************************端口输入相关函数************************************************/
//读取全部有效电平配置
FPGA_PORT_VALID_LEVEL BoardFPGA_PortInReadValidLevelAll(FPGA_IN_PORT port);

//读取单个有效电平配置
GPIO_PinState BoardFPGA_PortInReadValidLevelSingle(FPGA_IN_PORT port,FPGA_IN_PIN pin);

//写入全部有效电平配置
ERROR_SUB BoardFPGA_PortInWriteValidLevelAll(FPGA_IN_PORT port,FPGA_PORT_VALID_LEVEL validLevelSet);

//写入单个有效电平配置
ERROR_SUB BoardFPGA_PortInWriteValidLevelSingle(FPGA_IN_PORT port,FPGA_IN_PIN pin,GPIO_PinState validLevel);

//读取全部端口状态
FPGA_PORT_IN_STATE BoardFPGA_PortInReadStateAll(FPGA_IN_PORT port);

//读取单个端口状态
PIN_STATE BoardFPGA_PortInReadStateSingle(FPGA_IN_PORT port,FPGA_IN_PIN pin);

//测试端口输入
ERROR_SUB BoardFPGA_PortInTestA(void);

#endif

