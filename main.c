#include "SchedulerConfig.h"
#include "Scheduler.h"
#include "Config.h"

#include "Rcc.h"
#include "Afio.h"
#include "Gpio.h"
#include "Adc.h"
#include "Timer.h"
#include "Spi.h"

#include "Led.h"
#include "Nrf.h"
#include "Irsnsr.h"
#include "Lcd.h"
#include "Station.h"
#include "Rtryencdr.h"
#include "Pid.h"
#include "Motor.h"
#include "Train.h"

typedef enum
{
	SERVER_NODE,
	STATION1_NODE,
	STATION2_NODE,
	TRAIN1_NODE,
	TRAIN2_NODE,
} SmartRailway_t;

#define TRAIN2_NODE

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
	while(1);
	return SCH_RETURN_NORMAL;
}

UBaseType_t serverTask( void )
{
	Rcc_enableClock( RCC_ID, AFIO );
	Rcc_enableClock( RCC_ID, GPIOA );
	Rcc_enableClock( RCC_ID, GPIOB );
	Rcc_enableClock( RCC_ID, GPIOC );
	Rcc_enableClock( RCC_ID, SPI2 );
	
	DELAY_US( 1000 );
	
	Led_init( LED_ID_1, GPIO_ID_C, GPIO_PIN_13 );
	Led_init( LED_ID_2, GPIO_ID_C, GPIO_PIN_14 );
	Nrf_init( NRF_ID_1, GPIO_ID_A, GPIO_PIN_9, GPIO_ID_A, GPIO_PIN_8, GPIO_ID_B, GPIO_PIN_12, SPI_ID_2 );
	Nrf_setNodeNumber( NRF_ID_1, NRF_NODE_0 );

	DELAY_US( 1000 );

	Scheduler_init();
	Scheduler_addTask( Led_update, (void *) LED_ID_1, MS_TO_TICKS( 1000 ), MS_TO_TICKS( 1 ) );
	Scheduler_addTask( Led_update, (void *) LED_ID_2, MS_TO_TICKS( 1000 ), MS_TO_TICKS( 1 ) );
	Scheduler_addTask( Nrf_updateMaster, (void *) NRF_ID_1, MS_TO_TICKS( 1000 ), MS_TO_TICKS( 1 ) );
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
	
	DELAY_US( 1000 );

	Led_init( LED_ID_1, GPIO_ID_C, GPIO_PIN_13 );
	Nrf_init( NRF_ID_1, GPIO_ID_A, GPIO_PIN_9, GPIO_ID_A, GPIO_PIN_8, GPIO_ID_B, GPIO_PIN_12, SPI_ID_2 );
	Nrf_setNodeNumber( NRF_ID_1, NRF_NODE_1 );
	Irsnsr_init( IRSNSR_ID_1, GPIO_ID_B, GPIO_PIN_7 );
	Irsnsr_init( IRSNSR_ID_2, GPIO_ID_B, GPIO_PIN_8 );
	Irsnsr_init( IRSNSR_ID_3, GPIO_ID_B, GPIO_PIN_9 );
	Lcd_init( LCD_ID_1, GPIO_ID_C, GPIO_PIN_14, GPIO_ID_C, GPIO_PIN_15, GPIO_ID_A, GPIO_PIN_0 );
	Station_init( STATION_ID_1, NRF_ID_1, IRSNSR_ID_1, IRSNSR_ID_2, IRSNSR_ID_3, LCD_ID_1 );

	DELAY_US( 1000 );

	Scheduler_init();
	Scheduler_addTask( Led_update, (void *) LED_ID_1, MS_TO_TICKS( 0 ), MS_TO_TICKS( 1 ) );
	Scheduler_addTask( Irsnsr_update, (void *) IRSNSR_ID_1, MS_TO_TICKS( 1 ), MS_TO_TICKS( 100 ) );
	Scheduler_addTask( Irsnsr_update, (void *) IRSNSR_ID_2, MS_TO_TICKS( 1 ), MS_TO_TICKS( 100 ) );
	Scheduler_addTask( Irsnsr_update, (void *) IRSNSR_ID_3, MS_TO_TICKS( 1 ), MS_TO_TICKS( 100 ) );
	Scheduler_addTask( Station_update, (void *) STATION_ID_1, MS_TO_TICKS( 2 ), MS_TO_TICKS( 100 ) );
	Scheduler_addTask( Lcd_update, (void *) LCD_ID_1, MS_TO_TICKS( 3 ), MS_TO_TICKS( 5 ) );
	Scheduler_addTask( Nrf_updateSlave, (void *) NRF_ID_1, MS_TO_TICKS( 4 ), MS_TO_TICKS( 5 ) );
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
	
	DELAY_US( 1000 );

	Led_init( LED_ID_1, GPIO_ID_C, GPIO_PIN_13 );
	Nrf_init( NRF_ID_1, GPIO_ID_A, GPIO_PIN_9, GPIO_ID_A, GPIO_PIN_8, GPIO_ID_B, GPIO_PIN_12, SPI_ID_2 );
	Nrf_setNodeNumber( NRF_ID_1, NRF_NODE_2 );
	Irsnsr_init( IRSNSR_ID_1, GPIO_ID_B, GPIO_PIN_7 );
	Irsnsr_init( IRSNSR_ID_2, GPIO_ID_B, GPIO_PIN_8 );
	Irsnsr_init( IRSNSR_ID_3, GPIO_ID_B, GPIO_PIN_9 );
	Lcd_init( LCD_ID_1, GPIO_ID_C, GPIO_PIN_14, GPIO_ID_C, GPIO_PIN_15, GPIO_ID_A, GPIO_PIN_0 );
	Station_init( STATION_ID_1, NRF_ID_1, IRSNSR_ID_1, IRSNSR_ID_2, IRSNSR_ID_3, LCD_ID_1 );

	DELAY_US( 1000 );

	Scheduler_init();
	Scheduler_addTask( Led_update, (void *) LED_ID_1, MS_TO_TICKS( 0 ), MS_TO_TICKS( 1 ) );
	Scheduler_addTask( Irsnsr_update, (void *) IRSNSR_ID_1, MS_TO_TICKS( 1 ), MS_TO_TICKS( 100 ) );
	Scheduler_addTask( Irsnsr_update, (void *) IRSNSR_ID_2, MS_TO_TICKS( 1 ), MS_TO_TICKS( 100 ) );
	Scheduler_addTask( Irsnsr_update, (void *) IRSNSR_ID_3, MS_TO_TICKS( 1 ), MS_TO_TICKS( 100 ) );
	Scheduler_addTask( Station_update, (void *) STATION_ID_1, MS_TO_TICKS( 2 ), MS_TO_TICKS( 100 ) );
	Scheduler_addTask( Lcd_update, (void *) LCD_ID_1, MS_TO_TICKS( 3 ), MS_TO_TICKS( 5 ) );
	Scheduler_addTask( Nrf_updateSlave, (void *) NRF_ID_1, MS_TO_TICKS( 4 ), MS_TO_TICKS( 5 ) );
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
	
	DELAY_US( 1000 );

	Led_init( LED_ID_1, GPIO_ID_C, GPIO_PIN_13 );
	Nrf_init( NRF_ID_1, GPIO_ID_A, GPIO_PIN_9, GPIO_ID_A, GPIO_PIN_8, GPIO_ID_B, GPIO_PIN_12, SPI_ID_2 );
	Nrf_setNodeNumber( NRF_ID_1, NRF_NODE_3 );
	Rtryencdr_init( RTRYENCDR_ID_1, GPIO_ID_A, GPIO_PIN_0, TIMER_ID_2, TIMER_CHANNEL_1 );
	Pid_init( PID_ID_1, 1, 0, 0 );
	Motor_init( MOTOR_ID_1, GPIO_ID_A, GPIO_PIN_6, TIMER_ID_3, TIMER_CHANNEL_1 );
	Train_init( TRAIN_ID_1, NRF_ID_1, RTRYENCDR_ID_1, PID_ID_1, MOTOR_ID_1 );
	
	DELAY_US( 1000 );

	Scheduler_init();
	Scheduler_addTask( Led_update, (void *) LED_ID_1, MS_TO_TICKS( 0 ), MS_TO_TICKS( 1 ) );
	Scheduler_addTask( Nrf_updateSlave, (void *) NRF_ID_1, MS_TO_TICKS( 1 ), MS_TO_TICKS( 5 ) );
	Scheduler_addTask( Rtryencdr_update, (void *) RTRYENCDR_ID_1, MS_TO_TICKS( 2 ), MS_TO_TICKS( 100 ) );
	Scheduler_addTask( Train_update, (void *) TRAIN_ID_1, MS_TO_TICKS( 2 ), MS_TO_TICKS( 100 ) );
	Scheduler_addTask( Pid_update, (void *) PID_ID_1, MS_TO_TICKS( 2 ), MS_TO_TICKS( 100 ) );
	Scheduler_addTask( Motor_update, (void *) MOTOR_ID_1, MS_TO_TICKS( 2 ), MS_TO_TICKS( 100 ) );
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
	
	DELAY_US( 1000 );

	Led_init( LED_ID_1, GPIO_ID_C, GPIO_PIN_13 );
	Nrf_init( NRF_ID_1, GPIO_ID_A, GPIO_PIN_9, GPIO_ID_A, GPIO_PIN_8, GPIO_ID_B, GPIO_PIN_12, SPI_ID_2 );
	Nrf_setNodeNumber( NRF_ID_1, NRF_NODE_4 );
	Rtryencdr_init( RTRYENCDR_ID_1, GPIO_ID_A, GPIO_PIN_0, TIMER_ID_2, TIMER_CHANNEL_1 );
	Pid_init( PID_ID_1, 1, 0, 0 );
	Motor_init( MOTOR_ID_1, GPIO_ID_A, GPIO_PIN_6, TIMER_ID_3, TIMER_CHANNEL_1 );
	Train_init( TRAIN_ID_1, NRF_ID_1, RTRYENCDR_ID_1, PID_ID_1, MOTOR_ID_1 );
	
	DELAY_US( 1000 );

	Scheduler_init();
	Scheduler_addTask( Led_update, (void *) LED_ID_1, MS_TO_TICKS( 0 ), MS_TO_TICKS( 1 ) );
	Scheduler_addTask( Nrf_updateSlave, (void *) NRF_ID_1, MS_TO_TICKS( 1 ), MS_TO_TICKS( 5 ) );
	Scheduler_addTask( Rtryencdr_update, (void *) RTRYENCDR_ID_1, MS_TO_TICKS( 2 ), MS_TO_TICKS( 100 ) );
	Scheduler_addTask( Train_update, (void *) TRAIN_ID_1, MS_TO_TICKS( 2 ), MS_TO_TICKS( 100 ) );
	Scheduler_addTask( Pid_update, (void *) PID_ID_1, MS_TO_TICKS( 2 ), MS_TO_TICKS( 100 ) );
	Scheduler_addTask( Motor_update, (void *) MOTOR_ID_1, MS_TO_TICKS( 2 ), MS_TO_TICKS( 100 ) );
	systickInterrupt = Scheduler_update;
	Scheduler_start();
	while( 1 )
	{
		Scheduler_dispatchTasks();
	}
}
