/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-11-22 09:01:17 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-12-25 10:22:39 +0800
************************************************************************************************/ 
#ifndef __MOD_IMPL_BASE_HAND_NEW_CUP_H_
#define __MOD_IMPL_BASE_HAND_NEW_CUP_H_
#include "ModImplHandNewCupConfig.h"

//机械手当前位置
typedef enum HAND_NEW_CUP_POS
{
    HAND_NEW_CUP_POS_NEW_TRAY       = 0X00,//新杯上方
    HAND_NEW_CUP_POS_GARBAGE_1      = 0X01,//垃圾桶1上方
    HAND_NEW_CUP_POS_GARBAGE_2      = 0X02,//垃圾桶2上方
    HAND_NEW_CUP_POS_PLATE_REACTION = 0X03,//反应盘上方
}HAND_NEW_CUP_POS;

//读取模块输入
void ModImplBaseHandNewCupReadPortIn(uint8_t portIdx,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);

//写入模块输出
void ModImplBaseHandNewCupWritePortOut(uint8_t portIdx,PIN_STATE setState,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);

//步进电机复位
void ModImplBaseHandNewCupStepMotorReset(uint8_t stepMotorIdx,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);

//步进电机走位
void ModImplBaseHandNewCupStepMotorRunSteps(uint8_t stepMotorIdx,int32_t runSteps,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);

//新杯机械手复位
void ModImplBaseHandNewCupReset(SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);

//新杯机械手移动到新杯区
void ModImplBaseHandNewCupMoveToNewCupTray(uint8_t lineBackFront,uint8_t colLeftRight,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);

//新杯机械手移动到反应盘
void ModImplBaseHandNewCupMoveToPlateReaction(SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);

//新杯机械手移动到垃圾桶
void ModImplBaseHandNewCupMoveToGarbage(GARBAGE_SELECT garbageNo,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);

//新杯机械手新杯区取杯
void ModImplBaseHandNewCupCatchCupNewTray(SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);

//新杯机械手反应盘取杯
void ModImplBaseHandNewCupCatchCupPlate(SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);

//新杯机械手反应盘放杯
void ModImplBaseHandNewCupPutCupPlate(SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);

//新杯机械手垃圾桶放杯
void ModImplBaseHandNewCupPutCupGarbage(GARBAGE_SELECT garbageNo,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);


#endif


















