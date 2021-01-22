#ifndef GPIO_H
#define GPIO_H

#include "CONFIG.h"
#include "IO.h"
#include "PORT.h"
#include "CORE.h"
#include "SCH_CONFIG.h"
#include "SCH.h"
#include "GPIO.h"
#include "AFIO.h"

#define INPUT (0)
#define OUTPUT (1)
#define LOW (0)
#define HIGH (1)

void GPIO_InitPin(GPIO_TypeDef* GPIOx, uint8_t PINx, uint8_t Mode);
void GPIO_SetPinState(GPIO_TypeDef* GPIOx, uint8_t PINx, uint8_t State);
uint8_t GPIO_GetPinState(GPIO_TypeDef* GPIOx, uint8_t PINx);
void GPIO_SetPortState(GPIO_TypeDef* GPIOx, uint16_t PINx, uint16_t State);
uint16_t GPIO_GetPortState(GPIO_TypeDef* GPIOx);

#endif /* GPIO_H */
