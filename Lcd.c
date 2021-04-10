#include "Lcd.h"

typedef struct
{
	GPIO_ID_t gpioIdRs;
	GPIO_PIN_t gpioPinRs;
	GPIO_ID_t gpioIdRw;
	GPIO_PIN_t gpioPinRw;
	GPIO_ID_t gpioIdE;
	GPIO_PIN_t gpioPinE;
	GPIO_ID_t gpioIdD;
	GPIO_PIN_t gpioPinD;
	LCD_ROW_t rowIndex;
	LCD_COL_t colIndex;
	char buffer[ LCD_ROW_MAX ][ LCD_COL_MAX ];
}	Lcd_t;

static Lcd_t lcd[ LCD_ID_MAX ];
static uint8_t lcdRowAddress[ 4 ] = { 0x80, 0xC0, 0x90, 0xD0 };

void initLcdHardware( LCD_ID_t id );
void setLcdHardwareCommand( LCD_ID_t id, char command );
void setLcdHardwareCursor( LCD_ID_t id, LCD_ROW_t row, LCD_COL_t col );
void setLcdHardwareCharacter( LCD_ID_t id, char character );
void setLcdHardwareString( LCD_ID_t id, char *string );
void clearLcdHardware( LCD_ID_t id );

void Lcd_init( LCD_ID_t id, GPIO_ID_t gpioIdRs, GPIO_PIN_t gpioPinRs, GPIO_ID_t gpioIdE, GPIO_PIN_t gpioPinE, GPIO_ID_t gpioIdD, GPIO_PIN_t gpioPinD )
{
	lcd[ id ].gpioIdRs = gpioIdRs;
	lcd[ id ].gpioPinRs = gpioPinRs;
	lcd[ id ].gpioIdE = gpioIdE;
	lcd[ id ].gpioPinE = gpioPinE;
	lcd[ id ].gpioIdD = gpioIdD;
	lcd[ id ].gpioPinD = gpioPinD;
	lcd[ id ].rowIndex = LCD_ROW_0;
	lcd[ id ].colIndex = LCD_COL_0;
	Lcd_clear( id );
	initLcdHardware( id );
	setLcdHardwareCommand( id, RETURN_HOME );
	DELAY_US( 100 );
	setLcdHardwareCommand( id, DISPLAY_2_LINES_5x7_MATRIX_8_BIT );
	DELAY_US( 100 );
	setLcdHardwareCommand( id, DISPLAY_ON_CURSOR_OFF );
	DELAY_US( 100 );
	setLcdHardwareCommand( id, INCREMENT_CURSOR );
	DELAY_US( 100 );
	clearLcdHardware( id );
}

void Lcd_update( void *paramter )
{
	LCD_ID_t id = (LCD_ID_t) paramter;
	setLcdHardwareCursor( id, lcd[ id ].rowIndex, lcd[ id ].colIndex );
	setLcdHardwareCharacter( id, lcd[ id ].buffer[ lcd[ id ].rowIndex ][ lcd[ id ].colIndex ] );
	lcd[ id ].colIndex++;
	if( lcd[ id ].colIndex == LCD_COL_MAX )
	{
			lcd[ id ].colIndex = 0;
			lcd[ id ].rowIndex++;
			if( lcd[ id ].rowIndex == LCD_ROW_MAX )
			{
					lcd[ id ].rowIndex = 0;
			}
	}
}

void Lcd_setCharacter( LCD_ID_t id, char character, LCD_ROW_t row, LCD_COL_t col )
{
	lcd[ id ].buffer[ row ][ col ] = character;
}

void Lcd_setString( LCD_ID_t id, char *string, LCD_ROW_t row, LCD_COL_t col )
{
	size_t index = 0;
	for( index = 0; string[ index ] != 0; index++ )
	{
		lcd[ id ].buffer[ row ][ col ] = string[ index ];
		col++;
		if( col == LCD_COL_MAX )
		{
			col = 0;
			row++;
			if( row == LCD_ROW_MAX )
			{
					row = 0;
			}
		}
	}
}

void Lcd_clear( LCD_ID_t id )
{
	LCD_ROW_t row = 0;
	LCD_COL_t col = 0;
	for( ; ; )
	{
		lcd[ id ].buffer[ row ][ col ] = ' ';
		col++;
		if( col == LCD_COL_MAX )
		{
			col = 0;
			row++;
			if( row == LCD_ROW_MAX )
			{
				break;
			}
		}
	}
}

void initLcdHardware( LCD_ID_t id )
{
	Gpio_initPin( lcd[ id ].gpioIdRs, lcd[ id ].gpioPinRs, GPIO_MODE_OUTPUT_DHZ, GPIO_TYPE_GP_PP );
	Gpio_initPin( lcd[ id ].gpioIdE, lcd[ id ].gpioPinE, GPIO_MODE_OUTPUT_DHZ, GPIO_TYPE_GP_PP );
	Gpio_initPin( lcd[ id ].gpioIdD, lcd[ id ].gpioPinD, GPIO_MODE_OUTPUT_DHZ, GPIO_TYPE_GP_PP );
	Gpio_initPin( lcd[ id ].gpioIdD, lcd[ id ].gpioPinD + 1, GPIO_MODE_OUTPUT_DHZ, GPIO_TYPE_GP_PP );
	Gpio_initPin( lcd[ id ].gpioIdD, lcd[ id ].gpioPinD + 2, GPIO_MODE_OUTPUT_DHZ, GPIO_TYPE_GP_PP );
	Gpio_initPin( lcd[ id ].gpioIdD, lcd[ id ].gpioPinD + 3, GPIO_MODE_OUTPUT_DHZ, GPIO_TYPE_GP_PP );
	Gpio_initPin( lcd[ id ].gpioIdD, lcd[ id ].gpioPinD + 4, GPIO_MODE_OUTPUT_DHZ, GPIO_TYPE_GP_PP );
	Gpio_initPin( lcd[ id ].gpioIdD, lcd[ id ].gpioPinD + 5, GPIO_MODE_OUTPUT_DHZ, GPIO_TYPE_GP_PP );
	Gpio_initPin( lcd[ id ].gpioIdD, lcd[ id ].gpioPinD + 6, GPIO_MODE_OUTPUT_DHZ, GPIO_TYPE_GP_PP );
	Gpio_initPin( lcd[ id ].gpioIdD, lcd[ id ].gpioPinD + 7, GPIO_MODE_OUTPUT_DHZ, GPIO_TYPE_GP_PP );
}

void setLcdHardwareCommand( LCD_ID_t id, char command )
{
	Gpio_setPortState( lcd[ id ].gpioIdD, (GPIO_PIN_t) ( 0xFF << lcd[ id ].gpioPinD ), command );
	Gpio_setPinState( lcd[ id ].gpioIdRs, lcd[ id ].gpioPinRs, GPIO_STATE_LOW );
	Gpio_setPinState( lcd[ id ].gpioIdE, lcd[ id ].gpioPinE, GPIO_STATE_HIGH );
	DELAY_US( 1 );
	Gpio_setPinState( lcd[ id ].gpioIdE, lcd[ id ].gpioPinE, GPIO_STATE_LOW );
	DELAY_US( 50 );
}

void setLcdHardwareCursor( LCD_ID_t id, LCD_ROW_t row, LCD_COL_t col )
{
	setLcdHardwareCommand( id, lcdRowAddress[ row ] | col );
}

void setLcdHardwareCharacter( LCD_ID_t id, char character )
{
	Gpio_setPortState( lcd[ id ].gpioIdD, (GPIO_PIN_t) ( 0xFFU << lcd[ id ].gpioPinD ), character );
	Gpio_setPinState( lcd[ id ].gpioIdRs, lcd[ id ].gpioPinRs, GPIO_STATE_HIGH );
	Gpio_setPinState( lcd[ id ].gpioIdE, lcd[ id ].gpioPinE, GPIO_STATE_HIGH );
	DELAY_US( 1 );
	Gpio_setPinState( lcd[ id ].gpioIdE, lcd[ id ].gpioPinE, GPIO_STATE_LOW );
}

void setLcdHardwareString( LCD_ID_t id, char *string )
{
	size_t index = 0;
	for( index = 0; string[ index ] != 0; index++ )
	{
		setLcdHardwareCharacter( id, string[ index ] );
	}
}

void clearLcdHardware( LCD_ID_t id )
{
	setLcdHardwareCommand( id, CLEAR_DISPLAY );
	setLcdHardwareCommand( id, FORCE_CURSOR_HOME );
	DELAY_US( 2000 );
}
