/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-11-22 17:36:26 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-11-23 11:31:36 +0800
************************************************************************************************/ 
#include "H360SysActPortIn.h"
#include "SysState.h"

//获取板号和模块号
static ERROR_SUB H360SysActPortInGetModuleBoard(H360_SYS_PORT_IN portIndex,uint8_t* moduleNo,uint8_t* boardID,
                                            uint8_t* convertPinIndex,FlagStatus* isSelfBoard)
{
    *isSelfBoard = RESET;
    if(portIndex <= H360_SYS_PORT_IN_BOARD1_YM1_ARM2_PB5)
    {
        //下位机板1
        *moduleNo = CAN_MODULE_NO_BOARD1;
        *boardID = CAN_BOARD_ID_BOARD1;
        *convertPinIndex = (uint8_t)(((uint16_t)(portIndex)) - ((uint16_t)(H360_SYS_PORT_IN_BOARD1_CPLD_NOP1)));
    }
    else if(portIndex <= H360_SYS_PORT_IN_BOARD2_YM1_ARM2_PB5)
    {
        //下位机板2
        *moduleNo = CAN_MODULE_NO_BOARD2;
        *boardID = CAN_BOARD_ID_BOARD2;
        *convertPinIndex = (uint8_t)(((uint16_t)(portIndex)) - ((uint16_t)(H360_SYS_PORT_IN_BOARD2_CPLD_NOP1)));
    }
    else if(portIndex <= H360_SYS_PORT_IN_BOARD3_YM1_ARM2_PB5)
    {
        //下位机板3
        *moduleNo = CAN_MODULE_NO_BOARD3;
        *boardID = CAN_BOARD_ID_BOARD3;
        *convertPinIndex = (uint8_t)(((uint16_t)(portIndex)) - ((uint16_t)(H360_SYS_PORT_IN_BOARD3_CPLD_NOP1)));
    }
    else if(portIndex <= H360_SYS_PORT_IN_BOARD4_YM1_ARM2_PB5)
    {
        //下位机板4
        *moduleNo = CAN_MODULE_NO_BOARD4;
        *boardID = CAN_BOARD_ID_BOARD4;
        *convertPinIndex = (uint8_t)(((uint16_t)(portIndex)) - ((uint16_t)(H360_SYS_PORT_IN_BOARD4_CPLD_NOP1)));
    }
    else if(portIndex <= H360_SYS_PORT_IN_BOARD5_YM1_ARM2_PB5)
    {
        //下位机板5
        *moduleNo = CAN_MODULE_NO_BOARD5;
        *boardID = CAN_BOARD_ID_BOARD5;
        *convertPinIndex = (uint8_t)(((uint16_t)(portIndex)) - ((uint16_t)(H360_SYS_PORT_IN_BOARD5_CPLD_NOP1)));
    }
    else if(portIndex <= H360_SYS_PORT_IN_MAIN_M_SMBUS_SEN_48)
    {
        //主控板
        SYS_STATE_MAIN_CONTROL_MSG* mainControlMsgPtr = SysStateGetMainControlMsgPtr();
        if(mainControlMsgPtr->onlineFlag == RESET)
        {
            return ERROR_SUB_SYS_READ_PORT_IN_MAIN_NOT_ONLINE;
        }
        *moduleNo = mainControlMsgPtr->moduleNo;
        *boardID = mainControlMsgPtr->boardID;
        *convertPinIndex = (uint8_t)(((uint16_t)(portIndex)) - ((uint16_t)(H360_SYS_PORT_IN_MAIN_M_SMBUS_SEN_1)));
    }
    else
    {
        return ERROR_SUB_SYS_READ_PORT_IN_INDEX;
    }
    if((*moduleNo == SELF_MODULE_NO)&&(*boardID == SELF_BOARD_ID))
    {
        *isSelfBoard = SET;
    }
    return ERROR_SUB_OK;
}

//读取系统输入
ERROR_SUB H360SysActPortInRead(H360_SYS_PORT_IN portIndex,GPIO_PinState* readStatePtr)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    uint8_t moduleNo = 0;
    uint8_t boardID = 0;
    uint8_t convertPinIndex = 0;
    FlagStatus isSelfBoard = RESET;
    //识别端口号
    errorSub = H360SysActPortInGetModuleBoard(portIndex,&moduleNo,&boardID,&convertPinIndex,&isSelfBoard);
    if(errorSub != ERROR_SUB_OK)
    {
        return errorSub;
    }
    //检查是否自身
    if(isSelfBoard == SET)
    {
        //调用板卡输入读取
        *readStatePtr = BoardPortInReadState((BOARD_PORT_IN)convertPinIndex);
        return ERROR_SUB_OK;
    }
    //CAN读取
    SYS_RESULT_PACK sysResultPack;
    CanMasterPortInReadSingleWhileReturn(moduleNo,boardID,convertPinIndex,&sysResultPack,readStatePtr);
    //检查结果
    if(sysResultPack.resultCode == PROT_RESULT_FAIL)
    {
        return sysResultPack.errorSub;
    }
    return ERROR_SUB_OK;
}





















