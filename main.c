#include "SchedulerConfig.h"
#include "Scheduler.h"

#include "Rcc.h"
#include "Afio.h"
#include "Gpio.h"
#include "Adc.h"
#include "Timer.h"
#include "Spi.h"

#include "Led.h"
#include "LedTask.h"
#include "Infrared.h"
#include "InfraredTask.h"
#include "Lcd.h"
#include "LcdTask.h"
#include "Nrf.h"
#include "NrfTask.h"
#include "Station.h"
#include "Encoder.h"
#include "Pid.h"
#include "Motor.h"
#include "Train.h"

typedef enum
{
	SERVER_NODE,
	STATION1_NODE,
	STATION2_NODE,
	TRAIN1_NODE,
	TRAIN2_NODE
} SmartRailway_t;

#define SERVER_NODE

void SysTick_Handler( void );
UBaseType_t serverTask( void );
UBaseType_t station1Task( void );
UBaseType_t station2Task( void );
UBaseType_t train1Task( void );
UBaseType_t train2Task( void );

static void (*systickInterrupt)( void ) = NULL;

void SysTick_Handler( void )
{
	(*systickInterrupt)();
}

int main( void )
{
	#ifdef SERVER_NODE
	serverTask();
	#endif
	#ifdef STATION1_NODE
	station1Task();
	#endif
	#ifdef STATION2_NODE
	station2Task();
	#endif
	#ifdef TRAIN1_NODE
	train1Task();
	#endif
	#ifdef TRAIN2_NODE
	train2Task();
	#endif
	return SCH_RETURN_NORMAL;
}

UBaseType_t serverTask( void )
{
	Rcc_enableClock( 0, AFIO );
	Rcc_enableClock( 0, GPIOA );
	Rcc_enableClock( 0, GPIOB );
	Rcc_enableClock( 0, GPIOC );
	Rcc_enableClock( 0, SPI2 );
	
	Afio_disableJTAG( 0 );
	
	DELAY_US( 1000 );
	
	LedTask_init( 0, GPIOC_ID, 14 );
	LedTask_init( 1, GPIOC_ID, 13 );

	NrfTaskSlave_setNodeNumber( 0, 0 );
	NrfTaskMaster_init( 0, GPIOA_ID, 9, GPIOA_ID, 8, GPIOB_ID, 12, SPI2_ID );

	DELAY_US( 1000 );

	Scheduler_init();
	Scheduler_addTask( LedTask_update, (void *) 0, 1000, 1 );
	Scheduler_addTask( LedTask_update, (void *) 1, 1000, 1 );
	Scheduler_addTask( NrfTaskMaster_update, (void *) 0, 1000, 1 );
	systickInterrupt = Scheduler_update;
	Scheduler_start();
	while( 1 )
	{
		Scheduler_dispatchTasks();
	}
}

UBaseType_t station1Task( void )
{
	Rcc_enableClock( 0, AFIO );
	Rcc_enableClock( 0, GPIOA );
	Rcc_enableClock( 0, GPIOB );
	Rcc_enableClock( 0, GPIOC );
	Rcc_enableClock( 0, SPI2 );

	Afio_disableJTAG( 0 );
	
	DELAY_US( 1000 );

	LedTask_init( 1, GPIOC_ID, 13 );

	NrfTaskSlave_setNodeNumber( 0, 1 );
	NrfTaskSlave_init( 0, GPIOA_ID, 9, GPIOA_ID, 8, GPIOB_ID, 12, SPI2_ID );

	InfraredTask_init( 0, GPIOB_ID, 7 );
	InfraredTask_init( 1, GPIOB_ID, 8 );
	InfraredTask_init( 2, GPIOB_ID, 9 );
	
	LcdTask_init( 0, GPIOC_ID, 14, GPIOC_ID, 15, GPIOA_ID, 0 );
	
	StationTask_setNodeNumber( 0, 1 );
	StationTask_init( 0 );

	DELAY_US( 1000 );

	Scheduler_init();
	Scheduler_addTask( LedTask_update, (void *) 1, 0, 100 );
	Scheduler_addTask( InfraredTask_update, (void *) 0, 1, 100 );
	Scheduler_addTask( InfraredTask_update, (void *) 1, 1, 100 );
	Scheduler_addTask( InfraredTask_update, (void *) 2, 1, 100 );
	Scheduler_addTask( StationTask_update, (void *) 0, 2, 100 );
	Scheduler_addTask( LcdTask_update, (void *) 0, 3, 5 );
	Scheduler_addTask( NrfTaskSlave_update, (void *) 0, 4, 5 );
	systickInterrupt = Scheduler_update;
	Scheduler_start();
	while( 1 )
	{
		Scheduler_dispatchTasks();
	}
}

UBaseType_t station2Task( void )
{
	Rcc_enableClock( 0, AFIO );
	Rcc_enableClock( 0, GPIOA );
	Rcc_enableClock( 0, GPIOB );
	Rcc_enableClock( 0, GPIOC );
	Rcc_enableClock( 0, SPI2 );

	Afio_disableJTAG( 0 );
	
	DELAY_US( 1000 );

	LedTask_init( 1, GPIOC_ID, 13 );

	NrfTaskSlave_setNodeNumber( 0, 2 );
	NrfTaskSlave_init( 0, GPIOA_ID, 9, GPIOA_ID, 8, GPIOB_ID, 12, SPI2_ID );

	InfraredTask_init( 0, GPIOB_ID, 7 );
	InfraredTask_init( 1, GPIOB_ID, 8 );
	InfraredTask_init( 2, GPIOB_ID, 9 );
	
	LcdTask_init( 0, GPIOC_ID, 14, GPIOC_ID, 15, GPIOA_ID, 0 );
	
	StationTask_setNodeNumber( 0, 2 );
	StationTask_init( 0 );

	DELAY_US( 1000 );

	Scheduler_init();
	Scheduler_addTask( LedTask_update, (void *) 1, 0, 100 );
	Scheduler_addTask( InfraredTask_update, (void *) 0, 1, 100 );
	Scheduler_addTask( InfraredTask_update, (void *) 1, 1, 100 );
	Scheduler_addTask( InfraredTask_update, (void *) 2, 1, 100 );
	Scheduler_addTask( StationTask_update, (void *) 0, 2, 100 );
	Scheduler_addTask( LcdTask_update, (void *) 0, 3, 5 );
	Scheduler_addTask( NrfTaskSlave_update, (void *) 0, 4, 5 );
	systickInterrupt = Scheduler_update;
	Scheduler_start();
	while( 1 )
	{
		Scheduler_dispatchTasks();
	}
}

UBaseType_t train1Task( void )
{
	Rcc_enableClock( 0, AFIO );
	Rcc_enableClock( 0, GPIOA );
	Rcc_enableClock( 0, GPIOB );
	Rcc_enableClock( 0, GPIOC );
	Rcc_enableClock( 0, TIM2 );
	Rcc_enableClock( 0, TIM3 );
	Rcc_enableClock( 0, SPI2 );

	Afio_disableJTAG( 0 );
	
	DELAY_US( 1000 );

	LedTask_init( 1, GPIOC_ID, 13 );

	NrfTaskSlave_setNodeNumber( 0, 3 );
	NrfTaskSlave_init( 0, GPIOA_ID, 9, GPIOA_ID, 8, GPIOB_ID, 12, SPI2_ID );

	Encoder_init( 0, GPIOA_ID, 0, TIMER2_ID );
	
	Motor_init( 0, GPIOA_ID, 6, TIMER3_ID );
	
	TrainTask_init( 0 );
	
	DELAY_US( 1000 );

	Scheduler_init();
	Scheduler_addTask( LedTask_update, (void *) 1, 0, 100 );
	Scheduler_addTask( NrfTaskSlave_update, (void *) 0, 1, 5 );
	Scheduler_addTask( TrainTask_update, (void *) 0, 2, 100 );
	systickInterrupt = Scheduler_update;
	Scheduler_start();
	while( 1 )
	{
		Scheduler_dispatchTasks();
	}
}

UBaseType_t train2Task( void )
{
	Rcc_enableClock( 0, AFIO );
	Rcc_enableClock( 0, GPIOA );
	Rcc_enableClock( 0, GPIOB );
	Rcc_enableClock( 0, GPIOC );
	Rcc_enableClock( 0, TIM2 );
	Rcc_enableClock( 0, TIM3 );
	Rcc_enableClock( 0, SPI2 );

	Afio_disableJTAG( 0 );
	
	DELAY_US( 1000 );

	LedTask_init( 1, GPIOC_ID, 13 );

	NrfTaskSlave_setNodeNumber( 0, 4 );
	NrfTaskSlave_init( 0, GPIOA_ID, 9, GPIOA_ID, 8, GPIOB_ID, 12, SPI2_ID );

	Encoder_init( 0, GPIOA_ID, 0, TIMER2_ID );
	
	Motor_init( 0, GPIOA_ID, 6, TIMER3_ID );
	
	TrainTask_init( 0 );
	
	DELAY_US( 1000 );

	Scheduler_init();
	Scheduler_addTask( LedTask_update, (void *) 1, 0, 100 );
	Scheduler_addTask( NrfTaskSlave_update, (void *) 0, 1, 5 );
	Scheduler_addTask( TrainTask_update, (void *) 0, 2, 100 );
	systickInterrupt = Scheduler_update;
	Scheduler_start();
	while( 1 )
	{
		Scheduler_dispatchTasks();
	}
}
