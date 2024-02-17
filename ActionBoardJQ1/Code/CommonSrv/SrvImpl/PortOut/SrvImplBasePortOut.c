/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-10-25 18:38:52 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-10-25 19:29:37 +0800
************************************************************************************************/ 
#include "SrvImplPortOut.h"
#include "SrvImplBasePortOut.h"
#include "SrvTaskHeader.h"

//输出单个写入            
void SrvImplBasePortOutWriteSingle(uint8_t writeSingleIndex,uint8_t writeSingleVal,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,
                                        uint16_t* dataBufLen)
{
    //返回内存长度设定
    *dataBufLen = 0;
    *dataBufPtrPtr = NULL;
    if(writeSingleIndex >= BOARD_PORT_OUT_COUNT)
    {
        //报错
        SystemPrintf("%s,WrireOutIndexMax : %d\r\n",__func__,BOARD_PORT_OUT_COUNT);
        //不支持的读取序号
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_ACT_PORT_WRITE,ERROR_LEVEL_WARNING,
                                        ERROR_SUB_BOARD_PORT_OUT_INDEX_MAX);
        return;
    }
    //写入
    BoardPortOutWriteSingleState((BOARD_PORT_OUT)writeSingleIndex,(GPIO_PinState)writeSingleVal);
    //构建完成
    CoreUtilSetSystemResultPackNoError(resultPackPtr);
}

//输出批量写入 
void SrvImplBasePortOutWriteAny(uint8_t writeSinglePair,uint8_t* writePairDataArrayPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,
                                        uint16_t* dataBufLen)
{
    //返回内存长度设定
    *dataBufLen = 0;
    *dataBufPtrPtr = NULL;
    uint8_t indexUtil = 0;
    //调用相应的API写入
    for(indexUtil = 0; indexUtil < writeSinglePair; indexUtil++)
    {
        if(writePairDataArrayPtr[indexUtil*2] >= BOARD_PORT_OUT_COUNT)
        {
            //报错
            SystemPrintf("%s,WrireOutIndexMax : %d\r\n",__func__,BOARD_PORT_OUT_COUNT);
            //不支持的读取序号
            CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_ACT_PORT_WRITE,ERROR_LEVEL_WARNING,
                                            ERROR_SUB_BOARD_PORT_OUT_INDEX_MAX);
            return;
        }
        else
        {
            BoardPortOutWriteSingleState((BOARD_PORT_OUT)(writePairDataArrayPtr[indexUtil*2]),
                                            (GPIO_PinState)(writePairDataArrayPtr[(indexUtil*2)+1]));
        }
    }
    //构建完成
    CoreUtilSetSystemResultPackNoError(resultPackPtr);
    return;
}












