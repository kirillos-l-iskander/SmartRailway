#include "Scheduler.h"

typedef struct
{
	void (*pointer)( void *paramter );
	void *paramter;
	UBaseType_t delay;
	UBaseType_t period;
	UBaseType_t runMe;
}Task_t;

typedef struct
{
	Task_t task[ SCH_MAX_TASKS ];
	UBaseType_t errorCode;
	UBaseType_t interruptFlag;
}Scheduler_t;

volatile static Scheduler_t scheduler;

void Scheduler_init( void )
{
	size_t index = 0;
	for( index = 0; index < SCH_MAX_TASKS; index++ )
	{
		Scheduler_deleteTask( index );
	}
	scheduler.errorCode = 0;
	scheduler.interruptFlag = 0;
}

UBaseType_t Scheduler_addTask( void (*task)( void *paramter ), void *paramter, UBaseType_t delay, UBaseType_t period )
{
	size_t index = 0;
	for( index = 0; index < SCH_MAX_TASKS; index++ )
	{
		if( scheduler.task[ index ].pointer == 0 )
		{
			break;
		}
	}
	if( index == SCH_MAX_TASKS )
	{
		return SCH_MAX_TASKS;
	}
	scheduler.task[ index ].pointer = task;
	scheduler.task[ index ].paramter = paramter;
	scheduler.task[ index ].delay = delay;
	scheduler.task[ index ].period = period;
	scheduler.task[ index ].runMe = 0;
	return index;
}

UBaseType_t Scheduler_deleteTask( UBaseType_t index )
{
	if( scheduler.task[ index ].pointer == 0 )
	{
		return SCH_RETURN_ERROR;
	}
	scheduler.task[ index ].pointer = 0x00000000;
	scheduler.task[ index ].paramter = 0x00000000;
	scheduler.task[ index ].delay = 0;
	scheduler.task[ index ].period = 0;
	scheduler.task[ index ].runMe = 0;
	return SCH_RETURN_NORMAL;
}

void Scheduler_start( void )
{
	Scheduler_initSysTick( SCH_MS_TICK_PERIOD );
}

void Scheduler_reportStatus( void )
{
	if( scheduler.interruptFlag == 1 )
	{
		scheduler.errorCode = SCH_ERROR_LONG_TASK;
	}
	Scheduler_displayError( scheduler.errorCode );
}

void Scheduler_goToSleep( void )
{
	if( scheduler.interruptFlag == 1 )
	{
		scheduler.interruptFlag = 0;
	}
	while( scheduler.interruptFlag == 0 )
	{
		Scheduler_sleepHardware();
	}
	scheduler.interruptFlag = 0;
}

void Scheduler_update( void )
{
	size_t index = 0;
	for( index = 0; index < SCH_MAX_TASKS; index++ )
	{
		if( scheduler.task[ index ].pointer != 0 )
		{
			if( scheduler.task[ index ].delay == 0 )
			{
				scheduler.task[ index ].runMe = 1;
				scheduler.task[ index ].delay = scheduler.task[ index ].period;
			}
			if( scheduler.task[ index ].delay != 0 )
			{
				scheduler.task[ index ].delay--;
			}
		}
	}
	scheduler.interruptFlag = 1;
}

void Scheduler_dispatchTasks( void )
{
	size_t index = 0;
	for( index = 0; index < SCH_MAX_TASKS; index++ )
	{
		if( scheduler.task[ index ].runMe != 0 )
		{
			(*scheduler.task[ index ].pointer)( scheduler.task[ index ].paramter );
			scheduler.task[ index ].runMe = 0;
			if( scheduler.task[ index ].period == 0 )
			{
				Scheduler_deleteTask( index );
			}
		}
	}
	Scheduler_reportStatus();
	Scheduler_goToSleep();
}
