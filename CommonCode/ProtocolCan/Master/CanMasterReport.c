/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-10-20 18:20:58 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-10-20 18:31:47 +0800
************************************************************************************************/ 
#include "CanMasterReport.h"

//错误上报到PC
void CanMasterReportErrorToPC(ERROR_MAIN errorMain,ERROR_LEVEL errorLevel,ERROR_SUB errorSub)
{
    uint8_t errorReportBuf[7] = {0};
    uint32_t utilTemp = 0;
    SYS_RESULT_PACK sysResultPack;
    uint32_t appendErrorSubBoardID = 0X00000000;
    PORT_CAN protCanReportPC = CanProtPortCalcPort(SYSTEM_MODULE_NO_PC,SYSTEM_BOARD_ID_PC);
    //检查首字节是否带板号,不带板号加入板号
    if((errorSub != 0X00000000)&&((errorSub&0XFF000000) == 0X00000000))
    {
        //加入板号
        if(PORT_CAN1 == protCanReportPC)
        {
            appendErrorSubBoardID += Can1ProtPortGetSelfBoardID();
        }
        else
        {
            appendErrorSubBoardID += Can2ProtPortGetSelfBoardID();
        }
        appendErrorSubBoardID <<= 24;
        errorSub += appendErrorSubBoardID;
    }
    //ERROR MAIN
    utilTemp = (uint32_t)errorMain;
    errorReportBuf[0] = (uint8_t)utilTemp;
    utilTemp >>= 8;
    errorReportBuf[1] = (uint8_t)utilTemp;
    //ERROR LEVEL
    errorReportBuf[2] = (uint8_t)errorLevel;
    //ERROR SUB
    utilTemp = (uint32_t)errorSub;
    errorReportBuf[3] = (uint8_t)utilTemp;
    utilTemp >>= 8;
    errorReportBuf[4] = (uint8_t)utilTemp;
    utilTemp >>= 8;
    errorReportBuf[5] = (uint8_t)utilTemp;
    utilTemp >>= 8;
    errorReportBuf[6] = (uint8_t)utilTemp;
    //上传
    if(PORT_CAN1 == protCanReportPC)
    {
        Can1ProtSendUpload(SYSTEM_MODULE_NO_PC,SYSTEM_BOARD_ID_PC,(uint32_t)CAN_MASTER_REPORT_ID_ERR,
                                            errorReportBuf,7,&sysResultPack);
    }
    else
    {
        Can2ProtSendUpload(SYSTEM_MODULE_NO_PC,SYSTEM_BOARD_ID_PC,(uint32_t)CAN_MASTER_REPORT_ID_ERR,
                                            errorReportBuf,7,&sysResultPack);
    }
    //错误打印
    if(sysResultPack.resultCode != PROT_RESULT_SUCCESS)
    {
        //错误日志打印
        CAN_PROT_LOG("%s,%d,Result: %d,ErrorMain: 0X%04X,ErrorLevel: %d,ErrorSub: 0X%08X\r\n",
                        __func__,__LINE__,sysResultPack.resultCode,sysResultPack.errorMain,
                        sysResultPack.errorLevel,sysResultPack.errorSub);
    }
}




























