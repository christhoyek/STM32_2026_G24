/*
 * ultrasonic.h
 *
 *  Created on: Mar 12, 2026
 *      Author: christhoyek
 */

#ifndef INC_ULTRASONIC_H_
#define INC_ULTRASONIC_H_

#include "stm32l4xx_hal.h"


static void delay_us(uint32_t us);
void Ultrasonic_Init(void);
float Ultrasonic_Read(void);


#endif /* INC_ULTRASONIC_H_ */
