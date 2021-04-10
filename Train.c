#include "Train.h"

typedef struct
{
	NRF_ID_t xNrfId;
	RTRYENCDR_ID_t xRtryencdrId;
	PID_ID_t xPidId;
	MOTOR_ID_t xMotorId;
}	Train_t;

static Train_t train[ TRAIN_ID_MAX ];

void Train_init( TRAIN_ID_t id, NRF_ID_t xNrfId, RTRYENCDR_ID_t xRtryencdrId, PID_ID_t xPidId, MOTOR_ID_t xMotorId )
{
	train[ id ].xNrfId = xNrfId;
	train[ id ].xRtryencdrId = xRtryencdrId;
	train[ id ].xPidId = xPidId;
	train[ id ].xMotorId = xMotorId;
}

void Train_update( void *paramter )
{
	TRAIN_ID_t id = (TRAIN_ID_t) paramter;
	uint8_t nrfRxBuffer = Nrf_getRxBuffer( train[ id ].xNrfId, NRF_NODE_0, 2 );
	
	RTRYENCDR_COUNTER_t rtryencdrCurrentSpeedBuffer = Rtryencdr_getSpeed( train[ id ].xRtryencdrId );

	RTRYENCDR_COUNTER_t rtryencdrRequiredSpeedBuffer = (RTRYENCDR_COUNTER_t) ( ( ( (float) nrfRxBuffer / 6 ) * 32 ) / 10 );
	
	Pid_setError( train[ id ].xPidId, rtryencdrRequiredSpeedBuffer, rtryencdrCurrentSpeedBuffer );
	int32_t pidCorrection = Pid_getCorrection( train[ id ].xPidId );
	
	if( rtryencdrRequiredSpeedBuffer == 0 && rtryencdrCurrentSpeedBuffer == 0 )
	{
		Motor_setSpeed( train[ id ].xMotorId, MOTOR_SPEED_0 );
	}else if( pidCorrection <= 0 )
	{
		Motor_setSpeed( train[ id ].xMotorId, MOTOR_SPEED_0 );
	}else if( pidCorrection <= 10 )
	{
		Motor_setSpeed( train[ id ].xMotorId, MOTOR_SPEED_10 );
	}else if( pidCorrection <= 20 )
	{
		Motor_setSpeed( train[ id ].xMotorId, MOTOR_SPEED_20 );
	}else if( pidCorrection <= 30 )
	{
		Motor_setSpeed( train[ id ].xMotorId, MOTOR_SPEED_30 );
	}else if( pidCorrection <= 40 )
	{
		Motor_setSpeed( train[ id ].xMotorId, MOTOR_SPEED_40 );
	}else if( pidCorrection <= 50 )
	{
		Motor_setSpeed( train[ id ].xMotorId, MOTOR_SPEED_50 );
	}else if( pidCorrection <= 60)
	{
		Motor_setSpeed( train[ id ].xMotorId, MOTOR_SPEED_60 );
	}else if( pidCorrection <= 70 )
	{
		Motor_setSpeed( train[ id ].xMotorId, MOTOR_SPEED_70 );
	}else if( pidCorrection <= 80 )
	{
		Motor_setSpeed( train[ id ].xMotorId, MOTOR_SPEED_80 );
	}else if( pidCorrection <= 90 )
	{
		Motor_setSpeed( train[ id ].xMotorId, MOTOR_SPEED_90 );
	}else if( pidCorrection <= 100 )
	{
		Motor_setSpeed( train[ id ].xMotorId, MOTOR_SPEED_100 );
	}
}
