/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-11-03 16:12:17 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-11-06 10:56:28 +0800
************************************************************************************************/ 
#ifndef __CORE_DATA_TYPE_LOCAL_H_
#define __CORE_DATA_TYPE_LOCAL_H_
#include "CoreDataType.h"

//网口数据源
typedef enum NET_PACK_SRC
{
    NET_PACK_SRC_MAIN     = 0X00,//主通讯端口
    NET_PACK_SRC_LOG      = 0X01,//日志通讯端口
    NET_PACK_SRC_PROBE    = 0X02,//探针通讯端口
}NET_PACK_SRC;

//网口发送数据的识别ID
typedef enum NET_PACK_DATA_ID
{
    NET_PACK_DATA_ID_DEBUG  = 0X00000000,
    //探针数据
    NET_PACK_DATA_ID_PROBE  = 0X00000001,
    NET_PACK_DATA_ID_MAX    = 0XFFFFFFFF,
}NET_PACK_DATA_ID;

//网口数据包
typedef struct NET_RECV_PACK
{
    NET_PACK_SRC netPackSrc;//数据源
    uint32_t peerIpAddress;    //发送数据的远端IP
    uint32_t peerPort;         //发送数据的远端端口
    uint32_t recvDataLength;   //接收数据长度
    uint8_t* recvDatBufPtr; //接收数据指针
}NET_RECV_PACK;
#define LENGTH_NET_RECV_PACK        (sizeof(NET_RECV_PACK)/sizeof(uint8_t))

//数据发送包
typedef struct NET_SEND_PACK
{
    NET_PACK_SRC netPackTarget;//数据目标
    //不限制数据元素,但是应用层可以约定不同的数据元素
    NET_PACK_DATA_ID elamentDataPtrID;//数据元素类型ID
    void* dataElementPtr;//数据元素指针
}NET_SEND_PACK;
#define LENGTH_NET_SEND_PACK        (sizeof(NET_SEND_PACK)/sizeof(uint8_t))

/*********************************步进电机相关定义*******************************************/
//步进电机序号,系统最大可以带动八个电机
typedef enum STEP_MOTOR_NO
{
    STEP_MOTOR_NO1   = 0,
    STEP_MOTOR_NO2   = 1,
    STEP_MOTOR_NO3   = 2,
    STEP_MOTOR_NO4   = 3,
    STEP_MOTOR_NO5   = 4,
    STEP_MOTOR_NO6   = 5,
    STEP_MOTOR_NO7   = 6,
    STEP_MOTOR_NO8   = 7,
    STEP_MOTOR_NO9   = 8,
    STEP_MOTOR_NO10  = 9,
    STEP_MOTOR_NO11  = 10,
    STEP_MOTOR_NO12  = 11,
    STEP_MOTOR_NO13  = 12,
    STEP_MOTOR_NO14  = 13,
    STEP_MOTOR_NO15  = 14,
    STEP_MOTOR_NO16  = 15,
    STEP_MOTOR_NO_MAX = STEP_MOTOR_NO16,
}STEP_MOTOR_NO;

//步进电机方向
typedef enum STEP_MOTOR_DIR
{
    STEP_MOTOR_DIR_CCW = 0,//逆时针
    STEP_MOTOR_DIR_CW  = 1,//顺时针
}STEP_MOTOR_DIR;

//速度曲线计算比率
typedef enum SPEED_RATIO_MODE
{
    SPEED_RATIO_MODE_GLOBAL     = 0,//速度比例的生效模式,全局比例速度
    SPEED_RATIO_MODE_UP_START   = 1,//速度比例的生效模式,比例速度加上启动速度
}SPEED_RATIO_MODE;

//步数配比模式
typedef enum STEP_MODE
{
    FIXED           = 0,//固定步数
    SAME_TIME_FRAME = 1,//等时间帧,每一个加速点使用的时间是固定的
}STEP_MODE;


//电机速度曲线选择
typedef enum SM_CURVE_SELECT
{
    SM_CURVE_0      = 0X00,
    SM_CURVE_1      = 0X01,
    SM_CURVE_2      = 0X02,
    SM_CURVE_3      = 0X03,
    SM_CURVE_4      = 0X04,
    SM_CURVE_5      = 0X05,
    SM_CURVE_6      = 0X06,
    SM_CURVE_7      = 0X07,
    SM_CURVE_8      = 0X08,
    SM_CURVE_9      = 0X09,
    SM_CURVE_10     = 0X0A,
    SM_CURVE_11     = 0X0B,
    SM_CURVE_12     = 0X0C,
    SM_CURVE_13     = 0X0D,
    SM_CURVE_14     = 0X0E,
    SM_CURVE_15     = 0X0F,    
    SM_CURVE_16     = 0X10,
    SM_CURVE_17     = 0X11,
    SM_CURVE_18     = 0X12,
    SM_CURVE_19     = 0X13,
    SM_CURVE_20     = 0X14,
}SM_CURVE_SELECT;

#endif















