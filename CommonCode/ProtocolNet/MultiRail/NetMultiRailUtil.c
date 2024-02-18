#include "NetMultiRailUtil.h"


//多联机网口发送结果报
void NetMultiRailSendToPCResult(uint32_t cmdVal,SYS_RESULT_PACK* resultPackPtr,uint8_t* resultParamBuffer,uint16_t resultBufferLen)
{
    NetProtSendResult(SYSTEM_MODULE_NO_PC,SYSTEM_BOARD_ID_PC,cmdVal,
                        resultPackPtr->resultCode,resultPackPtr->errorMain,resultPackPtr->errorLevel,
                        resultPackPtr->errorSub,resultParamBuffer,resultBufferLen,resultPackPtr);
    if(resultPackPtr->resultCode != PROT_RESULT_SUCCESS)
    {
        //错误日志打印
        NET_PROT_LOG("%s,%d,Result: %d,ErrorMain: 0X%04X,ErrorLevel: %d,ErrorSub: 0X%08X\r\n",
                        __func__,__LINE__,resultPackPtr->resultCode,resultPackPtr->errorMain,
                        resultPackPtr->errorLevel,resultPackPtr->errorSub);
    }
}