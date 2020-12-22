#include "encoder_task.h"
#include "stm32f4xx.h"

#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "task.h"

fp32 Encoder_velocity[2] = {0.0, 0.0};


void EcoderTask(void *pvParameters){
    // EncoderA_velocity = (fp32*)malloc(sizeof(fp32));
    // EncoderB_velocity = (fp32*)malloc(sizeof(fp32));
    int16_t countA;
    int16_t countB;
    int16_t dt_ms = 10;
    // int32_t maxCount = 50;
    while (1)
    {
        countA = (int16_t)TIM5->CNT;
        countB = (int16_t)TIM2->CNT;
        // countA 
        TIM5->CNT = 0;
        TIM2->CNT = 0;  
        Encoder_velocity[0] = (fp32)countA*2.0/65536.0/dt_ms*1000*PI;
        Encoder_velocity[1] = (fp32)countB*2.0/65536.0/dt_ms*1000*PI;
        vTaskDelay(dt_ms);
    }
    
}


const fp32 *get_encoder_point(void){
    return Encoder_velocity;
}
