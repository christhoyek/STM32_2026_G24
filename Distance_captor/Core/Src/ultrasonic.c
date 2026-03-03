#include "ultrasonic.h"

static GPIO_TypeDef* us_gpio = GPIOA;
static uint16_t us_pin = GPIO_PIN_8;

// Fonction interne pour un délai précis en microsecondes
static void delay_us(uint32_t us) {
    uint32_t startTick = DWT->CYCCNT;
    uint32_t delayTicks = us * (HAL_RCC_GetHCLKFreq() / 1000000);
    while (DWT->CYCCNT - startTick < delayTicks);
}

void Ultrasonic_Init(void) {
    // Le compteur DWT doit être activé pour que delay_us fonctionne
    CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
    DWT->CYCCNT = 0;
    DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;

    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = us_pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    HAL_GPIO_Init(us_gpio, &GPIO_InitStruct);

    HAL_GPIO_WritePin(us_gpio, us_pin, GPIO_PIN_RESET);
    HAL_Delay(50); // Attente stabilisation capteur
}

float Ultrasonic_Read(void) {
    uint32_t start = 0, stop = 0;
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    // 1. Envoyer le Pulse Trigger (10us)
    GPIO_InitStruct.Pin = us_pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    HAL_GPIO_Init(us_gpio, &GPIO_InitStruct);

    HAL_GPIO_WritePin(us_gpio, us_pin, GPIO_PIN_SET);
    delay_us(10);
    HAL_GPIO_WritePin(us_gpio, us_pin, GPIO_PIN_RESET);

    // 2. Passer immédiatement en Entrée pour l'Echo
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(us_gpio, &GPIO_InitStruct);

    // 3. Attendre le front montant (début de l'écho) avec timeout
    uint32_t timeout = 100000; // Timeout de sécurité
    while(HAL_GPIO_ReadPin(us_gpio, us_pin) == GPIO_PIN_RESET && timeout--);
    start = DWT->CYCCNT;

    // 4. Attendre le front descendant (fin de l'écho)
    timeout = 100000;
    while(HAL_GPIO_ReadPin(us_gpio, us_pin) == GPIO_PIN_SET && timeout--);
    stop = DWT->CYCCNT;

    if(timeout == 0 || stop <= start) return -1.0f;

    // 5. Calcul de la distance
    // temps (us) = cycles / (Fréquence CPU en MHz)
    float time_us = (float)(stop - start) / (HAL_RCC_GetHCLKFreq() / 1000000.0f);

    // Distance = (temps * vitesse du son 0.0343 cm/us) / 2
    return (time_us * 0.0343f) / 2.0f;
}
