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


#if INCLUDE_uxTaskGetStackHighWaterMark
#endif

//姿态角 单位度



PidTypeDef wheelPIDA;
PidTypeDef wheelPIDB;
fp32 motor_speed_pid1[3] = {200, 1, 200};
fp32 motor_speed_pid2[3] = {200, 1, 200};
fp32 motorTargets[2] = {0.0f,0.0f};
fp32 tempMotorVelA = 0.0;
fp32 tempMotorVelB = 0.0;
fp32 tempCmdA = 0;
fp32 tempCmdB = 0;
int mode = 0;

const volatile fp32 *velocity;


void WheelTask(void *pvParameters)
{
    
		velocity = get_encoder_point();
    // const volatile fp32 *velocityB = get_encoderB_point();

    //底盘速度环pid值
    //底盘旋转环pid值
    uint8_t i;

    PID_Init(&wheelPIDA, PID_POSITION, motor_speed_pid1, 100.0, 100.0);
    PID_Init(&wheelPIDB, PID_POSITION, motor_speed_pid2, 100.0, 100.0);
    
    while (1)
    {
			wheelPIDA.Kp = motor_speed_pid1[0];
			wheelPIDA.Ki = motor_speed_pid1[1];
			wheelPIDA.Kd = motor_speed_pid1[2];
			wheelPIDB.Kp = motor_speed_pid2[0];
			wheelPIDB.Ki = motor_speed_pid2[1];
			wheelPIDB.Kd = motor_speed_pid2[2];
      tempMotorVelA = *(velocity);
      tempMotorVelB = *(velocity+1);
     tempCmdA += PID_Calc(&wheelPIDA,motorTargets[0],tempMotorVelA);
     tempCmdB += PID_Calc(&wheelPIDB,motorTargets[1],tempMotorVelB);
      if (tempCmdA>2000)
      {
        tempCmdA = 2000;
      }
      if (tempCmdA<-2000)
      {
        tempCmdA = -2000;
      }
      if (tempCmdB>2000)
      {
        tempCmdB = 2000;
      }
      if (tempCmdB<-2000)
      {
        tempCmdB = -2000;
      }

      if (tempCmdA>=0)
      {
        motor1_on(2000);
        motor2_on(2000-tempCmdA);
      }else
      { 
        motor1_on(2000+tempCmdA);
        motor2_on(2000);
      }

      
      if (tempCmdB>=0)
      {
        motor3_on(2000);
        motor4_on(2000-tempCmdB);
      }else
      { 
        motor3_on(2000+tempCmdB);
        motor4_on(2000);
      }      

    // motor1_on(tempCmdA);
    // motor2_on(tempCmdB);
    // motor3_on(tempCmdA);
    // motor4_on(tempCmdB);

      vTaskDelay(20);
    }
}


fp32 *getMotorVelTargetsPoint(){
  return motorTargets;
}