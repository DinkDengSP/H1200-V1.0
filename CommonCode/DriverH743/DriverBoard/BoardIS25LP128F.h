/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-09-15 09:57:20 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-09-15 18:24:31 +0800
************************************************************************************************/ 
#ifndef __BOARD_IS25LP128F_H_
#define __BOARD_IS25LP128F_H_
#include "BoardIS25LP128F_Config.h"

//导出QSPI Flash辅助读写数组
extern uint8_t is25lp128UtilBuffer[BOARD_IS25LP128F_SECTOR_SIZE];

//端口初始化
void BoardIS25LP128F_PortInit(void);

//设备复位
ERROR_SUB BoardIS25LP128F_DeviceReset(void);

//设置设备默认配置
ERROR_SUB BoardIS25LP128F_SetDefaultConfig(void);

//等待Flash就绪
ERROR_SUB BoardIS25LP128F_WaitDeviceIdle(uint32_t timeOutMs);

//芯片进入低功耗模式
ERROR_SUB BoardIS25LP128F_PowerDown(void);

//芯片退出低功耗模式
ERROR_SUB BoardIS25LP128F_ReleasePowerDown(void);

//读取板上设备ID
ERROR_SUB BoardIS25LP128F_ReadID(uint32_t* deviceIdPtr);

//擦除指定扇区,扇区地址，以4KB为单位的地址
ERROR_SUB BoardIS25LP128F_EraseSector(uint32_t sectorNo);

//擦除块
ERROR_SUB BoardIS25LP128F_EraseBlock(uint32_t blockNo);

//擦除全芯片,等待时间500S
ERROR_SUB BoardIS25LP128F_EraseChip(void);

//读取指定地址数据IT方式读取
ERROR_SUB BoardIS25LP128F_ReadBuffer(uint8_t* pBuffer, uint32_t addr, uint16_t dataLength);

//无检验写SPI FLASH 
ERROR_SUB BoardIS25LP128F_WriteNoCheck(uint8_t* pBuffer,uint32_t WriteAddr,uint16_t NumByteToWrite);  

//写入指定地址指定长度数据
ERROR_SUB BoardIS25LP128F_WriteBufferSafe(uint8_t* pBuffer, uint32_t addr, uint16_t dataLength);

//程序自检
ERROR_SUB BoardIS25LP128F_Check(void);

//测试用例
#if(BOARD_IS25LP128F_TEST_CASE_ENABLE == CONFIG_FUNC_ENABLE)
//测试用例初始化
void BoardIS25LP128F_TestCaseInit(void);

//测试用例周期运转
void BoardIS25LP128F_TestCaseRunPeriod(void);

#endif




#endif











