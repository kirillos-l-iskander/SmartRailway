#ifndef MOTOR_H
#define	MOTOR_H

#include "SchedulerConfig.h"
#include "Gpio.h"
#include "Timer.h"

void Motor_init( void );
void Motor_setSpeed( Id_t id, UBaseType_t speed );
UBaseType_t Motor_getSpeed( Id_t id );
void Motor_setGpio( Id_t id, Id_t gpio_id, uint8_t pin );
void Motor_setTimer( Id_t id, Id_t timer_id );

#endif	/* MOTOR_H */
