/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-09-19 16:10:52 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-09-19 16:10:52 +0800
************************************************************************************************/ 
#include "DriverHeaderCore.h"
#include "DriverConfigCAN.h"

//CAN总线在不同的速率下的配置
//brp :波特率分频器.范围:1~511; tq = brp/PCANCLK = brp/PLLQ
//tsjw:重新同步跳跃时间单元.默认为1,取值范围1-127,计算默认取1
//tbs1:时间段1的时间单元.取值1-255
//tbs2:时间段2的时间单元,取值1-127
//波特率 = PLLQ/((tbs1+tbs2+1)*brp);
//根据CIA官方建议,(SJW+BS1)/(SJW+BS1+BS2)
// 速度 > 800K以上时候,采样点 75%  
// 速度 > 500K,采样点 80%
// 速度 <= 500K,采样点 87.5%
/*tqWithSJW*//*tqWithBS1*//*tqWithBS2*//*brpDiv*/
//位时间最大25个时间片,最小4个时间片,不能为了匹配采样点随便搞
/*----------------------------CAN仲裁段时序----------------------------------*/
//100下的时钟配置
const MCU_CAN_BPS_SETTING MCU_CAN_ArbitrationBaudArray[5] = {
    {5  /*1-128*/, 19  /*2-256*/, 5  /*2-128*/,    4   /*1-512*/  },//1000K 80%(19+1)/(19+1+5)=20/25=0.8
    {5  /*1-128*/, 19  /*2-256*/, 5  /*2-128*/,    8   /*1-512*/  },//500K 80%
    {3  /*1-128*/, 21  /*2-256*/, 3  /*2-128*/,    16  /*1-512*/  },//250K 88%
    {3  /*1-128*/, 21  /*2-256*/, 3  /*2-128*/,    32  /*1-512*/  },//125K 88%
    {3  /*1-128*/, 21  /*2-256*/, 3  /*2-128*/,    80  /*1-512*/  },//50K 88%
};

//100M下的时钟配置
const MCU_CAN_BPS_SETTING MCU_CAN_DataBaudArray[] = {
    {5  /*1-16*/,  14  /*1-32*/,  5  /*1-16*/,    1   /*1-32*/  },//5000K 70%(14+1)/(14+1+5)=0.75
    {6  /*1-16*/,  18  /*1-32*/,  6  /*1-16*/,    2   /*1-32*/  },//2000K 76%
    {7  /*1-128*/, 17  /*2-256*/, 7  /*2-128*/,   4   /*1-512*/ },//1000K 72%
    {5  /*1-128*/, 19  /*2-256*/, 5  /*2-128*/,   8   /*1-512*/ },//500K 80%
    {3  /*1-128*/, 21  /*2-256*/, 3  /*2-128*/,   16  /*1-512*/ },//250K 88%
    {3  /*1-128*/, 21  /*2-256*/, 3  /*2-128*/,   32  /*1-512*/ },//125K 88%
    {3  /*1-128*/, 21  /*2-256*/, 3  /*2-128*/,   80  /*1-512*/ },//50K 88%
};


//检验发送长度并转换为HAL库可以识别的字长
ERROR_SUB MCU_CAN_ConvertLengthToBytes(CAN_PACK_SRC packSrc,uint8_t length,uint32_t* dataLen)
{
    *dataLen = FDCAN_DLC_BYTES_0;
    switch(length)
    {
        case 0:
            *dataLen = FDCAN_DLC_BYTES_0;
            return ERROR_SUB_OK;
        case 1:
            *dataLen = FDCAN_DLC_BYTES_1;
            return ERROR_SUB_OK;
        case 2:
            *dataLen = FDCAN_DLC_BYTES_2;
            return ERROR_SUB_OK;
        case 3:
            *dataLen = FDCAN_DLC_BYTES_3;
            return ERROR_SUB_OK;
        case 4:
            *dataLen = FDCAN_DLC_BYTES_4;
            return ERROR_SUB_OK;
        case 5:
            *dataLen = FDCAN_DLC_BYTES_5;
            return ERROR_SUB_OK;
        case 6:
            *dataLen = FDCAN_DLC_BYTES_6;
            return ERROR_SUB_OK;
        case 7:
            *dataLen = FDCAN_DLC_BYTES_7;
            return ERROR_SUB_OK;
        case 8:
            *dataLen = FDCAN_DLC_BYTES_8;
            return ERROR_SUB_OK;
        case 12:
            *dataLen = FDCAN_DLC_BYTES_12;
            return ERROR_SUB_OK;
        case 16:
            *dataLen = FDCAN_DLC_BYTES_16;
            return ERROR_SUB_OK;
        case 20:
            *dataLen = FDCAN_DLC_BYTES_20;
            return ERROR_SUB_OK;
        case 24:
            *dataLen = FDCAN_DLC_BYTES_24;
            return ERROR_SUB_OK;
        case 32:
            *dataLen = FDCAN_DLC_BYTES_32;
            return ERROR_SUB_OK;
        case 48:
            *dataLen = FDCAN_DLC_BYTES_48;
            return ERROR_SUB_OK;
        case 64:
            *dataLen = FDCAN_DLC_BYTES_64;
            return ERROR_SUB_OK;
        default:
            *dataLen = FDCAN_DLC_BYTES_0;
            if(packSrc == CAN_PACK_SRC_1)
                return ERROR_SUB_MCU_CAN1_TX_LENGTH;
            else
                return ERROR_SUB_MCU_CAN2_TX_LENGTH;
    }
}

//将HAL库的数据长度转换为应用识别的数据长度
uint8_t MCU_CAN_ConvertBytesToLength(uint32_t dataLen)
{
    switch(dataLen)
    {
        case FDCAN_DLC_BYTES_0:
            return 0;
        case FDCAN_DLC_BYTES_1:
            return 1;
        case FDCAN_DLC_BYTES_2:
            return 2;
        case FDCAN_DLC_BYTES_3:
            return 3;
        case FDCAN_DLC_BYTES_4:
            return 4;
        case FDCAN_DLC_BYTES_5:
            return 5;
        case FDCAN_DLC_BYTES_6:
            return 6;
        case FDCAN_DLC_BYTES_7:
            return 7;
        case FDCAN_DLC_BYTES_8:
            return 8;
        case FDCAN_DLC_BYTES_12:
            return 12;
        case FDCAN_DLC_BYTES_16:
            return 16;
        case FDCAN_DLC_BYTES_20:
            return 20;
        case FDCAN_DLC_BYTES_24:
            return 24;
        case FDCAN_DLC_BYTES_32:
            return 32;
        case FDCAN_DLC_BYTES_48:
            return 48;
        case FDCAN_DLC_BYTES_64:
            return 64;
        default:
            return 0;
    }
}

static uint8_t fdcanResetFlag = 0;

void MCU_CAN_ClkReset(void)
{
    if(fdcanResetFlag == 0)
    {
        //使能FDCAN时钟
        __HAL_RCC_FDCAN_CLK_ENABLE();
        //复位FDCAN
        __HAL_RCC_FDCAN_FORCE_RESET();
        //结束复位
        __HAL_RCC_FDCAN_RELEASE_RESET();
        //只触发一次
        fdcanResetFlag = 1;
    }
}

