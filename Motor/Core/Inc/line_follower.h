/*
 * line_follower.h
 *
 *  Created on: Mar 12, 2026
 *      Author: christhoyek
 */

#ifndef LINE_FOLLOWER_H
#define LINE_FOLLOWER_H

#include "main.h"
#include "usart.h"
#include "motor.h"
#include "TRSensors.h"



// lecture des capteurs
void LineFollower_Init(void);

// algorithme suivi de ligne
void LineFollower_Update(void);

#endif
