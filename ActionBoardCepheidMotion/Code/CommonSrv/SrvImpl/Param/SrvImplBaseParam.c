/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-10-31 08:43:34 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-10-31 09:55:52 +0800
************************************************************************************************/ 
#include "SrvImplParam.h"
#include "SrvImplBaseParam.h"
#include "CommonSrvTaskHeader.h"

//参数单个读取        
void SrvImplBaseParamReadSingle(uint8_t mainIndex,uint8_t subIndex,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    int32_t paramReadInt32;
    //返回内存长度设定
    *dataBufLen = 4;
    *dataBufPtrPtr = UserMemMallocWhileSuccess(MEM_REGION_SRV_TASK,*dataBufLen);
    uint8_t* resultBufferPtr = *dataBufPtrPtr;
    //调用相应的API读取
    errorSub = SysParamReadSingle((INDEX_MAIN_SYS_PARAM)mainIndex,subIndex,&paramReadInt32);
    if(errorSub != ERROR_SUB_OK)
    {
        //报错
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_ACT_PARAM_READ,ERROR_LEVEL_WARNING,
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
void SrvImplBaseParamReadAny(uint8_t readParamPair,uint8_t* readParaPairDataArrayPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,
                                    uint16_t* dataBufLen)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    int32_t paramReadInt32;
    //返回内存长度设定
    *dataBufLen = 4*readParamPair;
    *dataBufPtrPtr = UserMemMallocWhileSuccess(MEM_REGION_SRV_TASK,*dataBufLen);
    uint8_t* resultBufferPtr = *dataBufPtrPtr;
    uint16_t indexUtil = 0;
    for(indexUtil = 0; indexUtil < readParamPair; indexUtil++)
    {
        errorSub = SysParamReadSingle((INDEX_MAIN_SYS_PARAM)(readParaPairDataArrayPtr[indexUtil*2]),
                                        readParaPairDataArrayPtr[(indexUtil*2)+1],&paramReadInt32);
        
        if(errorSub != ERROR_SUB_OK)
        {
            //报错
            CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_ACT_PARAM_READ,ERROR_LEVEL_WARNING,
                                            errorSub);
            return;
        }
        else
        {
            //写入
            ProtUtilWriteInt32ToBuffer(paramReadInt32,resultBufferPtr+(indexUtil*4));
        }
    }
    //构建完成
    CoreUtilSetSystemResultPackNoError(resultPackPtr);
    return;
} 
 

//参数单个写入   
void SrvImplBaseParamWriteSingle(uint8_t mainIndex,uint8_t subIndex,int32_t writeParam,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,
                                        uint16_t* dataBufLen)
{
    //返回内存长度设定
    *dataBufLen = 0;
    *dataBufPtrPtr = NULL;
    ERROR_SUB errorSub = ERROR_SUB_OK;
    //数据写入
    errorSub = SysParamWriteSingle((INDEX_MAIN_SYS_PARAM)mainIndex,subIndex,writeParam);
    if(errorSub != ERROR_SUB_OK)
    {
        //报错
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_ACT_PARAM_WRITE,ERROR_LEVEL_WARNING,
                                            errorSub);
        return;
    }
    //构建完成
    CoreUtilSetSystemResultPackNoError(resultPackPtr);
    return;
} 

                                        
//参数批量写入
void SrvImplBaseParamWriteAny(uint8_t writeParamPair,uint8_t* writeParamPairArrayPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,
                                    uint16_t* dataBufLen)
{
    //返回内存长度设定
    *dataBufLen = 0;
    *dataBufPtrPtr = NULL;
    uint8_t indexUtil = 0;
    ERROR_SUB errorSub = ERROR_SUB_OK;
    int32_t writeParamInt;
    for(indexUtil = 0; indexUtil < writeParamPair; indexUtil++)
    {
        //读取参数
        writeParamInt = ProtUtilReadInt32Buffer(writeParamPairArrayPtr+2+(6*indexUtil));
        //执行写入
        errorSub = SysParamWriteSingle((INDEX_MAIN_SYS_PARAM)(writeParamPairArrayPtr[indexUtil*6]),
                                        writeParamPairArrayPtr[(indexUtil*6)+1],writeParamInt);
        
        if(errorSub != ERROR_SUB_OK)
        {
            //报错
            CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_ACT_PARAM_WRITE,ERROR_LEVEL_WARNING,
                                            errorSub);
            return;
        }
    }
    //构建完成
    CoreUtilSetSystemResultPackNoError(resultPackPtr);
    return;
} 
  




















