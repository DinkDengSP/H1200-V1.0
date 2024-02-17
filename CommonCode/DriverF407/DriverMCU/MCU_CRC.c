/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-08-23 09:44:56 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-08-23 14:02:31 +0800
************************************************************************************************/ 
#include "MCU_CRC.h"
#include "tx_mutex.h"
#include "DriverConfigMCU.h"
#include "MCU_LogShow.h"

//CRC句柄
static CRC_HandleTypeDef crcHandle;
//互斥信号量
static TX_MUTEX mutexCRC;

//获取互斥锁
void MCU_CRC_GetLock(void)
{
    //获取互斥信号量
    tx_mutex_get(&mutexCRC,TX_WAIT_FOREVER);
}

//释放互斥锁
void MCU_CRC_ReleaseLock(void)
{
    //释放互斥信号量
    tx_mutex_put(&mutexCRC);
}

//CRC初始化
void MCU_CRC_Init(void)
{
    HAL_StatusTypeDef halState = HAL_OK;
    //外设时钟使能
    __HAL_RCC_CRC_CLK_ENABLE();
    if(mutexCRC.tx_mutex_id != TX_MUTEX_ID)
    {
        tx_mutex_create(&mutexCRC,"mutexCRC",TX_INHERIT);
    }
    crcHandle.Instance = CRC;
    //先复位
    halState = HAL_CRC_DeInit(&crcHandle);
    if(halState != HAL_OK)
    {
        MCU_LogShowHalState(halState,0);
        while(1);
    }
    halState = HAL_CRC_Init(&crcHandle);
    if(halState != HAL_OK)
    {
        MCU_LogShowHalState(halState,0);
        while(1);
    }
}

//CRC计算,计算中会用到上一次的CRC值,可以用作多段式的CRC校验
uint32_t MCU_CRC_CalcWithLastResult(uint32_t* bufferPtr,uint32_t dataLength)
{
    uint32_t crcResult;
    //获取互斥信号量
    MCU_CRC_GetLock();
    //计算
    crcResult = HAL_CRC_Accumulate(&crcHandle,bufferPtr,dataLength);
    //释放互斥信号量
    MCU_CRC_ReleaseLock();
    return crcResult;
}

//CRC计算,计算中复位上一次的CRC值,单次CRC用这个函数
uint32_t MCU_CRC_CalcClearLastResult(uint32_t* bufferPtr,uint32_t dataLength)
{
    uint32_t crcResult;
    //获取互斥信号量
    MCU_CRC_GetLock();
    //计算
    crcResult = HAL_CRC_Calculate(&crcHandle,bufferPtr,dataLength);
    //释放互斥信号量
    MCU_CRC_ReleaseLock();
    return crcResult;
}

//CRC计算辅助
static uint32_t dwPolynomial = 0x04c11db7;
//软件CRC计算
uint32_t MCU_CRC_SoftCalcClearLastResult(uint32_t* bufferPtr,uint32_t dataLength)
{
    uint32_t xbit;
    uint32_t data;
    uint32_t crcCalcResult = 0xFFFFFFFF;
    //获取互斥信号量
    MCU_CRC_GetLock();
    while (dataLength--)
    {
        xbit = 0X01U << 31;
        data = *bufferPtr++;
        for (int bits = 0; bits < 32; bits++)
        {
            if (crcCalcResult & 0x80000000)
            {
                crcCalcResult <<= 1;
                crcCalcResult ^= dwPolynomial;
            }
            else
            {
                crcCalcResult <<= 1;
            }
            if (data & xbit)
            {
                crcCalcResult ^= dwPolynomial;
            }
            xbit >>= 1;
        }
    }
    //释放互斥信号量
    MCU_CRC_ReleaseLock();
    return crcCalcResult;
}

//crc测试用例
static const uint32_t crcTestDemo[] =
{
	0x00001021, 0x20423063, 0x408450a5, 0x60c670e7, 0x9129a14a, 0xb16bc18c,
	0xd1ade1ce, 0xf1ef1231, 0x32732252, 0x52b54294, 0x72f762d6, 0x93398318,
	0xa35ad3bd, 0xc39cf3ff, 0xe3de2462, 0x34430420, 0x64e674c7, 0x44a45485,
	0xa56ab54b, 0x85289509, 0xf5cfc5ac, 0xd58d3653, 0x26721611, 0x063076d7,
	0x569546b4, 0xb75ba77a, 0x97198738, 0xf7dfe7fe, 0xc7bc48c4, 0x58e56886,
	0x78a70840, 0x18612802, 0xc9ccd9ed, 0xe98ef9af, 0x89489969, 0xa90ab92b,
	0x4ad47ab7, 0x6a961a71, 0x0a503a33, 0x2a12dbfd, 0xfbbfeb9e, 0x9b798b58,
	0xbb3bab1a, 0x6ca67c87, 0x5cc52c22, 0x3c030c60, 0x1c41edae, 0xfd8fcdec,
	0xad2abd0b, 0x8d689d49, 0x7e976eb6, 0x5ed54ef4, 0x2e321e51, 0x0e70ff9f,
	0xefbedfdd, 0xcffcbf1b, 0x9f598f78, 0x918881a9, 0xb1caa1eb, 0xd10cc12d,
	0xe16f1080, 0x00a130c2, 0x20e35004, 0x40257046, 0x83b99398, 0xa3fbb3da,
	0xc33dd31c, 0xe37ff35e, 0x129022f3, 0x32d24235, 0x52146277, 0x7256b5ea,
	0x95a88589, 0xf56ee54f, 0xd52cc50d, 0x34e224c3, 0x04817466, 0x64475424,
	0x4405a7db, 0xb7fa8799, 0xe75ff77e, 0xc71dd73c, 0x26d336f2, 0x069116b0,
	0x76764615, 0x5634d94c, 0xc96df90e, 0xe92f99c8, 0xb98aa9ab, 0x58444865,
	0x78066827, 0x18c008e1, 0x28a3cb7d, 0xdb5ceb3f, 0xfb1e8bf9, 0x9bd8abbb,
	0x4a755a54, 0x6a377a16, 0x0af11ad0, 0x2ab33a92, 0xed0fdd6c, 0xcd4dbdaa,
	0xad8b9de8, 0x8dc97c26, 0x5c644c45, 0x3ca22c83, 0x1ce00cc1, 0xef1fff3e,
	0xdf7caf9b, 0xbfba8fd9, 0x9ff86e17, 0x7e364e55, 0x2e933eb2, 0x0ed11ef0
};
//数组长度
#define LENGTH_CRC_TEST_DEMO_ARRAY_LENGTH       DIM_ARRAY_ELEMENT_COUNT(crcTestDemo)

void MCU_CRC_TestDemoRunOnce(void)
{
    uint32_t crcResult1 = MCU_CRC_CalcClearLastResult((uint32_t*)crcTestDemo,LENGTH_CRC_TEST_DEMO_ARRAY_LENGTH);
    uint32_t crcResult2 = MCU_CRC_SoftCalcClearLastResult((uint32_t*)crcTestDemo,LENGTH_CRC_TEST_DEMO_ARRAY_LENGTH);
    MCU_LogPrintf("%s,HardResult: 0X%08X, SoftResult: 0X%08X\r\n",__func__,crcResult1,crcResult2);
}


//16位CRC计算
uint16_t MCU_Calc2ByteCRC(uint8_t* bufferPtr,uint16_t bufferLength)
{
    uint16_t jg   = 0xFFFF;
    uint16_t Poly = 0xA001;
    uint16_t temp;
    uint16_t flag;
    uint32_t i,n;
    //参数检查
    if (NULL == bufferPtr || 0 == bufferLength)
    {
        return 0xFFFF;
    }
    //计算
    for(i=0 ; i < bufferLength ; ++i)
    {
        jg ^= bufferPtr[i];
        for(n=0;n<8;++n)
        {
            temp = jg;
            jg = jg>>1;
            flag = 0x0001&temp;
            if(flag)
            {
                jg^=Poly;
            }
        }
    }
    return jg;
}

