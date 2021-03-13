#ifndef TIMER_H
#define	TIMER_H

#include "SchedulerConfig.h"

typedef enum
{
	TIMER1_ID,
	TIMER2_ID,
	TIMER3_ID
}TIMER_t;

void Timer_initCounter( Id_t id );
void Timer_setCounterValue( Id_t id, UBaseType_t counterValue );
UBaseType_t Timer_getCounterValue( Id_t id );
void Timer_initPwm( Id_t id );
void Timer_setPwmDutyCycle( Id_t id, UBaseType_t pwmDutyCycle );
UBaseType_t Timer_getPwmDutyCycle( Id_t id );

#endif	/* TIMER_H */
