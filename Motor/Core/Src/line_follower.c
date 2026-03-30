/*
 * line_follower.c
 *
 *  Created on: Mar 12, 2026
 *      Author: christhoyek
 */

#include "line_follower.h"
#include "TRSensors.h"
#include <stdio.h>
#include <string.h>
#include "motor.h"

#define NUM_SENSORS 5
#define LINE_THRESHOLD 2400   // noir ~2368, blanc > 2400
#define BASE_SPEED 200
#define LOST_LINE_SPEED 0

extern UART_HandleTypeDef huart3;

/* Tableau capteurs */
uint16_t sensorValues[NUM_SENSORS];

/* Variables PID */
static int last_proportional = 0;
static long integral = 0;
static uint16_t position = 2000;

/**
 * Initialisation des capteurs et moteurs
 */
void LineFollower_Init(void)
{
    TR_Sensors_Init();
    Motor_Init();
}

/**
 * Mise à jour du suiveur de ligne
 */
void LineFollower_Update(void)
{
    TR_Update_Sensors(sensorValues);

    int weighted = 0;
    int sum = 0;
    int black_detected = 0;

    /* Détection de la ligne noire :
       noir  -> valeur plus faible (~2368)
       blanc -> valeur plus grande (>2400)
    */
    for (int i = 0; i < NUM_SENSORS; i++)
    {
        if (sensorValues[i] <= LINE_THRESHOLD)
        {
            weighted += i * 1000;
            sum += 1;
            black_detected = 1;
        }
    }

    /* Calcul de position :
       0    = extrême gauche
       1000 = gauche
       2000 = centre
       3000 = droite
       4000 = extrême droite
    */
    if (sum > 0)
    {
        position = weighted / sum;
    }
    else
    {
        /* si ligne perdue, garder une direction cohérente */
        if (last_proportional < 0)
            position = 0;
        else
            position = 4000;
    }

    /* PID / PD */
    int proportional = (int)position - 2000;
    int derivative = proportional - last_proportional;
    integral += proportional;
    last_proportional = proportional;

    /* Réglage doux */
    int power_difference = (proportional / 18) + (derivative * 6);

    if (power_difference > BASE_SPEED)
        power_difference = BASE_SPEED;
    if (power_difference < -BASE_SPEED)
        power_difference = -BASE_SPEED;

    int left_speed;
    int right_speed;

    if (power_difference < 0)
    {
        left_speed = BASE_SPEED + power_difference;
        right_speed = BASE_SPEED;
    }
    else
    {
        left_speed = BASE_SPEED;
        right_speed = BASE_SPEED - power_difference;
    }

    if (left_speed < 0) left_speed = 0;
    if (right_speed < 0) right_speed = 0;

    if (!black_detected)
    {
        Motor_Left(LOST_LINE_SPEED);
        Motor_Right(LOST_LINE_SPEED);
    }
    else
    {
        Motor_Left(left_speed);
        Motor_Right(right_speed);
    }

    /* Debug UART */
    char msg[128];
    sprintf(msg,
            "IR: %4d %4d %4d %4d %4d | pos=%4d | L=%3d R=%3d\r\n",
            sensorValues[0],
            sensorValues[1],
            sensorValues[2],
            sensorValues[3],
            sensorValues[4],
            position,
            left_speed,
            right_speed);

    HAL_UART_Transmit(&huart3, (uint8_t*)msg, strlen(msg), 100);
}
