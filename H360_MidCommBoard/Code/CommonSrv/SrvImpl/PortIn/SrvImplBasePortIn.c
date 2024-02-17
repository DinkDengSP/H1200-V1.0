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
#include "SrvImplBasePortIn.h"
#include "CommonSrvTaskHeader.h"


//输入单个读取
__weak void SrvImplBasePortInReadSingle(uint8_t readSingleIndex,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen)
{
    //返回内存长度设定
    *dataBufPtrPtr = UserMemMallocWhileSuccess(MEM_REGION_SRV_TASK,1);
    *dataBufLen = 1;
    uint8_t* resultBufferPtr = *dataBufPtrPtr;
    //读取输入
    if(readSingleIndex > BOARD_DIP_KEY_S8)
    {
        //报错
        SystemPrintf("%s,ReadIndexMax : %d\r\n",__func__,BOARD_DIP_KEY_S8);
        //不支持的读取序号
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_UTIL_PORT_READ,ERROR_LEVEL_WARNING,
                                        ERROR_SUB_BOARD_PORT_IN_INDEX_MAX);
        return;
    }
    resultBufferPtr[0] = (uint8_t)BoardDipKeyReadSingle((BOARD_DIP_KEY)readSingleIndex);
    //构建完成
    CoreUtilSetSystemResultPackNoError(resultPackPtr);
}

//输入区间读取        
__weak void SrvImplBasePortInReadRange(uint8_t readSingleStartIdx,uint8_t readSingleCount,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,
                                        uint16_t* dataBufLen)
{
    //返回内存长度设定
    *dataBufLen = readSingleCount;
    *dataBufPtrPtr = UserMemMallocWhileSuccess(MEM_REGION_SRV_TASK,*dataBufLen);
    uint8_t* resultBufferPtr = *dataBufPtrPtr;
    //读取输入
    if((readSingleStartIdx+readSingleCount) > BOARD_DIP_KEY_S8)
    {
        //报错
        SystemPrintf("%s,ReadIndexMax : %d\r\n",__func__,BOARD_DIP_KEY_S8);
        //不支持的读取序号
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_UTIL_PORT_READ,ERROR_LEVEL_WARNING,
                                        ERROR_SUB_BOARD_PORT_IN_INDEX_MAX);
        return;
    }
    uint8_t indexPortIn = 0;
    for(indexPortIn = 0; indexPortIn < readSingleCount;indexPortIn++)
    {
        resultBufferPtr[indexPortIn] = (uint8_t)BoardDipKeyReadSingle((BOARD_DIP_KEY)(indexPortIn+readSingleStartIdx));
    }
    //构建完成
    CoreUtilSetSystemResultPackNoError(resultPackPtr);
}  

//输入批量读取
__weak void SrvImplBasePortInReadAny(uint8_t readSingleCount,uint8_t* readSingalIdxArrayPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,
                                    uint16_t* dataBufLen)
{
    //返回内存长度设定
    *dataBufLen = readSingleCount;
    *dataBufPtrPtr = UserMemMallocWhileSuccess(MEM_REGION_SRV_TASK,*dataBufLen);
    uint8_t* resultBufferPtr = *dataBufPtrPtr;
    uint8_t indexPortIn = 0;
    for(indexPortIn = 0; indexPortIn < readSingleCount;indexPortIn++)
    {
        if(readSingalIdxArrayPtr[indexPortIn] > BOARD_DIP_KEY_S8)
        {
             //报错
            SystemPrintf("%s,ReadIndexMax : %d\r\n",__func__,BOARD_DIP_KEY_S8);
            //不支持的读取序号
            CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_UTIL_PORT_READ,ERROR_LEVEL_WARNING,
                                            ERROR_SUB_BOARD_PORT_IN_INDEX_MAX);
            return;
        }
        resultBufferPtr[indexPortIn] = (uint8_t)BoardDipKeyReadSingle((BOARD_DIP_KEY)(readSingalIdxArrayPtr[indexPortIn]));
    }
    //构建完成
    CoreUtilSetSystemResultPackNoError(resultPackPtr);
}












