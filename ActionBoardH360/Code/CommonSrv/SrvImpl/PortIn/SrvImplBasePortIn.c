/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-10-31 08:43:34 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-10-31 09:55:52 +0800
************************************************************************************************/ 
#include "SrvImplPortIn.h"
#include "SrvImplBasePortIn.h"
#include "CommonSrvTaskHeader.h"


//输入单个读取
void SrvImplBasePortInReadSingle(uint8_t readSingleIndex,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen)
{
    //返回内存长度设定
    *dataBufPtrPtr = UserMemMallocWhileSuccess(MEM_REGION_SRV_TASK,1);
    *dataBufLen = 1;
    uint8_t* resultBufferPtr = *dataBufPtrPtr;
    //读取输入
    if(readSingleIndex >= BOARD_PORT_IN_COUNT)
    {
        //报错
        SystemPrintf("%s,ReadIndexMax : %d\r\n",__func__,BOARD_PORT_IN_COUNT);
        //不支持的读取序号
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_ACT_PORT_READ,ERROR_LEVEL_WARNING,
                                        ERROR_SUB_BOARD_PORT_IN_INDEX_MAX);
        return;
    }
    resultBufferPtr[0] = (uint8_t)BoardPortInReadState((BOARD_PORT_IN)readSingleIndex);
    //构建完成
    CoreUtilSetSystemResultPackNoError(resultPackPtr);
}

//输入区间读取        
void SrvImplBasePortInReadRange(uint8_t readSingleStartIdx,uint8_t readSingleCount,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,
                                        uint16_t* dataBufLen)
{
    //返回内存长度设定
    *dataBufLen = readSingleCount;
    *dataBufPtrPtr = UserMemMallocWhileSuccess(MEM_REGION_SRV_TASK,*dataBufLen);
    uint8_t* resultBufferPtr = *dataBufPtrPtr;
    //读取输入
    if((readSingleStartIdx+readSingleCount) >= BOARD_PORT_IN_COUNT)
    {
        //报错
        SystemPrintf("%s,ReadIndexMax : %d\r\n",__func__,BOARD_PORT_IN_COUNT);
        //不支持的读取序号
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_ACT_PORT_READ,ERROR_LEVEL_WARNING,
                                        ERROR_SUB_BOARD_PORT_IN_INDEX_MAX);
        return;
    }
    uint8_t indexUtil = 0;
    for(indexUtil = 0; indexUtil < readSingleCount;indexUtil++)
    {
        resultBufferPtr[indexUtil] = (uint8_t)BoardPortInReadState((BOARD_PORT_IN)(indexUtil+readSingleStartIdx));
    }
    //构建完成
    CoreUtilSetSystemResultPackNoError(resultPackPtr);
}

//输入批量读取
void SrvImplBasePortInReadAny(uint8_t readSingleCount,uint8_t* readSingalIdxArrayPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,
                                    uint16_t* dataBufLen)
{
    //返回内存长度设定
    *dataBufLen = readSingleCount;
    *dataBufPtrPtr = UserMemMallocWhileSuccess(MEM_REGION_SRV_TASK,*dataBufLen);
    uint8_t* resultBufferPtr = *dataBufPtrPtr;
    uint8_t indexUtil = 0;
    for(indexUtil = 0; indexUtil < readSingleCount;indexUtil++)
    {
        if(readSingalIdxArrayPtr[indexUtil] >= BOARD_PORT_IN_COUNT)
        {
             //报错
            SystemPrintf("%s,ReadIndexMax : %d\r\n",__func__,BOARD_PORT_IN_COUNT);
            //不支持的读取序号
            CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_ACT_PORT_READ,ERROR_LEVEL_WARNING,
                                            ERROR_SUB_BOARD_PORT_IN_INDEX_MAX);
            return;
        }
        resultBufferPtr[indexUtil] = (uint8_t)BoardPortInReadState((BOARD_PORT_IN)(readSingalIdxArrayPtr[indexUtil]));
    }
    //构建完成
    CoreUtilSetSystemResultPackNoError(resultPackPtr);
}





















