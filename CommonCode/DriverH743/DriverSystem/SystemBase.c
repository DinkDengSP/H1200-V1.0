/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-08-21 10:19:22 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-08-21 13:56:28 +0800
************************************************************************************************/ 
#include "SystemBase.h"

#ifdef NX_INCLUDE_USER_DEFINE_FILE

//NETX状态映射表
static const NETX_STATE_MAP_STR NetX_StateStrMapArray[] = {
    {NX_TCP_CLOSED          ,"NX_TCP_CLOSED"      },
    {NX_TCP_LISTEN_STATE    ,"NX_TCP_LISTEN_STATE"},
    {NX_TCP_SYN_SENT        ,"NX_TCP_SYN_SENT"    },
    {NX_TCP_SYN_RECEIVED    ,"NX_TCP_SYN_RECEIVED"},
    {NX_TCP_ESTABLISHED     ,"NX_TCP_ESTABLISHED" },
    {NX_TCP_CLOSE_WAIT      ,"NX_TCP_CLOSE_WAIT"  },
    {NX_TCP_FIN_WAIT_1      ,"NX_TCP_FIN_WAIT_1"  },
    {NX_TCP_FIN_WAIT_2      ,"NX_TCP_FIN_WAIT_2"  },
    {NX_TCP_CLOSING         ,"NX_TCP_CLOSING"     },
    {NX_TCP_TIMED_WAIT      ,"NX_TCP_TIMED_WAIT"  },
    {NX_TCP_LAST_ACK        ,"NX_TCP_LAST_ACK"    },
    {0                      ,"NX_TCP_UN_INIT"     },
};
#define LENGTH_NETX_STATE_MAP_STR_MAP       (sizeof(NetX_StateStrMapArray)/sizeof(NetX_StateStrMapArray[0]))
//未知状态描述表
static const uint8_t NetX_DespStrUnknow[] = "NX_TCP_UNKNOW!!";

//通过Socket状态值获取Socket状态描述字符串
uint8_t* SocketNetStateGetSocketState(ULONG state)
{
    uint16_t indexUtil = 0;
    for(indexUtil = 0; indexUtil < LENGTH_NETX_STATE_MAP_STR_MAP;indexUtil++)
    {
        if(state == NetX_StateStrMapArray[indexUtil].state)
        {
            return (uint8_t*)(NetX_StateStrMapArray[indexUtil].despStr);
        }
    }
    return (uint8_t*)(NetX_DespStrUnknow);
}


#endif








