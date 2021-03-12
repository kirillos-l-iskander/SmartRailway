#include "LedTask.h"

typedef struct
{
	uint8_t state;
	uint8_t blink;
	uint16_t period;
	uint16_t delay;
}LedTask_t;

static LedTask_t ledTask[ LED_TASK_NUMBER ];

void LedTask_init( Id_t id, Id_t gpio_id, uint8_t pin )
{
	ledTask[ id ].state = LOW;
	ledTask[ id ].blink = 0;
	ledTask[ id ].period = 0;
	ledTask[ id ].delay = 0;
	Led_init( id, gpio_id, pin );
}

void LedTask_setState( Id_t id, uint8_t state, uint16_t period )
{
	ledTask[ id ].state = state;
	ledTask[ id ].period = C_MS_TO_TICKS( period ) / C_MS_TO_TICKS( 100 );
}

void LedTask_update( void *paramter )
{
	Id_t id = (Id_t) paramter;
	if( ledTask[ id ].delay )
	{
		ledTask[ id ].delay--;
	}else if( ledTask[ id ].period )
	{
		ledTask[ id ].blink ^= ledTask[ id ].state;
		ledTask[ id ].delay = ledTask[ id ].period;
	}else
	{
		ledTask[ id ].blink = 0;
	}
	Led_setState( id, !( ledTask[ id ].state & !ledTask[ id ].blink ) );
}
