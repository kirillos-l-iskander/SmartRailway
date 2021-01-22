#include "CONFIG.h"
#include "IO.h"
#include "PORT.h"
#include "CORE.h"
#include "SCH_CONFIG.h"
#include "SCH.h"
#include "TIM.h"
#include "LED.h"

#include "GPIO.h"
#include "AFIO.h"
#include "SPI.h"
#include "NRF24L01+.h"
#include "SR.h"

#include "PID.h"
#include "TR.h"

#include "ADC.h"
#include "IR.h"
#include "LCD.h"
#include "ST.h"

void SR_Task(void);
void ST1_Task(void);
void ST2_Task(void);
void TR1_Task(void);
void TR2_Task(void);

int main(void)
{
	SCH_Init();
	Disable_JTAG();
	Delay_us(500);
	//Un comment one of the following programs
	//SR_Task();
	//ST1_Task();
	//ST2_Task();
	//TR1_Task();
	//TR2_Task();
	Delay_us(500);
	SCH_Start();
	while(1)
	{
		SCH_Dispatch_Tasks();
	}
	return RETURN_NORMAL;
}

void SR_Task(void)
{
	LED_Init();
	NRF_InitSR();

	SCH_Add_Task(LED_Update, 0, 10);
	SCH_Add_Task(SR_Update, 1, 10);
}

void ST1_Task(void)
{
	LED_Init();
	IR_Init();
	LCD_Init();
	NRF_InitST1();
	
	SCH_Add_Task(LED_Update, 0, 10);
	SCH_Add_Task(IR_Update, 1, 100);
	SCH_Add_Task(ST_Update, 2, 100);
	SCH_Add_Task(LCD_Update, 3, 5);
	SCH_Add_Task(NRF_Update, 4, 10);
}

void ST2_Task(void)
{
	LED_Init();
	IR_Init();
	LCD_Init();
	NRF_InitST2();
	
	SCH_Add_Task(LED_Update, 0, 10);
	SCH_Add_Task(IR_Update, 1, 100);
	SCH_Add_Task(ST_Update, 2, 100);
	SCH_Add_Task(LCD_Update, 3, 5);
	SCH_Add_Task(NRF_Update, 4, 10);
}

void TR1_Task(void)
{
	LED_Init();
	PID_Init();
	TIM2_Init();
	TIM3_Init();
	NRF_InitTR1();
	
	SCH_Add_Task(LED_Update, 0, 10);
	SCH_Add_Task(NRF_Update, 1, 10);
	SCH_Add_Task(TR_Update, 2, 100);
}

void TR2_Task(void)
{
	LED_Init();
	PID_Init();
	TIM2_Init();
	TIM3_Init();
	NRF_InitTR2();
	
	SCH_Add_Task(LED_Update, 0, 10);
	SCH_Add_Task(NRF_Update, 1, 10);
	SCH_Add_Task(TR_Update, 2, 100);
}
