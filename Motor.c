#include "Motor.h"

typedef struct
{
	Id_t xGpioId;
	uint8_t xPin;
	Id_t xTimerId;
}Motor_t;

static Motor_t motor[ MOTOR_NUMBER ];

void Motor_init( Id_t id, Id_t xGpioId, uint8_t xPin, Id_t xTimerId )
{
	motor[ id ].xGpioId = xGpioId;
	motor[ id ].xPin = xPin;
	motor[ id ].xTimerId = xTimerId;
	Gpio_initPin( motor[ id ].xGpioId, motor[ id ].xPin, OUTPUT_AF );
	Timer_initPwm( motor[ id ].xTimerId );
}

void Motor_setSpeed( Id_t id, UBaseType_t speed )
{
	Timer_setPwmDutyCycle( motor[ id ].xTimerId, speed );
}

UBaseType_t Motor_getSpeed( Id_t id )
{
	return Timer_getPwmDutyCycle( motor[ id ].xTimerId );
}
