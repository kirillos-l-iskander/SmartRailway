#include "LCD.h"

static const uint8_t LCD_RowAddress_G[] = {0x80, 0xC0, 0x90, 0xD0};
static uint8_t LCD_Buff_G[ROW_NUM][COL_NUM] = {{0}};
static uint8_t LCD_ReadingRowIndex_G;
static uint8_t LCD_ReadingColumnIndex_G;

void LCD_Init(void)
{
  LCD_ReadingRowIndex_G = 0;
  LCD_ReadingColumnIndex_G = 0;
	LCD_ClearBuffer();
	GPIO_InitPin(LCD_COM_REG, RS, GPIO_Mode_Out_PP | GPIO_Speed_2MHz);
	GPIO_InitPin(LCD_COM_REG, E, GPIO_Mode_Out_PP | GPIO_Speed_2MHz);
	GPIO_InitPin(LCD_DAT_REG, D0, GPIO_Mode_Out_PP | GPIO_Speed_2MHz);
	GPIO_InitPin(LCD_DAT_REG, D1, GPIO_Mode_Out_PP | GPIO_Speed_2MHz);
	GPIO_InitPin(LCD_DAT_REG, D2, GPIO_Mode_Out_PP | GPIO_Speed_2MHz);
	GPIO_InitPin(LCD_DAT_REG, D3, GPIO_Mode_Out_PP | GPIO_Speed_2MHz);
	GPIO_InitPin(LCD_DAT_REG, D4, GPIO_Mode_Out_PP | GPIO_Speed_2MHz);
	GPIO_InitPin(LCD_DAT_REG, D5, GPIO_Mode_Out_PP | GPIO_Speed_2MHz);
	GPIO_InitPin(LCD_DAT_REG, D6, GPIO_Mode_Out_PP | GPIO_Speed_2MHz);
	GPIO_InitPin(LCD_DAT_REG, D7, GPIO_Mode_Out_PP | GPIO_Speed_2MHz);
  LCD_SendCommand(RETURN_HOME);
  LCD_SendCommand(DISPLAY_2_LINES_5x7_MATRIX_8_BIT);
  LCD_SendCommand(DISPLAY_ON_CURSOR_OFF);
  LCD_SendCommand(INCREMENT_CURSOR);
  LCD_Clear();
}

void LCD_Update(void)
{
    //LCD_SetCursor(LCD_ReadingRowIndex_G, LCD_ReadingColumnIndex_G);
    LCD_SendCharacter(LCD_Buff_G[LCD_ReadingRowIndex_G][LCD_ReadingColumnIndex_G]);
    LCD_ReadingColumnIndex_G++;
    if(LCD_ReadingColumnIndex_G == COL_NUM)
    {
        LCD_ReadingColumnIndex_G = 0;
        LCD_ReadingRowIndex_G++;
        if(LCD_ReadingRowIndex_G == ROW_NUM)
        {
            LCD_ReadingRowIndex_G = 0;
        }
    }
}

void LCD_SendCommand(uint8_t Comm)
{
	GPIO_SetPortState(LCD_DAT_REG, DPINS, (uint16_t)Comm);
	GPIO_SetPinState(LCD_COM_REG, RS, LOW);
	GPIO_SetPinState(LCD_COM_REG, E, HIGH);
  Delay_us(1);
	GPIO_SetPinState(LCD_COM_REG, E, LOW);
  Delay_us(100);
}

void LCD_SendCharacter(uint8_t Char)
{
	GPIO_SetPortState(LCD_DAT_REG, DPINS, (uint16_t)Char);
	GPIO_SetPinState(LCD_COM_REG, RS, HIGH);
	GPIO_SetPinState(LCD_COM_REG, E, HIGH);
  Delay_us(1);
	GPIO_SetPinState(LCD_COM_REG, E, LOW);
  Delay_us(25);
}

void LCD_SendString(uint8_t* Str_P)
{
	uint8_t Index;
	for(Index=0; Str_P[Index]!=0; Index++)
	{
		LCD_SendCharacter(Str_P[Index]);
	}
}

void LCD_SetCursor(uint8_t Row, uint8_t Col)
{
    LCD_SendCommand(LCD_RowAddress_G[Row] | Col);
}

void LCD_Clear(void)
{
	LCD_SendCommand(CLEAR_DISPLAY);
	LCD_SendCommand(FORCE_CURSOR_HOME);
  Delay_us(2000);
}

void LCD_SetCharacterBuffer(uint8_t Char, uint8_t Row, uint8_t Col)
{
    LCD_Buff_G[Row][Col] = Char;
}

void LCD_SetStringBuffer(uint8_t* Str_P, uint8_t Row, uint8_t Col)
{
    uint8_t Index = 0;
    while(Str_P[Index] != 0)
    {
        LCD_Buff_G[Row][Col] = Str_P[Index++];
        Col++;
        if(Col == COL_NUM)
        {
            Col = 0;
            Row++;
            if(Row == ROW_NUM)
            {
                Row = 0;
            }
        }
    }
}

void LCD_ClearBuffer(void)
{
    uint8_t Row = 0;
    uint8_t Col = 0;
    while(1)
    {
        LCD_Buff_G[Row][Col] = ' ';
        Col++;
        if(Col == COL_NUM)
        {
            Col = 0;
            Row++;
            if(Row == ROW_NUM)
            {
                break;
            }
        }
    }
}
