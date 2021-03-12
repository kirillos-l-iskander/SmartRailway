#include "Train.h"

void TrainTask_init( Id_t id )
{
	Pid_init( id );
}

void TrainTask_update( void *paramter )
{
	Id_t id = (Id_t) paramter;
	int32_t correctionSpeed = 0;
	int32_t requiredSpeed = NrfTaskSlave_getBuffer( id , 2 );
	int32_t currentSpeed = ( uint16_t ) Encoder_getSpeed( id );
	
	requiredSpeed = ( int32_t ) ( ( ( (float) requiredSpeed / 6 ) * 32 ) / 10 );
	
	Pid_setError( id, requiredSpeed, currentSpeed );
	Pid_update( (void *) id );
	correctionSpeed = ( int32_t ) Motor_getSpeed( id ) + Pid_getCorrection( id );
	
	if( requiredSpeed == 0 && currentSpeed == 0 )
	{
		Motor_setSpeed( id, (uint16_t) 0 );
	}else if( ( requiredSpeed == 0 ) && ( currentSpeed > 0 ) )
	{
		Motor_setSpeed( id, (uint16_t) 0 );
	}else if( ( requiredSpeed > 0 ) && ( currentSpeed == 0 ) )
	{
		Motor_setSpeed( id, (uint16_t) 50000 );
	}else if( ( correctionSpeed >= 0 ) && ( correctionSpeed <= 65535 ) )
	{
		Motor_setSpeed( id, ( uint16_t ) correctionSpeed );
	}
	
	Encoder_reset( id );
}
