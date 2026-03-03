#ifndef MOTOR_H_
#define MOTOR_H_



#include "main.h"



void Motor_Init(void);

void Motor_Left(int speed);

void Motor_Right(int speed);

void Motor_SetSpeed(int left, int right);

void Motor_Stop(void);



#endif
