#include "motor.h"



extern TIM_HandleTypeDef htim2;

extern TIM_HandleTypeDef htim3;



#define PWM_MAX 1000



void Motor_Init(void)

{

 HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_3);

 HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);

}



void Motor_Left(int speed)
{
    if(speed > PWM_MAX) speed = PWM_MAX;
    if(speed < -PWM_MAX) speed = -PWM_MAX;

    if(speed > 0)
    {
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_SET);
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_RESET);
        __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_3, speed);
    }
    else if(speed < 0)
    {
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_SET);
        __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_3, -speed);
    }
    else
    {
        __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_3, 0);
    }
}




void Motor_Right(int speed)
{
    if(speed > PWM_MAX) speed = PWM_MAX;
    if(speed < -PWM_MAX) speed = -PWM_MAX;

    if(speed > 0)
    {
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET);
        __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, speed);
    }
    else if(speed < 0)
    {
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_SET);
        __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, -speed);
    }
    else
    {
        __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, 0);
    }
}


// Fonction globale
void Motor_SetSpeed(int left, int right)
{
    Motor_Left(left);
    Motor_Right(right);
}

void Motor_Stop(void)

{

 __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_3, 0);

 __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, 0);

}

