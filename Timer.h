#ifndef TIMER_H
#define	TIMER_H

#include "SchedulerConfig.h"

#define TIMER1_ID	( 0 )
#define TIMER2_ID	( 1 )
#define TIMER3_ID	( 2 )

void Timer_initCounter( Id_t id );
void Timer_setCounterValue( Id_t id, UBaseType_t counterValue );
UBaseType_t Timer_getCounterValue( Id_t id );
void Timer_initPwm( Id_t id );
void Timer_setPwmDutyCycle( Id_t id, UBaseType_t pwmDutyCycle );
UBaseType_t Timer_getPwmDutyCycle( Id_t id );

#endif	/* TIMER_H */
