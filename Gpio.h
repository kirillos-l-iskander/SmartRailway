#ifndef GPIO_H
#define GPIO_H

#include "Config.h"

typedef enum
{
	GPIO_ID_A = 0,
	GPIO_ID_B,
	GPIO_ID_C,
	GPIO_ID_MAX,
}	GPIO_ID_t;

typedef enum
{
	GPIO_PIN_0 = 0,
	GPIO_PIN_1,
	GPIO_PIN_2,
	GPIO_PIN_3,
	GPIO_PIN_4,
	GPIO_PIN_5,
	GPIO_PIN_6,
	GPIO_PIN_7,
	GPIO_PIN_8,
	GPIO_PIN_9,
	GPIO_PIN_10,
	GPIO_PIN_11,
	GPIO_PIN_12,
	GPIO_PIN_13,
	GPIO_PIN_14,
	GPIO_PIN_15,
	GPIO_PIN_MAX = 0xFFFF,
}	GPIO_PIN_t;

typedef enum
{
	GPIO_MODE_INPUT = 0,
	GPIO_MODE_OUTPUT_DHZ = 1,
	GPIO_MODE_OUTPUT_10MHZ = 1,
	GPIO_MODE_OUTPUT_2MHZ = 2,
	GPIO_MODE_OUTPUT_50MHZ = 3,
}	GPIO_MODE_t;

typedef enum
{
	GPIO_TYPE_ANALOG = 0x00,
	GPIO_TYPE_FLOATING = 0x04,
	GPIO_TYPE_PULLUP = 0x08,
	GPIO_TYPE_PULLDOWN = 0x08,
	GPIO_TYPE_GP_PP = 0x00,
	GPIO_TYPE_GP_OD = 0x04,
	GPIO_TYPE_AF_PP = 0x08,
	GPIO_TYPE_AF_OD = 0x0C,
}	GPIO_TYPE_t;

typedef enum
{
	GPIO_STATE_LOW = 0,
	GPIO_STATE_HIGH,
	GPIO_STATE_MAX = 0xFFFF,
}	GPIO_STATE_t;

void Gpio_initPin( GPIO_ID_t id, GPIO_PIN_t pin, GPIO_MODE_t mode, GPIO_TYPE_t type );
void Gpio_setPinState( GPIO_ID_t id, GPIO_PIN_t pin, GPIO_STATE_t state );
GPIO_STATE_t Gpio_getPinState( GPIO_ID_t id, GPIO_PIN_t pin );
void Gpio_setPortState( GPIO_ID_t id, GPIO_PIN_t pins, GPIO_STATE_t state );
GPIO_STATE_t Gpio_getPortState( GPIO_ID_t id, GPIO_PIN_t pins );

#endif /* PORT_H */
