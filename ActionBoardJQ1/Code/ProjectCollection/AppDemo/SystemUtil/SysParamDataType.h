/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-08-29 14:07:30 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-09-22 10:18:50 +0800
************************************************************************************************/ 
#ifndef __SYS_PARAM_DATA_TYPE_H_
#define __SYS_PARAM_DATA_TYPE_H_
#include "DriverHeaderBoard.h"

//应用配置的标志位,初始化检测该标志位,如果不对,需要设置为默认参数
#define SYS_PARAM_DEFAULT_FLAG              0XAA5555AA

//直线电机坐标参数集合
typedef struct SYS_PARAM_LATERALLY_MOTOR
{
    int32_t resetCorrect;       //复位修正
    int32_t dirSwitchCorrect;   //换向修正
    int32_t idlePos;            //空闲位置
    int32_t rackStore1Pos;      //试管架仓1号位置
    int32_t rackStore2Pos;      //试管架仓2号位置
    int32_t rackStore3Pos;      //试管架仓3号位置
    int32_t rackStore4Pos;      //试管架仓4号位置
    int32_t rackStore5Pos;      //试管架仓5号位置
    int32_t rackStore6Pos;      //试管架仓6号位置
    int32_t rackStore7Pos;      //试管架仓7号位置
    int32_t rackStore8Pos;      //试管架仓8号位置
    int32_t rackStore9Pos;      //试管架仓9号位置
    int32_t rackStore10Pos;     //试管架仓10号位置
    int32_t rackStore11Pos;     //试管架仓11号位置
    int32_t rackStore12Pos;     //试管架仓12号位置
    int32_t rackStore13Pos;     //试管架仓13号位置
    int32_t rackStore14Pos;     //试管架仓14号位置
    int32_t rackStore15Pos;     //试管架仓15号位置
    int32_t rackStore16Pos;     //试管架仓16号位置
    int32_t rackStore17Pos;     //试管架仓17号位置
    int32_t rackStore18Pos;     //试管架仓18号位置
    int32_t rackStore19Pos;     //试管架仓19号位置
    int32_t rackStore20Pos;     //试管架仓20号位置
    int32_t rackStore21Pos;     //试管架仓21号位置
    int32_t rackStore22Pos;     //试管架仓22号位置
    int32_t rackStore23Pos;     //试管架仓23号位置
    int32_t c21InterPos;        //C21接口位置
    int32_t resverse[50];       //保留参数
}SYS_PARAM_LATERALLY_MOTOR;
#define LENGTH_SYS_PARAM_LATERALLY_MOTOR                 DIM_STRUCT_LENGTH_BYTE(SYS_PARAM_LATERALLY_MOTOR)

//升降电机坐标参数集合
typedef struct SYS_PARAM_UPDOWN_MOTOR
{
    int32_t resetCorrect;           //复位修正
    int32_t dirSwitchCorrect;       //换向修正
    int32_t idlePos;                //空闲位置
    int32_t loadRackStore1Pos;      //试管架仓1装载位置
    int32_t unloadRackStore1Pos;    //试管架仓1卸载位置
    int32_t loadRackStore2Pos;      //试管架仓2装载位置
    int32_t unloadRackStore2Pos;    //试管架仓2卸载位置
    int32_t loadRackStore3Pos;      //试管架仓3装载位置
    int32_t unloadRackStore3Pos;    //试管架仓3卸载位置
    int32_t loadRackStore4Pos;      //试管架仓4装载位置
    int32_t unloadRackStore4Pos;    //试管架仓4卸载位置
    int32_t loadRackStore5Pos;      //试管架仓5装载位置
    int32_t unloadRackStore5Pos;    //试管架仓5卸载位置
    int32_t loadRackStore6Pos;      //试管架仓6装载位置
    int32_t unloadRackStore6Pos;    //试管架仓6卸载位置
    int32_t loadRackStore7Pos;      //试管架仓7装载位置
    int32_t unloadRackStore7Pos;    //试管架仓7卸载位置
    int32_t loadRackStore8Pos;      //试管架仓8装载位置
    int32_t unloadRackStore8Pos;    //试管架仓8卸载位置
    int32_t loadRackStore9Pos;      //试管架仓9装载位置
    int32_t unloadRackStore9Pos;    //试管架仓9卸载位置
    int32_t loadRackStore10Pos;     //试管架仓10装载位置
    int32_t unloadRackStore10Pos;   //试管架仓10卸载位置
    int32_t loadRackStore11Pos;     //试管架仓11装载位置
    int32_t unloadRackStore11Pos;   //试管架仓11卸载位置
    int32_t loadRackStore12Pos;     //试管架仓12装载位置
    int32_t unloadRackStore12Pos;   //试管架仓12卸载位置
    int32_t loadRackStore13Pos;     //试管架仓13装载位置
    int32_t unloadRackStore13Pos;   //试管架仓13卸载位置
    int32_t loadRackStore14Pos;     //试管架仓14装载位置
    int32_t unloadRackStore14Pos;   //试管架仓14卸载位置
    int32_t loadRackStore15Pos;     //试管架仓15装载位置
    int32_t unloadRackStore15Pos;   //试管架仓15卸载位置
    int32_t loadRackStore16Pos;     //试管架仓16装载位置
    int32_t unloadRackStore16Pos;   //试管架仓16卸载位置
    int32_t loadRackStore17Pos;     //试管架仓17装载位置
    int32_t unloadRackStore17Pos;   //试管架仓17卸载位置
    int32_t loadRackStore18Pos;     //试管架仓18装载位置
    int32_t unloadRackStore18Pos;   //试管架仓18卸载位置
    int32_t loadRackStore19Pos;     //试管架仓19装载位置
    int32_t unloadRackStore19Pos;   //试管架仓19卸载位置
    int32_t loadRackStore20Pos;     //试管架仓20装载位置
    int32_t unloadRackStore20Pos;   //试管架仓20卸载位置
    int32_t loadRackStore21Pos;     //试管架仓21装载位置
    int32_t unloadRackStore21Pos;   //试管架仓21卸载位置
    int32_t loadRackStore22Pos;     //试管架仓22装载位置
    int32_t unloadRackStore22Pos;   //试管架仓22卸载位置
    int32_t loadRackStore23Pos;     //试管架仓23装载位置
    int32_t unloadRackStore23Pos;   //试管架仓23卸载位置
    int32_t loadC21Pos;             //C21装载位置
    int32_t unloadC21Pos;           //C21卸载位置
    int32_t resverse[50];           //保留参数
}SYS_PARAM_UPDOWN_MOTOR;
#define LENGTH_SYS_PARAM_UPDOWN_MOTOR                   DIM_STRUCT_LENGTH_BYTE(SYS_PARAM_UPDOWN_MOTOR)

//顶杆电机坐标参数集合
typedef struct SYS_PARAM_ROD_MOTOR
{
    int32_t resetCorrect;           //复位修正
    int32_t dirSwitchCorrect;       //换向修正
    int32_t idlePos;                //空闲位置
    int32_t loadRackStore1Pos;      //试管架仓1装载位置
    int32_t unloadRackStore1Pos;    //试管架仓1卸载位置
    int32_t loadRackStore2Pos;      //试管架仓2装载位置
    int32_t unloadRackStore2Pos;    //试管架仓2卸载位置
    int32_t loadRackStore3Pos;      //试管架仓3装载位置
    int32_t unloadRackStore3Pos;    //试管架仓3卸载位置
    int32_t loadRackStore4Pos;      //试管架仓4装载位置
    int32_t unloadRackStore4Pos;    //试管架仓4卸载位置
    int32_t loadRackStore5Pos;      //试管架仓5装载位置
    int32_t unloadRackStore5Pos;    //试管架仓5卸载位置
    int32_t loadRackStore6Pos;      //试管架仓6装载位置
    int32_t unloadRackStore6Pos;    //试管架仓6卸载位置
    int32_t loadRackStore7Pos;      //试管架仓7装载位置
    int32_t unloadRackStore7Pos;    //试管架仓7卸载位置
    int32_t loadRackStore8Pos;      //试管架仓8装载位置
    int32_t unloadRackStore8Pos;    //试管架仓8卸载位置
    int32_t loadRackStore9Pos;      //试管架仓9装载位置
    int32_t unloadRackStore9Pos;    //试管架仓9卸载位置
    int32_t loadRackStore10Pos;     //试管架仓10装载位置
    int32_t unloadRackStore10Pos;   //试管架仓10卸载位置
    int32_t loadRackStore11Pos;     //试管架仓11装载位置
    int32_t unloadRackStore11Pos;   //试管架仓11卸载位置
    int32_t loadRackStore12Pos;     //试管架仓12装载位置
    int32_t unloadRackStore12Pos;   //试管架仓12卸载位置
    int32_t loadRackStore13Pos;     //试管架仓13装载位置
    int32_t unloadRackStore13Pos;   //试管架仓13卸载位置
    int32_t loadRackStore14Pos;     //试管架仓14装载位置
    int32_t unloadRackStore14Pos;   //试管架仓14卸载位置
    int32_t loadRackStore15Pos;     //试管架仓15装载位置
    int32_t unloadRackStore15Pos;   //试管架仓15卸载位置
    int32_t loadRackStore16Pos;     //试管架仓16装载位置
    int32_t unloadRackStore16Pos;   //试管架仓16卸载位置
    int32_t loadRackStore17Pos;     //试管架仓17装载位置
    int32_t unloadRackStore17Pos;   //试管架仓17卸载位置
    int32_t loadRackStore18Pos;     //试管架仓18装载位置
    int32_t unloadRackStore18Pos;   //试管架仓18卸载位置
    int32_t loadRackStore19Pos;     //试管架仓19装载位置
    int32_t unloadRackStore19Pos;   //试管架仓19卸载位置
    int32_t loadRackStore20Pos;     //试管架仓20装载位置
    int32_t unloadRackStore20Pos;   //试管架仓20卸载位置
    int32_t loadRackStore21Pos;     //试管架仓21装载位置
    int32_t unloadRackStore21Pos;   //试管架仓21卸载位置
    int32_t loadRackStore22Pos;     //试管架仓22装载位置
    int32_t unloadRackStore22Pos;   //试管架仓22卸载位置
    int32_t loadRackStore23Pos;     //试管架仓23装载位置
    int32_t unloadRackStore23Pos;   //试管架仓23卸载位置
    int32_t loadC21Pos;             //C21装载位置
    int32_t unloadC21Pos;           //C21卸载位置
    int32_t resverse[50];           //保留参数
}SYS_PARAM_ROD_MOTOR;
#define LENGTH_SYS_PARAM_ROD_MOTOR                      DIM_STRUCT_LENGTH_BYTE(SYS_PARAM_ROD_MOTOR)

//皮带电机坐标参数集合
typedef struct SYS_PARAM_BELT_MOTOR
{
    int32_t resetCorrect;           //复位修正
    int32_t dirSwitchCorrect;       //换向修正
    int32_t idlePos;                //空闲位置
    int32_t loadRackStore1Pos;      //试管架仓1装载位置
    int32_t unloadRackStore1Pos;    //试管架仓1卸载位置
    int32_t loadRackStore2Pos;      //试管架仓2装载位置
    int32_t unloadRackStore2Pos;    //试管架仓2卸载位置
    int32_t loadRackStore3Pos;      //试管架仓3装载位置
    int32_t unloadRackStore3Pos;    //试管架仓3卸载位置
    int32_t loadRackStore4Pos;      //试管架仓4装载位置
    int32_t unloadRackStore4Pos;    //试管架仓4卸载位置
    int32_t loadRackStore5Pos;      //试管架仓5装载位置
    int32_t unloadRackStore5Pos;    //试管架仓5卸载位置
    int32_t loadRackStore6Pos;      //试管架仓6装载位置
    int32_t unloadRackStore6Pos;    //试管架仓6卸载位置
    int32_t loadRackStore7Pos;      //试管架仓7装载位置
    int32_t unloadRackStore7Pos;    //试管架仓7卸载位置
    int32_t loadRackStore8Pos;      //试管架仓8装载位置
    int32_t unloadRackStore8Pos;    //试管架仓8卸载位置
    int32_t loadRackStore9Pos;      //试管架仓9装载位置
    int32_t unloadRackStore9Pos;    //试管架仓9卸载位置
    int32_t loadRackStore10Pos;     //试管架仓10装载位置
    int32_t unloadRackStore10Pos;   //试管架仓10卸载位置
    int32_t loadRackStore11Pos;     //试管架仓11装载位置
    int32_t unloadRackStore11Pos;   //试管架仓11卸载位置
    int32_t loadRackStore12Pos;     //试管架仓12装载位置
    int32_t unloadRackStore12Pos;   //试管架仓12卸载位置
    int32_t loadRackStore13Pos;     //试管架仓13装载位置
    int32_t unloadRackStore13Pos;   //试管架仓13卸载位置
    int32_t loadRackStore14Pos;     //试管架仓14装载位置
    int32_t unloadRackStore14Pos;   //试管架仓14卸载位置
    int32_t loadRackStore15Pos;     //试管架仓15装载位置
    int32_t unloadRackStore15Pos;   //试管架仓15卸载位置
    int32_t loadRackStore16Pos;     //试管架仓16装载位置
    int32_t unloadRackStore16Pos;   //试管架仓16卸载位置
    int32_t loadRackStore17Pos;     //试管架仓17装载位置
    int32_t unloadRackStore17Pos;   //试管架仓17卸载位置
    int32_t loadRackStore18Pos;     //试管架仓18装载位置
    int32_t unloadRackStore18Pos;   //试管架仓18卸载位置
    int32_t loadRackStore19Pos;     //试管架仓19装载位置
    int32_t unloadRackStore19Pos;   //试管架仓19卸载位置
    int32_t loadRackStore20Pos;     //试管架仓20装载位置
    int32_t unloadRackStore20Pos;   //试管架仓20卸载位置
    int32_t loadRackStore21Pos;     //试管架仓21装载位置
    int32_t unloadRackStore21Pos;   //试管架仓21卸载位置
    int32_t loadRackStore22Pos;     //试管架仓22装载位置
    int32_t unloadRackStore22Pos;   //试管架仓22卸载位置
    int32_t loadRackStore23Pos;     //试管架仓23装载位置
    int32_t unloadRackStore23Pos;   //试管架仓23卸载位置
    int32_t loadC21Pos;             //C21装载位置
    int32_t unloadC21Pos;           //C21卸载位置
    int32_t barscanStartPos;        //扫码起始位置
    int32_t barscanEndPos;          //扫码结束位置
    int32_t tube1ScanPos;           //试管扫码点1
    int32_t tube2ScanPos;           //试管扫码点2
    int32_t tube3ScanPos;           //试管扫码点3
    int32_t tube4ScanPos;           //试管扫码点4
    int32_t tube5ScanPos;           //试管扫码点5
    int32_t tube6ScanPos;           //试管扫码点6
    int32_t tube7ScanPos;           //试管扫码点7
    int32_t tube8ScanPos;           //试管扫码点8
    int32_t tube9ScanPos;           //试管扫码点9
    int32_t tube10ScanPos;          //试管扫码点10
    int32_t adjustStartPos;         //校准起始点
    int32_t adjustOverPos;          //校准结束点
    int32_t resverse[50];           //保留参数
}SYS_PARAM_BELT_MOTOR;
#define LENGTH_SYS_PARAM_BELT_MOTOR                      DIM_STRUCT_LENGTH_BYTE(SYS_PARAM_BELT_MOTOR)

//辅助参数
typedef struct SYS_PARAM_UTIL
{
    int32_t scanMode;               //扫码模式
    int32_t resverse[50];           //保留参数
}SYS_PARAM_UTIL;
#define LENGTH_SYS_PARAM_UTIL                           DIM_STRUCT_LENGTH_BYTE(SYS_PARAM_UTIL)


/*---------------------------------------------------------------------------------------------------------------*/
//总的系统参数
typedef struct SYS_PARAM
{
    uint32_t paramFlag;//参数已经配置过标识
    SYS_PARAM_LATERALLY_MOTOR laterallyMotorParam;//直线电机参数
    SYS_PARAM_UPDOWN_MOTOR updownMotorParam;//升降电机参数
    SYS_PARAM_ROD_MOTOR rodMotorParam;//顶杆电机参数
    SYS_PARAM_BELT_MOTOR beltMotorParam;//皮带电机参数
    SYS_PARAM_UTIL utilParam;//辅助参数
}SYS_PARAM;
//参数长度
#define LENGTH_SYS_PARAM     DIM_STRUCT_LENGTH_BYTE(SYS_PARAM)


#endif



