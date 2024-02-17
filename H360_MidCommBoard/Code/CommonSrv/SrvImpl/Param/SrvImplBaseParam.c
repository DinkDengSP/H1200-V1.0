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
#include "SrvImplBaseParam.h"
#include "CommonSrvTaskHeader.h"


//参数单个读取        
__weak void SrvImplBaseParamReadSingle(uint8_t mainIndex,uint8_t subIndex,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    int32_t paramReadInt32;
    //返回内存长度设定
    *dataBufLen = 4;
    *dataBufPtrPtr = UserMemMallocWhileSuccess(MEM_REGION_SRV_TASK,*dataBufLen);
    uint8_t* resultBufferPtr = *dataBufPtrPtr;
    //调用相应的API读取
    errorSub = SystemParamReadSingle((INDEX_MAIN_SYSTEM_PARAM)mainIndex,subIndex,&paramReadInt32);
    if(errorSub != ERROR_SUB_OK)
    {
        //报错
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_UTIL_PARAM_READ,ERROR_LEVEL_WARNING,
                                            errorSub);
        return;
    }
    else
    {
        //写入
        ProtUtilWriteInt32ToBuffer(paramReadInt32,resultBufferPtr);
    }
    //构建完成
    CoreUtilSetSystemResultPackNoError(resultPackPtr);
    return;
}

//参数批量读取
__weak void SrvImplBaseParamReadAny(uint8_t readParamPair,uint8_t* readParaPairDataArrayPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,
                                    uint16_t* dataBufLen)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    int32_t paramReadInt32;
    //返回内存长度设定
    *dataBufLen = 4*readParamPair;
    *dataBufPtrPtr = UserMemMallocWhileSuccess(MEM_REGION_SRV_TASK,*dataBufLen);
    uint8_t* resultBufferPtr = *dataBufPtrPtr;
    uint16_t indexParam = 0;
    for(indexParam = 0; indexParam < readParamPair; indexParam++)
    {
        errorSub = SystemParamReadSingle((INDEX_MAIN_SYSTEM_PARAM)(readParaPairDataArrayPtr[indexParam*2]),
                                        readParaPairDataArrayPtr[(indexParam*2)+1],&paramReadInt32);
        
        if(errorSub != ERROR_SUB_OK)
        {
            //报错
            CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_UTIL_PARAM_READ,ERROR_LEVEL_WARNING,
                                            errorSub);
            return;
        }
        else
        {
            //写入
            ProtUtilWriteInt32ToBuffer(paramReadInt32,resultBufferPtr+(indexParam*4));
        }
    }
    //构建完成
    CoreUtilSetSystemResultPackNoError(resultPackPtr);
    return;
}

//参数单个写入   
__weak void SrvImplBaseParamWriteSingle(uint8_t mainIndex,uint8_t subIndex,int32_t writeParam,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,
                                        uint16_t* dataBufLen)
{
    //返回内存长度设定
    *dataBufLen = 0;
    *dataBufPtrPtr = NULL;
    ERROR_SUB errorSub = ERROR_SUB_OK;
    //数据写入
    errorSub = SystemParamWriteSingle((INDEX_MAIN_SYSTEM_PARAM)mainIndex,subIndex,writeParam);
    if(errorSub != ERROR_SUB_OK)
    {
        //报错
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_UTIL_PARAM_WRITE,ERROR_LEVEL_WARNING,
                                            errorSub);
        return;
    }
    //构建完成
    CoreUtilSetSystemResultPackNoError(resultPackPtr);
    return;
}
                                        
//参数批量写入
__weak void SrvImplBaseParamWriteAny(uint8_t writeParamPair,uint8_t* writeParamPairArrayPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,
                                    uint16_t* dataBufLen)
{
    //返回内存长度设定
    *dataBufLen = 0;
    *dataBufPtrPtr = NULL;
    uint8_t indexParam = 0;
    ERROR_SUB errorSub = ERROR_SUB_OK;
    int32_t writeParamInt;
    for(indexParam = 0; indexParam < writeParamPair; indexParam++)
    {
        //读取参数
        writeParamInt = ProtUtilReadInt32Buffer(writeParamPairArrayPtr+2+(6*indexParam));
        //执行写入
        errorSub = SystemParamWriteSingle((INDEX_MAIN_SYSTEM_PARAM)(writeParamPairArrayPtr[indexParam*6]),
                                        writeParamPairArrayPtr[(indexParam*6)+1],writeParamInt);
        
        if(errorSub != ERROR_SUB_OK)
        {
            //报错
            CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_UTIL_PARAM_WRITE,ERROR_LEVEL_WARNING,
                                            errorSub);
            return;
        }
    }
    //构建完成
    CoreUtilSetSystemResultPackNoError(resultPackPtr);
    return;
} 












