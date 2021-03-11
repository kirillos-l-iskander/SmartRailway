#include "Train.h"

void TrainTask_init( void )
{
	Encoder_init();
	Pid_init();
	Motor_init();
}

void TrainTask_update( void *paramter )
{
	size_t id = 0;
	for ( id = 0; id < TRAIN_TASK_NUMBER; id++ )
	{
		int32_t correctionSpeed = 0;
		int32_t requiredSpeed = NrfTaskSlave_getBuffer( (Id_t) id , 2 );
		int32_t currentSpeed = ( uint16_t ) Encoder_getSpeed( (Id_t) id );
		
		requiredSpeed = ( int32_t ) ( ( ( (float) requiredSpeed / 6 ) * 32 ) / 10 );
		
		Pid_setError( (Id_t) id, requiredSpeed, currentSpeed );
		Pid_update( NULL );
		correctionSpeed = ( int32_t ) Motor_getSpeed( (Id_t) id ) + Pid_getCorrection( (Id_t) id );
		
		if( requiredSpeed == 0 && currentSpeed == 0 )
		{
			Motor_setSpeed( (Id_t) id, (uint16_t) 0 );
		}else if( ( requiredSpeed == 0 ) && ( currentSpeed > 0 ) )
		{
			Motor_setSpeed( (Id_t) id, (uint16_t) 0 );
		}else if( ( requiredSpeed > 0 ) && ( currentSpeed == 0 ) )
		{
			Motor_setSpeed( (Id_t) id, (uint16_t) 50000 );
		}else if( ( correctionSpeed >= 0 ) && ( correctionSpeed <= 65535 ) )
		{
			Motor_setSpeed( (Id_t) id, ( uint16_t ) correctionSpeed );
		}
		
		Encoder_reset( (Id_t) id );
	}
}
