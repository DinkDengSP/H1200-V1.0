#ifndef __BOARD_FPGA_H_
#define __BOARD_FPGA_H_
#include "BoardFPGA_PortIn.h"
#include "BoardFPGA_PortOut.h"
#include "BoardFPGA_StepMotor.h"

//端口初始化
void BoardFPGA_PortInit(void);

//获取设备访问权
void BoardFPGA_GetLock(void);

//释放设备访问权
void BoardFPGA_ReleaseLock(void);

//中断配置
void BoardFPGA_StepMotorIntConfig(BoardFPGA_StepMotorIntCallBack callBack);

//FPGA自检
ERROR_SUB BoardFPGA_CheckExist(void);

//获取软件版本
FPGA_SOFT_VERSION BoardFPGA_GetVersion(void);




#endif




