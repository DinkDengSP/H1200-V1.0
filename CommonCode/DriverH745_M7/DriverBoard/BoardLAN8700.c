/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-09-15 09:57:20 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-09-18 10:52:34 +0800
************************************************************************************************/ 
#include "BoardLAN8700.h"
#include "DriverConfigBoard.h"

typedef enum NET_PHY_INIT_STATE
{
    NET_PHY_INIT_STATE_NONE  = 0X00,//未初始化
    NET_PHY_INIT_STATE_MAC   = 0X01,//MAC层已经初始化了
    NET_PHY_INIT_STATE_OVER  = 0X02,//PHY初始化完成
}NET_PHY_INIT_STATE;

static NET_PHY_INIT_STATE netPhyChipInitState = NET_PHY_INIT_STATE_NONE;

//PHY端口复位
static void BoardLAN8700_IO_Reset(void)
{
    //复位引脚写入低电平
    MCU_PinWriteSingle(BOARD_LAN8700_PIN_RESET,VALID_LEVEL_BOARD_LAN8700_PIN_RESET);
    //持续5MS
    CoreDelayMs(5);
    //复位引脚恢复高电平
    MCU_PinWriteSingle(BOARD_LAN8700_PIN_RESET,PIN_INVALID_LEVEL(VALID_LEVEL_BOARD_LAN8700_PIN_RESET));
}

//PHY端口初始化
static void BoardLAN8700_IO_Init(void)
{
    //硬件复位引脚
    MCU_PinInit(BOARD_LAN8700_PIN_RESET,MCU_PIN_MODE_OUTPUT_PP,MCU_PIN_PULL_NOPULL,MCU_PIN_SPEED_FREQ_MEDIUM,0);
    //信号中断引脚
    MCU_PinInit(BOARD_LAN8700_PIN_INT,MCU_PIN_MODE_INPUT,MCU_PIN_PULL_NOPULL,MCU_PIN_SPEED_FREQ_MEDIUM,0);
    //复位
    BoardLAN8700_IO_Reset();
}


//读取网卡ID
ERROR_SUB BoardLAN8700_ReadID(uint32_t* idPtr)
{
    ERROR_SUB errorCodeSub = ERROR_SUB_OK;
    uint32_t readRegValue = 0;
    *idPtr = 0X00000000;
    //读取ID1
    errorCodeSub = MCU_ETH_ReadPhyReg(BOARD_LAN8700_PHY_ADDR,LAN8700_PHYID1,&readRegValue);
    if(errorCodeSub != ERROR_SUB_OK)
    {
        //读取失败
        MCU_LogShowErrorSubCode(errorCodeSub);
        return errorCodeSub;
    }
    *idPtr = readRegValue;
    (*idPtr) <<= 16;
    //读取ID2
    errorCodeSub = MCU_ETH_ReadPhyReg(BOARD_LAN8700_PHY_ADDR,LAN8700_PHYID2,&readRegValue);
    if(errorCodeSub != ERROR_SUB_OK)
    {
        //读取失败
        MCU_LogShowErrorSubCode(errorCodeSub);
        return errorCodeSub;
    }
    *idPtr += readRegValue;
    return ERROR_SUB_OK;
}


//LAN8700硬件复位
ERROR_SUB BoardLAN8700_HardWareReset(void)
{
    ERROR_SUB errorCodeSub = ERROR_SUB_OK;
    uint32_t readRegValue = 0;
    uint32_t phyID = 0;
    //设备复位
    BoardLAN8700_IO_Reset();
    //延时等待复位完成
    CoreDelayMs(20);
    //读取网卡ID
    errorCodeSub = BoardLAN8700_ReadID(&phyID);
    if(errorCodeSub != ERROR_SUB_OK)
    {
        //读取失败
        MCU_LogShowErrorSubCode(errorCodeSub);
        return errorCodeSub;
    }
    //匹配网卡ID
    if((phyID != BOARD_LAN8700_PHY_ID_A)&&(phyID != BOARD_LAN8700_PHY_ID_B))
    {
        errorCodeSub = ERROR_SUB_BOARD_LAN8700_PHY_ID_MATCH;
        //ID不匹配
        MCU_LogShowErrorSubCode(errorCodeSub);
        return errorCodeSub;
    }
    //写入软件复位
    errorCodeSub = MCU_ETH_WritePhyReg(BOARD_LAN8700_PHY_ADDR,LAN8700_BMCR,LAN8700_BMCR_RESET);
    if(errorCodeSub != ERROR_SUB_OK)
    {
        //读取失败
        MCU_LogShowErrorSubCode(errorCodeSub);
        return errorCodeSub;
    }
    //持续一定时间等待响应复位
    CoreDelayMs(5);
    //获取复位寄存器状态
    errorCodeSub = MCU_ETH_ReadPhyReg(BOARD_LAN8700_PHY_ADDR,LAN8700_BMCR,&readRegValue);
    if(errorCodeSub != ERROR_SUB_OK)
    {
        //读取失败
        MCU_LogShowErrorSubCode(errorCodeSub);
        return errorCodeSub;
    }
    //复位开始时间
    uint32_t resetStartTimeStamp = tx_time_get();
    //循环等待复位状态退出
    while(readRegValue & LAN8700_BMCR_RESET)
    {
        //读取状态寄存器
        errorCodeSub = MCU_ETH_ReadPhyReg(BOARD_LAN8700_PHY_ADDR,LAN8700_BMCR,&readRegValue);
        if(errorCodeSub != ERROR_SUB_OK)
        {
            //读取失败
            MCU_LogShowErrorSubCode(errorCodeSub);
            return errorCodeSub;
        }
        //检测复位是否完成
        if(readRegValue & LAN8700_BMCR_RESET)
        {
            if(tx_time_get() - resetStartTimeStamp > BOARD_LAN8700_INIT_TIME_OUT_MS)
            {
                //复位超时
                MCU_LogShowErrorSubCode(ERROR_SUB_BOARD_LAN8700_RESET_TIME_OUT);
                return ERROR_SUB_BOARD_LAN8700_RESET_TIME_OUT;
            }
            //延时
            CoreDelayMinTick();
        }
    }
    //设置网卡自协商,先读取
    errorCodeSub = MCU_ETH_ReadPhyReg(BOARD_LAN8700_PHY_ADDR,LAN8700_BMCR,&readRegValue);
    //写入自动协商
    readRegValue |= (LAN8700_BMCR_AN_EN|LAN8700_BMCR_RESTART_AN);
    errorCodeSub = MCU_ETH_WritePhyReg(BOARD_LAN8700_PHY_ADDR,LAN8700_BMCR,readRegValue);
    if(errorCodeSub != ERROR_SUB_OK)
    {
        //读取失败
        MCU_LogShowErrorSubCode(errorCodeSub);
        return errorCodeSub;
    }
    //打开线路自动翻转
    errorCodeSub = MCU_ETH_ReadPhyReg(BOARD_LAN8700_PHY_ADDR,LAN8700_SCSIR,&readRegValue);
    readRegValue |= LAN8700_SCSIR_AMDIXCTRL;
    errorCodeSub = MCU_ETH_WritePhyReg(BOARD_LAN8700_PHY_ADDR,LAN8700_SCSIR,readRegValue);
    if(errorCodeSub != ERROR_SUB_OK)
    {
        //读取失败
        MCU_LogShowErrorSubCode(errorCodeSub);
        return errorCodeSub;
    }
    //延时指定时间,等待网卡芯片初始化
    CoreDelayMs(BOARD_LAN8700_INIT_TIME_OUT_MS);
    //复位完成
    return errorCodeSub;
}

//设备初始化
ERROR_SUB BoardLAN8700_Init(void)
{
    ERROR_SUB errorCode = ERROR_SUB_OK;
    //PHY IO初始化
    BoardLAN8700_IO_Init();
    if(netPhyChipInitState == NET_PHY_INIT_STATE_OVER)
    {
        //已经初始化过了,不需要再次初始化
        return ERROR_SUB_OK;
    }
    if(netPhyChipInitState == NET_PHY_INIT_STATE_NONE)
    {
        //ETH端口复位
        MCU_ETH_PortInit();
        //设置状态为MAC初始化
        netPhyChipInitState = NET_PHY_INIT_STATE_MAC;
    }
    //硬件复位
    errorCode = BoardLAN8700_HardWareReset();
    if(errorCode != ERROR_SUB_OK)
    {
        return errorCode;
    }
    //状态设置为初始化成功
    netPhyChipInitState = NET_PHY_INIT_STATE_OVER;
    //复位完成
    return errorCode;
}

//获取设备网线连接状态
ERROR_SUB BoardLAN8700_GetLinkState(NET_PORT_STATE* netPortState)
{
    ERROR_SUB errorCodeSub = ERROR_SUB_OK;
    uint32_t readRegValue = 0;
    //初始化设置连接状态为未知
    *netPortState = NET_PORT_STATE_UNKNOW;
    //连续读取两次状态寄存器,第一次的不要
    errorCodeSub = MCU_ETH_ReadPhyReg(BOARD_LAN8700_PHY_ADDR,LAN8700_BMSR,&readRegValue);
    if(errorCodeSub != ERROR_SUB_OK)
    {
        MCU_LogShowErrorSubCode(errorCodeSub);
        return errorCodeSub;
    }
     //读取第二次
     errorCodeSub = MCU_ETH_ReadPhyReg(BOARD_LAN8700_PHY_ADDR,LAN8700_BMSR,&readRegValue);
     if(errorCodeSub != ERROR_SUB_OK)
     {
         MCU_LogShowErrorSubCode(errorCodeSub);
         return errorCodeSub;
     }
    //检查连接状态
    if ((readRegValue & LAN8700_BMSR_LINK_STATUS) == 0)
    {
        //网线未连接
        *netPortState = NET_PORT_STATE_LINK_DOWN;
        return ERROR_SUB_OK;
    }
    //检查是否启用自动协商
    errorCodeSub = MCU_ETH_ReadPhyReg(BOARD_LAN8700_PHY_ADDR,LAN8700_BMCR,&readRegValue);
    if(errorCodeSub != ERROR_SUB_OK)
    {
        MCU_LogShowErrorSubCode(errorCodeSub);
        return errorCodeSub;
    }
    //根据读取到的自动协商状态进行检查
    if ((readRegValue & LAN8700_BMCR_AN_EN) != LAN8700_BMCR_AN_EN)
    {
        //未启用自协商,设置网卡自协商并重新启动自协商
        errorCodeSub = MCU_ETH_ReadPhyReg(BOARD_LAN8700_PHY_ADDR,LAN8700_BMCR,&readRegValue);
        //写入自动协商
        readRegValue |= (LAN8700_BMCR_AN_EN|LAN8700_BMCR_RESTART_AN);
        errorCodeSub = MCU_ETH_WritePhyReg(BOARD_LAN8700_PHY_ADDR,LAN8700_BMCR,readRegValue);
        if(errorCodeSub != ERROR_SUB_OK)
        {
            //读取失败
            MCU_LogShowErrorSubCode(errorCodeSub);
            return errorCodeSub;
        }
        *netPortState = NET_PORT_STATE_100MBITS_FULLDUPLEX;
        return ERROR_SUB_OK;
    }
    else
    {
        //启用了自协商,读取phy寄存器
        errorCodeSub = MCU_ETH_ReadPhyReg(BOARD_LAN8700_PHY_ADDR,LAN8700_PSCSR,&readRegValue);
        if(errorCodeSub != ERROR_SUB_OK)
        {
            MCU_LogShowErrorSubCode(errorCodeSub);
            return errorCodeSub;
        }
        //自协商正在进行
        if ((readRegValue & LAN8700_PSCSR_AUTODONE) != LAN8700_PSCSR_AUTODONE)
        {
            //自协商未完成
            *netPortState = NET_PORT_STATE_AUTONEGO_NOTDONE;
        }
        //自协商结果
        else if ((readRegValue & LAN8700_PSCSR_HCDSPEED) == LAN8700_PSCSR_HCDSPEED_100BTX_FD)
        {
            //100M全双工
            *netPortState = NET_PORT_STATE_100MBITS_FULLDUPLEX;
        }
        else if ((readRegValue & LAN8700_PSCSR_HCDSPEED) == LAN8700_PSCSR_HCDSPEED_100BTX_HD)
        {
            //100M半双工
            *netPortState = NET_PORT_STATE_100MBITS_HALFDUPLEX;
        }
        else if ((readRegValue & LAN8700_PSCSR_HCDSPEED) == LAN8700_PSCSR_HCDSPEED_10BT_FD)
        {
            //10M全双工
            *netPortState = NET_PORT_STATE_10MBITS_FULLDUPLEX;
        }
        else
        {
            //10M半双工
            *netPortState = NET_PORT_STATE_10MBITS_HALFDUPLEX;
        }
        return ERROR_SUB_OK;
    }
}

//从连接状态获取工作模式和速度
void BoardLAN8700_GetModeSpeed(uint32_t* speedPtr,uint32_t* deplexModePtr)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    *speedPtr = PHY_SPEED_ETH_INVALID;
    *deplexModePtr = PHY_DUPLEX_MODE_ETH_INVALID;
    NET_PORT_STATE netPortState = NET_PORT_STATE_LINK_DOWN;
    errorSub = BoardLAN8700_GetLinkState(&netPortState);
    if(ERROR_SUB_OK != errorSub)
    {
        MCU_LogShowErrorSubCode(errorSub);
        return;
    }
    switch (netPortState)
    {
      case NET_PORT_STATE_100MBITS_FULLDUPLEX:
        *deplexModePtr = ETH_FULLDUPLEX_MODE;
        *speedPtr = ETH_SPEED_100M;
        break;
      case NET_PORT_STATE_100MBITS_HALFDUPLEX:
        *deplexModePtr = ETH_HALFDUPLEX_MODE;
        *speedPtr = ETH_SPEED_100M;
        break;
      case NET_PORT_STATE_10MBITS_FULLDUPLEX:
        *deplexModePtr = ETH_FULLDUPLEX_MODE;
        *speedPtr = ETH_SPEED_10M;
        break;
      case NET_PORT_STATE_10MBITS_HALFDUPLEX:
        *deplexModePtr = ETH_HALFDUPLEX_MODE;
        *speedPtr = ETH_SPEED_10M;
        break;
      default:
        *deplexModePtr = PHY_DUPLEX_MODE_ETH_INVALID;
        *speedPtr = PHY_SPEED_ETH_INVALID;
        break;
    }
}












