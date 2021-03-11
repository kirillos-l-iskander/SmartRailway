#include "LcdTask.h"

typedef struct
{
	uint8_t buffer[ LCD_ROW_NUMBER ][ LCD_COL_NUMBER ];
	uint8_t readingRowIndex;
	uint8_t readingColumnIndex;
}LcdTask_t;

static LcdTask_t lcdTask[ LCD_TASK_NUMBER ];

void LcdTask_init( void )
{
	size_t id = 0;
	for ( id = 0; id < LCD_TASK_NUMBER; id++ )
	{
		LcdTask_clear( (Id_t) id );
		lcdTask[ id ].readingRowIndex = 0;
		lcdTask[ id ].readingColumnIndex = 0;
	}
	Lcd_init();
}

void LcdTask_setCharacter( Id_t id, uint8_t character, uint8_t row, uint8_t col )
{
	lcdTask[ id ].buffer[ row ][ col ] = character;
}

void LcdTask_setString( Id_t id, uint8_t *string, uint8_t row, uint8_t col )
{
	size_t index = 0;
	for( index = 0; string[ index ] != 0; index++ )
	{
		lcdTask[ id ].buffer[ row ][ col ] = string[ index ];
		col++;
		if( col == LCD_COL_NUMBER )
		{
			col = 0;
			row++;
			if( row == LCD_ROW_NUMBER )
			{
					row = 0;
			}
		}
	}
}

void LcdTask_clear( Id_t id )
{
	uint8_t row = 0;
	uint8_t col = 0;
	for( ; ; )
	{
		lcdTask[ id ].buffer[ row ][ col ] = ' ';
		col++;
		if( col == LCD_COL_NUMBER )
		{
			col = 0;
			row++;
			if( row == LCD_ROW_NUMBER )
			{
				break;
			}
		}
	}
}

void LcdTask_update( void *paramter )
{
	size_t id = 0;
	for ( id = 0; id < LCD_TASK_NUMBER; id++ )
	{
		Lcd_setCursor( (Id_t) id, lcdTask[ id ].readingRowIndex, lcdTask[ id ].readingColumnIndex );
		Lcd_setCharacter( (Id_t) id, lcdTask[ id ].buffer[ lcdTask[ id ].readingRowIndex ][ lcdTask[ id ].readingColumnIndex ] );
		lcdTask[ id ].readingColumnIndex++;
		if( lcdTask[ id ].readingColumnIndex == LCD_COL_NUMBER )
		{
				lcdTask[ id ].readingColumnIndex = 0;
				lcdTask[ id ].readingRowIndex++;
				if( lcdTask[ id ].readingRowIndex == LCD_ROW_NUMBER )
				{
						lcdTask[ id ].readingRowIndex = 0;
				}
		}
	}
}
