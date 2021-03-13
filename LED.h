#ifndef LED_H
#define	LED_H

#include "SchedulerConfig.h"
#include "Gpio.h"

typedef enum
{
	LED1_ID,
	LED2_ID
}LED_t;

void Led_init( Id_t id, Id_t xGpioId, uint8_t xPin );
void Led_setState( Id_t id, uint8_t state );

#endif	/* LED_H */
