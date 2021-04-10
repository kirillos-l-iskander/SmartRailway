#include "Motor.h"

typedef struct
{
	GPIO_ID_t gpioId;
	GPIO_PIN_t gpioPin;
	TIMER_ID_t timerId;
	TIMER_CHANNEL_t timerChannel;
	MOTOR_SPEED_t speed;
}	Motor_t;

static Motor_t motor[ MOTOR_ID_MAX ];

void initMotorHardware( MOTOR_ID_t id );
void setMotorHardwareSpeed( MOTOR_ID_t id, MOTOR_SPEED_t speed );

void Motor_init( MOTOR_ID_t id, GPIO_ID_t gpioId, GPIO_PIN_t gpioPin, TIMER_ID_t timerId, TIMER_CHANNEL_t timerChannel )
{
	motor[ id ].gpioId = gpioId;
	motor[ id ].gpioPin = gpioPin;
	motor[ id ].timerId = timerId;
	motor[ id ].timerChannel = timerChannel;
	initMotorHardware( id );
}

void Motor_update( void *paramter )
{
	MOTOR_ID_t id = (MOTOR_ID_t) paramter;
	setMotorHardwareSpeed( id, motor[ id ].speed );
}

void Motor_setSpeed( MOTOR_ID_t id, MOTOR_SPEED_t speed )
{
	motor[ id ].speed = speed;
}

MOTOR_SPEED_t Motor_getSpeed( MOTOR_ID_t id )
{
	return motor[ id ].speed;
}

void initMotorHardware( MOTOR_ID_t id )
{
	Gpio_initPin( motor[ id ].gpioId, motor[ id ].gpioPin, GPIO_MODE_OUTPUT_DHZ, GPIO_TYPE_AF_PP );
	Timer_init( motor[ id ].timerId, TIMER_MODE_NORMAL, 10, motor[ id ].timerChannel, TIMER_EVENT_PWM_NONINVERT );
}

void setMotorHardwareSpeed( MOTOR_ID_t id, MOTOR_SPEED_t speed )
{
	if( speed == MOTOR_SPEED_0 )
	{
		Timer_setPwmDutyCycle( motor[ id ].timerId, motor[ id ].timerChannel, (TIMER_PWM_t)0 );
	}else if( speed == MOTOR_SPEED_10 )
	{
		Timer_setPwmDutyCycle( motor[ id ].timerId, motor[ id ].timerChannel, (TIMER_PWM_t)10 );
	}else if( speed == MOTOR_SPEED_20 )
	{
		Timer_setPwmDutyCycle( motor[ id ].timerId, motor[ id ].timerChannel, (TIMER_PWM_t)20 );
	}else if( speed == MOTOR_SPEED_30 )
	{
		Timer_setPwmDutyCycle( motor[ id ].timerId, motor[ id ].timerChannel, (TIMER_PWM_t)30 );
	}else if( speed == MOTOR_SPEED_40 )
	{
		Timer_setPwmDutyCycle( motor[ id ].timerId, motor[ id ].timerChannel, (TIMER_PWM_t)40 );
	}else if( speed == MOTOR_SPEED_50 )
	{
		Timer_setPwmDutyCycle( motor[ id ].timerId, motor[ id ].timerChannel, (TIMER_PWM_t)50 );
	}else if( speed == MOTOR_SPEED_60 )
	{
		Timer_setPwmDutyCycle( motor[ id ].timerId, motor[ id ].timerChannel, (TIMER_PWM_t)60 );
	}else if( speed == MOTOR_SPEED_70 )
	{
		Timer_setPwmDutyCycle( motor[ id ].timerId, motor[ id ].timerChannel, (TIMER_PWM_t)70 );
	}else if( speed == MOTOR_SPEED_80 )
	{
		Timer_setPwmDutyCycle( motor[ id ].timerId, motor[ id ].timerChannel, (TIMER_PWM_t)80 );
	}else if( speed == MOTOR_SPEED_90 )
	{
		Timer_setPwmDutyCycle( motor[ id ].timerId, motor[ id ].timerChannel, (TIMER_PWM_t)90 );
	}else if( speed == MOTOR_SPEED_100 )
	{
		Timer_setPwmDutyCycle( motor[ id ].timerId, motor[ id ].timerChannel, (TIMER_PWM_t)100 );
	}else
	{

	}
}
