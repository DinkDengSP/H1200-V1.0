/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-10-31 19:05:56 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-10-31 19:30:17 +0800
************************************************************************************************/ 
#include "BoardExti.h"

//是否支持中断配置
typedef enum EXTI_STATE
{
    EXTI_STATE_INVALID  = 0x00,//端口不支持中断
    EXTI_STATE_VALID    = 0x01,//端口支持中断
}EXTI_STATE;

//指示当前通道没有使用
#define BOARD_EXTI_WITH_NONE_IN             255
//指定通道的外部中断当前关联的引脚
static uint8_t userExtiAssociatePinArray[16] = {BOARD_EXTI_WITH_NONE_IN};
//指定通道的外部中断回调函数是否被设置
static MCU_Exti_CallBackFuncPtr userExtiCallBackArray[16] = {NULL};
//设定板上输入与外部中断线之间的关联数组
typedef struct BoardPortInWithExtiPort
{
    //板上输入端口
    BOARD_PORT_IN   portInBoard;
    //端口是否支持中断
    EXTI_STATE     validState;
    //与输入引脚关联的MCU引脚
    MCU_PIN         pinInMCU;
    //回调函数在回调函数数组中的序号
    uint8_t         callBackIndex;
}BoardPortInWithExtiPort;

//板上输入与中断端口的配置文件
static const BoardPortInWithExtiPort BoardPortInWithExtiPortConfigArray[] = {
    /**输入端口号定义***************端口是否支持中断***********与输入引脚关联的MCU引脚*****回调函数在回调函数数组中的序号*/
    {BOARD_PORT_IN_CPLD_NOP1        ,EXTI_STATE_INVALID     ,MCU_PIN_NULL   ,         255  },
    {BOARD_PORT_IN_CPLD_NOP2        ,EXTI_STATE_INVALID     ,MCU_PIN_NULL   ,         255  },
    {BOARD_PORT_IN_CPLD_NOP3        ,EXTI_STATE_INVALID     ,MCU_PIN_NULL   ,         255  },
    {BOARD_PORT_IN_CPLD_NOP4        ,EXTI_STATE_INVALID     ,MCU_PIN_NULL   ,         255  },
    {BOARD_PORT_IN_CPLD_NOP5        ,EXTI_STATE_INVALID     ,MCU_PIN_NULL   ,         255  },
    {BOARD_PORT_IN_CPLD_NOP6        ,EXTI_STATE_INVALID     ,MCU_PIN_NULL   ,         255  },
    {BOARD_PORT_IN_CPLD_NOP7        ,EXTI_STATE_INVALID     ,MCU_PIN_NULL   ,         255  },
    {BOARD_PORT_IN_CPLD_NOP8        ,EXTI_STATE_INVALID     ,MCU_PIN_NULL   ,         255  },
    {BOARD_PORT_IN_CPLD_NOP9        ,EXTI_STATE_INVALID     ,MCU_PIN_NULL   ,         255  },
    {BOARD_PORT_IN_CPLD_NOP10       ,EXTI_STATE_INVALID     ,MCU_PIN_NULL   ,         255  },
    {BOARD_PORT_IN_CPLD_NOP11       ,EXTI_STATE_INVALID     ,MCU_PIN_NULL   ,         255  },
    {BOARD_PORT_IN_CPLD_NOP12       ,EXTI_STATE_INVALID     ,MCU_PIN_NULL   ,         255  },
    {BOARD_PORT_IN_CPLD_NOP13       ,EXTI_STATE_INVALID     ,MCU_PIN_NULL   ,         255  },
    {BOARD_PORT_IN_ARM_SEN1_PC5     ,EXTI_STATE_VALID       ,MCU_PIN_C_5    ,         5    },
    {BOARD_PORT_IN_ARM_SEN2_PB0     ,EXTI_STATE_VALID       ,MCU_PIN_B_0    ,         0    },
    {BOARD_PORT_IN_ARM_SEN3_PB1     ,EXTI_STATE_VALID       ,MCU_PIN_B_1    ,         1    },
    {BOARD_PORT_IN_ARM_SEN4_PB10    ,EXTI_STATE_VALID       ,MCU_PIN_B_10   ,         10   },
    {BOARD_PORT_IN_ARM_SEN5_PB11    ,EXTI_STATE_VALID       ,MCU_PIN_B_11   ,         11   },
    {BOARD_PORT_IN_ARM_SEN6_PH6     ,EXTI_STATE_VALID       ,MCU_PIN_H_6    ,         6    },
    {BOARD_PORT_IN_ARM_SEN7_PH7     ,EXTI_STATE_VALID       ,MCU_PIN_H_7    ,         7    },
    {BOARD_PORT_IN_ARM_SEN8_PH8     ,EXTI_STATE_VALID       ,MCU_PIN_H_8    ,         8    },
    {BOARD_PORT_IN_ARM_SEN9_PH9     ,EXTI_STATE_VALID       ,MCU_PIN_H_9    ,         9    },
    {BOARD_PORT_IN_ARM_SEN10_PH10   ,EXTI_STATE_VALID       ,MCU_PIN_H_10   ,         10   },
    {BOARD_PORT_IN_ARM_SEN11_PH11   ,EXTI_STATE_VALID       ,MCU_PIN_H_11   ,         11   },
    {BOARD_PORT_IN_ARM_SEN12_PH12   ,EXTI_STATE_VALID       ,MCU_PIN_H_12   ,         12   },
    {BOARD_PORT_IN_ARM_SEN13_PA4    ,EXTI_STATE_VALID       ,MCU_PIN_A_4    ,         4    },
    {BOARD_PORT_IN_ARM_SEN14_PA5    ,EXTI_STATE_VALID       ,MCU_PIN_A_5    ,         5    },
    {BOARD_PORT_IN_ARM_SEN15_PA6    ,EXTI_STATE_VALID       ,MCU_PIN_A_6    ,         6    },
    {BOARD_PORT_IN_ARM_SEN16_PA7    ,EXTI_STATE_VALID       ,MCU_PIN_A_7    ,         7    },
    {BOARD_PORT_IN_ARM_SEN17_PC4    ,EXTI_STATE_VALID       ,MCU_PIN_C_4    ,         4    },
    {BOARD_PORT_IN_ARM_SEN18_PH3    ,EXTI_STATE_VALID       ,MCU_PIN_H_3    ,         3    },
    {BOARD_PORT_IN_ARM_SEN19_PH2    ,EXTI_STATE_INVALID     ,MCU_PIN_H_2    ,         2    },
    {BOARD_PORT_IN_ARM_SEN20_PC3    ,EXTI_STATE_VALID       ,MCU_PIN_C_3    ,         3    },
    {BOARD_PORT_IN_ARM_SEN21_PC2    ,EXTI_STATE_INVALID     ,MCU_PIN_C_2    ,         2    },
    {BOARD_PORT_IN_ARM_SEN22_PC1    ,EXTI_STATE_VALID       ,MCU_PIN_C_1    ,         1    },
    {BOARD_PORT_IN_ARM_SEN23_PC0    ,EXTI_STATE_VALID       ,MCU_PIN_C_0    ,         0    },
    {BOARD_PORT_IN_ARM_SEN24_PF10   ,EXTI_STATE_VALID       ,MCU_PIN_F_10   ,         10   },
    {BOARD_PORT_IN_ARM_SEN25_PB3    ,EXTI_STATE_VALID       ,MCU_PIN_B_3    ,         3    },
};

//板上中断全部初始化
void BoardExtiResetAll(void)
{
    EXTI_HandleTypeDef   EXTI_Handle;
    uint8_t indexUtil = 0;
    //回调函数和引脚IO配置设置为0
    for(indexUtil = 0; indexUtil < 15; indexUtil++)
    {
        //中断对应的回调函数
        userExtiCallBackArray[indexUtil] = NULL;
        //中断对应的系统引脚
        userExtiAssociatePinArray[indexUtil] = BOARD_EXTI_WITH_NONE_IN;
    }
    //将所有的中断线都关闭掉
    EXTI_Handle.PendingCallback = NULL;
    for(indexUtil= 0; indexUtil < 15;indexUtil++)
    {
        EXTI_Handle.Line = EXTI_LINE_0 + indexUtil;
        if(EXTI_Handle.Line == EXTI_LINE_2)
        {
            continue;
        }
        //清除中断配置
        HAL_EXTI_ClearConfigLine(&EXTI_Handle);
    }
}

//外部中断注册
ERROR_SUB BoardExtiRegister(BOARD_PORT_IN inPin,MCU_EXTI_MODE trigMode,MCU_Exti_CallBackFuncPtr callBack)
{
    BoardPortInWithExtiPort* extiMapPtr;
    uint32_t lastIntState;
    //回调函数不能为空
    if(callBack == NULL)
    {
        return ERROR_SUB_OK;
    }
    //引脚要在指定范围内
    if(inPin >= BOARD_PORT_IN_COUNT)
    {
        return ERROR_SUB_BOARD_EXT_INT_UNSUPPORT;
    }
    //查看输入引脚的状态,输入引脚与配置引脚匹配
    if(inPin == BoardPortInWithExtiPortConfigArray[inPin].portInBoard)
    {
        //取出配置信息
        extiMapPtr = (BoardPortInWithExtiPort*)(&(BoardPortInWithExtiPortConfigArray[inPin]));
        //查看是否支持中断
        if(extiMapPtr->validState == EXTI_STATE_INVALID)
        {
            return ERROR_SUB_BOARD_EXT_INT_UNSUPPORT;
        }
        //然后看其注册讯息是否已经被占用,被占用就不能使用了
        if(((NULL == userExtiCallBackArray[extiMapPtr->callBackIndex])&&(BOARD_EXTI_WITH_NONE_IN == 
                    userExtiAssociatePinArray[extiMapPtr->callBackIndex]))||
                    ((userExtiCallBackArray[extiMapPtr->callBackIndex] == callBack)&&
                    (userExtiAssociatePinArray[extiMapPtr->callBackIndex] == inPin)))
        {
            //先屏蔽中断
            lastIntState = CoreLockIntGlobal();
            //可以注册
            userExtiCallBackArray[extiMapPtr->callBackIndex] = callBack;
            userExtiAssociatePinArray[extiMapPtr->callBackIndex] = inPin;
            //设置中断
            MCU_PinExtiInit(extiMapPtr->pinInMCU,trigMode,MCU_PIN_PULL_PULLUP,MCU_PIN_SPEED_FREQ_HIGH,
                                USER_EXT_INT_PREE_PRI,USER_EXT_INT_SUB_PRI,callBack);
            //然后打开中断
            CoreUnlockIntGlobal(lastIntState);
            return ERROR_SUB_OK;
        }
        else
        {
            //被占用了
            return ERROR_SUB_BOARD_EXT_INT_CHANNEL_BUSY;
        }
    }
    else
    {
        return ERROR_SUB_BOARD_EXT_INT_UNSUPPORT;
    }
}

//外部中断解除注册
void BoardExtiUnRegister(BOARD_PORT_IN inPin)
{   
    BoardPortInWithExtiPort* extiMapPtr;
    uint32_t lastIntState;
    //引脚要在指定范围内
    if(inPin >= BOARD_PORT_IN_COUNT)
    {
        return ;
    }
    //查看输入引脚的状态
    if(inPin == BoardPortInWithExtiPortConfigArray[inPin].portInBoard)
    {
        //取出配置信息
        extiMapPtr = (BoardPortInWithExtiPort*)(&(BoardPortInWithExtiPortConfigArray[inPin]));
        //然后看其注册讯息与当前引脚是否匹配
        if(inPin == userExtiAssociatePinArray[extiMapPtr->callBackIndex])
        {
            //先屏蔽中断
            lastIntState = CoreLockIntGlobal();
            //清除内部状态讯息
            userExtiCallBackArray[extiMapPtr->callBackIndex] = NULL;
            userExtiAssociatePinArray[extiMapPtr->callBackIndex] = BOARD_EXTI_WITH_NONE_IN;
            //关闭中断
            MCU_PinExtiClearConfig(extiMapPtr->pinInMCU);
            //然后打开中断
            CoreUnlockIntGlobal(lastIntState);
        }
        else
        {
            //被占用了
            return;
        }
    }
    else
    {
        return;
    }
}
















