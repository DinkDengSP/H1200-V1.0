/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-08-21 10:19:22 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-08-21 13:56:28 +0800
************************************************************************************************/ 
#include "MCU_MPU.h"

//导入外部变量
extern MPU_REGION_CONFIG* mpuRegionConfigPtrArray[16];

//初始化芯片MPU配置
void MCU_MPU_ConfigInit(void)
{
    uint16_t indexUtil = 0;
    //MPU配置结构体
    MPU_Region_InitTypeDef MPU_InitStruct;
	/*配置MPU之前必须先禁止,禁止 MPU */
	HAL_MPU_Disable();
    //区域配置
    for(indexUtil = 0; indexUtil < MPU_REGION_COUNT;indexUtil++)
    {
        //MPU区域,15优先级最高,0优先级最低
        MPU_InitStruct.Number           = MPU_REGION_NUMBER0+indexUtil;
        MPU_InitStruct.Enable           = mpuRegionConfigPtrArray[indexUtil]->enableSet;//是否使能
        MPU_InitStruct.BaseAddress      = mpuRegionConfigPtrArray[indexUtil]->baseAddr;//地址
        MPU_InitStruct.Size             = mpuRegionConfigPtrArray[indexUtil]->regionLen;//区域长度
        MPU_InitStruct.AccessPermission = mpuRegionConfigPtrArray[indexUtil]->accessPermissions;//对该区域的访问级别设定
        MPU_InitStruct.DisableExec      = mpuRegionConfigPtrArray[indexUtil]->execSet;//指令访问使能
        MPU_InitStruct.TypeExtField     = mpuRegionConfigPtrArray[indexUtil]->texSet;//设置Cache策略
        MPU_InitStruct.IsBufferable     = mpuRegionConfigPtrArray[indexUtil]->bufferSet;//是否使能指令缓存 B参数
        MPU_InitStruct.IsCacheable      = mpuRegionConfigPtrArray[indexUtil]->cacheSet;//C参数
        MPU_InitStruct.IsShareable      = mpuRegionConfigPtrArray[indexUtil]->shareSet;//是否使能共享
        MPU_InitStruct.SubRegionDisable = mpuRegionConfigPtrArray[indexUtil]->subRegionEnableConfig;//控制MPU特定子区域是否使能
        //实际配置
        HAL_MPU_ConfigRegion(&MPU_InitStruct);
    }
	/*配置完成,使能MPU*/
	HAL_MPU_Enable(MPU_PRIVILEGED_DEFAULT);
}

//更新更新Cache
void MCU_MPU_CacheReflush(CACHE_REFLUSH_CONFIG config)
{
    if(config == CACHE_WRITE_BACK_ONLY)
    {
        //将CACHE中的数据写回到SRAM中
        SCB_CleanDCache();
    }
    else if(config == CACHE_READ_RELOAD_ONLY)
    {
        //丢弃掉当前Cache中的数据,从SRAM中重新加载数据,可能造成数据丢失
        SCB_InvalidateDCache();
    }
    else if(config == CACHE_READ_WRITE)
    {
        //将CACHE中的数据写回到SRAM并且重新加载到Cache中,消耗的时间最长
        SCB_CleanInvalidateDCache();
    }
}












