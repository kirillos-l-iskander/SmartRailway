#ifndef TRAIN_TASK_H
#define	TRAIN_TASK_H

#include "SchedulerConfig.h"
#include "NrfTask.h"
#include "Encoder.h"
#include "Pid.h"
#include "Motor.h"

typedef enum
{
	TRAIN1_ID
}TRAIN_t;

void TrainTask_init( Id_t id );
void TrainTask_update( void *paramter );

#endif	/* TRAIN_TASK_H */
