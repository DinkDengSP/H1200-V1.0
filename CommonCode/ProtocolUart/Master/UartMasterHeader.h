/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-10-19 10:08:44 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-10-19 12:04:01 +0800
************************************************************************************************/ 
#ifndef __UART_MASTER_HEADER_H_
#define __UART_MASTER_HEADER_H_
#include "UartMasterDataType.h"
#include "UartMasterReport.h"
#include "UartMasterUtil.h"

/*************************************************************************************************************/
//处理UART指令的实现代码
typedef void (*UserUartCmdProcFunc)(UART_PROT_CMD_PACK_RECV* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);

//串口指令处理单元
typedef struct USER_UART_CMD_PROC_UNIT
{
    uint32_t commandCode;//指令码
    uint32_t exceptParamLengthMin;//预期参数最小长度
    FunctionalState needResultReply;//是否需要结果回复
    UserUartCmdProcFunc uartCmdProcFuncPtr;//处理函数
}USER_UART_CMD_PROC_UNIT;

/*************************************************************************************************************/
//处理UART上报消息的实现代码
typedef void (*UserUartUploadProcFunc)(UART_PROT_UPLOAD_PACK_RECV* uploadPackPtr);
//UART上报消息处理单元
typedef struct USER_UART_UPLOAD_PROC_UNIT
{
    uint32_t uploadId;//上报ID
    uint32_t exceptParamLengthMin;//预期参数最小长度
    UserUartUploadProcFunc uartUploadProcFuncPtr;//上报ID处理函数
}USER_UART_UPLOAD_PROC_UNIT;


#endif


