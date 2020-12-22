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
#include "angle_loop_task.h"
#include "pid.h"
#include "motor.h"
#include "wheel_task.h"
#include "INS_task.h"


//姿态角 单位度

fp32 a = 0;
fp32 b = 0;
fp32 pitch = 0;

PidTypeDef AngleLoopPID;
fp32 angle_loop_pid[3] = {0.5, 0.0001, 1.5};
int clear = 0;
fp32 angleTarget = 0.0;
fp32 tempVel = 0.0;

void AngleLoopTask(void *pvParameters)
{
  volatile fp32 *motorTarget;
  const volatile fp32 *currentAngle;
  motorTarget = getMotorVelTargetsPoint();
  currentAngle = get_INS_angle_point();
  motorTarget[0] = 0;
  motorTarget[1] = 0;
  PID_Init(&AngleLoopPID, PID_POSITION, angle_loop_pid, 5, 5);
  while (1)
  {
    if (clear)
    {
      PID_clear(&AngleLoopPID); 
      motor1_on(0);
      motor2_on(0);
      motor3_on(0);
      motor4_on(0);
			vTaskDelay(50);
      continue;
    }


    AngleLoopPID.Kp = angle_loop_pid[0];
    AngleLoopPID.Ki = angle_loop_pid[1];  
    AngleLoopPID.Kd = angle_loop_pid[2];

    pitch = -*(currentAngle+INS_ROLL_ADDRESS_OFFSET)*57.3f;
    tempVel = PID_Calc(&AngleLoopPID,angleTarget,pitch);





    if (tempVel>20)
    {
      tempVel = 20;
    }
    
    if (tempVel<-20)
    {
      tempVel = -20;
    }
    
    

    motorTarget[0] = tempVel;
    motorTarget[1] = tempVel; 
    
    // if (tempVel>=0)
    // {
    //   motor1_on(tempVel);
    //   motor2_on(0);
    // }else
    // {
    //   motor1_on(0);
    //   motor2_on(-tempVel);
    // }

    // if (tempVel>=0)
    // {
    //   motor3_on(tempVel);
    //   motor4_on(0);
    // }else
    // {
    //   motor3_on(0);
    //   motor4_on(-tempVel);
    // }
    
    vTaskDelay(20);
  }
  
    
}
