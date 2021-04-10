#ifndef MOTOR_H
#define	MOTOR_H

#include "Config.h"
#include "Gpio.h"
#include "Timer.h"

typedef enum
{
	MOTOR_ID_1 = 0,
	MOTOR_ID_MAX,
}	MOTOR_ID_t;

typedef enum
{
	MOTOR_SPEED_0 = 0,
	MOTOR_SPEED_10 = 10,
	MOTOR_SPEED_20 = 20,
	MOTOR_SPEED_30 = 30,
	MOTOR_SPEED_40 = 40,
	MOTOR_SPEED_50 = 50,
	MOTOR_SPEED_60 = 60,
	MOTOR_SPEED_70 = 70,
	MOTOR_SPEED_80 = 80,
	MOTOR_SPEED_90 = 90,
	MOTOR_SPEED_100 = 100
}	MOTOR_SPEED_t;

void Motor_init( MOTOR_ID_t id, GPIO_ID_t gpioId, GPIO_PIN_t gpioPin, TIMER_ID_t timerId, TIMER_CHANNEL_t timerChannel );
void Motor_update( void *paramter );
void Motor_setSpeed( MOTOR_ID_t id, MOTOR_SPEED_t speed );
MOTOR_SPEED_t Motor_getSpeed( MOTOR_ID_t id );

#endif	/* MOTOR_H */
