#ifndef LCD_H
#define	LCD_H

#include "CONFIG.h"
#include "IO.h"
#include "PORT.h"
#include "CORE.h"
#include "SCH_CONFIG.h"
#include "SCH.h"
#include "GPIO.h"
#include "AFIO.h"

//LCD---------------------
#define LCD_COM_REG (GPIOC)
#define RS (14)	//0 -> command, 1 -> data
//#define RW (0)	//0 -> write, 1 -> read
#define E (15)	//0 -> disable LCD, 1 -> enable LCD

#define LCD_DAT_REG (GPIOA)
#define D0 (0)
#define D1 (1)
#define D2 (2)
#define D3 (3)
#define D4 (4)
#define D5 (5)
#define D6 (6)
#define D7 (7)
#define DPINS (0xFF)
//LCD---------------------

#define ROW_NUM 2
#define COL_NUM 40

#define CLEAR_DISPLAY 0x01
#define RETURN_HOME 0x02
#define INCREMENT_CURSOR 0x06
#define DISPLAY_ON_CURSOR_OFF 0x0C
#define DISPLAY_2_LINES_5x7_MATRIX_4_BIT 0x28
#define DISPLAY_2_LINES_5x7_MATRIX_8_BIT 0x38
#define FORCE_CURSOR_HOME 0x80

void LCD_Init(void);
void LCD_Update(void);
void LCD_SendCommand(uint8_t Command);
void LCD_SendCharacter(uint8_t Character);
void LCD_SendString(uint8_t* String_P);
void LCD_SetCursor(uint8_t Row, uint8_t Col);
void LCD_Clear(void);
void LCD_SetCharacterBuffer(uint8_t Char, uint8_t Row, uint8_t Col);
void LCD_SetStringBuffer(uint8_t* Str_P, uint8_t Row, uint8_t Col);
void LCD_ClearBuffer(void);

#endif	/* LCD_H */
