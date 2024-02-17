/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-08-28 08:57:19 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-08-28 10:53:09 +0800
************************************************************************************************/ 
#include "BoardDCMotor.h"
#include "BoardPortOut.h"
#include "DriverConfigBoard.h"

//一键全部关闭
void BoardDCMotorAllOff(void)
{
    BoardDCMotorWrite(BOARD_DC_MOTOR_1,BOARD_DC_MOTOR_STATE_OFF);
    BoardDCMotorWrite(BOARD_DC_MOTOR_2,BOARD_DC_MOTOR_STATE_OFF);
    BoardDCMotorWrite(BOARD_DC_MOTOR_3,BOARD_DC_MOTOR_STATE_OFF);
}

//设置DC电机状态
void BoardDCMotorWrite(BOARD_DC_MOTOR dcMotor,BOARD_DC_MOTOR_STATE setState)
{
    GPIO_PinState ioRealState1,ioRealState2;
    uint32_t lastIntState;
    //首先设置为关闭电机
    switch(dcMotor)
    {
        case BOARD_DC_MOTOR_1:
            ioRealState1 = PIN_INVALID_LEVEL(VALID_LEVEL_BOARD_DC_MOTOR1);
            ioRealState2 = PIN_INVALID_LEVEL(VALID_LEVEL_BOARD_DC_MOTOR1);
            //上锁,防止烧坏
            lastIntState = CoreLockIntGlobal();
            BoardPortOutWriteSingleState(BOARD_OUT_MCU_PC4_ARM_CW1,ioRealState1);
            BoardPortOutWriteSingleState(BOARD_OUT_MCU_PC5_ARM_CCW1,ioRealState2);
            //解锁
            CoreUnlockIntGlobal(lastIntState);
            break;
        case BOARD_DC_MOTOR_2:
            ioRealState1 = PIN_INVALID_LEVEL(VALID_LEVEL_BOARD_DC_MOTOR2);
            ioRealState2 = PIN_INVALID_LEVEL(VALID_LEVEL_BOARD_DC_MOTOR2);
            //上锁,防止烧坏
            lastIntState = CoreLockIntGlobal();
            BoardPortOutWriteSingleState(BOARD_OUT_MCU_PA6_ARM_CW2,ioRealState1);
            BoardPortOutWriteSingleState(BOARD_OUT_MCU_PA7_ARM_CCW2,ioRealState2);
            //解锁
            CoreUnlockIntGlobal(lastIntState);
            break;
        case BOARD_DC_MOTOR_3:
            ioRealState1 = PIN_INVALID_LEVEL(VALID_LEVEL_BOARD_DC_MOTOR3);
            ioRealState2 = PIN_INVALID_LEVEL(VALID_LEVEL_BOARD_DC_MOTOR3);
            //上锁,防止烧坏
            lastIntState = CoreLockIntGlobal();
            BoardPortOutWriteSingleState(BOARD_OUT_MCU_PA4_ARM_CW3,ioRealState1);
            BoardPortOutWriteSingleState(BOARD_OUT_MCU_PA5_ARM_CCW3,ioRealState2);
            //解锁
            CoreUnlockIntGlobal(lastIntState);
            break;
        default:
            break;
    }
    if(setState == BOARD_DC_MOTOR_STATE_OFF)
    {
        //如果是关闭,那么已经完成了
        return;
    }
    //然后设置为想要的状态
    switch(dcMotor)
    {
        case BOARD_DC_MOTOR_1:
            if(setState == BOARD_DC_MOTOR_STATE_CW)
            {
                ioRealState1 = VALID_LEVEL_BOARD_DC_MOTOR1;
                ioRealState2 = PIN_INVALID_LEVEL(VALID_LEVEL_BOARD_DC_MOTOR1);
            }
            else if(setState == BOARD_DC_MOTOR_STATE_CCW)
            {
                ioRealState1 = PIN_INVALID_LEVEL(VALID_LEVEL_BOARD_DC_MOTOR1);
                ioRealState2 = VALID_LEVEL_BOARD_DC_MOTOR1;
            }
            else
            {
                ioRealState1 = PIN_INVALID_LEVEL(VALID_LEVEL_BOARD_DC_MOTOR1);
                ioRealState2 = PIN_INVALID_LEVEL(VALID_LEVEL_BOARD_DC_MOTOR1);
            }
            //上锁,防止烧坏
            lastIntState = CoreLockIntGlobal();
            BoardPortOutWriteSingleState(BOARD_OUT_MCU_PC4_ARM_CW1,ioRealState1);
            BoardPortOutWriteSingleState(BOARD_OUT_MCU_PC5_ARM_CCW1,ioRealState2);
            //解锁
            CoreUnlockIntGlobal(lastIntState);
            break;
        case BOARD_DC_MOTOR_2:
            if(setState == BOARD_DC_MOTOR_STATE_CW)
            {
                ioRealState1 = VALID_LEVEL_BOARD_DC_MOTOR2;
                ioRealState2 = PIN_INVALID_LEVEL(VALID_LEVEL_BOARD_DC_MOTOR2);
            }
            else if(setState == BOARD_DC_MOTOR_STATE_CCW)
            {
                ioRealState1 = PIN_INVALID_LEVEL(VALID_LEVEL_BOARD_DC_MOTOR2);
                ioRealState2 = VALID_LEVEL_BOARD_DC_MOTOR2;
            }
            else
            {
                ioRealState1 = PIN_INVALID_LEVEL(VALID_LEVEL_BOARD_DC_MOTOR2);
                ioRealState2 = PIN_INVALID_LEVEL(VALID_LEVEL_BOARD_DC_MOTOR2);
            }
            //上锁,防止烧坏
            lastIntState = CoreLockIntGlobal();
            BoardPortOutWriteSingleState(BOARD_OUT_MCU_PA6_ARM_CW2,ioRealState1);
            BoardPortOutWriteSingleState(BOARD_OUT_MCU_PA7_ARM_CCW2,ioRealState2);
            //解锁
            CoreUnlockIntGlobal(lastIntState);
            break;
        case BOARD_DC_MOTOR_3:
            if(setState == BOARD_DC_MOTOR_STATE_CW)
            {
                ioRealState1 = VALID_LEVEL_BOARD_DC_MOTOR3;
                ioRealState2 = PIN_INVALID_LEVEL(VALID_LEVEL_BOARD_DC_MOTOR3);
            }
            else if(setState == BOARD_DC_MOTOR_STATE_CCW)
            {
                ioRealState1 = PIN_INVALID_LEVEL(VALID_LEVEL_BOARD_DC_MOTOR3);
                ioRealState2 = VALID_LEVEL_BOARD_DC_MOTOR3;
            }
            else
            {
                ioRealState1 = PIN_INVALID_LEVEL(VALID_LEVEL_BOARD_DC_MOTOR3);
                ioRealState2 = PIN_INVALID_LEVEL(VALID_LEVEL_BOARD_DC_MOTOR3);
            }
            //上锁,防止烧坏
            lastIntState = CoreLockIntGlobal();
            BoardPortOutWriteSingleState(BOARD_OUT_MCU_PA4_ARM_CW3,ioRealState1);
            BoardPortOutWriteSingleState(BOARD_OUT_MCU_PA5_ARM_CCW3,ioRealState2);
            //解锁
            CoreUnlockIntGlobal(lastIntState);
            break;
        default:
            break;
    }
}







