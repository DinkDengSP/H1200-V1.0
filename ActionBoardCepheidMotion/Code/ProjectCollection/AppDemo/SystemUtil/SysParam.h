/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-08-29 14:07:30 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-10-31 16:09:56 +0800
************************************************************************************************/ 
#ifndef __SYS_PARAM_H_
#define __SYS_PARAM_H_
#include "SysParamDefault.h"

//主序号
typedef enum INDEX_MAIN_SYS_PARAM
{
    INDEX_MAIN_SYS_PARAM_TEST_BASE      = 0X00,//测试参数,测试参数读写功能
}INDEX_MAIN_SYS_PARAM;

//系统测试参数集合
typedef enum INDEX_SUB_PARAM_TEST_BASE
{
    INDEX_SUB_PARAM_TEST_BASE_MAIN      = 0X00,//测试参数,测试参数读写功能 
}INDEX_SUB_PARAM_TEST_BASE;


//初始化系统参数
void SysParamInit(void);

//写入参数
ERROR_SUB SysParamWriteSingle(INDEX_MAIN_SYS_PARAM mainIndex,uint8_t subIndex,int32_t paramWrite);

//读取参数
ERROR_SUB SysParamReadSingle(INDEX_MAIN_SYS_PARAM mainIndex,uint8_t subIndex,int32_t* paramRead);


#endif








