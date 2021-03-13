#ifndef LED_TASK_H
#define	LED_TASK_H

#include "SchedulerConfig.h"
#include "Led.h"

void LedTask_init( Id_t id, Id_t xGpioId, uint8_t xPin );
void LedTask_setState( Id_t id, uint8_t state, uint16_t period );
void LedTask_update( void *paramter );

#endif	/* LED_TASK_H */
