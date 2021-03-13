#ifndef LCD_TASK_H
#define	LCD_TASK_H

#include "SchedulerConfig.h"
#include "Lcd.h"

void LcdTask_init( Id_t id, Id_t rsGpioId, uint8_t rsPin, Id_t eGpioId, uint8_t ePin, Id_t dGpioId, uint8_t dPin );
void LcdTask_setCharacter( Id_t id, uint8_t character, uint8_t row, uint8_t col );
void LcdTask_setString( Id_t id, uint8_t *string, uint8_t row, uint8_t col );
void LcdTask_clear( Id_t id );
void LcdTask_update( void *paramter );

#endif	/* LCD_TASK_H */
