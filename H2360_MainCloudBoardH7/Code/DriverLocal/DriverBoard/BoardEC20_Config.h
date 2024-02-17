/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-11-03 16:12:17 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-11-08 12:06:46 +0800
************************************************************************************************/ 
#ifndef __BOARD_EC20_CONFIG_H_
#define __BOARD_EC20_CONFIG_H_
#include "DriverHeaderMCU.h"

//EC20模块指令,检查模块是否存在
#define EC20_CMD_CHECK_MODULE_EXIST                 "AT"
//EC20模块指令,设置模块波特率
#define EC20_CMD_SET_MODULE_BAUD                    "AT+IPR="
//EC20模块指令,保存模块当前配置信息
#define EC20_CMD_SAVE_USER_CONFIG                   "AT&W"
//关闭设备流控制
#define EC20_CMD_CLOSE_UART_FLOW_CONROL             "AT+IFC=0,0"
//EC20模块指令,打开模块AT指令回显,将发送的数据打印出来
#define EC20_CMD_CLOSE_AT_ECHO                      "ATE0"
//EC20模块指令,关闭模块AT指令回显,不将发送的数据打印出来
#define EC20_CMD_OPEN_AT_ECHO                       "ATE1"
//EC20模块指令,检查模块是否已经准备好,可以接受指令
#define EC20_CMD_CHECK_MODULE_READY                 "AT+CPAS"
//EC20模块指令,检查SIM卡是否已经解锁或者SIM卡等待解锁
#define EC20_CMD_CHECK_SIM_CARD_PIN_READY           "AT+CPIN?"
//EC20模块指令,检查SIM卡是否功能初始化完成
#define EC20_CMD_CHECK_SIM_CARD_INIT_READY          "AT+QINISTAT"
//设置模块网络注册格式
#define EC20_CMD_SET_NOTWORK_REGISTER_FORMAT        "AT+CREG=2"
//EC20模块指令,检查模块是否已经注册上网络
#define EC20_CMD_CHECK_NETWORK_REGISTERED           "AT+CREG?"
//CGREG注册
#define EC20_CMD_SET_CG_REG_FORMAT                  "AT+CGREG=2"
//EC20模块指令,检查模块CGREG注册
#define EC20_CMD_CHECK_CG_REG_REGISTERED            "AT+CGREG?"
//CEREG注册
#define EC20_CMD_SET_CE_REG_FORMAT                  "AT+CEREG=2"
//EC20模块指令,检查模块CEREG注册
#define EC20_CMD_CHECK_CE_REG_REGISTERED            "AT+CEREG?"
//EC20模块指令,检查模块GPS是否已经附着
#define EC20_CMD_CHECK_GPS_ATTACH                   "AT+CGATT?"
//EC20模块指令,查询模块厂家编码
#define EC20_CMD_QUERY_MANUFACTURER_STR             "AT+GMI"
//EC20模块指令,查询模块类型编码
#define EC20_CMD_QUERY_MODULE_TYPE_STR              "AT+GMM"
//EC20模块指令,查询模块软件版本号编码
#define EC20_CMD_QUERY_SOFTWARE_VERSION_STR         "AT+GMR"
//EC20模块指令,查询模块入网许可IMEI号码
#define EC20_CMD_QUERY_MODULE_IMEI_STR              "AT+GSN"
//EC20模块指令,查询SIM卡入网许可IMSI号码
#define EC20_CMD_QUERY_SIM_IMSI_STR                 "AT+CIMI"
//EC20模块指令,查询SIM卡唯一ID ICCID
#define EC20_CMD_QUERY_SIM_ICCID_STR                "AT+QCCID"
//EC20模块指令,查询当前模块注册的网络名称
#define EC20_CMD_QUERY_NET_NAME                     "AT+QSPN"
//EC20模块指令,查询当前模块注册的网络类型
#define EC20_CMD_QUERY_NET_TYPE                     "AT+QNWINFO"
//EC20模块指令,查询当前信号强度
#define EC20_CMD_QUERY_SIGNAL_STRENGTH              "AT+CSQ"
//默认设置GPRS接入APN,上下文1,IPV4协议,CMNET中国移动
#define EC20_CMD_SET_GPRS_APN_POINT                 "AT+QICSGP=1,1,\"CMNET\""
//关闭GPRS移动场景
#define EC20_CMD_DEACT_GPRS                         "AT+QIDEACT=1"
//打开GPRS移动场景
#define EC20_CMD_ACT_GPRS                           "AT+QIACT=1"
//ping一下服务器,看看能否联网
#define EC20_CMD_PING_SERVER                        "AT+QPING=1,\"47.103.19.144\""
//ping一下百度,看看能否联网
#define EC20_CMD_PING_BAIDU                         "AT+QPING=1,\"baidu.com\""
//网络连接 1标识辅佐的网络为1
#define EC20_CMD_SOCKET_CONNECT                     "AT+QIOPEN=1"
//网络断开
#define EC20_CMD_SOCKET_DISCONNECT                  "AT+QICLOSE="
//网络数据包发送
#define EC20_CMD_SOCKET_SEND_DATA                   "AT+QISEND="
//检查当前网络需要读取的数据长度
#define EC20_CMD_SOCKET_CHECK_RECV_LENGTH           "AT+QIRD="
//读取指定的数据长度的数据,如果不够就读取全部的数据
#define EC20_CMD_SOCKET_READ_DATA                   "AT+QIRD="
//清除缓冲区,将数据全部读取出来
#define EC20_CMD_SOCKET_CLEAR_BUF                   "AT+QIRD="
//查询当前连接状态,默认带上下文
#define EC20_CMD_CHECK_CONNECT_STATE                "AT+QISTATE=1,"

//AT指令通用等待返回的时间
#define TIME_MS_MAX_BOARD_EC20_WAIT_RESP            300
//PING指令的返回时间
#define TIME_MS_MAX_BOARD_EC20_CMD_PING             1000
//SOCKET指令的返回时间
#define TIME_MS_MAX_BOARD_EC20_CMD_SOCKET           3000
//网络通讯超时时间
#define TIME_MS_OUT_NETWORK_SOCKET_COMM             15000
//PING超时时间
#define TIME_MS_OUT_NETWORK_SOCKET_PING             4000
//信号强度最小值,低于这个值,不能联网
#define EC20_SIGNAL_STRENGTH_MIN                    12
//EC20一包数据发送的最大长度
#define EC20_RECV_DATA_BUFFER_MAX                   1460

//EC20设备状态
typedef enum EC20_DEVICE_STA
{
    EC20_DEVICE_STA_POWER_ON        = 0X00,//上电状态
    EC20_DEVICE_STA_RESET           = 0X01,//复位中
    EC20_DEVICE_STA_CHK_EXIST       = 0X02,//检测是否存在
    EC20_DEVICE_STA_CONFIG          = 0X03,//配置
    EC20_DEVICE_STA_CHK_MOD_RDY     = 0X04,//检测模块是否准备好
    EC20_DEVICE_STA_CHK_SIM_RDY     = 0X05,//检测卡是否准备好
    EC20_DEVICE_STA_SET_CREG        = 0X06,//设置CS网络注册
    EC20_DEVICE_STA_QUERY_CREG      = 0X07,//检测CS网络注册
    EC20_DEVICE_STA_SET_CG_REG      = 0X08,//设置CG网络注册
    EC20_DEVICE_STA_QUERY_CG_REG    = 0X09,//检测CG网络注册
    EC20_DEVICE_STA_SET_CE_REG      = 0X0A,//设置CE网络注册
    EC20_DEVICE_STA_QUERY_CE_REG    = 0X0B,//检测CE网络注册
    EC20_DEVICE_STA_GPRS_DISABLE    = 0X0C,//关闭GPRS
    EC20_DEVICE_STA_GPRS_CONFIG     = 0X0D,//设置GPRS配置
    EC20_DEVICE_STA_GPRS_ENABLE     = 0X0E,//使能GPRS
    EC20_DEVICE_STA_GET_MSG         = 0X0F,//获取设备信息
    EC20_DEVICE_STA_PING_BAIDU      = 0X10,//ping百度测试网络连接
    EC20_DEVICE_STA_OK              = 0X11,//状态初始化完成
}EC20_DEVICE_STA;

//串口数据的标识
typedef enum EC20_UART_FLAG
{
    EC20_UART_FLAG_NONE     = 0X00,//没接收到标志
    EC20_UART_FLAG_OK       = 0X01,//返回完成
    EC20_UART_FLAG_ERROR    = 0X02,//返回错误
}EC20_UART_FLAG;

//连接状态
typedef enum EC20_CONNECT_STATE
{
    EC20_CONNECT_STATE_INIT     = 0,
    EC20_CONNECT_STATE_OPENING  = 1,
    EC20_CONNECT_STATE_CONNECT  = 2,
    EC20_CONNECT_STATE_LISTING  = 3,
    EC20_CONNECT_STATE_CLOSEING = 4,
}EC20_CONNECT_STATE;

//模块的各类信息字符串
#define LENGTH_EC20_CODE_STRING                     50
//EC20的模块状态讯息
typedef struct EC20_MESSAGE
{
    uint8_t manufacturerString[LENGTH_EC20_CODE_STRING];//模块厂家信息
    uint8_t moduleTypeString[LENGTH_EC20_CODE_STRING];//模块类型信息
    uint8_t softwareVersion[LENGTH_EC20_CODE_STRING];//软件版本讯息
    uint8_t imeiCodeString[LENGTH_EC20_CODE_STRING];//模块入网许可信息
    uint8_t imsiCodeString[LENGTH_EC20_CODE_STRING];//SIM卡IMSI串号讯息
    uint8_t iccidCodeString[LENGTH_EC20_CODE_STRING];//SIM卡ICCID讯息
    uint8_t fullNetWorkNameString[LENGTH_EC20_CODE_STRING];//网络全名
    uint8_t shortNetworkNameString[LENGTH_EC20_CODE_STRING];//网络短名
    uint8_t netServiceProviderNameString[LENGTH_EC20_CODE_STRING];//网络提供商名称
    uint8_t networkModeString[LENGTH_EC20_CODE_STRING];//网络制式
    uint8_t networkBandString[LENGTH_EC20_CODE_STRING];//网络制式选择的波段
    uint8_t baseStationCodeStringLAC[LENGTH_EC20_CODE_STRING];//基站位置区域码
    uint8_t baseStationCodeStringCID[LENGTH_EC20_CODE_STRING];//基站编号
    int singalStrength;//信号强度
    int errorValue;//误码率
    EC20_DEVICE_STA deviceState;//设备状态
    int32_t enterCurrentTimeStamp;
    int32_t currentStateTime;
}EC20_MESSAGE;


#endif


