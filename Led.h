#ifndef LED_H
#define	LED_H

#include "Config.h"
#include "Gpio.h"

typedef enum
{
	LED_ID_1 = 0,
	LED_ID_2,
	LED_ID_MAX,
}	LED_ID_t;

typedef enum
{
	LED_STATE_OFF = 0,
	LED_STATE_ON,
}	LED_STATE_t;

typedef enum
{
	LED_BLINK_0MS = 0,
	LED_BLINK_100MS = 100,
	LED_BLINK_500MS = 500,
	LED_BLINK_1000MS = 1000,
	LED_BLINK_5000MS = 5000,
}	LED_BLINK_t;

void Led_init( LED_ID_t id, GPIO_ID_t gpioId, GPIO_PIN_t gpioPin );
void Led_update( void *paramter );
void Led_setState( LED_ID_t id, LED_STATE_t state, LED_BLINK_t period );

#endif	/* LED_H */
