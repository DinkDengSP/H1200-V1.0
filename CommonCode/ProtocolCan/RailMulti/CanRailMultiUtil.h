#ifndef _CAN_RAIL_MULTI_UTIL_H_
#define _CAN_RAIL_MULTI_UTIL_H_

#include "CanRailMultiDataType.h"

//获取板号和模块化
void CanRailMultiUtilGetRailModuleBoardNo(RAIL_MODULE_NO railNo,uint8_t *targetModulePtr,uint8_t *targetBoardIDPtr);

#endif // _CAN_RAIL_MULTI_UTIL_H_
