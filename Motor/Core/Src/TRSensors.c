/*
 * TRSensors.c
 *
 *  Created on: Mar 12, 2026
 *      Author: christhoyek
 */


#include "TRSensors.h"


void TR_Sensors_Init(void) {
    HAL_GPIO_WritePin(TR_CS_PORT, TR_CS_PIN, GPIO_PIN_SET);
}



uint16_t TR_TLC2543_ReadChannel(uint8_t channel) {
    uint16_t tx_data;
    uint16_t rx_data = 0;

    if (channel > 10) return 0;


    tx_data = (channel << 8);

    HAL_GPIO_WritePin(TR_CS_PORT, TR_CS_PIN, GPIO_PIN_RESET);


    if (HAL_SPI_TransmitReceive(&hspi1, (uint8_t*)&tx_data, (uint8_t*)&rx_data, 1, 10) != HAL_OK) {
        HAL_GPIO_WritePin(TR_CS_PORT, TR_CS_PIN, GPIO_PIN_SET);
        return 0;
    }


    HAL_GPIO_WritePin(TR_CS_PORT, TR_CS_PIN, GPIO_PIN_SET);

    return rx_data;
}


void TR_Update_Sensors(uint16_t *sensor_array) {

    TR_TLC2543_ReadChannel(0);


    sensor_array[0] = TR_TLC2543_ReadChannel(1);


    sensor_array[1] = TR_TLC2543_ReadChannel(2);


    sensor_array[2] = TR_TLC2543_ReadChannel(3);


    sensor_array[3] = TR_TLC2543_ReadChannel(4);


    sensor_array[4] = TR_TLC2543_ReadChannel(0);
}
