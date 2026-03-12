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

#define NUM_SENSORS 5
#define LINE_THRESHOLD 1300  // ligne noire

extern UART_HandleTypeDef huart3;

/* Fonctions moteur déjà implémentées */
extern void Motor_Left(uint16_t speed);
extern void Motor_Right(uint16_t speed);

/* Tableau capteurs */
uint16_t sensorValues[NUM_SENSORS];

/* Variables PID */
int last_proportional = 0;
long integral = 0;

uint16_t position;

/**
 * Initialisation des capteurs
 */
void LineFollower_Init(void)
{
    TR_Sensors_Init();
}

/**
 * Mise à jour du suiveur de ligne
 */
void LineFollower_Update(void)
{
    /* Lecture capteurs via SPI */
    TR_Update_Sensors(sensorValues);

    /* Calcul position ligne */
    int weighted = 0;
    int sum = 0;

    for(int i = 0; i < NUM_SENSORS; i++)
    {
        if(sensorValues[i] > LINE_THRESHOLD) // détecte ligne noire
        {
            weighted += i * 1000;
            sum += 1;
        }
    }

    if(sum == 0)
    {
        position = 2000;  // centre par défaut
    }
    else
    {
        position = (weighted / sum) * 1000;
    }

    /* PID */

    int proportional = position - 2000;
    int derivative = proportional - last_proportional;

    last_proportional = proportional;

    // PID réajusté pour vitesse max = 200
    int power_difference = proportional / 10 + derivative * 8;

    /* Limitation de la correction */
    const int maximum = 200;   // Vitesse max PWM

    if(power_difference > maximum)
        power_difference = maximum;

    if(power_difference < -maximum)
        power_difference = -maximum;

    /* Appliquer vitesse moteurs selon la correction */

    if(power_difference < 0)
    {
        Motor_Left(maximum + power_difference);   // ralentir moteur gauche
        Motor_Right(maximum);                     // moteur droit constant
    }
    else
    {
        Motor_Left(maximum);                      // moteur gauche constant
        Motor_Right(maximum - power_difference);  // ralentir moteur droit
    }

    /* Ligne perdue : arrêter robot */
    if(sensorValues[1] < LINE_THRESHOLD &&
       sensorValues[2] < LINE_THRESHOLD &&
       sensorValues[3] < LINE_THRESHOLD)
    {
        Motor_Left(0);
        Motor_Right(0);
    }

    /* Debug UART */
    char msg[100];

    sprintf(msg,"%d %d %d %d %d | pos=%d\r\n",
            sensorValues[0],
            sensorValues[1],
            sensorValues[2],
            sensorValues[3],
            sensorValues[4],
            position);

    HAL_UART_Transmit(&huart3,(uint8_t*)msg,strlen(msg),100);
}
