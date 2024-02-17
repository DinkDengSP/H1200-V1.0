/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-09-27 09:26:29 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-09-27 09:26:29 +0800
************************************************************************************************/ 
#ifndef __BOARD_BARSCAN_CR100M_H_
#define __BOARD_BARSCAN_CR100M_H_
#include "DriverHeaderMCU.h"
#include "DriverConfigBoard.h"

typedef struct BARSCAN_CR100M_RESULT
{
    uint8_t buffer[COMM_CACHE_LENGTH_BOARD_BARSCAN_CR100M];//扫码数据
    uint16_t barcodeLength;//扫码数据长度
    uint32_t barcodeTimeTick;//获得条码数据的时间
}BARSCAN_CR100M_RESULT;

/*----------------------------------------------------------------------------------------*/
//端口初始化
void BoardBarScanCR100MA_PortInit();

//条码枪默认配置
ERROR_SUB BoardBarScanCR100MA_ConfigDefault(void);

//检查条码枪版本号
ERROR_SUB BoardBarScanCR100MA_CheckVersion(void);

//打开条码枪,单次扫描
ERROR_SUB BoardBarScanCR100MA_OpenSingle(void);

//打开条码枪,测试模式
ERROR_SUB BoardBarScanCR100MA_OpenTestMode(void);

//打开条码枪,多次扫码
ERROR_SUB BoardBarScanCR100MA_OpenMulitple(void);

//关闭条码枪
ERROR_SUB BoardBarScanCR100MA_Close(void);

//清除条码枪缓冲区
ERROR_SUB BoardBarScanCR100MA_ClearHistory(void);

//获取条码个数
uint16_t BoardBarScanCR100MA_GetBarCodeResultCount(void);

//获取上次条码结果,如果没有结果,返回空指针
BARSCAN_CR100M_RESULT* BoardBarScanCR100MA_GetLastResult(uint16_t* resultCount);

//释放扫码结果
void BoardBarScanCR100MA_ReleaseMem(BARSCAN_CR100M_RESULT* barscanResultPtr);

#endif





