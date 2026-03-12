/*
 * TRSensors.h
 *
 *  Created on: Mar 12, 2026
 *      Author: christhoyek
 */

#ifndef TRSENSORS_H_
#define TRSENSORS_H_
#include <stdint.h>
#include "main.h"
#include "spi.h"

#define TR_CS_PORT GPIOB
#define TR_CS_PIN  GPIO_PIN_6

void TR_Sensors_Init(void);
uint16_t TR_TLC2543_ReadChannel(uint8_t channel);
void TR_Update_Sensors(uint16_t *sensor_array);



#endif /* INC_TRSENSORS_H_ */
