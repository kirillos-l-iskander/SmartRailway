#ifndef MOTOR_H
#define	MOTOR_H

#include "SchedulerConfig.h"
#include "Gpio.h"
#include "Timer.h"

typedef enum
{
	MOTOR1_ID
}MOTOR_t;

void Motor_init( Id_t id, Id_t xGpioId, uint8_t xPin, Id_t pTimerId );
void Motor_setSpeed( Id_t id, UBaseType_t speed );
UBaseType_t Motor_getSpeed( Id_t id );

#endif	/* MOTOR_H */
