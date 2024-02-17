/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-09-15 09:57:20 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-09-15 19:03:19 +0800
************************************************************************************************/ 
#ifndef __BOARD_IS25LP128F_CONFIG_H_
#define __BOARD_IS25LP128F_CONFIG_H_
#include "DriverHeaderMCU.h"
#include "DriverConfigBoard.h"


/*-------------------------------------------状态寄存器标记设定--------------------------------------------*/
//IS25LP128F状态寄存器集合
typedef union IS25LP128F_STATUS_REG
{
    uint8_t regValueAll;
    struct 
    {//位段
        //小端模式下least significant bits,这个是字节的最低位
        uint8_t busy        : 1;//设备状态忙,只能读取状态
        uint8_t wel         : 1;//设备写入使能
        uint8_t bp          : 4;//块保护
        uint8_t qe          : 1;//使能QPI模式
        //大端模式下most significant bits,这个是字节的最低位
        uint8_t srwd         : 1;//状态寄存器写入保护
    }regValueSingle;
}IS25LP128F_STATUS_REG;

//IS25LP128F方法寄存器
typedef union IS25LP128F_FUNC_REG
{
    uint8_t regValueAll;
    struct 
    {//位段
        //小端模式下least significant bits,这个是字节的最低位
        uint8_t reserve1    : 1;//
        uint8_t tbs         : 1;//块保护选择
        uint8_t psus        : 1;//编程挂起标志
        uint8_t esus        : 1;//擦除挂起标志
        //大端模式下most significant bits,这个是字节的最低位
        uint8_t irlock      : 4;//列编程设置
    }regValueSingle;
}IS25LP128F_FUNC_REG;


//状态集合
typedef struct IS25LP128F_STATUS_REG_SET
{
    IS25LP128F_STATUS_REG statusReg;
    IS25LP128F_FUNC_REG funcReg;
}IS25LP128F_STATUS_REG_SET;


/*------------------------------------------------SPI/DSPI/QSPI地址指令表---------------------------------*/
//写入使能
#define CMD_IS25LP128F_WRITE_ENABLE                          0X06
//写入禁止
#define CMD_IS25LP128F_WRITE_DISABLE                         0X04
//多用途指令,芯片处于掉电模式的时候,该指令用于释放掉电模式,芯片处于正常模式的时候,该指令用于读取ID
#define CMD_IS25LP128F_RELEASE_POWER_DOWN                    0XAB
//多用途指令,芯片处于掉电模式的时候,该指令用于释放掉电模式,芯片处于正常模式的时候,该指令用于读取ID
#define CMD_IS25LP128F_STD_DEVICE_ID                         0XAB
//读取制造商ID和设备ID
#define CMD_IS25LP128F_DEVICE_ID                             0X90
//JEDEC标准的设备ID指令,芯片有多个读取ID指令是因为系统存在兼容性需求
#define CMD_IS25LP128F_JEDEC_ID                              0X9F
//读取flash存储器的唯一编码,64BIT,8字节
#define CMD_IS25LP128F_READ_UNIQUE_ID                        0X4B
//数据读取,24BIT地址,只支持STD模式
#define CMD_IS25LP128F_READ_DATA                             0X03
//数据读取,设备将以尽可能快的速度读取,但是会在地址之后插入八个BIT的无效数据,需要主控滤除
#define CMD_IS25LP128F_FAST_READ                             0X0B
//页编程
#define CMD_IS25LP128F_PAGE_PROGRAM                          0X02
//扇区擦除,擦除之前必须设定写使能,去除写保护,完成之后写保护自动打开,下一次还需要写使能
#define CMD_IS25LP128F_SECTOR_ERASE                          0X20
//32KB的块擦除,擦除之前必须先设定写使能,完成之后写保护自动打开,下一次还需要写使能
#define CMD_IS25LP128F_BLOCK_ERASE_32KB                      0X52
//64KB的扇区擦除,擦除之前必须先设定写使能,完成之后写保护自动打开,下一次还需要写使能
#define CMD_IS25LP128F_BLOCK_ERASE_64KB                      0XD8
//芯片擦除,C7与60均可,擦除之前必须先设定写使能,完成之后写保护自动打开,下一次还需要写使能
#define CMD_IS25LP128F_CHIP_ERASE1                           0XC7
//芯片擦除,C7与60均可,擦除之前必须先设定写使能,完成之后写保护自动打开,下一次还需要写使能
#define CMD_IS25LP128F_CHIP_ERASE2                           0X60
//读取状态寄存器
#define CMD_IS25LP128F_READ_STATUS_REG                       0X05
//写入状态寄存器,先要确保设备已经写使能,发送写状态寄存器之前要确保0X50已经正确写入
//状态寄存器写入完成之后,WEL自动清零
#define CMD_IS25LP128F_WRITE_STATUS_REG                      0X01
//进入QPI模式
#define CMD_IS25LP128F_ENTER_QPI_MODE                        0X35
//退出QPI模式
#define CMD_IS25LP128F_EXIT_QPI_MODE                         0XF5
//读取状态寄存器2
#define CMD_IS25LP128F_READ_FUNC_REG                         0X48
//写入状态寄存器2
#define CMD_IS25LP128F_WRITE_FUNC_REG                        0X42
//读取256字节的SFDP参数表
#define CMD_IS25LP128F_READ_SFDP_REG                         0X5A
//暂停扇区和块的擦除,可以在状态寄存器的SUSP看到相应的标记位,可能会造成数据损失,芯片擦除不能暂停
#define CMD_IS25LP128F_ERASE_PROG_SUSPEND                    0X75
//恢复暂停指令暂停的扇区擦除和块擦除
#define CMD_IS25LP128F_ERASE_PROG_RESUME                     0X7A
//芯片进入掉电模式,最低功耗运行,进入该模式之后只会响应RELEASE_POWER_DOWN指令
#define CMD_IS25LP128F_POWER_DOWN                            0XB9
//设备复位,先进行复位使能,后进行复位,复位使能之后多数指令被禁用,设计复位序列是为了防止SPI传输数据异常
#define CMD_IS25LP128F_ENABLE_RESET                          0X66
//开始复位,最少需要30US来进行软件复位
#define CMD_IS25LP128F_RESET_DEVICE                          0X99







#endif





