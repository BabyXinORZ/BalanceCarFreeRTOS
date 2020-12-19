#ifndef MOTOR_H
#define MOTOR_H
#include "main.h"

// #define Fric_UP 1400
// #define Fric_DOWN 1300
// #define Fric_OFF 1000

extern void motor_PWM_configuration(void);
extern void encoder_PWM_configuration(void);
// extern void fric_off(void);
extern void motor1_on(uint16_t cmd);
extern void motor2_on(uint16_t cmd);
extern void motor3_on(uint16_t cmd);
extern void motor4_on(uint16_t cmd);
extern void wheel1_control(fp32 velocity, bool_t direction);
extern void wheel2_control(fp32 velocity, bool_t direction);
#endif
