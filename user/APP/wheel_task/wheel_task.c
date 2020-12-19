/**
  ****************************(C) COPYRIGHT 2016 DJI****************************
  * @file       user_task.c/h
  * @brief      一个普通心跳程序，如果设备无错误，绿灯1Hz闪烁,然后获取姿态角
  * @note       
  * @history
  *  Version    Date            Author          Modification
  *  V1.0.0     Dec-26-2018     RM              1. 完成
  *
  @verbatim
  ==============================================================================

  ==============================================================================
  @endverbatim
  ****************************(C) COPYRIGHT 2016 DJI****************************
  */

#include "main.h"

#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "task.h"

// #include "led.h"
#include "encoder_task.h"
#include "pid.h"
#include "motor.h"
#include "wheel_task.h"

#define user_is_error() toe_is_error(errorListLength)

#if INCLUDE_uxTaskGetStackHighWaterMark
#endif

//姿态角 单位度



PidTypeDef wheelAPID;
fp32 motor_speed_pid[3] = {10.0, 0, 0};
int cmd = 0;
fp32 target = 1;
fp32 tempMotorVelA = 1.0;
fp32 tempCmd = 0;

  const volatile fp32 *velocity;


void WheelTask(void *pvParameters)
{

    //获取姿态角指针
		velocity = get_encoder_point();
    // const volatile fp32 *velocityB = get_encoderB_point();

    //底盘速度环pid值
    //底盘旋转环pid值
    uint8_t i;

    PID_Init(&wheelAPID, PID_POSITION, motor_speed_pid, 100.0, 100.0);
    
    while (1)
    {
			wheelAPID.Kp = motor_speed_pid[0];
			wheelAPID.Ki = motor_speed_pid[1];
			wheelAPID.Kd = motor_speed_pid[2];
      tempMotorVelA = *(velocity);
      tempCmd += PID_Calc(&wheelAPID,target,tempMotorVelA);
      if (tempCmd>2000)
      {
        tempCmd = 2000;
      }
      if (tempCmd<0)
      {
        tempCmd = 0;
      }
      
      motor1_on(tempCmd);
      
      motor2_on(0);

      vTaskDelay(50);
    }
}
