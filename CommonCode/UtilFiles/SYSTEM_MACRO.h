/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-10-16 13:16:14 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-10-20 11:42:50 +0800
************************************************************************************************/ 
#ifndef __SYSTEM_MACRO_H_
#define __SYSTEM_MACRO_H_

//设备A模块号
#define SYSTEM_MODULE_NO_DEVICE_A                     0X00
//设备B模块号
#define SYSTEM_MODULE_NO_DEVICE_B                     0X01
//设备C模块号
#define SYSTEM_MODULE_NO_DEVICE_C                     0X02
//设备D模块号
#define SYSTEM_MODULE_NO_DEVICE_D                     0X03
//设备E模块号
#define SYSTEM_MODULE_NO_DEVICE_E                     0X04
//设备内部下位机模块号              
#define SYSTEM_MODULE_NO_ACT                          0X05
//轨道模块号                
#define SYSTEM_MODULE_NO_RAIL                         0X06
//立禾云模块号                
#define SYSTEM_MODULE_NO_CLOUD                        0X06
//PC机模块号                
#define SYSTEM_MODULE_NO_PC                           0X07

//设备板号定义              
//PC板号                
#define SYSTEM_BOARD_ID_PC                            0X1F
//可能存在的中位机板号              
#define SYSTEM_BOARD_ID_MID                           0X1E
//主控板号起始              
#define SYSTEM_BOARD_ID_MAIN_START                    0X1A
//主控板号结束              
#define SYSTEM_BOARD_ID_MAIN_END                      0X1D
/***************************************************************/
//轨道主控板板号
#define SYSTEM_BOARD_ID_MAIN_CONTROL_RAIL             0X1A
//设备主控板板号
#define SYSTEM_BOARD_ID_MAIN_CONTROL_DEVICE           0X1A
//立禾云板号
#define SYSTEM_BOARD_ID_MAIN_CONTROL_CLOUD            0X1B
/***************************************************************/
//下位机板号起始                
#define SYSTEM_BOARD_ID_ACT_START                     0X01
//下位机板号               
#define SYSTEM_BOARD_ID_ACT_1                         0X01
//下位机板号               
#define SYSTEM_BOARD_ID_ACT_2                         0X02
//下位机板号               
#define SYSTEM_BOARD_ID_ACT_3                         0X03
//下位机板号               
#define SYSTEM_BOARD_ID_ACT_4                         0X04
//下位机板号               
#define SYSTEM_BOARD_ID_ACT_5                         0X05
//下位机板号               
#define SYSTEM_BOARD_ID_ACT_6                         0X06
//下位机板号               
#define SYSTEM_BOARD_ID_ACT_7                         0X07
//下位机板号               
#define SYSTEM_BOARD_ID_ACT_8                         0X08
//温控版               
#define SYSTEM_BOARD_ID_TC_H360                       0X18
//下位机板号结束                
#define SYSTEM_BOARD_ID_ACT_END                       0X19
//H360一号板
#define SYSTEM_BOARD_ID_H360_ACT_1                    0X01
//H360二号板
#define SYSTEM_BOARD_ID_H360_ACT_2                    0X02
//H360三号板
#define SYSTEM_BOARD_ID_H360_ACT_3                    0X03
//H360四号板
#define SYSTEM_BOARD_ID_H360_ACT_4                    0X04
//H360五号板
#define SYSTEM_BOARD_ID_H360_ACT_5                    0X05

#endif




