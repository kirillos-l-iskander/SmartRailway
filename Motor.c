#include "Motor.h"

typedef struct
{
	Id_t gpio_id;
	uint8_t pin;
	Id_t timer_id;
}Motor_t;

static Motor_t motor[ MOTOR_NUMBER ];

void Motor_init( void )
{
	size_t id = 0;
	for ( id = 0; id < MOTOR_NUMBER; id++ )
	{
		Gpio_initPin( motor[ id ].gpio_id, motor[ id ].pin, OUTPUT_AF );
		Timer_initPwm( motor[ id ].timer_id );
	}
}

void Motor_setSpeed( Id_t id, UBaseType_t speed )
{
	Timer_setPwmDutyCycle( motor[ id ].timer_id, speed );
}

UBaseType_t Motor_getSpeed( Id_t id )
{
	return Timer_getPwmDutyCycle( motor[ id ].timer_id );
}

void Motor_setGpio( Id_t id, Id_t gpio_id, uint8_t pin )
{
	motor[ id ].gpio_id = gpio_id;
	motor[ id ].pin = pin;
}

void Motor_setTimer( Id_t id, Id_t timer_id )
{
	motor[ id ].timer_id = timer_id;
}
