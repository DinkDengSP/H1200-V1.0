/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-11-03 21:00:23 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-11-06 17:01:05 +0800
************************************************************************************************/ 
#include "MCU_ETH.h"
#include "MCU_PIN.h"
#include "UserMemManage.h"
#include "MCU_LogShow.h"
#include "DriverConfigMCU.h"
#include "MCU_ChipID.h"

//MAC数据包格式
//7字节前导字段,前导字段，也称报头,这是一段方波,用于使收发节点的时钟同步.内容为连续 7 个字节的0x55。字段和帧起始定界符在MAC 收到数据包后会自动过滤掉
//1字节帧起始定界符,用于区分前导段与数据段的,内容为0xD5,接收时被MAC滤除.
//6字节的目标地址DA 可以是组播,单播,广播,MAC 地址由 48 位数字组成，它是网卡的物理地址
//6字节源地址,MAC 地址由 48 位数字组成，它是网卡的物理地址

//2字节数据包类型或者长度,区域可以用来描述本MAC 数据包是属于TCP/IP 协议层的IP 包,ARP 包还是SNMP包,也可以用来描述本MAC数据包数据段的长度,如果该值被设置大于0x0600，
//不用于长度描述.而是用于类型描述功能，表示与以太网帧相关的 MAC 客户端协议的种类

//46-1500的数据包有效负载(可选填充字段),它包含的数据来自 MAC 的上层。其长度可以从0~1500 字节间变化.协议要求整个 MAC 数据包的长度至少为64字节(接收到的数据包如果少于
//64字节会被认为发生冲突，数据包被自动丢弃)，当数据段的字节少于 46 字节时，在填充域会自动填上无效数据，以使数据包符合长度要求。

//4字节的帧校验序列FCS,保存了CRC 校验序列，用于检错,发送FCS的时候,先发送bit31,最后发送bit0

//以上是标准的MAC数据包，IEEE 802.3 同时还规定了扩展的MAC数据包，它是在标准的MAC数据包的 SA 和数据
//包类型之间添加4个字节的 QTag前缀字段，用于获取标志的 MAC帧。前2个字节固定为0x8100，用于识别QTag
//前缀的存在；后两个字节内容分别为 3 个位的用户优先级、1个位的标准格式指示符(CFI) 和一个12位的VLAN标识符

//ETH_DMADescTypeDef
//这个结构体的本质就是4个32Bit的变量，和STM32参考手册中定义的一样,就是告诉MAC层DMA,要发送和接受的数据放在那里
//DESC0 主要用来表示描述符的状态和控制信息
//这个使用权的控制，就通过DESC0寄存器中的OWN位体现出来，对应到发送描述符结构体就是DESC0变量的最高位
//DESC1 表示该描述符缓冲区数据的有效长度
//DESC2 表示描述符缓冲区的地址，我们要发送的数据，就是放在这个地址所指向的内存中

//Ethernet Rx DMA 描述符,一个描述符是24字节,
static ETH_DMADescTypeDef  DMARxDscrTab[ETH_RX_DESC_CNT]@".d2_sram3_dma";
//Ethernet Tx DMA 描述符,一个描述符是24字节
static ETH_DMADescTypeDef  DMATxDscrTab[ETH_TX_DESC_CNT]@".d2_sram3_dma";


#pragma pack(push) //保存对齐状态
#pragma pack(4)//设定为4字节对齐
//以太网接收缓冲区
static uint8_t ethRecvBufferArray[ETH_RX_DESC_CNT][ETH_MAX_PACKET_SIZE]@".d2_sram3_dma";  
//以太网发送缓冲区
static uint8_t ethSendBufferArray[ETH_TX_DESC_CNT][ETH_MAX_PACKET_SIZE]@".d2_sram3_dma";  
//以太网发送描述符
static ETH_BufferTypeDef ethSendPackBuffer[ETH_TX_DESC_CNT]@".d2_sram3_dma";
#pragma pack(pop) //保存对齐状态


//数据发送配置
static ETH_TxPacketConfig ethTxConfig; 
//MAC层的配置讯息
static ETH_MACConfigTypeDef MACConfig;
//MAC过滤层的信息
ETH_MACFilterConfigTypeDef ethFilterConfig;

//MAC地址数组
static uint8_t ethMacArray[6] = {0};
//以太网配置
ETH_HandleTypeDef ETH_Handler = {0};
//发送信号量
TX_SEMAPHORE semaphoreMCU_ETH = {0};

//释放接收到的内存
void MCU_ETH_ReleaseRecvBuf(uint8_t* recvBufPtr)
{
    if(recvBufPtr == NULL)
    {
        return;
    }
    UserMemFree(MCU_ETH_MEM_REGION,recvBufPtr);
}

//读取PHY 寄存器数据
ERROR_SUB MCU_ETH_ReadPhyReg(uint32_t phyAddr, uint32_t regAddr, uint32_t *readValuePtr)
{
    HAL_StatusTypeDef halState = HAL_OK;
    halState = HAL_ETH_ReadPHYRegister(&ETH_Handler, phyAddr, regAddr, readValuePtr);
    if(halState != HAL_OK)
    {
        MCU_LogShowHalState(halState,ETH_Handler.ErrorCode);
        return ERROR_SUB_MCU_ETH_READ_PHY_REG;
    }
    return ERROR_SUB_OK;
}

//写入PHY 寄存器数据
ERROR_SUB MCU_ETH_WritePhyReg(uint32_t phyAddr, uint32_t regAddr, uint32_t writeValue)
{
    HAL_StatusTypeDef halState = HAL_OK;
    halState = HAL_ETH_WritePHYRegister(&ETH_Handler, phyAddr, regAddr, writeValue);
    if(halState != HAL_OK)
    {
        MCU_LogShowHalState(halState,ETH_Handler.ErrorCode);
        return ERROR_SUB_MCU_ETH_WRITE_PHY_REG;
    }
    return ERROR_SUB_OK;
}


//根据网卡连接状态修改MAC状态
void MCU_ETH_ReflushModeSpeed(NET_PORT_STATE netPortState)
{
    uint32_t speed;                           
    uint32_t duplexMode;
    HAL_StatusTypeDef halState = HAL_OK;
    //获取现在的MAC配置
    HAL_ETH_GetMACConfig(&ETH_Handler, &MACConfig); 
    if(halState != HAL_OK)
    {
        MCU_LogShowHalState(halState,ETH_Handler.ErrorCode);
        while(1);
    }
    switch(netPortState)
    {
        case NET_PORT_STATE_100MBITS_FULLDUPLEX:
            speed = ETH_SPEED_100M;
            duplexMode = ETH_FULLDUPLEX_MODE;
            break;
        case NET_PORT_STATE_100MBITS_HALFDUPLEX:
            speed = ETH_SPEED_100M;
            duplexMode = ETH_HALFDUPLEX_MODE;
            break;
        case NET_PORT_STATE_10MBITS_FULLDUPLEX:
            speed = ETH_SPEED_10M;
            duplexMode = ETH_FULLDUPLEX_MODE;
            break;
        case NET_PORT_STATE_10MBITS_HALFDUPLEX:
            speed = ETH_SPEED_10M;
            duplexMode = ETH_HALFDUPLEX_MODE;
            break;
        default:
            speed = PHY_SPEED_ETH_INVALID;
            duplexMode = PHY_DUPLEX_MODE_ETH_INVALID;
            break;
    }
    //如果连接状态不支持,不更新MAC层
    if((speed == PHY_SPEED_ETH_INVALID)||(duplexMode == PHY_DUPLEX_MODE_ETH_INVALID))
    {
        return;
    }
    //链接状态变化了才配置
    if((MACConfig.DuplexMode != duplexMode)||(MACConfig.Speed != speed))
    {
        //先关闭接收
        halState = HAL_ETH_Stop_IT(&ETH_Handler);
        if(halState != HAL_OK)
        {
            MCU_LogShowHalState(halState,ETH_Handler.ErrorCode);
            while(1);
        }
        //设置MAC状态
        MACConfig.DuplexMode = duplexMode;
        MACConfig.Speed = speed;
        halState = HAL_ETH_SetMACConfig(&ETH_Handler, &MACConfig);
        if(halState != HAL_OK)
        {
            MCU_LogShowHalState(halState,ETH_Handler.ErrorCode);
            while(1);
        }
        //重新打开接收
        halState = HAL_ETH_Start_IT(&ETH_Handler);
        if(halState != HAL_OK)
        {
            MCU_LogShowHalState(halState,ETH_Handler.ErrorCode);
            while(1);
        }
    }
}


//使能多播
void MCU_ETH_EnableMultiCast(void)
{
    HAL_StatusTypeDef halState = HAL_OK;
    ethFilterConfig.PromiscuousMode = DISABLE;
    ethFilterConfig.HashUnicast = DISABLE;
    ethFilterConfig.HashMulticast = DISABLE;
    ethFilterConfig.DestAddrInverseFiltering = DISABLE;
    //通过多播包
    ethFilterConfig.PassAllMulticast = ENABLE;
    //通过广播包
    ethFilterConfig.BroadcastFilter = ENABLE;
    ethFilterConfig.SrcAddrInverseFiltering = DISABLE;
    ethFilterConfig.SrcAddrFiltering = DISABLE;
    ethFilterConfig.HachOrPerfectFilter = DISABLE;
    ethFilterConfig.ReceiveAllMode = DISABLE;
    ethFilterConfig.ControlPacketsFilter = 0x00;
    halState = HAL_ETH_SetMACFilterConfig(&ETH_Handler, &ethFilterConfig);
    if(halState != HAL_OK)
    {
        MCU_LogShowHalState(halState,ETH_Handler.ErrorCode);
        while(1);
    }
}

//禁止多播
void MCU_ETH_DisableMultiCast(void)
{
    HAL_StatusTypeDef halState = HAL_OK;
    ethFilterConfig.PromiscuousMode = DISABLE;
    ethFilterConfig.HashUnicast = DISABLE;
    ethFilterConfig.HashMulticast = DISABLE;
    ethFilterConfig.DestAddrInverseFiltering = DISABLE;
    //过滤多播包
    ethFilterConfig.PassAllMulticast = DISABLE;
    //通过广播包
    ethFilterConfig.BroadcastFilter = ENABLE;
    ethFilterConfig.SrcAddrInverseFiltering = DISABLE;
    ethFilterConfig.SrcAddrFiltering = DISABLE;
    ethFilterConfig.HachOrPerfectFilter = DISABLE;
    ethFilterConfig.ReceiveAllMode = DISABLE;
    ethFilterConfig.ControlPacketsFilter = 0x00;
    halState = HAL_ETH_SetMACFilterConfig(&ETH_Handler, &ethFilterConfig);
    if(halState != HAL_OK)
    {
        MCU_LogShowHalState(halState,ETH_Handler.ErrorCode);
        while(1);
    }
}

//eth端口复位
void HAL_ETH_MspDeInit(ETH_HandleTypeDef *heth)
{
    if(heth->Instance == ETH)
    {
        HAL_ETH_Stop(&ETH_Handler);
        //关闭外设时钟
        __HAL_RCC_ETH1MAC_CLK_DISABLE();
        __HAL_RCC_ETH1TX_CLK_DISABLE();
        __HAL_RCC_ETH1RX_CLK_DISABLE();
        //端口状态复位
        MCU_PinDeInit(MCU_ETH_MII_RX_CLK);
        MCU_PinDeInit(MCU_ETH_MII_RXD3);
        MCU_PinDeInit(MCU_ETH_MII_RXD2);
        MCU_PinDeInit(MCU_ETH_MII_RXD1);
        MCU_PinDeInit(MCU_ETH_MII_RXD0);
        MCU_PinDeInit(MCU_ETH_MII_RX_DV);
        MCU_PinDeInit(MCU_ETH_MII_RX_ER);
        MCU_PinDeInit(MCU_ETH_MII_TX_CLK);
        MCU_PinDeInit(MCU_ETH_MII_TX_EN);
        MCU_PinDeInit(MCU_ETH_MII_TXD0);
        MCU_PinDeInit(MCU_ETH_MII_TXD1);
        MCU_PinDeInit(MCU_ETH_MII_TXD2);
        MCU_PinDeInit(MCU_ETH_MII_TXD3);
        MCU_PinDeInit(MCU_ETH_MII_MDIO);
        MCU_PinDeInit(MCU_ETH_MII_MDC);
        MCU_PinDeInit(MCU_ETH_MII_CRS);
        MCU_PinDeInit(MCU_ETH_MII_COL);
        //关闭ETH中断
        HAL_NVIC_DisableIRQ(ETH_IRQn);	
    }
}

//ETH端口初始化
void HAL_ETH_MspInit(ETH_HandleTypeDef *heth)
{ 
    if(heth->Instance == ETH)
    {
        /* 使能外设时钟 */
        __HAL_RCC_ETH1MAC_CLK_ENABLE();
        __HAL_RCC_ETH1TX_CLK_ENABLE();
        __HAL_RCC_ETH1RX_CLK_ENABLE();
        //MII模式
        HAL_SYSCFG_AnalogSwitchConfig(SYSCFG_SWITCH_PA0, SYSCFG_SWITCH_PA0_CLOSE);
        HAL_SYSCFG_AnalogSwitchConfig(SYSCFG_SWITCH_PA1, SYSCFG_SWITCH_PA1_CLOSE);
        HAL_SYSCFG_AnalogSwitchConfig(SYSCFG_SWITCH_PC2, SYSCFG_SWITCH_PC2_CLOSE);
        HAL_SYSCFG_AnalogSwitchConfig(SYSCFG_SWITCH_PC3, SYSCFG_SWITCH_PC3_CLOSE);
        //端口初始化
        MCU_PinInit(MCU_ETH_MII_RX_CLK ,MCU_PIN_MODE_AF_PP,MCU_PIN_PULL_NOPULL,MCU_PIN_SPEED_FREQ_MEDIUM,GPIO_AF11_ETH);
        MCU_PinInit(MCU_ETH_MII_RXD3   ,MCU_PIN_MODE_AF_PP,MCU_PIN_PULL_NOPULL,MCU_PIN_SPEED_FREQ_MEDIUM,GPIO_AF11_ETH);
        MCU_PinInit(MCU_ETH_MII_RXD2   ,MCU_PIN_MODE_AF_PP,MCU_PIN_PULL_NOPULL,MCU_PIN_SPEED_FREQ_MEDIUM,GPIO_AF11_ETH);
        MCU_PinInit(MCU_ETH_MII_RXD1   ,MCU_PIN_MODE_AF_PP,MCU_PIN_PULL_NOPULL,MCU_PIN_SPEED_FREQ_MEDIUM,GPIO_AF11_ETH);
        MCU_PinInit(MCU_ETH_MII_RXD0   ,MCU_PIN_MODE_AF_PP,MCU_PIN_PULL_NOPULL,MCU_PIN_SPEED_FREQ_MEDIUM,GPIO_AF11_ETH);
        MCU_PinInit(MCU_ETH_MII_RX_DV  ,MCU_PIN_MODE_AF_PP,MCU_PIN_PULL_NOPULL,MCU_PIN_SPEED_FREQ_MEDIUM,GPIO_AF11_ETH);
        MCU_PinInit(MCU_ETH_MII_RX_ER  ,MCU_PIN_MODE_AF_PP,MCU_PIN_PULL_NOPULL,MCU_PIN_SPEED_FREQ_MEDIUM,GPIO_AF11_ETH);
        MCU_PinInit(MCU_ETH_MII_TX_CLK ,MCU_PIN_MODE_AF_PP,MCU_PIN_PULL_NOPULL,MCU_PIN_SPEED_FREQ_MEDIUM,GPIO_AF11_ETH);
        MCU_PinInit(MCU_ETH_MII_TX_EN  ,MCU_PIN_MODE_AF_PP,MCU_PIN_PULL_NOPULL,MCU_PIN_SPEED_FREQ_MEDIUM,GPIO_AF11_ETH);
        MCU_PinInit(MCU_ETH_MII_TXD0   ,MCU_PIN_MODE_AF_PP,MCU_PIN_PULL_NOPULL,MCU_PIN_SPEED_FREQ_MEDIUM,GPIO_AF11_ETH);
        MCU_PinInit(MCU_ETH_MII_TXD1   ,MCU_PIN_MODE_AF_PP,MCU_PIN_PULL_NOPULL,MCU_PIN_SPEED_FREQ_MEDIUM,GPIO_AF11_ETH);
        MCU_PinInit(MCU_ETH_MII_TXD2   ,MCU_PIN_MODE_AF_PP,MCU_PIN_PULL_NOPULL,MCU_PIN_SPEED_FREQ_MEDIUM,GPIO_AF11_ETH);
        MCU_PinInit(MCU_ETH_MII_TXD3   ,MCU_PIN_MODE_AF_PP,MCU_PIN_PULL_NOPULL,MCU_PIN_SPEED_FREQ_MEDIUM,GPIO_AF11_ETH);
        MCU_PinInit(MCU_ETH_MII_MDIO   ,MCU_PIN_MODE_AF_PP,MCU_PIN_PULL_NOPULL,MCU_PIN_SPEED_FREQ_MEDIUM,GPIO_AF11_ETH);
        MCU_PinInit(MCU_ETH_MII_MDC    ,MCU_PIN_MODE_AF_PP,MCU_PIN_PULL_NOPULL,MCU_PIN_SPEED_FREQ_MEDIUM,GPIO_AF11_ETH);
        MCU_PinInit(MCU_ETH_MII_CRS    ,MCU_PIN_MODE_AF_PP,MCU_PIN_PULL_NOPULL,MCU_PIN_SPEED_FREQ_MEDIUM,GPIO_AF11_ETH);
        MCU_PinInit(MCU_ETH_MII_COL    ,MCU_PIN_MODE_AF_PP,MCU_PIN_PULL_NOPULL,MCU_PIN_SPEED_FREQ_MEDIUM,GPIO_AF11_ETH);
        //清除中断标记
        HAL_NVIC_ClearPendingIRQ(ETH_IRQn);
        //关闭ETH中断
        HAL_NVIC_DisableIRQ(ETH_IRQn);	
        //设置中断优先级
        HAL_NVIC_SetPriority(ETH_IRQn, MCU_ETH_PREE_PRI, MCU_ETH_SUB_PRI);
    }
}

//获取MAC层MAC地址指针
uint8_t* MCU_ETH_GetMacAddrPtr(void)
{
    return (uint8_t*)ethMacArray;
}

//打开ETH中断
void MCU_ETH_EnableInt(void)
{
    //打开ETH中断
    HAL_NVIC_EnableIRQ(ETH_IRQn);
}

//ETH端口初始化
void MCU_ETH_PortInit(void)
{
    HAL_StatusTypeDef halState = HAL_OK;
    uint8_t indexUtil = 0;
    //计算MAC地址
    MCU_CHIP_ID chipID;
    MCU_ChipID_Read(&chipID);
    uint8_t macAddr0 = (uint8_t)(chipID.idSn0>>24);
    macAddr0 += (uint8_t)(chipID.idSn0>>16);
    if(macAddr0%2 != 0)
    {
        macAddr0 += 1;
    } 
    if(macAddr0 == 0XFF)macAddr0 = 0xFE;
    uint8_t macAddr1 = (uint8_t)(chipID.idSn0>>8);
    macAddr1 += (uint8_t)(chipID.idSn0>>0);
    uint8_t macAddr2 = (uint8_t)(chipID.idSn1>>24);
    macAddr2 += (uint8_t)(chipID.idSn1>>16);
    uint8_t macAddr3 = (uint8_t)(chipID.idSn1>>8);
    macAddr3 += (uint8_t)(chipID.idSn1>>0);
    uint8_t macAddr4 = (uint8_t)(chipID.idSn2>>24);
    macAddr4 += (uint8_t)(chipID.idSn2>>16);
    uint8_t macAddr5 = (uint8_t)(chipID.idSn2>>8);
    macAddr5 += (uint8_t)(chipID.idSn2>>0);
    //创建信号量
    if(ethMacArray[0] != macAddr0)
    {
        tx_semaphore_create(&semaphoreMCU_ETH,"semaphoreMCU_ETH",1);
    }
    //外设初始化
    ETH_Handler.Instance = ETH;
    //首先ETH复位
    halState = HAL_ETH_DeInit(&ETH_Handler);
    if(halState != HAL_OK)
    {
        MCU_LogShowHalState(halState,ETH_Handler.ErrorCode);
        while(1);
    }
    //设置MAC地址,使用数组存放MAC地址
    ethMacArray[0] = macAddr0;
    ethMacArray[1] = macAddr1;
    ethMacArray[2] = macAddr2;
    ethMacArray[3] = macAddr3;
    ethMacArray[4] = macAddr4;
    ethMacArray[5] = macAddr5;
    //地址接入
    ETH_Handler.Init.MACAddr = ethMacArray;
    //选择RMII模式
    ETH_Handler.Init.MediaInterface = HAL_ETH_MII_MODE;
    //DMA发送描述符
    ETH_Handler.Init.TxDesc = DMATxDscrTab;
    //DMA接收描述符
    ETH_Handler.Init.RxDesc = DMARxDscrTab;
    //接收缓存
    ETH_Handler.Init.RxBuffLen = ETH_MAX_PACKET_SIZE;
    //等待ETH初始化完成
    while(HAL_ETH_Init(&ETH_Handler) != HAL_OK);
    //建立接受描述符和接收缓存之间的关系
    for(indexUtil = 0; indexUtil < ETH_RX_DESC_CNT; indexUtil++)
    {
        halState = HAL_ETH_DescAssignMemory(&ETH_Handler, indexUtil, ethRecvBufferArray[indexUtil], NULL);
        if(halState != HAL_OK)
        {
            MCU_LogShowHalState(halState,ETH_Handler.ErrorCode);
            while(1);
        }
    }
    //建立描述符
    for(indexUtil = 0; indexUtil < ETH_TX_DESC_CNT; indexUtil++)
    {
        DMATxDscrTab[indexUtil].DESC2 = (uint32_t)1<<31;
        DMATxDscrTab[indexUtil].DESC3 = 0;
    }
     /* 设置发送配置结构体 */
    UserMemSet((uint8_t*)&ethTxConfig, 0 , sizeof(ETH_TxPacketConfig));
    //发送配置,计算校验和,插入填充位和校验和
    ethTxConfig.Attributes = ETH_TX_PACKETS_FEATURES_CSUM | ETH_TX_PACKETS_FEATURES_CRCPAD;
    //使能CRC和填充位的插入
    ethTxConfig.CRCPadCtrl = ETH_CRC_PAD_INSERT;
    //禁止插入校验和
    ethTxConfig.ChecksumCtrl = ETH_CHECKSUM_IPHDR_PAYLOAD_INSERT_PHDR_CALC;
    //关闭多播
    MCU_ETH_DisableMultiCast();
    //打开MDIO时钟输出
    HAL_ETH_SetMDIOClockRange(&ETH_Handler);
    //显示MAC地址
    MCU_LogPrintf("MAC: %02X.%02X.%02X.%02X.%02X.%02X\r\n",macAddr0,macAddr1,macAddr2,macAddr3,macAddr4,macAddr5);
}

//读取网卡接收到的数据
void MCU_ETH_ReadRecvData(uint8_t** recvBuf,uint32_t* length)
{
    HAL_StatusTypeDef halState = HAL_OK;
    uint16_t indexUtilMainBlock = 0;
    uint16_t indexUtilSubArray = 0;
    //首先设置数据为空
    *recvBuf = NULL;
    *length = 0;
    //首先查看是否存在数据包等待接收
    if(0 == HAL_ETH_IsRxDataAvailable(&ETH_Handler))
    {
        return;
    }
    //读取接收到的数据长度
    halState = HAL_ETH_GetRxDataLength(&ETH_Handler,length);
    if(halState != HAL_OK)
    {
        MCU_LogShowHalState(halState,ETH_Handler.ErrorCode);
        while(1);
    }
    if(*length == 0)
    {
        return;
    }
    //现在根据Length申请内存
    *recvBuf = UserMemMallocWhileSuccess(MCU_ETH_MEM_REGION,*length);
    uint16_t recvDataPackCount = 0;
    //长度决定需要几包
    recvDataPackCount = ((*length)/(ETH_Handler.Init.RxBuffLen))+1;
    //根据初始化的时候的最大长度决定需要申请多少个RxBuff
    ETH_BufferTypeDef* recvDataPackPtr = UserMemMallocWhileSuccess(MCU_ETH_MEM_REGION,recvDataPackCount*(sizeof(ETH_BufferTypeDef)/sizeof(uint8_t)));
    //把数组链接到一起
    UserMemClear((uint8_t*)recvDataPackPtr,recvDataPackCount*(sizeof(ETH_BufferTypeDef)/sizeof(uint8_t)));
    //只有一个数据包就不需要链接
    if(recvDataPackCount != 1)
    {
        for(indexUtilMainBlock = 0; indexUtilMainBlock < recvDataPackCount -1; indexUtilMainBlock++)
        {
            recvDataPackPtr[indexUtilMainBlock].next=&recvDataPackPtr[indexUtilMainBlock+1];
        }
    }
    //读取数据
    halState = HAL_ETH_GetRxDataBuffer(&ETH_Handler,recvDataPackPtr);
    if(halState != HAL_OK)
    {
        MCU_LogShowHalState(halState,ETH_Handler.ErrorCode);
        while(1);
    }
    //执行数据拷贝
    for(indexUtilMainBlock = 0; indexUtilMainBlock < recvDataPackCount;indexUtilMainBlock++)
    {
        for(indexUtilSubArray = 0; indexUtilSubArray < recvDataPackPtr[indexUtilMainBlock].len;indexUtilSubArray++)
        {
            (*recvBuf)[indexUtilMainBlock*(ETH_Handler.Init.RxBuffLen) + indexUtilSubArray] = 
                recvDataPackPtr[indexUtilMainBlock].buffer[indexUtilSubArray];
        }
    }
    //数据拷贝完成,释放内存,释放的是中间内存
    UserMemFree(MCU_ETH_MEM_REGION,recvDataPackPtr);
    //重建接收
    halState = HAL_ETH_BuildRxDescriptors(&ETH_Handler);
    if(halState != HAL_OK)
    {
        MCU_LogShowHalState(halState,ETH_Handler.ErrorCode);
        while(1);
    }
}


//写入网卡发送数据
ERROR_SUB MCU_ETH_WriteMultiPack(uint32_t checksumCtrl,ETH_MULTI_PACK* multiPackPtr)
{
    uint16_t indexUtil = 0;
    uint32_t sendLength = 0;
    uint32_t txResult = TX_SUCCESS;
    HAL_StatusTypeDef halState = HAL_OK;
    if(multiPackPtr->packCount == 0)
    {
        return ERROR_SUB_OK;
    }
    //申请信号量
    txResult = tx_semaphore_get(&semaphoreMCU_ETH,TIME_OUT_MS_ETH_SEND);
    if(TX_SUCCESS != txResult)
    {
        //信号量申请失败,返回错误
        MCU_LogPrintf("%s,semaphoreMCU_ETH GetTimeOut\r\n",__func__);
        //停止ETH
        halState = HAL_ETH_Stop_IT(&ETH_Handler);
        if(halState != HAL_OK)
        {
            MCU_LogShowHalState(halState,ETH_Handler.ErrorCode);
            while(1);
        }
        //启动ETH
        halState = HAL_ETH_Start_IT(&ETH_Handler);
        if(halState != HAL_OK)
        {
            MCU_LogShowHalState(halState,ETH_Handler.ErrorCode);
            while(1);
        }
        //发送信号量
        tx_semaphore_ceiling_put(&semaphoreMCU_ETH,1);
        return ERROR_SUB_MCU_ETH_SEND_TIME_OUT;
    }
    //清除发送结构
    UserMemSet((uint8_t*)ethSendPackBuffer,0,(ETH_TX_DESC_CNT*(sizeof(ETH_BufferTypeDef)/sizeof(uint8_t))));
    //清除发送缓存
    UserMemSet((uint8_t*)ethSendBufferArray,0,(ETH_TX_DESC_CNT*ETH_MAX_PACKET_SIZE));
    //配置发送结构
    for(indexUtil = 0;indexUtil < multiPackPtr->packCount;indexUtil++)
    {
        //数据拷贝到发送缓存
        UserMemCopy((uint8_t*)ethSendBufferArray[indexUtil],(void*)multiPackPtr->dataBufferPtrArray[indexUtil],
                        multiPackPtr->dataLengthArray[indexUtil]);
        //发送环境创建
        ethSendPackBuffer[indexUtil].buffer = ethSendBufferArray[indexUtil];
        ethSendPackBuffer[indexUtil].len = multiPackPtr->dataLengthArray[indexUtil];
        if(indexUtil != multiPackPtr->packCount -1)
        {
            //多包链接
            ethSendPackBuffer[indexUtil].next = &ethSendPackBuffer[indexUtil+1];
        }
        //总长度计算
        sendLength += multiPackPtr->dataLengthArray[indexUtil];
    }
    ethTxConfig.Length = sendLength;
    ethTxConfig.TxBuffer = ethSendPackBuffer;
    ethTxConfig.ChecksumCtrl = checksumCtrl;
    //中断发送
    halState = HAL_ETH_Transmit_IT(&ETH_Handler,&ethTxConfig);
    if(halState != HAL_OK)
    {
        MCU_LogShowHalState(halState,ETH_Handler.ErrorCode);
        while(1);
    }
    return ERROR_SUB_OK;
}

//端口开始
void MCU_ETH_Start(void)
{
    HAL_StatusTypeDef halState = HAL_OK;
    halState = HAL_ETH_Start_IT(&ETH_Handler);
    if(halState != HAL_OK)
    {
        MCU_LogShowHalState(halState,ETH_Handler.ErrorCode);
        while(1);
    }
}

//端口停止
void MCU_ETH_Stop(void)
{
    HAL_StatusTypeDef halState = HAL_OK;
    halState = HAL_ETH_Stop(&ETH_Handler);
    if(halState != HAL_OK)
    {
        MCU_LogShowHalState(halState,ETH_Handler.ErrorCode);
        while(1);
    }
}

/* 中断服务程序 */
void ETH_IRQHandler(void)
{
    //进入中断,调用ThreadX方法告知系统
    _tx_thread_context_save();
    //调用中断处理程序
    HAL_ETH_IRQHandler(&ETH_Handler);
    //退出中断,调用ThreadX方法告知系统
    _tx_thread_context_restore();
}

//发送完成回调
void HAL_ETH_TxCpltCallback(ETH_HandleTypeDef *heth)
{
    if(heth->Instance == ETH)
    {
        //发送信号量
        tx_semaphore_ceiling_put(&semaphoreMCU_ETH,1);
    }
}

//DMA错误回调
void HAL_ETH_DMAErrorCallback(ETH_HandleTypeDef *heth)
{
    if(heth->Instance == ETH)
    {
#if(MCU_ETH_INT_SHOW_ERR == CONFIG_FUNC_ENABLE)
        MCU_LogSendStringInt("HAL_ETH_DMAErrorCallback\r\n");
#endif
        asm("nop");
    }
}

//MAC层错误回调
void HAL_ETH_MACErrorCallback(ETH_HandleTypeDef *heth)
{
    if(heth->Instance == ETH)
    {
#if(MCU_ETH_INT_SHOW_ERR == CONFIG_FUNC_ENABLE)
        MCU_LogSendStringInt("HAL_ETH_MACErrorCallback\r\n");
#endif
        asm("nop");
    }
}

//PMT计数器错误回调
void HAL_ETH_PMTCallback(ETH_HandleTypeDef *heth)
{
    if(heth->Instance == ETH)
    {
#if(MCU_ETH_INT_SHOW_ERR == CONFIG_FUNC_ENABLE)
        MCU_LogSendStringInt("HAL_ETH_PMTCallback\r\n");
#endif
        asm("nop");
    }
}

//电气错误回调
void HAL_ETH_EEECallback(ETH_HandleTypeDef *heth)
{
    if(heth->Instance == ETH)
    {
#if(MCU_ETH_INT_SHOW_ERR == CONFIG_FUNC_ENABLE)
        MCU_LogSendStringInt("HAL_ETH_EEECallback\r\n");
#endif
        asm("nop");
    }
}

//唤醒回调
void HAL_ETH_WakeUpCallback(ETH_HandleTypeDef *heth)
{
    if(heth->Instance == ETH)
    {
#if(MCU_ETH_INT_SHOW_ERR == CONFIG_FUNC_ENABLE)
        MCU_LogSendStringInt("HAL_ETH_WakeUpCallback\r\n");
#endif
        asm("nop");
    }
}













