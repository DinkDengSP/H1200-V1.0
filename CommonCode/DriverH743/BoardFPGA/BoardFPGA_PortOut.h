/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-04-26 10:17:55 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-04-26 16:25:09 +0800
************************************************************************************************/ 
#ifndef __BOARD_FPGA_PORT_OUT_H_
#define __BOARD_FPGA_PORT_OUT_H_
#include "BoardFPGA_Base.h"

/*******************************************定义端口输入操作函数类型*****************************************************/
//读取输出有效电平
typedef FPGA_PORT_VALID_LEVEL (*BoardFPGA_PortOutReadValidLevelFunc)(void);
//设置输出有效电平
typedef ERROR_SUB (*BoardFPGA_PortOutWriteValidLevelFunc)(FPGA_PORT_VALID_LEVEL validLevelSet);
//设置输出状态
typedef ERROR_SUB (*BoardFPGA_PortOutWriteSetValFunc)(FPGA_PORT_OUT_SET portValSet);
//读取设置的输出状态
typedef FPGA_PORT_OUT_SET (*BoardFPGA_PortOutReadSetValFunc)(void);
//读取实际输出状态
typedef FPGA_PORT_OUT_REALLY (*BoardFPGA_PortOutReadReallyStateFunc)(void);


/**************************************端口输出相关函数************************************************/
//单次设置输出端口的全部引脚的有效电平
ERROR_SUB BoardFPGA_PortOutWriteValidLevelAll(FPGA_OUT_PORT port,FPGA_PORT_VALID_LEVEL portValidLevelSet);

//如果设置为低电平有效,那么当端口低电平时,读取到的状态为1
//写入输出端口单个引脚有效电平,如果是高电平有效,那么当端口高电品时读取到的时1
ERROR_SUB BoardFPGA_PortOutWriteValidLevelSingle(FPGA_OUT_PORT port,FPGA_OUT_PIN pin,GPIO_PinState pinValidLevelSet);

//读取输出端口的有效电平
FPGA_PORT_VALID_LEVEL BoardFPGA_PortOutReadValidLevelAll(FPGA_OUT_PORT port);

//读取指定端口的指定引脚的有效电平
GPIO_PinState BoardFPGA_PortOutReadValidLevelSingle(FPGA_OUT_PORT port,FPGA_OUT_PIN pin);

//单次设置输出端口的输出状态
ERROR_SUB BoardFPGA_PortOutWriteSetValAll(FPGA_OUT_PORT port,FPGA_PORT_OUT_SET portVal);

//写入指定单个端口的输出状态
ERROR_SUB BoardFPGA_PortOutWriteSetValSingle(FPGA_OUT_PORT port,FPGA_OUT_PIN pin,PIN_STATE pinState);

//读取输出端口设置的输出状态
FPGA_PORT_OUT_SET BoardFPGA_PortOutReadSetValAll(FPGA_OUT_PORT port);

//读取指定端口设置的输出状态
PIN_STATE BoardFPGA_PortOutReadSetValSingle(FPGA_OUT_PORT port,FPGA_OUT_PIN pin);

//读取指定端口实际输出电平
FPGA_PORT_OUT_REALLY BoardFPGA_PortOutReadReallyStateAll(FPGA_OUT_PORT port);

//读取指定端口单个真实状态
GPIO_PinState BoardFPGA_PortOutReadReallyStateSingle(FPGA_OUT_PORT port,FPGA_OUT_PIN pin);

//输出端口测试
ERROR_SUB  BoardFPGA_PortOutTestA(void);


#endif


