/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-11-14 11:31:37 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-11-14 11:33:57 +0800
************************************************************************************************/ 
#ifndef __BOARD_SSROUT_H_
#define __BOARD_SSROUT_H_
#include "DriverHeaderMCU.h"

/*--------------------------------SSR OUT1-----------------------------*/
//初始化
void BoardSSROut1Init(void);
//启动
void BoardSSROut1Start(float strength);
//设置强度
void BoardSSROut1SetStrength(float strength);
//停止
void BoardSSROut1Stop(void);

/*--------------------------------SSR OUT2-----------------------------*/
//初始化
void BoardSSROut2Init(void);
//启动
void BoardSSROut2Start(float strength);
//设置强度
void BoardSSROut2SetStrength(float strength);
//停止
void BoardSSROut2Stop(void);

/*--------------------------------SSR OUT3-----------------------------*/
//初始化
void BoardSSROut3Init(void);
//启动
void BoardSSROut3Start(float strength);
//设置强度
void BoardSSROut3SetStrength(float strength);
//停止
void BoardSSROut3Stop(void);

/*--------------------------------SSR OUT4-----------------------------*/
//初始化
void BoardSSROut4Init(void);
//启动
void BoardSSROut4Start(float strength);
//设置强度
void BoardSSROut4SetStrength(float strength);
//停止
void BoardSSROut4Stop(void);


#endif















