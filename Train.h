#ifndef TRAIN_TASK_H
#define	TRAIN_TASK_H

#include "SchedulerConfig.h"
#include "NrfTask.h"
#include "Encoder.h"
#include "Pid.h"
#include "Motor.h"

void TrainTask_init( void );
void TrainTask_update( void *paramter );

#endif	/* TRAIN_TASK_H */
