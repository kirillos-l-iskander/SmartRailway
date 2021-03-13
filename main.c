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
	Rcc_enableClock( RCC_ID, AFIO );
	Rcc_enableClock( RCC_ID, GPIOA );
	Rcc_enableClock( RCC_ID, GPIOB );
	Rcc_enableClock( RCC_ID, GPIOC );
	Rcc_enableClock( RCC_ID, SPI2 );

	Afio_disableJTAG( AFIO_ID );
	
	DELAY_US( 1000 );
	
	LedTask_init( LED1_ID, GPIOC_ID, 14 );
	LedTask_init( LED2_ID, GPIOC_ID, 13 );

	NrfTaskSlave_setNodeNumber( NRF1_ID, 0 );
	NrfTaskMaster_init( NRF1_ID, GPIOA_ID, 9, GPIOA_ID, 8, GPIOB_ID, 12, SPI2_ID );

	DELAY_US( 1000 );

	Scheduler_init();
	Scheduler_addTask( LedTask_update, (void *) LED2_ID, MS_TO_TICKS( 1000 ), MS_TO_TICKS( 1 ) );
	Scheduler_addTask( NrfTaskMaster_update, (void *) NRF1_ID, MS_TO_TICKS( 1000 ), MS_TO_TICKS( 1 ) );
	Scheduler_addTask( LedTask_update, (void *) LED1_ID, MS_TO_TICKS( 1000 ), MS_TO_TICKS( 1 ) );
	systickInterrupt = Scheduler_update;
	Scheduler_start();
	while( 1 )
	{
		Scheduler_dispatchTasks();
	}
}

UBaseType_t station1Task( void )
{
	Rcc_enableClock( RCC_ID, AFIO );
	Rcc_enableClock( RCC_ID, GPIOA );
	Rcc_enableClock( RCC_ID, GPIOB );
	Rcc_enableClock( RCC_ID, GPIOC );
	Rcc_enableClock( RCC_ID, SPI2 );

	Afio_disableJTAG( AFIO_ID );
	
	DELAY_US( 1000 );

	LedTask_init( LED2_ID, GPIOC_ID, 13 );

	NrfTaskSlave_setNodeNumber( NRF1_ID, 1 );
	NrfTaskSlave_init( NRF1_ID, GPIOA_ID, 9, GPIOA_ID, 8, GPIOB_ID, 12, SPI2_ID );

	InfraredTask_init( INFRARED1_ID, GPIOB_ID, 7 );
	InfraredTask_init( INFRARED2_ID, GPIOB_ID, 8 );
	InfraredTask_init( INFRARED3_ID, GPIOB_ID, 9 );
	
	LcdTask_init( LCD1_ID, GPIOC_ID, 14, GPIOC_ID, 15, GPIOA_ID, 0 );
	
	StationTask_setNodeNumber( STATION1_ID, 1 );
	StationTask_init( STATION1_ID );

	DELAY_US( 1000 );

	Scheduler_init();
	Scheduler_addTask( LedTask_update, (void *) LED2_ID, MS_TO_TICKS( 0 ), MS_TO_TICKS( 100 ) );
	Scheduler_addTask( InfraredTask_update, (void *) INFRARED1_ID, MS_TO_TICKS( 1 ), MS_TO_TICKS( 100 ) );
	Scheduler_addTask( InfraredTask_update, (void *) INFRARED2_ID, MS_TO_TICKS( 1 ), MS_TO_TICKS( 100 ) );
	Scheduler_addTask( InfraredTask_update, (void *) INFRARED3_ID, MS_TO_TICKS( 1 ), MS_TO_TICKS( 100 ) );
	Scheduler_addTask( StationTask_update, (void *) STATION1_ID, MS_TO_TICKS( 2 ), MS_TO_TICKS( 100 ) );
	Scheduler_addTask( LcdTask_update, (void *) LCD1_ID, MS_TO_TICKS( 3 ), MS_TO_TICKS( 5 ) );
	Scheduler_addTask( NrfTaskSlave_update, (void *) NRF1_ID, MS_TO_TICKS( 4 ), MS_TO_TICKS( 5 ) );
	systickInterrupt = Scheduler_update;
	Scheduler_start();
	while( 1 )
	{
		Scheduler_dispatchTasks();
	}
}

UBaseType_t station2Task( void )
{
	Rcc_enableClock( RCC_ID, AFIO );
	Rcc_enableClock( RCC_ID, GPIOA );
	Rcc_enableClock( RCC_ID, GPIOB );
	Rcc_enableClock( RCC_ID, GPIOC );
	Rcc_enableClock( RCC_ID, SPI2 );

	Afio_disableJTAG( AFIO_ID );
	
	DELAY_US( 1000 );

	LedTask_init( LED2_ID, GPIOC_ID, 13 );

	NrfTaskSlave_setNodeNumber( NRF1_ID, 2 );
	NrfTaskSlave_init( NRF1_ID, GPIOA_ID, 9, GPIOA_ID, 8, GPIOB_ID, 12, SPI2_ID );

	InfraredTask_init( INFRARED1_ID, GPIOB_ID, 7 );
	InfraredTask_init( INFRARED2_ID, GPIOB_ID, 8 );
	InfraredTask_init( INFRARED3_ID, GPIOB_ID, 9 );
	
	LcdTask_init( LCD1_ID, GPIOC_ID, 14, GPIOC_ID, 15, GPIOA_ID, 0 );
	
	StationTask_setNodeNumber( STATION1_ID, 2 );
	StationTask_init( STATION1_ID );

	DELAY_US( 1000 );

	Scheduler_init();
	Scheduler_addTask( LedTask_update, (void *) LED2_ID, MS_TO_TICKS( 0 ), MS_TO_TICKS( 100 ) );
	Scheduler_addTask( InfraredTask_update, (void *) INFRARED1_ID, MS_TO_TICKS( 1 ), MS_TO_TICKS( 100 ) );
	Scheduler_addTask( InfraredTask_update, (void *) INFRARED2_ID, MS_TO_TICKS( 1 ), MS_TO_TICKS( 100 ) );
	Scheduler_addTask( InfraredTask_update, (void *) INFRARED3_ID, MS_TO_TICKS( 1 ), MS_TO_TICKS( 100 ) );
	Scheduler_addTask( StationTask_update, (void *) STATION1_ID, MS_TO_TICKS( 2 ), MS_TO_TICKS( 100 ) );
	Scheduler_addTask( LcdTask_update, (void *) LCD1_ID, MS_TO_TICKS( 3 ), MS_TO_TICKS( 5 ) );
	Scheduler_addTask( NrfTaskSlave_update, (void *) NRF1_ID, MS_TO_TICKS( 4 ), MS_TO_TICKS( 5 ) );
	systickInterrupt = Scheduler_update;
	Scheduler_start();
	while( 1 )
	{
		Scheduler_dispatchTasks();
	}
}

UBaseType_t train1Task( void )
{
	Rcc_enableClock( RCC_ID, AFIO );
	Rcc_enableClock( RCC_ID, GPIOA );
	Rcc_enableClock( RCC_ID, GPIOB );
	Rcc_enableClock( RCC_ID, GPIOC );
	Rcc_enableClock( RCC_ID, TIM2 );
	Rcc_enableClock( RCC_ID, TIM3 );
	Rcc_enableClock( RCC_ID, SPI2 );

	Afio_disableJTAG( AFIO_ID );
	
	DELAY_US( 1000 );

	LedTask_init( LED2_ID, GPIOC_ID, 13 );

	NrfTaskSlave_setNodeNumber( NRF1_ID, 3 );
	NrfTaskSlave_init( NRF1_ID, GPIOA_ID, 9, GPIOA_ID, 8, GPIOB_ID, 12, SPI2_ID );

	Encoder_init( ENCODER1_ID, GPIOA_ID, 0, TIMER2_ID );
	Pid_init( PID1_ID );
	Motor_init( MOTOR1_ID, GPIOA_ID, 6, TIMER3_ID );
	
	TrainTask_init( TRAIN1_ID );
	
	DELAY_US( 1000 );

	Scheduler_init();
	Scheduler_addTask( LedTask_update, (void *) LED2_ID, MS_TO_TICKS( 0 ), MS_TO_TICKS( 100 ) );
	Scheduler_addTask( NrfTaskSlave_update, (void *) NRF1_ID, MS_TO_TICKS( 1 ), MS_TO_TICKS( 5 ) );
	Scheduler_addTask( Pid_update, (void *) PID1_ID, MS_TO_TICKS( 2 ), MS_TO_TICKS( 100 ) );
	Scheduler_addTask( TrainTask_update, (void *) TRAIN1_ID, MS_TO_TICKS( 2 ), MS_TO_TICKS( 100 ) );
	systickInterrupt = Scheduler_update;
	Scheduler_start();
	while( 1 )
	{
		Scheduler_dispatchTasks();
	}
}

UBaseType_t train2Task( void )
{
	Rcc_enableClock( RCC_ID, AFIO );
	Rcc_enableClock( RCC_ID, GPIOA );
	Rcc_enableClock( RCC_ID, GPIOB );
	Rcc_enableClock( RCC_ID, GPIOC );
	Rcc_enableClock( RCC_ID, TIM2 );
	Rcc_enableClock( RCC_ID, TIM3 );
	Rcc_enableClock( RCC_ID, SPI2 );

	Afio_disableJTAG( AFIO_ID );
	
	DELAY_US( 1000 );

	LedTask_init( LED2_ID, GPIOC_ID, 13 );

	NrfTaskSlave_setNodeNumber( NRF1_ID, 4 );
	NrfTaskSlave_init( NRF1_ID, GPIOA_ID, 9, GPIOA_ID, 8, GPIOB_ID, 12, SPI2_ID );

	Encoder_init( ENCODER1_ID, GPIOA_ID, 0, TIMER2_ID );
	Pid_init( PID1_ID );
	Motor_init( MOTOR1_ID, GPIOA_ID, 6, TIMER3_ID );
	
	TrainTask_init( TRAIN1_ID );

	DELAY_US( 1000 );

	Scheduler_init();
	Scheduler_addTask( LedTask_update, (void *) LED2_ID, MS_TO_TICKS( 0 ), MS_TO_TICKS( 100 ) );
	Scheduler_addTask( NrfTaskSlave_update, (void *) NRF1_ID, MS_TO_TICKS( 1 ), MS_TO_TICKS( 5 ) );
	Scheduler_addTask( Pid_update, (void *) PID1_ID, MS_TO_TICKS( 2 ), MS_TO_TICKS( 100 ) );
	Scheduler_addTask( TrainTask_update, (void *) TRAIN1_ID, MS_TO_TICKS( 2 ), MS_TO_TICKS( 100 ) );
	systickInterrupt = Scheduler_update;
	Scheduler_start();
	while( 1 )
	{
		Scheduler_dispatchTasks();
	}
}
