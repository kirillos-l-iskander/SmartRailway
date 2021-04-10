#ifndef LCD_H
#define	LCD_H

#include "Config.h"
#include "Gpio.h"

#define CLEAR_DISPLAY ( 0x01 )
#define RETURN_HOME ( 0x02 )
#define INCREMENT_CURSOR ( 0x06 )
#define DISPLAY_ON_CURSOR_OFF ( 0x0C )
#define DISPLAY_2_LINES_5x7_MATRIX_4_BIT ( 0x28 )
#define DISPLAY_2_LINES_5x7_MATRIX_8_BIT ( 0x38 )
#define FORCE_CURSOR_HOME ( 0x80 )

typedef enum
{
	LCD_ID_1 = 0,
	LCD_ID_MAX,
}	LCD_ID_t;

typedef enum
{
	LCD_ROW_0 = 0,
	LCD_ROW_1,
	LCD_ROW_MAX,
}	LCD_ROW_t;

typedef enum
{
	LCD_COL_0 = 0,
	LCD_COL_1,
	LCD_COL_2,
	LCD_COL_3,
	LCD_COL_4,
	LCD_COL_5,
	LCD_COL_6,
	LCD_COL_7,
	LCD_COL_8,
	LCD_COL_9,
	LCD_COL_10,
	LCD_COL_11,
	LCD_COL_12,
	LCD_COL_13,
	LCD_COL_14,
	LCD_COL_15,
	LCD_COL_MAX,
}	LCD_COL_t;

void Lcd_init( LCD_ID_t id, GPIO_ID_t gpioIdRs, GPIO_PIN_t gpioPinRs, GPIO_ID_t gpioIdE, GPIO_PIN_t gpioPinE, GPIO_ID_t gpioIdD, GPIO_PIN_t gpioPinD );
void Lcd_update( void *paramter );
void Lcd_setCharacter( LCD_ID_t id, char character, LCD_ROW_t row, LCD_COL_t col );
void Lcd_setString( LCD_ID_t id, char *string, LCD_ROW_t row, LCD_COL_t col );
void Lcd_clear( LCD_ID_t id );

#endif	/* LCD_H */
