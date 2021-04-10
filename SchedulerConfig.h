#ifndef SCHEDULER_CONFIG_H
#define SCHEDULER_CONFIG_H

//--------------------------------------------------------------------------------------------------------------------

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

//--------------------------------------------------------------------------------------------------------------------

#define SCH_CPU_F																						( 72000000 )
#define SCH_CPU_INSTRUCTION																	( 1 )

#define SCH_MS_TICK_PERIOD																	( ( TickType_t ) 1 )
#define SCH_TIMER_PRESCALER																	( ( TickType_t ) 1 )
#define SCH_SYSTICK_MAX																			( ( TickType_t ) 16777216 )

#define SCH_MAX_TASKS																				( ( UBaseType_t ) 8 )

#ifndef SCH_TRUE
#define SCH_FALSE																						( 0 )
#define SCH_TRUE																						( !SCH_FALSE )
#endif

#define SCH_RETURN_ERROR																		( 0 )
#define SCH_RETURN_NORMAL																		( !SCH_RETURN_ERROR )

#define SCH_ERROR_LONG_TASK																	( 1 )
#define SCH_ERROR_TOO_MANY_TASKS														( 2 )
#define SCH_ERROR_CANNOT_DELETE_TASK												( 3 )

#define SCH_ERROR_WAITING_FOR_SLAVE_TO_ACK									( 0xAA )
#define SCH_ERROR_WAITING_FOR_START_COMMAND_FROM_MASTER			( 0xAA )

#define SCH_ERROR_ONE_OR_MORE_SLAVES_DID_NOT_START					( 0xA0 )
#define SCH_ERROR_LOST_SLAVE																( 0x80 )

//--------------------------------------------------------------------------------------------------------------------

#define BV( n )													( 1 << n )
/*
#define SET_BIT( reg, bit )							( reg |= ( 1 << bit ) )
#define CLEAR_BIT( reg, bit )						( reg &= ~( 1 << bit ) )
#define TOGGLE_BIT( reg, bit )					( reg ^= ( 1 << bit ) )
#define ROR( reg, bit )									( reg = ( reg >> bit | reg << ( 32 - bit ) ) )
#define ROL( reg, bit )									( reg = ( reg << bit | reg >> ( 32 - bit ) ) )
#define IS_BIT_SET( reg, bit )					( reg & ( 1 << bit ) )
#define IS_BIT_CLEAR( reg, bit )				( !( reg & ( 1 << bit ) ) )
*/
#define MS_TO_TICKS( t )								( t / SCH_MS_TICK_PERIOD )
#define DELAY_MS( t )										Scheduler_delaySoftwareMs( t )
#define DELAY_US( t )										Scheduler_delaySoftwareUs( t )

//--------------------------------------------------------------------------------------------------------------------

typedef int32_t								BaseType_t;
typedef uint32_t							UBaseType_t;
typedef uint32_t							TickType_t;

typedef union
{
    struct
    {
        UBaseType_t bit0:1;
				UBaseType_t bit1:1;
				UBaseType_t bit2:1;
        UBaseType_t bit3:1;
        UBaseType_t bit4:1;
        UBaseType_t bit5:1;
        UBaseType_t bit6:1;
        UBaseType_t bit7:1;
        UBaseType_t bit8:1;
        UBaseType_t bit9:1;
        UBaseType_t bit10:1;
        UBaseType_t bit11:1;
        UBaseType_t bit12:1;
        UBaseType_t bit13:1;
        UBaseType_t bit14:1;
        UBaseType_t bit15:1;
        UBaseType_t bit16:1;
        UBaseType_t bit17:1;
        UBaseType_t bit18:1;
        UBaseType_t bit19:1;
        UBaseType_t bit20:1;
        UBaseType_t bit21:1;
        UBaseType_t bit22:1;
        UBaseType_t bit23:1;
        UBaseType_t bit24:1;
        UBaseType_t bit25:1;
        UBaseType_t bit26:1;
        UBaseType_t bit27:1;
        UBaseType_t bit28:1;
        UBaseType_t bit29:1;
        UBaseType_t bit30:1;
        UBaseType_t bit31:1;
    }Bits_t;
    UBaseType_t reg:32;
}Reg_t;

//--------------------------------------------------------------------------------------------------------------------

void Scheduler_initSysTick( TickType_t msTicksPeriod );
void Scheduler_displayError( UBaseType_t errorCode );
void Scheduler_sleepHardware( void );
void Scheduler_delayHardwareMs( volatile uint32_t msDelay );
void Scheduler_delayHardwareUs( volatile uint32_t usDelay );
void Scheduler_delaySoftwareMs( volatile uint32_t msDelay );
void Scheduler_delaySoftwareUs( volatile uint32_t usDelay );

//--------------------------------------------------------------------------------------------------------------------

#endif /* SCHEDULER_CONFIG_H */
