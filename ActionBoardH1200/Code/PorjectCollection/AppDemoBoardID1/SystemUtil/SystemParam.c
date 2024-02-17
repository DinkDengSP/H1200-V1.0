/************************************************************************ 
 ** Author       : 邓小俊
 ** Date         : 2024-01-16 11:35:28
 ** Description  : When I Has Free Time ,I Will Write Description Below.
 ** description  : 
 ** LastEditors  : 邓小俊
 ** LastEditTime : 2024-01-17 11:54:00
 ** Email: xiaojun.deng@aliyun.com
 ** Copyright (c) 2024 by 公司名称, All Rights Reserved. 
 ************************************************************************/
#include "SystemParam.h"
#include "SystemParamDefault.h"
#include "DriverConfigSystem.h"
#include "CommonSrvImplBase.h"

SYS_PARAM* sysParamPtr = NULL;

//App写入单个参数的指针
typedef ERROR_SUB (*SysParamWriteParamSingleFuncPtr)(uint8_t subIndex,int32_t writeParam);
//App读取单个参数的指针
typedef ERROR_SUB (*SysParamReadParamSingleFuncPtr)(uint8_t subIndex,int32_t* readParam);

//初始化系统参数为默认参数
static void SysParamSetDefault(void);

//参数系统初始化
void SystemParamInit()
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    //首先申请内存
    sysParamPtr = NULL;
    //申请内存
    sysParamPtr = UserMemMallocWhileSuccess(MEM_REGION_SYS_PARAM,LENGTH_SYS_PARAM);
    //从Flash中加载参数
    errorSub = BOARD_PARAM_READ(ADDR_BOARD_SRV_PARAM_START,(uint8_t*)sysParamPtr,LENGTH_SYS_PARAM);
    if(errorSub != ERROR_SUB_OK)
    {
        SystemPrintf("%s,BOARD_PARAM_READ Failed: 0X%08X\r\n",__func__,errorSub);
        while(1);
    }
    //数据校验
    if(sysParamPtr->paramFlag == SYS_PARAM_DEFAULT_FLAG)
    {
        //校验通过
        return;
    }
    //校验不通过,设定为默认配置
    SysParamSetDefault();
}

//初始化系统参数为默认参数
static void SysParamSetDefault(void)
{
    //设定为默认配置
    sysParamPtr->paramFlag = SYS_PARAM_DEFAULT_FLAG;
    //系统测试参数集合
    UserMemCopy((void*)(&(sysParamPtr->paramBoardInfo)),(void*)(&paramBoardInfoDefault),DIM_STRUCT_LENGTH_BYTE(PARAM_BOARD_INFO));
    //将设定好的值写回外置Flash
    BOARD_PARAM_WRITE(ADDR_BOARD_SRV_PARAM_START,(uint8_t*)sysParamPtr,LENGTH_SYS_PARAM);
}

//通用的写入Flash的函数
static void SysParamWriteSingleCommon(int32_t* targetParamPtr,int32_t writeParam)
{
    *targetParamPtr = writeParam;
    int32_t targetAddr = (int32_t)((char*)targetParamPtr);
    int32_t baseAddr = (int32_t)((char*)(sysParamPtr));
    //计算偏移
    int32_t offset = targetAddr - baseAddr;
    if(offset < 0)
    {
        //偏移必须是正数
        offset = 0 - offset;
    }
    //写入参数
    BOARD_PARAM_WRITE(ADDR_BOARD_SRV_PARAM_START+offset,(uint8_t*)targetParamPtr,4);
}


/*----------------------------------------------------------------------------------------------------*/
//读写 清洗盘 参数集合
ERROR_SUB SysParamReadParamBoardInfo(uint8_t subIndex,int32_t* readParam)
{
    switch(subIndex)
    {
        case INDEX_SUB_PARAM_BOARD_INFO_ITEM1:
            *readParam = sysParamPtr->paramBoardInfo.boardInfoItem1;
            break;
        default://不支持的参数
            *readParam = 0;
            return ERROR_SUB_SYS_PARAM_READ_SUB_INDEX;
    }
    //返回结果
    return ERROR_SUB_OK;
}

ERROR_SUB SysParamWriteParamBoardInfo(uint8_t subIndex,int32_t writeParam)
{
    int32_t* targetParamPtr = NULL;
    switch(subIndex)
    {
        case INDEX_SUB_PARAM_BOARD_INFO_ITEM1:
            targetParamPtr = &(sysParamPtr->paramBoardInfo.boardInfoItem1);
            break;
        default:
            return ERROR_SUB_SYS_PARAM_WRITE_SUB_INDEX;//不支持的参数
    }
    //参数写入Flash
    SysParamWriteSingleCommon(targetParamPtr,writeParam);
    //返回结果
    return ERROR_SUB_OK;
}

/*----------------------------------------------------------------------------------------------------*/

/****************************************参数读取函数指针数组**************************************************/
static const SysParamReadParamSingleFuncPtr SysParamReadParamSingleArray[] = {
    SysParamReadParamBoardInfo,
};

/****************************************参数写入函数指针数组**************************************************/
static const SysParamWriteParamSingleFuncPtr SysParamWriteParamSingleArray[] = {
    SysParamWriteParamBoardInfo,
};


//读取参数
ERROR_SUB SysParamReadSingle(INDEX_MAIN_SYS_PARAM mainIndex,uint8_t subIndex,int32_t* paramRead)
{
    if(mainIndex >= DIM_ARRAY_ELEMENT_COUNT(SysParamReadParamSingleArray))
    {
        return ERROR_SUB_SYS_PARAM_READ_MAIN_INDEX;
    }
    return SysParamReadParamSingleArray[mainIndex](subIndex,paramRead);
}

//写入参数
ERROR_SUB SysParamWriteSingle(INDEX_MAIN_SYS_PARAM mainIndex,uint8_t subIndex,int32_t paramWrite)
{
    if(mainIndex >= DIM_ARRAY_ELEMENT_COUNT(SysParamWriteParamSingleArray))
    {
        return ERROR_SUB_SYS_PARAM_WRITE_MAIN_INDEX;
    }
    return SysParamWriteParamSingleArray[mainIndex](subIndex,paramWrite);
}







