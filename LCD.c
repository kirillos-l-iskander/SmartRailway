#include "Lcd.h"

typedef struct
{
	Id_t rs_gpio_id;
	uint8_t pinRs;
	Id_t rw_gpio_id;
	uint8_t pinRw;
	Id_t e_gpio_id;
	uint8_t pinE;
	Id_t d0_gpio_id;
	uint8_t pinD0;
}Lcd_t;

static Lcd_t lcd[ LCD_NUMBER ];
static uint8_t lcdRowAddress[ 4 ] = { 0x80, 0xC0, 0x90, 0xD0 };

void Lcd_init( Id_t id )
{
	size_t pin = 0;
	Gpio_initPin( lcd[ id ].rs_gpio_id, lcd[ id ].pinRs, OUTPUT );
	Gpio_initPin( lcd[ id ].e_gpio_id, lcd[ id ].pinE, OUTPUT );
	for ( pin = lcd[ id ].pinD0; pin < ( lcd[ id ].pinD0 + 8 ); pin++ )
	{
		Gpio_initPin( lcd[ id ].d0_gpio_id, pin, OUTPUT );
	}
	Lcd_setCommand( id, RETURN_HOME );
	DELAY_US( 100 );
	Lcd_setCommand( id, DISPLAY_2_LINES_5x7_MATRIX_8_BIT );
	DELAY_US( 100 );
	Lcd_setCommand( id, DISPLAY_ON_CURSOR_OFF );
	Lcd_setCommand( id, INCREMENT_CURSOR );
	DELAY_US( 100 );
	Lcd_clear( id );
}

void Lcd_setCommand( Id_t id, uint8_t command )
{
	Gpio_setPortState( lcd[ id ].d0_gpio_id, ( 0xFFU << lcd[ id ].pinD0 ), command );
	Gpio_setPinState( lcd[ id ].rs_gpio_id, lcd[ id ].pinRs, LOW );
	Gpio_setPinState( lcd[ id ].e_gpio_id, lcd[ id ].pinE, HIGH );
	DELAY_US( 1 );
	Gpio_setPinState( lcd[ id ].e_gpio_id, lcd[ id ].pinE, LOW );
	DELAY_US( 50 );
}

void Lcd_setCharacter( Id_t id, uint8_t character )
{
	Gpio_setPortState( lcd[ id ].d0_gpio_id, ( 0xFFU << lcd[ id ].pinD0 ), character );
	Gpio_setPinState( lcd[ id ].rs_gpio_id, lcd[ id ].pinRs, HIGH );
	Gpio_setPinState( lcd[ id ].e_gpio_id, lcd[ id ].pinE, HIGH );
	DELAY_US( 1 );
	Gpio_setPinState( lcd[ id ].e_gpio_id, lcd[ id ].pinE, LOW );
}

void Lcd_setString( Id_t id, uint8_t *string )
{
	size_t index = 0;
	for( index = 0; string[ index ] != 0; index++ )
	{
		Lcd_setCharacter( id, string[ index ] );
	}
}

void Lcd_setCursor( Id_t id, uint8_t row, uint8_t col )
{
	Lcd_setCommand( id, lcdRowAddress[ row ] | col );
}

void Lcd_clear( Id_t id )
{
	Lcd_setCommand( id, CLEAR_DISPLAY );
	Lcd_setCommand( id, FORCE_CURSOR_HOME );
	DELAY_US( 2000 );
}

void Lcd_setGpioRs( Id_t id, Id_t gpio_id, uint8_t pin )
{
	lcd[ id ].rs_gpio_id = gpio_id;
	lcd[ id ].pinRs = pin;
}

void Lcd_setGpioE( Id_t id, Id_t gpio_id, uint8_t pin )
{
	lcd[ id ].e_gpio_id = gpio_id;
	lcd[ id ].pinE = pin;
}

void Lcd_setGpioD0( Id_t id, Id_t gpio_id, uint8_t pin )
{
	lcd[ id ].d0_gpio_id = gpio_id;
	lcd[ id ].pinD0 = pin;
}
