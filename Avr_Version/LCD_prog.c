/*
 * LCD_prog.c
 *
 *  Created on: Jan 27, 2020
 *      Author: IOT scientist
 */
#include"avr/delay.h"
#include"STD_TYPES.H"
#include"BIT_math.h"
#include"DIO_int.h"
#include"LCD_int.h"
#include"LCD_cfg.h"


/*****************************************************************************************
 * LCD_vdWriteCommand
 * func_arg:void
 * to write command
 * clear RS Pin for command
 * clear RW Pin
 * put data in DATA_PORT
 * send a high to low pulse
 * return void
 * ***************************************************************************************
 */
void LCD_vdWriteCommand(u8 u8comm)
{
//	clear RS Pin for command
	DIO_vdSetPinValue(LCD_CTRL_PORT,LCD_RS_PIN,LOW);
//	clear RW Pin
	DIO_vdSetPinValue(LCD_CTRL_PORT,LCD_RW_PIN,LOW);

// 	put data in DATA_PORT
#if LCD_DATA_BUS_MODE == LCD_8BIT_MODE
	DIO_vdSetPortValue(LCD_DATA_PORT,u8comm);
#endif
	//4 bit mode
#if LCD_DATA_BUS_MODE == LCD_4BIT_MODE
	//send high nipple
	DIO_vidSetPortMuxVal(LCD_DATA_PORT,((1<<LCD_DATA_PIN0)|(1<<LCD_DATA_PIN1)|(1<<LCD_DATA_PIN2)|(1<<LCD_DATA_PIN3)),((GET_BIT((HIGH_NIPPLE(u8comm)),0)<<LCD_DATA_PIN0))|((GET_BIT((HIGH_NIPPLE(u8comm)),1)<<LCD_DATA_PIN1)|((GET_BIT(HIGH_NIPPLE(u8comm),2)<<LCD_DATA_PIN2)|((GET_BIT((HIGH_NIPPLE(u8comm)),3))<<LCD_DATA_PIN3))));
//	DIO_vdSetPortValue(LCD_DATA_PORT,HIGH_NIPPLE(u8comm));
//

	DIO_vdSetPinValue(LCD_CTRL_PORT,LCD_E_PIN,HIGH);
	_delay_ms(1);
	DIO_vdSetPinValue(LCD_CTRL_PORT,LCD_E_PIN,LOW);

//	_delay_ms(1);
//	//send low nipple
	DIO_vidSetPortMuxVal(LCD_DATA_PORT,((1<<LCD_DATA_PIN0)|(1<<LCD_DATA_PIN1)|(1<<LCD_DATA_PIN2)|(1<<LCD_DATA_PIN3)),((GET_BIT((LOW_NIPPLE(u8comm)),0)<<LCD_DATA_PIN0))|((GET_BIT((LOW_NIPPLE(u8comm)),1)<<LCD_DATA_PIN1)|((GET_BIT(LOW_NIPPLE(u8comm),2)<<LCD_DATA_PIN2)|((GET_BIT((LOW_NIPPLE(u8comm)),3))<<LCD_DATA_PIN3))));

//	DIO_vdSetPortValue(LCD_DATA_PORT,LOW_NIPPLE(u8comm));


#endif
//	Sent High to low pulse
	DIO_vdSetPinValue(LCD_CTRL_PORT,LCD_E_PIN,HIGH);
	_delay_ms(1);
	DIO_vdSetPinValue(LCD_CTRL_PORT,LCD_E_PIN,LOW);

	_delay_ms(1);
}

/*****************************************************************************************
 * to write Data
 * set RS Pin for data
 * clear RW Pin for write
 * put data in DATA_PORT
 * send a high to low pulse
 * ***************************************************************************************
 */
void LCD_vdWriteData(u8 u8data)
{
//	set RS Pin for data
	DIO_vdSetPinValue(LCD_CTRL_PORT,LCD_RS_PIN,HIGH);
//	clear RW Pin for write
	DIO_vdSetPinValue(LCD_CTRL_PORT,LCD_RW_PIN,LOW);

// 	put data in DATA_PORT
#if LCD_DATA_BUS_MODE == LCD_8BIT_MODE
	DIO_vdSetPortValue(LCD_DATA_PORT,u8data);
#endif
	//4 bit mode
#if LCD_DATA_BUS_MODE == LCD_4BIT_MODE
	//send high nipple
	DIO_vidSetPortMuxVal(LCD_DATA_PORT,((1<<LCD_DATA_PIN0)|(1<<LCD_DATA_PIN1)|(1<<LCD_DATA_PIN2)|(1<<LCD_DATA_PIN3)),((GET_BIT((HIGH_NIPPLE(u8data)),0)<<LCD_DATA_PIN0))|((GET_BIT((HIGH_NIPPLE(u8data)),1)<<LCD_DATA_PIN1)|((GET_BIT(HIGH_NIPPLE(u8data),2)<<LCD_DATA_PIN2)|((GET_BIT((HIGH_NIPPLE(u8data)),3))<<LCD_DATA_PIN3))));

//	DIO_vdSetPortValue(LCD_DATA_PORT,HIGH_NIPPLE(u8data));
	//send pulse
	DIO_vdSetPinValue(LCD_CTRL_PORT,LCD_E_PIN,HIGH);
		_delay_us(1);
	DIO_vdSetPinValue(LCD_CTRL_PORT,LCD_E_PIN,LOW);
//	_delay_ms(1);
	//send low nipple
//	DIO_vdSetPortValue(LCD_DATA_PORT,LOW_NIPPLE(u8data));
	DIO_vidSetPortMuxVal(LCD_DATA_PORT,((1<<LCD_DATA_PIN0)|(1<<LCD_DATA_PIN1)|(1<<LCD_DATA_PIN2)|(1<<LCD_DATA_PIN3)),((GET_BIT((LOW_NIPPLE(u8data)),0)<<LCD_DATA_PIN0))|((GET_BIT((LOW_NIPPLE(u8data)),1)<<LCD_DATA_PIN1)|((GET_BIT(LOW_NIPPLE(u8data),2)<<LCD_DATA_PIN2)|((GET_BIT((LOW_NIPPLE(u8data)),3))<<LCD_DATA_PIN3))));


#endif
//	Sent High to low pulse
	DIO_vdSetPinValue(LCD_CTRL_PORT,LCD_E_PIN,HIGH);
	_delay_us(1);
	DIO_vdSetPinValue(LCD_CTRL_PORT,LCD_E_PIN,LOW);

	_delay_us(100);
}
/*
 * to initialize LCD
 *
 */
void LCD_vdInit(void)
{
	_delay_ms(15);

	/*Entry mode
	 *set number of bits
	 *number of lines
	 *character size
	 *
	 */
#if LCD_DATA_BUS_MODE == LCD_8BIT_MODE
#if LCD_NUM_OF_LINES == LCD_2LINE_MODE
#if LCD_CHAR_SIZE == LCD_5_8_DOT_MODE

	LCD_vdWriteCommand(LCD_8Bit_2_Line_5_8_Dot_Mode);
#endif
#endif
#endif

#if LCD_DATA_BUS_MODE == LCD_4BIT_MODE
#if LCD_NUM_OF_LINES == LCD_2LINE_MODE
#if LCD_CHAR_SIZE == LCD_5_8_DOT_MODE

	LCD_vdWriteCommand(0x33);
	LCD_vdWriteCommand(0x32);
	LCD_vdWriteCommand(LCD_4Bit_2_Line_5_8_Dot_Mode);
#endif
#endif
#endif

#if LCD_DATA_BUS_MODE == LCD_8BIT_MODE
#if LCD_NUM_OF_LINES == LCD_1LINE_MODE
#if LCD_CHAR_SIZE == LCD_5_8_DOT_MODE

	LCD_vdWriteCommand(LCD_8Bit_1_Line_5_8_Dot_Mode);
#endif
#endif
#endif

#if LCD_DATA_BUS_MODE == LCD_4BIT_MODE
#if LCD_NUM_OF_LINES == LCD_1LINE_MODE
#if LCD_CHAR_SIZE == LCD_5_8_DOT_MODE
	LCD_vdWriteCommand(0x33);
	LCD_vdWriteCommand(0x32);
	LCD_vdWriteCommand(LCD_4Bit_1_Line_5_8_Dot_Mode);
#endif
#endif
#endif

#if LCD_DATA_BUS_MODE == LCD_8BIT_MODE
#if LCD_NUM_OF_LINES == LCD_1LINE_MODE
#if LCD_CHAR_SIZE == LCD_5_11_DOT_MODE

	LCD_vdWriteCommand(LCD_8Bit_1_Line_5_11_Dot_Mode);
#endif
#endif
#endif

#if LCD_DATA_BUS_MODE == LCD_4BIT_MODE
#if LCD_NUM_OF_LINES == LCD_1LINE_MODE
#if LCD_CHAR_SIZE == LCD_5_11_DOT_MODE
	LCD_vdWriteCommand(0x33);
	LCD_vdWriteCommand(0x32);
	LCD_vdWriteCommand(LCD_4Bit_1_Line_5_11_Dot_Mode);
#endif
#endif
#endif

	/*
	 *choose cursor mode and blinking or not
	 */
#if LCD_CURSOR_STATUS == ON
#if LCD_CURSOR_BLINK_STATUS ==ON
	LCD_vdWriteCommand(LCD_DisplayOnCursorOnBlinkOn);
#endif
#endif

#if LCD_CURSOR_STATUS == ON
#if LCD_CURSOR_BLINK_STATUS ==OFF
	LCD_vdWriteCommand(LCD_DisplayOnCursorOnBlinkOff);
#endif
#endif

#if LCD_CURSOR_STATUS == OFF
#if LCD_CURSOR_BLINK_STATUS ==OFF
	LCD_vdWriteCommand(LCD_DisplayOnCursorOffBlinkOff);
#endif
#endif
/*
 * clear display
 *
 */
	LCD_vdWriteCommand(LCD_ClearDisplay);
}

//to clear display
void LCD_vdClearDisplay(void)
{
	LCD_vdWriteCommand(LCD_ClearDisplay);
}
/*
 * LCD_vdGoToXY
 * fun_arg : row ,column
 * return : void
 * description :
 * moves the cursor to specific position
 */
void LCD_vdGoToXY(u8 u8Row, u8 u8Col)
{
	LCD_vdWriteCommand(0x80|((u8Row * 0x40)+u8Col));
}

/*LCD_vdShiftLeft
 * fun_arg : void
 * return : void
 * description :
 * shift the display left
 *
 */

void LCD_vdShiftLeft(void)
{
	LCD_vdWriteCommand(LCD_ShiftLift);
}

/*LCD_vdShift right
 * fun_arg : void
 * return : void
 * description :
 * shift the display right
 *
 */
void LCD_vdShiftRight(void)
{
	LCD_vdWriteCommand(LCD_ShiftRight);
}

/*
 * LCD_vdWriteString
 * func_arg: pointer to string you want to display
 * return  : void
 * Description :
 * this function take a pointer to string and print it
 */
void LCD_vdWriteString(u8* pu8Data)
{
	u8 u8LoopCounter=0;
		for(u8LoopCounter=0;pu8Data[u8LoopCounter]!='\0';u8LoopCounter++)
		{
			LCD_vdWriteData(pu8Data[u8LoopCounter]);
			if(u8LoopCounter == 15)
			{
				LCD_vdGoToXY(ROW1,COL0);
			}
			if(u8LoopCounter == 31)
			{
				LCD_vdGoToXY(ROW0,COL0);
			}
		}
}

/*LCD_vdWriteNumber
 * func_arg : a signed number to display
 * return : void
 * Description :
 * this function take a number and print it on lcd
 *
 */
void LCD_vdWriteNumber(s16 s16num)
{
	static u8 u8_fn_counter=0;
	s16 s16tmp = s16num;
	s8 u8DigitArr[10]={0};
	u8 u8flag = 0,u8counter =1;      //number is positive
	if (s16num == 0)
	{
		u8DigitArr[u8counter]=0;
		u8counter++;

	}
	else
	{

		while(s16tmp!=0)
		{
			if (s16num<0)   //negative
			{
				u8flag =1;
				u8DigitArr[u8counter] = -s16tmp%10;
				s16tmp/=10;
				u8counter++;

			}

			else if (s16num>0)
			{

				u8DigitArr[u8counter] = s16tmp%10;
				s16tmp/=10;
				u8counter++;

			}
			else
			{
				//do nothing
			}
		}
	}

	//printing number on LCD
	for(u8counter-=1;u8counter>0;u8counter--)
	{
		if (u8flag == 1)
		{
			LCD_vdWriteData('-');
			u8flag =0;
		}
		LCD_vdWriteData(u8DigitArr[u8counter]+48);
		if(u8_fn_counter == 16)
		{
			LCD_vdGoToXY(ROW1,COL0);
		}
		if(u8_fn_counter == 32)
		{
			LCD_vdGoToXY(ROW0,COL0);
			u8_fn_counter=0;
		}
		u8_fn_counter++;
	}

}
void vdWriteMyNameArabic(void)
{
	LCD_vdWriteCommand(0x40);
	LCD_vdWriteData(0x04);
	LCD_vdWriteData(0x04);
	LCD_vdWriteData(0x04);
	LCD_vdWriteData(0x04);
	LCD_vdWriteData(0x04);
	LCD_vdWriteData(0x04);
	LCD_vdWriteData(0x04);
	LCD_vdWriteData(0x04);
//	LCD_vdWriteCommand(0x8f);
//	LCD_vdWriteData(0x00);

	LCD_vdWriteCommand(0x48);
	LCD_vdWriteData(0x00);
	LCD_vdWriteData(0x00);
	LCD_vdWriteData(0x00);
	LCD_vdWriteData(0x00);
	LCD_vdWriteData(0x00);
	LCD_vdWriteData(0x00);
	LCD_vdWriteData(0x00);
	LCD_vdWriteData(0x1f);
//	LCD_vdWriteCommand(0x8e);
//	LCD_vdWriteData(0x01);

	LCD_vdWriteCommand(0x50);
	LCD_vdWriteData(0x05);
	LCD_vdWriteData(0x05);
	LCD_vdWriteData(0x05);
	LCD_vdWriteData(0x05);
	LCD_vdWriteData(0x05);
	LCD_vdWriteData(0x05);
	LCD_vdWriteData(0x05);
	LCD_vdWriteData(0x1f);
//	LCD_vdWriteCommand(0x8d);
//	LCD_vdWriteData(0x02);

	LCD_vdWriteCommand(0x58);
	LCD_vdWriteData(0x07);
	LCD_vdWriteData(0x05);
	LCD_vdWriteData(0x1f);
	LCD_vdWriteData(0x10);
	LCD_vdWriteData(0x10);
	LCD_vdWriteData(0x10);
	LCD_vdWriteData(0x10);
	LCD_vdWriteData(0x10);
//	LCD_vdWriteCommand(0x8c);
//	LCD_vdWriteData(0x03);

	LCD_vdWriteCommand(0x60);
	LCD_vdWriteData(0x04);
	LCD_vdWriteData(0x04);
	LCD_vdWriteData(0x04);
	LCD_vdWriteData(0x04);
	LCD_vdWriteData(0x04);
	LCD_vdWriteData(0x04);
	LCD_vdWriteData(0x04);
	LCD_vdWriteData(0x04);
//	LCD_vdWriteCommand(0x8a);
//	LCD_vdWriteData(0x04);

	LCD_vdWriteCommand(0x68);
	LCD_vdWriteData(0x00);
	LCD_vdWriteData(0x00);
	LCD_vdWriteData(0x00);
	LCD_vdWriteData(0x00);
	LCD_vdWriteData(0x00);
	LCD_vdWriteData(0x07);
	LCD_vdWriteData(0x01);
	LCD_vdWriteData(0x1f);
//	LCD_vdWriteCommand(0x89);
//	LCD_vdWriteData(0x05);

	LCD_vdWriteCommand(0x70);
	LCD_vdWriteData(0x00);
	LCD_vdWriteData(0x00);
	LCD_vdWriteData(0x00);
	LCD_vdWriteData(0x00);
	LCD_vdWriteData(0x00);
	LCD_vdWriteData(0x0e);
	LCD_vdWriteData(0x0a);
	LCD_vdWriteData(0x1f);
//	LCD_vdWriteCommand(0x88);
//	LCD_vdWriteData(0x06);

	LCD_vdWriteCommand(0x78);
	LCD_vdWriteData(0x00);
	LCD_vdWriteData(0x00);
	LCD_vdWriteData(0x00);
	LCD_vdWriteData(0x00);
	LCD_vdWriteData(0x00);
	LCD_vdWriteData(0x02);
	LCD_vdWriteData(0x02);
	LCD_vdWriteData(0x1f);
//	LCD_vdWriteCommand(0x87);
//	LCD_vdWriteData(0x07);

	//	LCD_vdWriteCommand(0x8C);

}

 void writeSpecialChar()
{
	 /*Set CGRAM Address IN AC*/
	LCD_vdWriteCommand(0b01000000);
	LCD_vdWriteData(0b00000100);
	LCD_vdWriteData(0b00001110);
	LCD_vdWriteData(0b00000100);
	LCD_vdWriteData(0b00011111);
	LCD_vdWriteData(0b00000100);
	LCD_vdWriteData(0b00000100);
	LCD_vdWriteData(0b00001010);
	LCD_vdWriteData(0b00010001);
	LCD_vdWriteCommand(0x48);
	LCD_vdWriteData(0b00000100);
	LCD_vdWriteData(0b00001110);
	LCD_vdWriteData(0b00010101);
	LCD_vdWriteData(0b00001110);
	LCD_vdWriteData(0b00000100);
	LCD_vdWriteData(0b00000100);
	LCD_vdWriteData(0b00001010);
	LCD_vdWriteData(0b00010001);
	LCD_vdWriteCommand(0x50);
	LCD_vdWriteData(0b00000100);
	LCD_vdWriteData(0b00001110);
	LCD_vdWriteData(0b00010101);
	LCD_vdWriteData(0b00001110);
	LCD_vdWriteData(0b00000100);
	LCD_vdWriteData(0b00011111);
	LCD_vdWriteData(0b00000000);
	LCD_vdWriteData(0b00000000);

}
 void writeSpecialChar2()
{
	 /*Set CGRAM Address IN AC*/
	LCD_vdWriteCommand(0b01000000);
	LCD_vdWriteData(0b00000100);
	LCD_vdWriteData(0b00001110);
	LCD_vdWriteData(0b00000100);
	LCD_vdWriteData(0b00011111);
	LCD_vdWriteData(0b00000100);
	LCD_vdWriteData(0b00001010);
	LCD_vdWriteData(0b00001010);
	LCD_vdWriteData(0b00001010);

}
