#ifndef LCD_H
#define	LCD_H

#include "SchedulerConfig.h"
#include "Gpio.h"

#define CLEAR_DISPLAY ( 0x01 )
#define RETURN_HOME ( 0x02 )
#define INCREMENT_CURSOR ( 0x06 )
#define DISPLAY_ON_CURSOR_OFF ( 0x0C )
#define DISPLAY_2_LINES_5x7_MATRIX_4_BIT ( 0x28 )
#define DISPLAY_2_LINES_5x7_MATRIX_8_BIT ( 0x38 )
#define FORCE_CURSOR_HOME ( 0x80 )

void Lcd_init( Id_t id, Id_t rs_gpio_id, uint8_t rs_pin, Id_t e_gpio_id, uint8_t e_pin, Id_t d0_gpio_id, uint8_t d0_pin );
void Lcd_setCommand( Id_t id, uint8_t command );
void Lcd_setCharacter( Id_t id, uint8_t character );
void Lcd_setString( Id_t id, uint8_t *string );
void Lcd_setCursor( Id_t id, uint8_t row, uint8_t col );
void Lcd_clear( Id_t id );

#endif	/* LCD_H */
