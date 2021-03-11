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
	//Un comment only one of the following Nodes Programs
	//serverTask();
	//station1Task();
	//station2Task();
	//train1Task();
	//train2Task();
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
	
	Led_setGpio( 0, GPIOC_ID, 14 );
	Led_setGpio( 1, GPIOC_ID, 13 );
	Led_init();
	LedTask_init();

	Nrf_setGpioCe( 0, GPIOA_ID, 9 );
	Nrf_setGpioCsn( 0, GPIOA_ID, 8 );
	Nrf_setGpioSpi( 0, GPIOB_ID, 12 );
	Nrf_setSpi( 0, SPI2_ID );
	Nrf_init();
	NrfTaskSlave_setNodeNumber( 0, 0 );
	NrfTaskMaster_init();

	DELAY_US( 1000 );

	Scheduler_init();
	Scheduler_addTask( LedTask_update, NULL, 1000, 1 );
	Scheduler_addTask( NrfTaskMaster_update, NULL, 1000, 1 );
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

	Led_setGpio( 0, GPIOC_ID, 13 );
	//Led_setGpio( 1, GPIOC_ID, 14 );
	Led_init();
	LedTask_init();

	Nrf_setGpioCe( 0, GPIOA_ID, 9 );
	Nrf_setGpioCsn( 0, GPIOA_ID, 8 );
	Nrf_setGpioSpi( 0, GPIOB_ID, 12 );
	Nrf_setSpi( 0, SPI2_ID );
	Nrf_init();
	NrfTaskSlave_setNodeNumber( 0, 1 );
	NrfTaskSlave_init();

	Infrared_setGpio( 0, GPIOB_ID, 7 );
	Infrared_setGpio( 1, GPIOB_ID, 8 );
	Infrared_setGpio( 2, GPIOB_ID, 9 );
	Infrared_init();
	InfraredTask_init();
	
	Lcd_setGpioRs( 0, GPIOC_ID, 14 );
	Lcd_setGpioE( 0, GPIOC_ID, 15 );
	Lcd_setGpioD0( 0, GPIOA_ID, 0 );
	Lcd_init();
	LcdTask_init();
	
	StationTask_setNodeNumber( 0, 1 );
	StationTask_init();

	DELAY_US( 1000 );

	Scheduler_init();
	Scheduler_addTask( LedTask_update, NULL, 0, 100 );
	Scheduler_addTask( InfraredTask_update, NULL, 1, 100 );
	Scheduler_addTask( StationTask_update, NULL, 2, 100 );
	Scheduler_addTask( LcdTask_update, NULL, 3, 5 );
	Scheduler_addTask( NrfTaskSlave_update, NULL, 4, 5 );
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

	Led_setGpio( 0, GPIOC_ID, 13 );
	//Led_setGpio( 1, GPIOC_ID, 14 );
	Led_init();
	LedTask_init();

	Nrf_setGpioCe( 0, GPIOA_ID, 9 );
	Nrf_setGpioCsn( 0, GPIOA_ID, 8 );
	Nrf_setGpioSpi( 0, GPIOB_ID, 12 );
	Nrf_setSpi( 0, SPI2_ID );
	Nrf_init();
	NrfTaskSlave_setNodeNumber( 0, 2 );
	NrfTaskSlave_init();

	Infrared_setGpio( 0, GPIOB_ID, 7 );
	Infrared_setGpio( 1, GPIOB_ID, 8 );
	Infrared_setGpio( 2, GPIOB_ID, 9 );
	Infrared_init();
	InfraredTask_init();
	
	Lcd_setGpioRs( 0, GPIOC_ID, 14 );
	Lcd_setGpioE( 0, GPIOC_ID, 15 );
	Lcd_setGpioD0( 0, GPIOA_ID, 0 );
	Lcd_init();
	LcdTask_init();
	
	StationTask_setNodeNumber( 0, 2 );
	StationTask_init();

	DELAY_US( 1000 );

	Scheduler_init();
	Scheduler_addTask( LedTask_update, NULL, 0, 100 );
	Scheduler_addTask( InfraredTask_update, NULL, 1, 100 );
	Scheduler_addTask( StationTask_update, NULL, 2, 100 );
	Scheduler_addTask( LcdTask_update, NULL, 3, 5 );
	Scheduler_addTask( NrfTaskSlave_update, NULL, 4, 5 );
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

	Led_setGpio( 0, GPIOC_ID, 13 );
	//Led_setGpio( 1, GPIOC_ID, 14 );
	Led_init();
	LedTask_init();

	Nrf_setGpioCe( 0, GPIOA_ID, 9 );
	Nrf_setGpioCsn( 0, GPIOA_ID, 8 );
	Nrf_setGpioSpi( 0, GPIOB_ID, 12 );
	Nrf_setSpi( 0, SPI2_ID );
	Nrf_init();
	NrfTaskSlave_setNodeNumber( 0, 3 );
	NrfTaskSlave_init();

	Encoder_setGpio( 0, GPIOA_ID, 0 );
	Encoder_setTimer( 0, TIMER2_ID );
	Encoder_init();
	
	Motor_setGpio( 0, GPIOA_ID, 6 );
	Motor_setTimer( 0, TIMER3_ID );
	Motor_init();
	
	TrainTask_init();
	
	DELAY_US( 1000 );

	Scheduler_init();
	Scheduler_addTask( LedTask_update, NULL, 0, 100 );
	Scheduler_addTask( NrfTaskSlave_update, NULL, 1, 5 );
	Scheduler_addTask( TrainTask_update, NULL, 2, 100 );
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

	Led_setGpio( 0, GPIOC_ID, 13 );
	//Led_setGpio( 1, GPIOC_ID, 14 );
	Led_init();
	LedTask_init();

	Nrf_setGpioCe( 0, GPIOA_ID, 9 );
	Nrf_setGpioCsn( 0, GPIOA_ID, 8 );
	Nrf_setGpioSpi( 0, GPIOB_ID, 12 );
	Nrf_setSpi( 0, SPI2_ID );
	Nrf_init();
	NrfTaskSlave_setNodeNumber( 0, 4 );
	NrfTaskSlave_init();

	Encoder_setGpio( 0, GPIOA_ID, 0 );
	Encoder_setTimer( 0, TIMER2_ID );
	Encoder_init();
	
	Motor_setGpio( 0, GPIOA_ID, 6 );
	Motor_setTimer( 0, TIMER3_ID );
	Motor_init();
	
	TrainTask_init();
	
	DELAY_US( 1000 );

	Scheduler_init();
	Scheduler_addTask( LedTask_update, NULL, 0, 100 );
	Scheduler_addTask( NrfTaskSlave_update, NULL, 1, 5 );
	Scheduler_addTask( TrainTask_update, NULL, 2, 100 );
	systickInterrupt = Scheduler_update;
	Scheduler_start();
	while( 1 )
	{
		Scheduler_dispatchTasks();
	}
}
