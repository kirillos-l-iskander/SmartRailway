#ifndef LED_H
#define	LED_H

#include "SchedulerConfig.h"
#include "Gpio.h"

void Led_init( Id_t id, Id_t gpio_id, uint8_t pin );
void Led_setState( Id_t id, uint8_t state );

#endif	/* LED_H */
