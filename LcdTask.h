#ifndef LCD_TASK_H
#define	LCD_TASK_H

#include "SchedulerConfig.h"
#include "Lcd.h"

void LcdTask_init( Id_t id, Id_t rs_gpio_id, uint8_t rs_pin, Id_t e_gpio_id, uint8_t e_pin, Id_t d0_gpio_id, uint8_t d0_pin );
void LcdTask_setCharacter( Id_t id, uint8_t character, uint8_t row, uint8_t col );
void LcdTask_setString( Id_t id, uint8_t *string, uint8_t row, uint8_t col );
void LcdTask_clear( Id_t id );
void LcdTask_update( void *paramter );

#endif	/* LCD_TASK_H */
