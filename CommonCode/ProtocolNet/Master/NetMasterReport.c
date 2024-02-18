/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-10-19 10:08:51 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-10-19 10:10:53 +0800
************************************************************************************************/ 
#include "NetMasterReport.h"

//网口报错
void NetMasterReportErrorToPC(ERROR_MAIN errorMain,ERROR_LEVEL errorLevel,ERROR_SUB errorSub)
{
    uint8_t errorReportBuf[7] = {0};
    uint32_t utilTemp = 0;
    SYS_RESULT_PACK sysResultPack;
    //ErrorMain
    utilTemp = (uint32_t)errorMain;
    errorReportBuf[0] = (uint8_t)utilTemp;
    utilTemp >>= 8;
    errorReportBuf[1] = (uint8_t)utilTemp;
    //ErrorLevel
    errorReportBuf[2] = (uint8_t)errorLevel;
    //ErrorSub
    utilTemp = (uint32_t)errorSub;
    uint32_t boardIdCode = NetProtGetSelfBoardID();
    boardIdCode <<= 24;
    //对错误子码进行处理附加板号
    if((utilTemp != 0X00000000)&&((utilTemp&0XFF000000) == 0X00000000))
    {
        //最大区域等于0
        utilTemp |= boardIdCode;
    }
    errorReportBuf[3] = (uint8_t)utilTemp;
    utilTemp >>= 8;
    errorReportBuf[4] = (uint8_t)utilTemp;
    utilTemp >>= 8;
    errorReportBuf[5] = (uint8_t)utilTemp;
    utilTemp >>= 8;
    errorReportBuf[6] = (uint8_t)utilTemp;
    //上报
    NetProtSendUpload(SYSTEM_MODULE_NO_PC,SYSTEM_BOARD_ID_PC,(uint32_t)NET_MASTER_REPORT_ERROR,
                        errorReportBuf,7,&sysResultPack);
    //错误打印
    if(sysResultPack.resultCode != PROT_RESULT_SUCCESS)
    {
        //错误日志打印
        NET_PROT_LOG("%s,%d,Result: %d,ErrorMain: 0X%04X,ErrorLevel: %d,ErrorSub: 0X%08X\r\n",
                        __func__,__LINE__,sysResultPack.resultCode,sysResultPack.errorMain,
                        sysResultPack.errorLevel,sysResultPack.errorSub);
    }
}

//网口上报消息
void NetMasterReportMsgToPC(uint32_t reportCmd,uint8_t *reportBuf,uint16_t reportLength)
{
    SYS_RESULT_PACK sysResultPack;
    //上报
    NetProtSendUpload(SYSTEM_MODULE_NO_PC,SYSTEM_BOARD_ID_PC,(uint32_t)reportCmd,
                        reportBuf,reportLength,&sysResultPack);
    //错误打印
    if(sysResultPack.resultCode != PROT_RESULT_SUCCESS)
    {
        //错误日志打印
        NET_PROT_LOG("%s,%d,Result: %d,ErrorMain: 0X%04X,ErrorLevel: %d,ErrorSub: 0X%08X\r\n",
                        __func__,__LINE__,sysResultPack.resultCode,sysResultPack.errorMain,
                        sysResultPack.errorLevel,sysResultPack.errorSub);
    }
}







