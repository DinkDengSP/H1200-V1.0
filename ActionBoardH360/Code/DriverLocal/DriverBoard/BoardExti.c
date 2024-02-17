/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-10-31 19:05:56 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-11-10 14:23:35 +0800
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
    {BOARD_PORT_IN_CPLD_NOP1             ,EXTI_STATE_INVALID     ,MCU_PIN_NULL   ,         255  },
    {BOARD_PORT_IN_CPLD_NOP2             ,EXTI_STATE_INVALID     ,MCU_PIN_NULL   ,         255  },
    {BOARD_PORT_IN_CPLD_NOP3             ,EXTI_STATE_INVALID     ,MCU_PIN_NULL   ,         255  },
    {BOARD_PORT_IN_CPLD_NOP4             ,EXTI_STATE_INVALID     ,MCU_PIN_NULL   ,         255  },
    {BOARD_PORT_IN_CPLD_NOP5             ,EXTI_STATE_INVALID     ,MCU_PIN_NULL   ,         255  },
    {BOARD_PORT_IN_CPLD_NOP6             ,EXTI_STATE_INVALID     ,MCU_PIN_NULL   ,         255  },
    {BOARD_PORT_IN_CPLD_NOP7             ,EXTI_STATE_INVALID     ,MCU_PIN_NULL   ,         255  },
    {BOARD_PORT_IN_CPLD_NOP8             ,EXTI_STATE_INVALID     ,MCU_PIN_NULL   ,         255  },
    {BOARD_PORT_IN_CPLD_NOP9             ,EXTI_STATE_INVALID     ,MCU_PIN_NULL   ,         255  },
    {BOARD_PORT_IN_CPLD_NOP10            ,EXTI_STATE_INVALID     ,MCU_PIN_NULL   ,         255  },
    {BOARD_PORT_IN_CPLD_NOP11            ,EXTI_STATE_INVALID     ,MCU_PIN_NULL   ,         255  },
    {BOARD_PORT_IN_CPLD_NOP12            ,EXTI_STATE_INVALID     ,MCU_PIN_NULL   ,         255  },
    {BOARD_PORT_IN_CPLD_NOP13            ,EXTI_STATE_INVALID     ,MCU_PIN_NULL   ,         255  },
    {BOARD_PORT_IN_ARM_SEN1_PI10         ,EXTI_STATE_VALID       ,MCU_PIN_I_10   ,         10   },
    {BOARD_PORT_IN_ARM_SEN2_PI9          ,EXTI_STATE_VALID       ,MCU_PIN_I_9    ,         9    },
    {BOARD_PORT_IN_ARM_SEN3_PC13         ,EXTI_STATE_VALID       ,MCU_PIN_C_13   ,         13   },
    {BOARD_PORT_IN_ARM_SEN4_PI8          ,EXTI_STATE_VALID       ,MCU_PIN_I_8    ,         8    },
    {BOARD_PORT_IN_ARM_SEN5_PE6          ,EXTI_STATE_VALID       ,MCU_PIN_E_6    ,         6    },
    {BOARD_PORT_IN_ARM_SEN6_PE5          ,EXTI_STATE_VALID       ,MCU_PIN_E_5    ,         5    },
    {BOARD_PORT_IN_ARM_SEN7_PE4          ,EXTI_STATE_VALID       ,MCU_PIN_E_4    ,         4    },
    {BOARD_PORT_IN_ARM_SEN8_PE3          ,EXTI_STATE_VALID       ,MCU_PIN_E_3    ,         3    },
    {BOARD_PORT_IN_ARM_SEN9_PE2          ,EXTI_STATE_INVALID     ,MCU_PIN_E_2    ,         255  },
    {BOARD_PORT_IN_ARM_SEN10_PI7         ,EXTI_STATE_VALID       ,MCU_PIN_I_7    ,         7    },
    {BOARD_PORT_IN_ARM_SEN11_PI6         ,EXTI_STATE_VALID       ,MCU_PIN_I_6    ,         6    },
    {BOARD_PORT_IN_ARM_SEN12_PI5         ,EXTI_STATE_VALID       ,MCU_PIN_I_5    ,         5    },
    {BOARD_PORT_IN_ARM_SEN13_PI4         ,EXTI_STATE_VALID       ,MCU_PIN_I_4    ,         4    },
    {BOARD_PORT_IN_ARM_SEN14_PB9         ,EXTI_STATE_VALID       ,MCU_PIN_B_9    ,         9    },
    {BOARD_PORT_IN_ARM_SEN15_PB8         ,EXTI_STATE_VALID       ,MCU_PIN_B_8    ,         8    },
    {BOARD_PORT_IN_ARM_SEN16_PB7         ,EXTI_STATE_VALID       ,MCU_PIN_B_7    ,         7    },
    {BOARD_PORT_IN_SLAVE_ARM_SEN1_PH12   ,EXTI_STATE_VALID       ,MCU_PIN_H_12   ,         12   },
    {BOARD_PORT_IN_SLAVE_ARM_SEN2_PH11   ,EXTI_STATE_VALID       ,MCU_PIN_H_11   ,         11   },
    {BOARD_PORT_IN_SLAVE_ARM_SEN3_PH10   ,EXTI_STATE_VALID       ,MCU_PIN_H_10   ,         10   },
    {BOARD_PORT_IN_SLAVE_ARM_SEN4_PH9    ,EXTI_STATE_VALID       ,MCU_PIN_H_9    ,         9    },
    {BOARD_PORT_IN_SLAVE_ARM_SEN5_PH8    ,EXTI_STATE_VALID       ,MCU_PIN_H_8    ,         8    },
    {BOARD_PORT_IN_SLAVE_ARM_SEN6_PH7    ,EXTI_STATE_VALID       ,MCU_PIN_H_7    ,         7    },
    {BOARD_PORT_IN_SLAVE_ARM_SEN7_PH6    ,EXTI_STATE_VALID       ,MCU_PIN_H_6    ,         6    },
    {BOARD_PORT_IN_SLAVE_ARM_SEN8_PB11   ,EXTI_STATE_VALID       ,MCU_PIN_B_11   ,         11   },
    {BOARD_PORT_IN_SLAVE_ARM_SEN9_PB10   ,EXTI_STATE_VALID       ,MCU_PIN_B_10   ,         10   },
    {BOARD_PORT_IN_SLAVE_ARM_SEN10_PC0   ,EXTI_STATE_VALID       ,MCU_PIN_C_0    ,         0    },
    {BOARD_PORT_IN_SLAVE_ARM_SEN11_PC1   ,EXTI_STATE_VALID       ,MCU_PIN_C_1    ,         1    },
    {BOARD_PORT_IN_SLAVE_ARM_SEN12_PC2   ,EXTI_STATE_INVALID     ,MCU_PIN_C_2    ,         255  },
    {BOARD_PORT_IN_SLAVE_ARM_SEN13_PC3   ,EXTI_STATE_VALID       ,MCU_PIN_C_3    ,         3    },
    {BOARD_PORT_IN_SLAVE_ARM_SEN14_PB1   ,EXTI_STATE_VALID       ,MCU_PIN_B_1    ,         1    },
    {BOARD_PORT_IN_SLAVE_ARM_SEN15_PB0   ,EXTI_STATE_VALID       ,MCU_PIN_B_0    ,         0    },
    {BOARD_PORT_IN_SLAVE_ARM_SEN16_PC5   ,EXTI_STATE_VALID       ,MCU_PIN_C_5    ,         5    },
    {BOARD_PORT_IN_FRUBBISH1_PH15        ,EXTI_STATE_VALID       ,MCU_PIN_H_15   ,         15   },
    {BOARD_PORT_IN_FRUBBISH2_PH14        ,EXTI_STATE_VALID       ,MCU_PIN_H_14   ,         14   },
    {BOARD_PORT_IN_FRUBBISH3_PH13        ,EXTI_STATE_VALID       ,MCU_PIN_H_13   ,         13   },
    {BOARD_PORT_IN_FRUBBISH4_PC9         ,EXTI_STATE_VALID       ,MCU_PIN_C_9    ,         9    },
    {BOARD_PORT_IN_FRUBBISH_OVER1_PI0    ,EXTI_STATE_VALID       ,MCU_PIN_I_0    ,         0    },
    {BOARD_PORT_IN_FRUBBISH_OVER2_PI1    ,EXTI_STATE_VALID       ,MCU_PIN_I_1    ,         1    },
    {BOARD_PORT_IN_PR1_ARM1_PG14         ,EXTI_STATE_VALID       ,MCU_PIN_G_14   ,         14   },
    {BOARD_PORT_IN_PR1_ARM2_PG13         ,EXTI_STATE_VALID       ,MCU_PIN_G_13   ,         13   },
    {BOARD_PORT_IN_PR2_ARM1_PG11         ,EXTI_STATE_VALID       ,MCU_PIN_G_11   ,         11   },
    {BOARD_PORT_IN_PR2_ARM2_PG10         ,EXTI_STATE_VALID       ,MCU_PIN_G_10   ,         10   },
    {BOARD_PORT_IN_YM1_ARM1_PB6          ,EXTI_STATE_VALID       ,MCU_PIN_B_6    ,         6    },
    {BOARD_PORT_IN_YM1_ARM2_PB5          ,EXTI_STATE_VALID       ,MCU_PIN_B_5    ,         5    },
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
















