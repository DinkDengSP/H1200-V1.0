/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-11-09 15:03:07 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-11-09 16:51:16 +0800
************************************************************************************************/ 
#include "IPC_CommonSrvBase.h"
#include "IPC_Port.h"

//释放内存
void IPC_CommonFreeReturnData(IPC_RETURN_DATA* returnDataPtr)
{
    if(returnDataPtr == NULL)
    {
        return;
    }
    if((returnDataPtr->returnBufferPtr != NULL)&&(returnDataPtr->returnDataLength != 0))
    {
        IPC_PortMemFree(returnDataPtr->returnBufferPtr);
    }
    return;
}


















