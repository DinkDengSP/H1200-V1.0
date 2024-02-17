/************************************************************************ 
 ** Author       : 邓小俊
 ** Date         : 2024-01-31 16:34:18
 ** Description  : When I Has Free Time ,I Will Write Description Below.
 ** description  : 
 ** LastEditors  : 邓小俊
 ** LastEditTime : 2024-02-01 11:27:17
 ** Email: xiaojun.deng@aliyun.com
 ** Copyright (c) 2024 by 公司名称, All Rights Reserved. 
 ************************************************************************/
#include "SrvImplBasePortOut.h"
#include "CommonSrvTaskHeader.h"


//输出单个写入            
__weak void SrvImplBasePortOutWriteSingle(uint8_t writeSingleIndex,uint8_t writeSingleVal,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,
                                        uint16_t* dataBufLen)
{
    //返回内存长度设定
    *dataBufLen = 0;
    *dataBufPtrPtr = NULL;
    if(writeSingleIndex > BOARD_POWER_MODULE_5)
    {
        //报错
        SystemPrintf("%s,WrireOutIndexMax : %d\r\n",__func__,BOARD_POWER_MODULE_5);
        //不支持的读取序号
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_UTIL_PORT_WRITE,ERROR_LEVEL_WARNING,
                                        ERROR_SUB_BOARD_PORT_OUT_INDEX_MAX);
        return;
    }
    //写入
    BoardPowerControlWriteSingle((BOARD_POWER_NO)writeSingleIndex,(PIN_STATE)writeSingleVal);
    //构建完成
    CoreUtilSetSystemResultPackNoError(resultPackPtr);
}

//输出批量写入 
__weak void SrvImplBasePortOutWriteAny(uint8_t writeSinglePair,uint8_t* writePairDataArrayPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,
                                        uint16_t* dataBufLen)
{
    //返回内存长度设定
    *dataBufLen = 0;
    *dataBufPtrPtr = NULL;
    uint8_t indexPortOut = 0;
    //调用相应的API写入
    for(indexPortOut = 0; indexPortOut < writeSinglePair; indexPortOut++)
    {
        if(writePairDataArrayPtr[indexPortOut*2] > BOARD_POWER_MODULE_5)
        {
            //报错
            SystemPrintf("%s,WrireOutIndexMax : %d\r\n",__func__,BOARD_POWER_MODULE_5);
            //不支持的读取序号
            CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_UTIL_PORT_WRITE,ERROR_LEVEL_WARNING,
                                            ERROR_SUB_BOARD_PORT_OUT_INDEX_MAX);
            return;
        }
        else
        {
            BoardPowerControlWriteSingle((BOARD_POWER_NO)(writePairDataArrayPtr[indexPortOut*2]),
                                            (PIN_STATE)(writePairDataArrayPtr[(indexPortOut*2)+1]));
        }
    }
    //构建完成
    CoreUtilSetSystemResultPackNoError(resultPackPtr);
    return;
} 












