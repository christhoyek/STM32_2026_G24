
#include "sht31.h"

HAL_StatusTypeDef SHT31_Init(I2C_HandleTypeDef *hi2c)
{
    uint8_t cmd[2] = {0x30, 0xA2}; // Soft reset
    return HAL_I2C_Master_Transmit(hi2c, SHT31_ADDR, cmd, 2, HAL_MAX_DELAY);
}

HAL_StatusTypeDef SHT31_Read(I2C_HandleTypeDef *hi2c,
                             float *temperature,
                             float *humidity)
{
    uint8_t cmd[2]  = {0x24, 0x00}; // High repeatability
    uint8_t data[6];

    if (HAL_I2C_Master_Transmit(hi2c, SHT31_ADDR, cmd, 2, HAL_MAX_DELAY) != HAL_OK)
        return HAL_ERROR;

    HAL_Delay(5);

    if (HAL_I2C_Master_Receive(hi2c, SHT31_ADDR, data, 6, HAL_MAX_DELAY) != HAL_OK)
        return HAL_ERROR;

    uint16_t rawT = (data[0] << 8) | data[1];
    uint16_t rawH = (data[3] << 8) | data[4];

    *temperature = -45.0f + 175.0f * ((float)rawT / 65535.0f);
    *humidity    = 100.0f * ((float)rawH / 65535.0f);

    return HAL_OK;
}
