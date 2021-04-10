#ifndef TRAIN_TASK_H
#define	TRAIN_TASK_H

#include "Config.h"
#include "Nrf.h"
#include "Rtryencdr.h"
#include "Pid.h"
#include "Motor.h"

typedef enum
{
	TRAIN_ID_1,
	TRAIN_ID_MAX,
}	TRAIN_ID_t;

void Train_init( TRAIN_ID_t id, NRF_ID_t xNrfId, RTRYENCDR_ID_t xRtryencdrId, PID_ID_t xPidId, MOTOR_ID_t xMotorId );
void Train_update( void *paramter );

#endif	/* TRAIN_TASK_H */
