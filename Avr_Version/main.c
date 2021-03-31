
#include"avr/delay.h"
#include "STD_TYPES.H"
#include "BIT_math.h"
#include "DIO_int.h"
#include "LCD_int.h"
#include "LCD_cfg.h"
#include "ADC_int.h"
#include "FreeRTOS.h"
#include "task.h"
#include "event_groups.h"
#include "semphr.h"
#include "usart_driver.h"

u16  LDR_GetLux (void);
u32 LM35_GetTemp(void);
void System_init (void);
#define NULLPTR  (void *(0))

#define DISP_MAIN_L0	(u8 *)"C:    T:    AS: "
#define DISP_MAIN_L1	(u8 *)"  TC:C   AT:T   "

#define DISP_CTEMP_R	0
#define DISP_CTEMP_C	2

#define DISP_TTEMP_R	0
#define DISP_TTEMP_C	8

#define DISP_AS_R		0
#define DISP_AS_C		15


#define DISP_CONFIG_L0	(u8 *)"      T:        "
#define DISP_CONFIG_L1	(u8 *)"  OK:O   CN:C   "

#define DISP_ALARM_L0	(u8 *)"  !!!ALARM!!!   "
#define DISP_ALARM_L1	(u8 *)"      AS:S      "

#define DISP_LED_L0 	(u8 *)"    LED ON :)   "
#define DISP_LED_L1	(u8 *)"      AS:S      "


/********************************************************/


typedef enum{
	SS_MAIN,
	SS_CONFIG,
	SS_ALARM,
	SS_LED

}SystemState_t;

typedef struct{
	SystemState_t SysState;
	u8 AlarmStatus;		/* 'D' Disable of 'E' Enable */
	u8 CTemp;           //current temp
	u8 TTemp;           //thresh hold temp
	u8 CFlux;
	u8 TFlux;
}TAS_t;

TAS_t	TAS = {SS_MAIN,'E',0,100,0,50};

u8 buffer[10];

/******************** Prototypes ********************/

void T_Alarm (void* pvParam);
void T_Syscheck (void* pvParam);
void T_Temp (void* pvParam);
void T_UserTerm (void* pvParam);
void T_Display (void* pvParam);
void T_LED(void * pvParam);


TaskHandle_t th_T_Alarm;
TaskHandle_t th_T_Syscheck;
TaskHandle_t th_T_Temp;
TaskHandle_t th_T_LED;
TaskHandle_t th_T_UserTerm;
TaskHandle_t th_T_Display;

#define E_MAIN_SCREEN 				(1<<0)
#define E_ALARM_SCREEN				(1<<1)
#define E_CONFIG_SCREEN				(1<<2)
#define E_CTEMP_UPDATE				(1<<3)
#define E_TTEMP_UPDATE				(1<<4)
#define E_TTEMP_DIGIT_DISP			(1<<5)
#define E_ALARM_STATUS_UPDATE		(1<<6)

#define E_DISP_EVENTS 				(0x7F)

#define E_ALARM						(1<<7)
#define E_Light_ON          		(1<<8)
#define E_SYS_TRIG          		(1<<9)    /* */


EventGroupHandle_t	egEvents =NULL;
//xSemaphoreHandle bsCheck;

/******************** main *************************/

void TAS_DispStateScreens(SystemState_t  state)
{
	switch (state)
	{
		case SS_MAIN:

		LCD_vdGoToXY(0,0);
		LCD_vdWriteString(DISP_MAIN_L0);
		LCD_vdGoToXY(1,0);
		LCD_vdWriteString(DISP_MAIN_L1);

		break;

		case SS_CONFIG:
		LCD_vdGoToXY(0,0);
		LCD_vdWriteString(DISP_CONFIG_L0);
		LCD_vdGoToXY(1,0);
		LCD_vdWriteString(DISP_CONFIG_L1);
//		LCD_WriteStringXY(1,0,DISP_CONFIG_L1);
		break;

		case SS_ALARM:
		LCD_vdGoToXY(0,0);
		LCD_vdWriteString(DISP_ALARM_L0);
		LCD_vdGoToXY(1,0);
		LCD_vdWriteString(DISP_ALARM_L1);
		break;
		case SS_LED:
		LCD_vdGoToXY(0,0);
		LCD_vdWriteString(DISP_LED_L0);
		LCD_vdGoToXY(1,0);
		LCD_vdWriteString(DISP_LED_L1);
		break;

	}


}


u8 digitCounter = 0;
u8 digitValue = 0;
int main(void)
{
	/*Init Pr.*/
	System_init();
//	DIO_vdSetPinValue(PORTC)
	/* Create OS Objects */
	egEvents= xEventGroupCreate();
//	bsCheck = xSemaphoreCreateBinary();

	/* Create Tasks */

	xTaskCreate(T_UserTerm,"T_UserTerm",100,NULL,5,&th_T_UserTerm);
	xTaskCreate(T_Alarm,"T_Alarm",100,NULL,4,&th_T_Alarm);
	xTaskCreate(T_Syscheck,"T_Syscheck",100,NULL,3,&th_T_Syscheck);
	xTaskCreate(T_Temp,"T_Temp",100,NULL,2,&th_T_Temp);
//	xTaskCreate(T_Temp,"T_Temp",100,NULL,2,&th_T_Temp);
	xTaskCreate(T_Display,"T_Display",100,NULL,1,&th_T_Display);

	/*OS Start*/
	vTaskStartScheduler();

	return 0;
}

void T_Alarm (void* pvParam)
{
	usart_puts((u8*)("Alarm started"));
	EventBits_t ebBits= 0;
	while (1)
	{
		ebBits= xEventGroupWaitBits(egEvents,((E_ALARM)|(E_Light_ON)),0,0,portMAX_DELAY);
		if (ebBits & (E_ALARM))
		{
//			DIO_WritePin(LED,HIGH);
//			DIO_WritePin(Buzzer,HIGH);
//			vTaskDelay(500);
//
//			DIO_WritePin(LED,LOW);
//			DIO_WritePin(Buzzer,LOW);
//			vTaskDelay(500);
			xEventGroupClearBits(egEvents,E_ALARM);
		}
		if (ebBits & (E_Light_ON))
		{

			/** todo
			 * event action
			 */
//			DIO_WritePin(LED,HIGH);
//			DIO_WritePin(Buzzer,HIGH);
//			vTaskDelay(500);
//
//			DIO_WritePin(LED,LOW);
//			DIO_WritePin(Buzzer,LOW);
//			vTaskDelay(500);
			xEventGroupClearBits(egEvents,E_Light_ON);
		}


	}
}

void T_Syscheck (void* pvParam)
{
	EventBits_t ebBits= 0;
	while (1)
	{
			ebBits= xEventGroupWaitBits(egEvents,(E_SYS_TRIG),0,0,portMAX_DELAY);
			if (ebBits & (E_SYS_TRIG))
			{
			if (TAS.SysState == SS_MAIN  &&  TAS.AlarmStatus == 'E'  &&  TAS.CTemp > TAS.TTemp)
			{
				TAS.SysState=SS_ALARM;
				xEventGroupSetBits(egEvents,(E_ALARM_SCREEN | E_ALARM));
			}
			else if (TAS.SysState == SS_ALARM  &&  (TAS.AlarmStatus == 'D'  ||  TAS.CTemp < TAS.TTemp) )
			{
				xEventGroupClearBits(egEvents,(E_ALARM));
				TAS.SysState=SS_MAIN;
				xEventGroupSetBits(egEvents,(E_MAIN_SCREEN|E_CTEMP_UPDATE|E_TTEMP_UPDATE|E_ALARM_STATUS_UPDATE));
			}
			else if (TAS.SysState == SS_MAIN  &&  ( TAS.CFlux < 100) )/* @todo check on light  */
			{
				xEventGroupSetBits(egEvents,(E_Light_ON));
				TAS.SysState=SS_ALARM;
//				xEventGroupSetBits(egEvents,(E_MAIN_SCREEN|E_CTEMP_UPDATE|E_TTEMP_UPDATE|E_ALARM_STATUS_UPDATE));
			}
		}

	}
}


void T_Temp (void* pvParam)
{
	u16 temp=0;
	u16 Flux_val=0;
	while (1)
	{
		temp = LM35_GetTemp();
		Flux_val = LDR_GetLux();
		/*temperature not changed */
		if (temp != TAS.CTemp  &&  TAS.SysState != SS_CONFIG)
		{
			TAS.CTemp=temp;
			if (TAS.SysState == SS_MAIN)		xEventGroupSetBits(egEvents,(E_CTEMP_UPDATE));
			xEventGroupSetBits(egEvents,E_SYS_TRIG);

		//vTaskDelay(100);
		if (TAS.CFlux != Flux_val  &&  TAS.SysState != SS_CONFIG)
		{
			TAS.CFlux=Flux_val;
			if (TAS.SysState == SS_MAIN)		xEventGroupSetBits(egEvents,(E_Light_ON));
			xEventGroupSetBits(egEvents,E_SYS_TRIG);

	}
		vTaskDelay(100);
	}
}
}


void T_UserTerm (void* pvParam)
{
	u8 ch=0;
	u16 recTTemp=0;
	while (1)
	{
		ch=usart_getc();
		if(ch)
		{
			switch(TAS.SysState)
			{

				case SS_MAIN:

					if(ch == 'C')
					{
						TAS.SysState = SS_CONFIG;
						digitCounter=0;
						recTTemp = 0;
						xEventGroupSetBits(egEvents,E_CONFIG_SCREEN);
					}
					else if (ch == 'T')
					{
						if (TAS.AlarmStatus == 'E')					TAS.AlarmStatus= 'D';
						else										TAS.AlarmStatus= 'E';

						xEventGroupSetBits(egEvents,E_ALARM_STATUS_UPDATE);
						xEventGroupSetBits(egEvents,E_SYS_TRIG);
					}

				break;

				case SS_CONFIG:

					if (ch == 'O')
					{
						TAS.TTemp = recTTemp;
						TAS.SysState = SS_MAIN;
						xEventGroupSetBits(egEvents,(E_MAIN_SCREEN| E_CTEMP_UPDATE | E_TTEMP_UPDATE | E_ALARM_STATUS_UPDATE));
						xEventGroupSetBits(egEvents,E_SYS_TRIG);					}

					else if (ch == 'C')
					{
						TAS.SysState = SS_MAIN;
						xEventGroupSetBits(egEvents,(E_MAIN_SCREEN|E_CTEMP_UPDATE|E_TTEMP_UPDATE|E_ALARM_STATUS_UPDATE));

					}

					else if ( (ch >= '0'  &&  ch <= '9')  &&  digitCounter < 3)
					{
						digitCounter++;
						recTTemp = (recTTemp * 10) + ( ch - '0' );
						digitValue = ch;
						xEventGroupSetBits(egEvents,E_TTEMP_DIGIT_DISP);
					}

				break;

				case SS_ALARM:
					if (ch == 'S')
					{
						TAS.AlarmStatus = 'D';
						/*to do
						 * set event tast sys*/
						xEventGroupSetBits(egEvents,E_SYS_TRIG);					}
				break;
			}
		}
		vTaskDelay(10);
	}
}


void T_Display (void* pvParam)
{
	EventBits_t			ebBits = 0;
	while (1)
	{
		ebBits= xEventGroupWaitBits(egEvents,(E_DISP_EVENTS),1,0,portMAX_DELAY);
		if (ebBits & E_MAIN_SCREEN)				TAS_DispStateScreens(SS_MAIN);
		if (ebBits & E_ALARM_SCREEN)			TAS_DispStateScreens(SS_ALARM);
		if (ebBits & E_CONFIG_SCREEN)			TAS_DispStateScreens(SS_CONFIG);

		if (ebBits & E_CTEMP_UPDATE)
			{
				LCD_vdGoToXY(DISP_CTEMP_R,DISP_CTEMP_C);
				LCD_vdWriteNumber(TAS.CTemp);
//			LCD_WriteNumXY(DISP_CTEMP_R,DISP_CTEMP_C,TAS.CTemp);
			}
		if (ebBits & E_TTEMP_UPDATE)
			{
				LCD_vdGoToXY(DISP_TTEMP_R,DISP_CTEMP_C);
				LCD_vdWriteNumber(TAS.TTemp);
//				LCD_WriteNumXY(DISP_TTEMP_R,DISP_TTEMP_C,TAS.TTemp);
			}
		if (ebBits & E_TTEMP_DIGIT_DISP)
		{
				LCD_vdGoToXY(DISP_TTEMP_R,(DISP_TTEMP_C + digitCounter));
				LCD_vdWriteData(digitValue);
//			LCD_WriteCharXY(DISP_TTEMP_R , (DISP_TTEMP_C + digitCounter) , digitValue);
		}
		if (ebBits & E_ALARM_STATUS_UPDATE)
			{
				LCD_vdGoToXY(DISP_AS_R,DISP_AS_C);
				LCD_vdWriteData(TAS.AlarmStatus);
//				LCD_WriteCharXY(DISP_AS_R,DISP_AS_C,TAS.AlarmStatus);
			}
	}
}


void System_init (void)
{
	/*initialize pins directions */
	DIO_vdSetPortDirection(LCD_CTRL_PORT,OUTPUT);
	DIO_vdSetPortDirection(LCD_DATA_PORT,OUTPUT);
	DIO_vdSetPortDirection(DIO_PORTA,INPUT);
	/* initialize ADC */
	ADC_vdIntialize();

	LCD_vdInit();
	usart_init(9800);

	usart_puts((u8 *)("System starting....\r\n"));
	TAS_DispStateScreens(SS_MAIN);
	LCD_vdGoToXY(DISP_CTEMP_R,DISP_CTEMP_C);
	LCD_vdWriteNumber((TAS.CTemp));
	LCD_vdGoToXY(DISP_TTEMP_R,DISP_TTEMP_C);
	LCD_vdWriteNumber((TAS.TTemp));
	LCD_vdGoToXY(DISP_AS_R,DISP_AS_C);
	LCD_vdWriteData(TAS.AlarmStatus);
}

u16  LDR_GetLux (void)
{

	u16 Light=ADC_u16GetADC_Value(CHANNEL1);
	Light=((Light*5000)/1024);        /*Change range from 0~1024 to 0~5000mv*/
	return Light;
}
u32 LM35_GetTemp(void)
{
	u32 buffer=ADC_u16GetADC_Value(CHANNEL0);
	buffer=(((buffer * 5000)/1024));        /*Change range from 0~1024 to 0~5000mv*/
	u32 temp=buffer/10;
	usart_putNumber(buffer);
	return buffer;
}



