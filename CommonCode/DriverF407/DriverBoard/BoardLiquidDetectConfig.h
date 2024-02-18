/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-03-02 17:36:16 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-12-08 10:38:14 +0800
************************************************************************************************/ 
#ifndef __BOARD_LIQUID_DETECT_CONFIG_H_
#define __BOARD_LIQUID_DETECT_CONFIG_H_
#include "DriverHeaderMCU.h"
#include "DriverConfigBoard.h"

//命令定义
//主动上报基准值
#define CMD_LIQUID_DETECT_AUTO_UPDATE_BASE_ADC                  0X00
//主动上报跟随值
#define CMD_LIQUID_DETECT_AUTO_UPDATE_FOLLOW_ADC                0X01
//主动上报滤波值
#define CMD_LIQUID_DETECT_AUTO_UPDATE_FILTER_ADC                0X02
//主动上报液面探测状态
#define CMD_LIQUID_DETECT_AUTO_UPDATE_TRIG_FLAG                 0X03
//主动上报校准状态
#define CMD_LIQUID_DETECT_AUTO_UPDATE_ADJUST_VALUE              0X04
//读取液面探测阈值
#define LIQUID_DETECT_CMD_READ_THRESHOLD_VALUE                  0X20
//读取液面探测阈值计数器
#define LIQUID_DETECT_CMD_READ_THRESHOLD_COUNT_LIMIT            0X21
//读取液面探测斜率
#define LIQUID_DETECT_CMD_READ_SLOPE_VALUE                      0X22
//读取液面探测斜率计数器
#define LIQUID_DETECT_CMD_READ_SLOPE_COUNT_LIMIT                0X23
//读取电子电阻值
#define LIQUID_DETECT_CMD_READ_AD5272_RES                       0X24
//读取液面探测信号保持时间
#define LIQUID_DETECT_CMD_READ_SIGNAL_HOLD_MS                   0X25
//读取探液过程中的基准电压最大值
#define LIQUID_DETECT_CMD_READ_MAX_NEEDLE_BASE_VOL              0X26
//读取探液过程中的针尖电压最大值
#define LIQUID_DETECT_CMD_READ_MAX_NEEDLE_VOL                   0X27
//读取探液板程序版本
#define LIQUID_DETECT_CMD_READ_LIQUID_BOARD_VERSION             0X28
//写入电子电阻值
#define LIQUID_DETECT_CMD_WRITE_AD5272_RES                      0X40
//基准值校准
#define LIQUID_DETECT_CMD_FUNC_ADJUST_BASE                      0X60
//开关液面探测数据上传
#define LIQUID_DETECT_CMD_FUNC_SWITCH_LIQUID_DATA_UPDATE        0X61
//开关液面探测使能
#define LIQUID_DETECT_CMD_FUNC_SWITCH_LIQUID_ENABLE             0X62
//读取基准电压值
#define LIQUID_DETECT_CMD_READ_NEEDLE_BASE_VOL                  0X65
//读取针尖电压值
#define LIQUID_DETECT_CMD_READ_NEEDLE_CURRENT_VOL               0X66

//基准电阻最大值
#define LIQUID_DETECT_BOARD_BASE_RES_MAX                        1023
//错误重试次数上限
#define LIQUID_DETECT_BOARD_COMM_ERR_RETRY_COUNT                5

//自动上报数据包类型
typedef enum BOARD_LIQUID_DETECT_UPLOAD_PACK_TYPE
{
    BOARD_LIQUID_DETECT_UPLOAD_PACK_TYPE_BASE_VOL              = 0,//探液电压基准值
    BOARD_LIQUID_DETECT_UPLOAD_PACK_TYPE_NEEDLE_VOL_ORIGINAL   = 1,//针尖电压原始值
    BOARD_LIQUID_DETECT_UPLOAD_PACK_TYPE_NEEDLE_VOL_FILTER     = 2,//针尖电压滤波值
    BOARD_LIQUID_DETECT_UPLOAD_PACK_TYPE_STATE                 = 3,//探液状态上报
}BOARD_LIQUID_DETECT_UPLOAD_PACK_TYPE;

//自动上传数据包的回调处理函数
typedef void (*BoardLiquidDetectAutoUploadPackProc)(BOARD_LIQUID_DETECT_UPLOAD_PACK_TYPE packType,uint16_t val);

//探液板的开关功能
typedef enum BOARD_LIQUID_DETECT_FUNC
{
    BOARD_LIQUID_DETECT_DISABLE   = 0,
    BOARD_LIQUID_DETECT_FUNC_1    = 1,
    BOARD_LIQUID_DETECT_FUNC_2    = 2,
    BOARD_LIQUID_DETECT_FUNC_3    = 3,
    BOARD_LIQUID_DETECT_FUNC_4    = 4,
    BOARD_LIQUID_DETECT_FUNC_5    = 5,
    BOARD_LIQUID_DETECT_FUNC_6    = 6,
    BOARD_LIQUID_DETECT_FUNC_7    = 7,
    BOARD_LIQUID_DETECT_FUNC_8    = 8,
    BOARD_LIQUID_DETECT_FUNC_9    = 9,
    BOARD_LIQUID_DETECT_FUNC_10   = 10,
}BOARD_LIQUID_DETECT_FUNC;

//自动上传数据包
typedef struct BOARD_LIQUID_DETECT_UPLOAD_PACK
{
    uint16_t baseVol;//基准值
    uint16_t needleTipVolOriginal;//针尖电压当前值
    uint16_t needleTipVolFilter;//针尖电压滤波值
    uint8_t liquidDetectState;//页面检测状态
}BOARD_LIQUID_DETECT_UPLOAD_PACK;

//探液板的参数
typedef struct BOARD_LIQUID_DETECT_PARAM
{
    uint16_t thresholdVal;//触发阈值
    uint16_t thresholdCount;//阈值触发计数限
    uint16_t slopeVal;//触发斜率
    uint16_t slopeCount;//斜率触发计数限
}BOARD_LIQUID_DETECT_PARAM;

//探液板的配置
typedef struct BOARD_LIQUID_DETECT_CONFIG
{
    uint16_t singleHoldTimeMs;//信号保持时间
    uint16_t softwareVersionRespFlag;//读取版本号的标记
    uint8_t softwareVersionLength;//软件版本号长度
    uint8_t softwareVersionBuf[COMM_CACHE_LENGTH_BOARD_LIQUID_DETECT];//版本号字符串
}BOARD_LIQUID_DETECT_CONFIG;

//探液板的状态
typedef struct BOARD_LIQUID_DETECT_STATE
{
    uint16_t needleTipVol;//针尖电压
    uint16_t baseVol;//基准电压
}BOARD_LIQUID_DETECT_STATE;

//探液板触发讯息
typedef struct BOARD_LIQUID_DETECT_TRIG_MSG
{
    uint16_t needleTipVolMax;//过程中针尖电压最大值
    uint16_t baseVolMax;//过程中基准电压最大值
}BOARD_LIQUID_DETECT_TRIG_MSG;

typedef enum LIQUID_DETECT_BOARD_STATE
{
    LIQUID_DETECT_BOARD_EXIST       = 0x00,//液面探测版存在,可以联机通讯
    LIQUID_DETECT_BOARD_NOT_EXIST   = 0x01,//液面探测板不存在,通讯无法联机
}LIQUID_DETECT_BOARD_STATE;

#endif


