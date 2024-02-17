/************************************************************************ 
 ** Author       : 邓小俊
 ** Date         : 2024-01-29 11:51:45
 ** Description  : When I Has Free Time ,I Will Write Description Below.
 ** description  : 
 ** LastEditors  : 邓小俊
 ** LastEditTime : 2024-01-29 14:26:29
 ** Email: xiaojun.deng@aliyun.com
 ** Copyright (c) 2024 by 公司名称, All Rights Reserved. 
 ************************************************************************/
#include "CoreDataType.h"

/*------------MPU组件区域设定,15优先级最高,优先级最低,高优先级覆盖低优先级----------------------*/
//按照优先级从高到低往下排,高优先级覆盖低优先级

//Region15 AXI SRAM 作为D1域的DMA缓存存在
//64KB
const MPU_REGION_CONFIG mpuRegionConfigNBR15 = {
    MPU_REGION_SET_ENABLE,                      //MPU区域功能设置
    0x24000000,                                 //MPU管理的地址起点
    MPU_REGION_LEN_64KB,                        //MPU管理的地址长度
    MPU_REGION_ACCESS_PERMISSIONS_FULL_ACCESS,  //MPU访问权限
    MPU_EXEC_SET_ENABLE,                        //该区域是否可以运行代码
    MPU_TEX_SET_LEVEL1,                         //该区域的Cache策略
    MPU_BUFFER_SET_DISABLE,                     //CACHE模式下是否使用缓冲
    MPU_CACHE_SET_DISABLE,                      //是否开启cache功能
    MPU_SHARE_SET_DISABLE,                      //是否多核共享内存区域
    0x00,                                       //子区域控制,一共八个,一般设置为0X00
};

//Region14 AXI SRAM 作为系统常规内存使用,打开了CACHE功能
//512-64=448KB
const MPU_REGION_CONFIG mpuRegionConfigNBR14 = {
    MPU_REGION_SET_ENABLE,                      //MPU区域功能设置
    0x24000000,                                 //MPU管理的地址起点
    MPU_REGION_LEN_512KB,                       //MMPU管理的地址长度
    MPU_REGION_ACCESS_PERMISSIONS_FULL_ACCESS,  //MPU访问权限
    MPU_EXEC_SET_ENABLE,                        //该区域是否可以运行代码
    MPU_TEX_SET_LEVEL1,                         //该区域的Cache策略
    MPU_BUFFER_SET_ENABLE,                      //CACHE模式下是否使用缓冲
    MPU_CACHE_SET_ENABLE,                       //是否开启cache功能
    MPU_SHARE_SET_DISABLE,                      //是否多核共享内存区域
    0x00,                                       //子区域控制,一共八个,一般设置为0X00
};

//Region13 配置D2_SRAM1作为系统常规内存使用,打开了CACHE功能
//128KB
const MPU_REGION_CONFIG mpuRegionConfigNBR13 = {
    MPU_REGION_SET_ENABLE,                      //MPU区域功能设置
    0x30000000,                                 //MPU管理的地址起点
    MPU_REGION_LEN_128KB,                        //MMPU管理的地址长度
    MPU_REGION_ACCESS_PERMISSIONS_FULL_ACCESS,  //MPU访问权限
    MPU_EXEC_SET_ENABLE,                        //该区域是否可以运行代码
    MPU_TEX_SET_LEVEL1,                         //该区域的Cache策略
    MPU_BUFFER_SET_ENABLE,                      //CACHE模式下是否使用缓冲
    MPU_CACHE_SET_ENABLE,                       //是否开启cache功能
    MPU_SHARE_SET_DISABLE,                      //是否多核共享内存区域
    0x00,                                       //子区域控制,一共八个,一般设置为0X00
};

//Region12 配置D2_SRAM2的整块属性都是关闭cache,供DMA1 2使用
//128KB
const MPU_REGION_CONFIG mpuRegionConfigNBR12 = {
    MPU_REGION_SET_ENABLE,                      //MPU区域功能设置
    0x30020000,                                 //MPU管理的地址起点
    MPU_REGION_LEN_128KB,                       //MMPU管理的地址长度
    MPU_REGION_ACCESS_PERMISSIONS_FULL_ACCESS,  //MPU访问权限
    MPU_EXEC_SET_ENABLE,                        //该区域是否可以运行代码
    MPU_TEX_SET_LEVEL1,                         //该区域的Cache策略
    MPU_BUFFER_SET_DISABLE,                     //CACHE模式下是否使用缓冲
    MPU_CACHE_SET_DISABLE,                      //是否开启cache功能
    MPU_SHARE_SET_DISABLE,                      //是否多核共享内存区域
    0x00,                                       //子区域控制,一共八个,一般设置为0X00
};

//Region11 配置D2_SRAM3的MPU属性,主要用于USB和网络,关闭cache,供DMA使用
//32KB
const MPU_REGION_CONFIG mpuRegionConfigNBR11 = {
    MPU_REGION_SET_ENABLE,                      //MPU区域功能设置
    0x30040000,                                 //MPU管理的地址起点
    MPU_REGION_LEN_32KB,                        //MMPU管理的地址长度
    MPU_REGION_ACCESS_PERMISSIONS_FULL_ACCESS,  //MPU访问权限
    MPU_EXEC_SET_ENABLE,                        //该区域是否可以运行代码
    MPU_TEX_SET_LEVEL0,                         //该区域的Cache策略
    MPU_BUFFER_SET_DISABLE,                     //CACHE模式下是否使用缓冲
    MPU_CACHE_SET_DISABLE,                      //是否开启cache功能
    MPU_SHARE_SET_DISABLE,                      //是否多核共享内存区域
    0x00,                                       //子区域控制,一共八个,一般设置为0X00
};

//Region10 配置D3_SRAM4的MPU DMA属性 64KB一半给DMA一个正常使用,D3域上有SPI和LPUART,BDMA
//32KB
const MPU_REGION_CONFIG mpuRegionConfigNBR10 = {
    MPU_REGION_SET_ENABLE,                      //MPU区域功能设置
    0x38000000,                                 //MPU管理的地址起点
    MPU_REGION_LEN_32KB,                        //MMPU管理的地址长度
    MPU_REGION_ACCESS_PERMISSIONS_FULL_ACCESS,  //MPU访问权限
    MPU_EXEC_SET_ENABLE,                        //该区域是否可以运行代码
    MPU_TEX_SET_LEVEL1,                         //该区域的Cache策略
    MPU_BUFFER_SET_DISABLE,                     //CACHE模式下是否使用缓冲
    MPU_CACHE_SET_DISABLE,                      //是否开启cache功能
    MPU_SHARE_SET_DISABLE,                      //是否多核共享内存区域
    0x00,                                       //子区域控制,一共八个,一般设置为0X00
};

//Region9 配置D3_SRAM4的MPU属性 32KB正常内存,有cache
//32KB
const MPU_REGION_CONFIG mpuRegionConfigNBR9 = {
    MPU_REGION_SET_ENABLE,                      //MPU区域功能设置
    0x38000000,                                 //MPU管理的地址起点
    MPU_REGION_LEN_64KB,                        //MMPU管理的地址长度
    MPU_REGION_ACCESS_PERMISSIONS_FULL_ACCESS,  //MPU访问权限
    MPU_EXEC_SET_ENABLE,                        //该区域是否可以运行代码
    MPU_TEX_SET_LEVEL1,                         //该区域的Cache策略
    MPU_BUFFER_SET_ENABLE,                      //CACHE模式下是否使用缓冲
    MPU_CACHE_SET_ENABLE,                       //是否开启cache功能
    MPU_SHARE_SET_DISABLE,                      //是否多核共享内存区域
    0x00,                                       //子区域控制,一共八个,一般设置为0X00
};

//Region8 BK_RAM,关闭cache
//4KB
static const MPU_REGION_CONFIG mpuRegionConfigNBR8 = {
    MPU_REGION_SET_ENABLE,                      //MPU区域功能设置
    0x38800000,                                 //MPU管理的地址起点
    MPU_REGION_LEN_4KB,                         //MMPU管理的地址长度
    MPU_REGION_ACCESS_PERMISSIONS_FULL_ACCESS,  //MPU访问权限
    MPU_EXEC_SET_ENABLE,                        //该区域是否可以运行代码
    MPU_TEX_SET_LEVEL1,                         //该区域的Cache策略
    MPU_BUFFER_SET_DISABLE,                     //CACHE模式下是否使用缓冲
    MPU_CACHE_SET_DISABLE,                      //是否开启cache功能
    MPU_SHARE_SET_DISABLE,                      //是否多核共享内存区域
    0x00,                                       //子区域控制,一共八个,一般设置为0X00
};

//Region7 QSPI Cache
//16MB
static const MPU_REGION_CONFIG mpuRegionConfigNBR7 = {
    MPU_REGION_SET_ENABLE,                      //MPU区域功能设置
    0X90000000,                                 //MPU管理的地址起点
    MPU_REGION_LEN_16MB,                        //MMPU管理的地址长度
    MPU_REGION_ACCESS_PERMISSIONS_FULL_ACCESS,  //MPU访问权限
    MPU_EXEC_SET_ENABLE,                        //该区域是否可以运行代码
    MPU_TEX_SET_LEVEL1,                         //该区域的Cache策略
    MPU_BUFFER_SET_ENABLE,                      //CACHE模式下是否使用缓冲
    MPU_CACHE_SET_ENABLE,                       //是否开启cache功能
    MPU_SHARE_SET_DISABLE,                      //是否多核共享内存区域
    0x00,                                       //子区域控制,一共八个,一般设置为0X00
};

//系统配置MPU区域6 SDRAM1的MPU属性,有cache
//32MB
const MPU_REGION_CONFIG mpuRegionConfigNBR6 = {
    MPU_REGION_SET_ENABLE,                      //MPU_REGION_FUNC_STATE MPU区域功能设置
    0XC0000000,                                 //uint32_t baseAddr;//MPU管理的地址起点
    MPU_REGION_LEN_32MB,                        //MPU_REGION_LEN MPU管理的地址长度
    MPU_REGION_ACCESS_PERMISSIONS_FULL_ACCESS,  //MPU_REGION_ACCESS_PERMISSIONS MPU访问权限
    MPU_EXEC_SET_ENABLE,                        //MPU_EXEC_SET 该区域是否可以运行代码
    MPU_TEX_SET_LEVEL1,                         //MPU_TEX_SET 该区域的Cache策略
    MPU_BUFFER_SET_ENABLE,                      //MPU_BUFFER_SET CACHE模式下是否使用缓冲
    MPU_CACHE_SET_ENABLE,                       //MPU_CACHE_SET 是否开启cache功能
    MPU_SHARE_SET_DISABLE,                      //MPU_SHARE_SET Cache模式下是否多核共享内存区域
    0x00,                                       //subRegionEnableConfig子区域控制,一共八个,一般设置为0X00
};

//系统配置MPU区域5 SDRAM2的MPU属性
//32MB
const MPU_REGION_CONFIG mpuRegionConfigNBR5 = {
    MPU_REGION_SET_ENABLE,                      //MPU_REGION_FUNC_STATE MPU区域功能设置
    0xD0000000,                                 //uint32_t baseAddr;//MPU管理的地址起点
    MPU_REGION_LEN_32MB,                        //MPU_REGION_LEN MPU管理的地址长度
    MPU_REGION_ACCESS_PERMISSIONS_FULL_ACCESS,  //MPU_REGION_ACCESS_PERMISSIONS MPU访问权限
    MPU_EXEC_SET_ENABLE,                        //MPU_EXEC_SET 该区域是否可以运行代码
    MPU_TEX_SET_LEVEL1,                         //MPU_TEX_SET 该区域的Cache策略
    MPU_BUFFER_SET_ENABLE,                      //MPU_BUFFER_SET CACHE模式下是否使用缓冲
    MPU_CACHE_SET_ENABLE,                       //MPU_CACHE_SET 是否开启cache功能
    MPU_SHARE_SET_DISABLE,                      //MPU_SHARE_SET Cache模式下是否多核共享内存区域
    0x00,                                       //subRegionEnableConfig子区域控制,一共八个,一般设置为0X00
};

//系统配置MPU区域4
const MPU_REGION_CONFIG mpuRegionConfigNBR4 = {
    MPU_REGION_SET_DISABLE,                 //MPU_REGION_FUNC_STATE MPU区域功能设置
    0x00000000,                               //uint32_t baseAddr;//MPU管理的地址起点
    MPU_REGION_LEN_4GB,                    //MPU_REGION_LEN MPU管理的地址长度
    MPU_REGION_ACCESS_PERMISSIONS_FULL_ACCESS,//MPU_REGION_ACCESS_PERMISSIONS MPU访问权限
    MPU_EXEC_SET_ENABLE,                    //MPU_EXEC_SET 该区域是否可以运行代码
    MPU_TEX_SET_LEVEL1,                       //MPU_TEX_SET 该区域的Cache策略
    MPU_BUFFER_SET_ENABLE,                  //MPU_BUFFER_SET CACHE模式下是否使用缓冲
    MPU_CACHE_SET_ENABLE,                   //MPU_CACHE_SET 是否开启cache功能
    MPU_SHARE_SET_DISABLE,                  //MPU_SHARE_SET Cache模式下是否多核共享内存区域
    0x00,                                     //subRegionEnableConfig子区域控制,一共八个,一般设置为0X00
};

//系统配置MPU区域3
const MPU_REGION_CONFIG mpuRegionConfigNBR3 = {
    MPU_REGION_SET_DISABLE,                 //MPU_REGION_FUNC_STATE MPU区域功能设置
    0x00000000,                               //uint32_t baseAddr;//MPU管理的地址起点
    MPU_REGION_LEN_4GB,                    //MPU_REGION_LEN MPU管理的地址长度
    MPU_REGION_ACCESS_PERMISSIONS_FULL_ACCESS,//MPU_REGION_ACCESS_PERMISSIONS MPU访问权限
    MPU_EXEC_SET_ENABLE,                    //MPU_EXEC_SET 该区域是否可以运行代码
    MPU_TEX_SET_LEVEL1,                       //MPU_TEX_SET 该区域的Cache策略
    MPU_BUFFER_SET_ENABLE,                  //MPU_BUFFER_SET CACHE模式下是否使用缓冲
    MPU_CACHE_SET_ENABLE,                   //MPU_CACHE_SET 是否开启cache功能
    MPU_SHARE_SET_DISABLE,                  //MPU_SHARE_SET Cache模式下是否多核共享内存区域
    0x00,                                     //subRegionEnableConfig子区域控制,一共八个,一般设置为0X00
};

//系统配置MPU区域2
const MPU_REGION_CONFIG mpuRegionConfigNBR2 = {
    MPU_REGION_SET_DISABLE,                 //MPU_REGION_FUNC_STATE MPU区域功能设置
    0x00000000,                               //uint32_t baseAddr;//MPU管理的地址起点
    MPU_REGION_LEN_4GB,                    //MPU_REGION_LEN MPU管理的地址长度
    MPU_REGION_ACCESS_PERMISSIONS_FULL_ACCESS,//MPU_REGION_ACCESS_PERMISSIONS MPU访问权限
    MPU_EXEC_SET_ENABLE,                    //MPU_EXEC_SET 该区域是否可以运行代码
    MPU_TEX_SET_LEVEL1,                       //MPU_TEX_SET 该区域的Cache策略
    MPU_BUFFER_SET_ENABLE,                  //MPU_BUFFER_SET CACHE模式下是否使用缓冲
    MPU_CACHE_SET_ENABLE,                   //MPU_CACHE_SET 是否开启cache功能
    MPU_SHARE_SET_DISABLE,                  //MPU_SHARE_SET Cache模式下是否多核共享内存区域
    0x00,                                     //subRegionEnableConfig子区域控制,一共八个,一般设置为0X00
};

//系统配置MPU区域1
const MPU_REGION_CONFIG mpuRegionConfigNBR1 = {
    MPU_REGION_SET_DISABLE,                 //MPU_REGION_FUNC_STATE MPU区域功能设置
    0x00000000,                               //uint32_t baseAddr;//MPU管理的地址起点
    MPU_REGION_LEN_4GB,                    //MPU_REGION_LEN MPU管理的地址长度
    MPU_REGION_ACCESS_PERMISSIONS_FULL_ACCESS,//MPU_REGION_ACCESS_PERMISSIONS MPU访问权限
    MPU_EXEC_SET_ENABLE,                    //MPU_EXEC_SET 该区域是否可以运行代码
    MPU_TEX_SET_LEVEL1,                       //MPU_TEX_SET 该区域的Cache策略
    MPU_BUFFER_SET_ENABLE,                  //MPU_BUFFER_SET CACHE模式下是否使用缓冲
    MPU_CACHE_SET_ENABLE,                   //MPU_CACHE_SET 是否开启cache功能
    MPU_SHARE_SET_DISABLE,                  //MPU_SHARE_SET Cache模式下是否多核共享内存区域
    0x00,                                     //subRegionEnableConfig子区域控制,一共八个,一般设置为0X00
};

//系统配置MPU区域0
const MPU_REGION_CONFIG mpuRegionConfigNBR0 = {
    MPU_REGION_SET_DISABLE,                 //MPU_REGION_FUNC_STATE MPU区域功能设置
    0x00000000,                               //uint32_t baseAddr;//MPU管理的地址起点
    MPU_REGION_LEN_4GB,                    //MPU_REGION_LEN MPU管理的地址长度
    MPU_REGION_ACCESS_PERMISSIONS_FULL_ACCESS,//MPU_REGION_ACCESS_PERMISSIONS MPU访问权限
    MPU_EXEC_SET_ENABLE,                    //MPU_EXEC_SET 该区域是否可以运行代码
    MPU_TEX_SET_LEVEL1,                       //MPU_TEX_SET 该区域的Cache策略
    MPU_BUFFER_SET_ENABLE,                  //MPU_BUFFER_SET CACHE模式下是否使用缓冲
    MPU_CACHE_SET_ENABLE,                   //MPU_CACHE_SET 是否开启cache功能
    MPU_SHARE_SET_DISABLE,                  //MPU_SHARE_SET Cache模式下是否多核共享内存区域
    0x00,                                     //subRegionEnableConfig子区域控制,一共八个,一般设置为0X00
};



//MPU管理数组
MPU_REGION_CONFIG* mpuRegionConfigPtrArray[MPU_REGION_COUNT] = {
    (MPU_REGION_CONFIG*)&mpuRegionConfigNBR0,
    (MPU_REGION_CONFIG*)&mpuRegionConfigNBR1,
    (MPU_REGION_CONFIG*)&mpuRegionConfigNBR2,
    (MPU_REGION_CONFIG*)&mpuRegionConfigNBR3,
    (MPU_REGION_CONFIG*)&mpuRegionConfigNBR4,
    (MPU_REGION_CONFIG*)&mpuRegionConfigNBR5,
    (MPU_REGION_CONFIG*)&mpuRegionConfigNBR6,
    (MPU_REGION_CONFIG*)&mpuRegionConfigNBR7,
    (MPU_REGION_CONFIG*)&mpuRegionConfigNBR8,
    (MPU_REGION_CONFIG*)&mpuRegionConfigNBR9,
    (MPU_REGION_CONFIG*)&mpuRegionConfigNBR10,
    (MPU_REGION_CONFIG*)&mpuRegionConfigNBR11,
    (MPU_REGION_CONFIG*)&mpuRegionConfigNBR12,
    (MPU_REGION_CONFIG*)&mpuRegionConfigNBR13,
    (MPU_REGION_CONFIG*)&mpuRegionConfigNBR14,
    (MPU_REGION_CONFIG*)&mpuRegionConfigNBR15,
};




