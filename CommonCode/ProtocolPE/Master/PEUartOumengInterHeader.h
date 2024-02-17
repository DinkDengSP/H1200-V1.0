/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-09-04 11:57:26 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-09-05 20:08:39 +0800
************************************************************************************************/ 
#ifndef __PEUART_OUMENG_INTER_HEADER_H_
#define __PEUART_OUMENG_INTER_HEADER_H_
#include "PEUartOumengInterDataType.h"
#include "PEUartOumengInterBase.h"

/*************************************************************************************************************/
//处理NET指令的实现代码
typedef void (*UserPEUartCmdProcessFunc)(PE_UART_PACKAGE* peUartCmdMsgPtr);

//网口指令处理单元
typedef struct PE_UART_CMD_PROC_UNIT
{
    MACRO_PE_UART_CMD commandCode;//指令码
    uint16_t exceptParamLengthMin;//预期参数最小长度
    UserPEUartCmdProcessFunc peUartCmdProcFuncPtr;
}PE_UART_CMD_PROC_UNIT;


#endif









