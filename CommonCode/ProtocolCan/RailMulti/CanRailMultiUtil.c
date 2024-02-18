#include "CanRailMultiUtil.h"

//获取板号和模块化
void CanRailMultiUtilGetRailModuleBoardNo(RAIL_MODULE_NO railNo,uint8_t *targetModulePtr,uint8_t *targetBoardIDPtr)
{
    if(railNo == RAIL_MODULE_NO_1)
    {
        *targetModulePtr = CAN_MODULE_NO_BOARD3;
        *targetBoardIDPtr = CAN_BOARD_ID_BOARD3;
    }
    else if(railNo == RAIL_MODULE_NO_2)
    {
        *targetModulePtr = CAN_MODULE_NO_BOARD4;
        *targetBoardIDPtr = CAN_BOARD_ID_BOARD4;
    }
    else if(railNo == RAIL_MODULE_NO_3)
    {
        *targetModulePtr = CAN_MODULE_NO_BOARD5;
        *targetBoardIDPtr = CAN_BOARD_ID_BOARD5;
    }
    else if(railNo == RAIL_MODULE_NO_4)
    {
        *targetModulePtr = CAN_MODULE_NO_BOARD6;
        *targetBoardIDPtr = CAN_BOARD_ID_BOARD6;
    }
    else
    {
        //默认轨道1
        *targetModulePtr = CAN_MODULE_NO_BOARD3;
        *targetBoardIDPtr = CAN_BOARD_ID_BOARD3;
    }
}

