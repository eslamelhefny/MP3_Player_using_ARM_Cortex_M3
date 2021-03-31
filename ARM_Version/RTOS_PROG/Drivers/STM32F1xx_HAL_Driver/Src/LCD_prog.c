

#include "Std_types.h"
#include "Bit_Math.h"
#include "main.h"
#include "cmsis_os.h"
#include "stm32f1xx_hal_gpio.h"

#include "LCD_int.h"
#include "LCD_priv.h"
#include "LCD_config.h"



void _delay_ms(uint32_t copy_u32time)
{
	for(uint32_t i=0;i<copy_u32time;i++)
	{
		for(uint16_t j=0;j<565;j++)
		{
			asm("NOP");
		}
	}
}


void LCD_vidInit( void )
{
	/* wait 35 */
	//_delay_ms(35);
HAL_Delay(35);
	/*  function set */

	LCD_vidSendCmd(0b00111000);
	/* wait 1 */
//	_delay_ms(1);
	HAL_Delay(1);
	/*  function set (on/off) */
	LCD_vidSendCmd(0b00001100);
	/* wait 1 */
//	_delay_ms(1);
	HAL_Delay(1);
	/*  function set (Clear  Display) */


	/*****************************/
	/*LCD_vidSendCmd(0b00000001);*/
	/*	 wait 2                  */
	/*_delay_ms(2);	             */
	/*****************************/

	LCD_vidClear();

	/*  function set (Entry mode set - cursor increase or decrease ) */
	LCD_vidSendCmd(0b00000110);
	/* wait 2 */
	//_delay_ms(2);
	HAL_Delay(2);
	/* latch = 0 */
	//DIO_voidSetPinValue(DIO_U8_PORTA, LCD_u8EN , DIO_U8_LOW );
	HAL_GPIO_WritePin( GPIOA, LCD_u8EN , GPIO_PIN_RESET );

}

void LCD_vidSendCmd( uint8_t u8CmdCpy )
{
	/** to send command  ( RS = 0 and R/W = 0 ) =====> (write instruction) */

	/* RS  0 CMD */  
	//DIO_voidSetPinValue(DIO_U8_PORTA, LCD_u8RS , DIO_U8_LOW );
	HAL_GPIO_WritePin( GPIOA, LCD_u8RS , GPIO_PIN_RESET );
	/* write 0 CMD*/
	//DIO_voidSetPinValue( DIO_U8_PORTA,LCD_u8RW , DIO_U8_LOW );
	HAL_GPIO_WritePin( GPIOA, LCD_u8RW , GPIO_PIN_RESET );

	/* take data pass data bu */
	vidPutOnBus(u8CmdCpy);

}

void LCD_vidWriteChar( uint8_t u8CharCpy )
{
	/** to write command   ( RS = 1 and R/W = 0 ) =====> (write to DR Reg) */

	/* RS  1 CMD */
	//DIO_voidSetPinValue(DIO_U8_PORTA, LCD_u8RS , DIO_U8_HIGH );
	HAL_GPIO_WritePin( GPIOA, LCD_u8RS  , GPIO_PIN_SET );
	/* write 0 CMD*/
	//DIO_voidSetPinValue(DIO_U8_PORTA, LCD_u8RW , DIO_U8_LOW );
	HAL_GPIO_WritePin( GPIOA, LCD_u8RW  , GPIO_PIN_RESET );

	/* take data pass data bu */
	vidPutOnBus(u8CharCpy);
}

static void vidPutOnBus( uint8_t u8CmdDataCpy )
{
	uint8_t LOC_u8Loop = LCD_DATA_D0_START ;

	HAL_GPIO_WritePin( GPIOA, LCD_u8DATA_0 , GET_BIT(u8CmdDataCpy , LOC_u8Loop++ ));
	HAL_GPIO_WritePin( GPIOA, LCD_u8DATA_1 , GET_BIT(u8CmdDataCpy , LOC_u8Loop++ ));
	HAL_GPIO_WritePin( GPIOA, LCD_u8DATA_2 , GET_BIT(u8CmdDataCpy , LOC_u8Loop++ ));
	HAL_GPIO_WritePin( GPIOA, LCD_u8DATA_3 , GET_BIT(u8CmdDataCpy , LOC_u8Loop++ ));
	HAL_GPIO_WritePin( GPIOA, LCD_u8DATA_4 , GET_BIT(u8CmdDataCpy , LOC_u8Loop++ ));
	HAL_GPIO_WritePin( GPIOB, LCD_u8DATA_5 , GET_BIT(u8CmdDataCpy , LOC_u8Loop++ ));
	HAL_GPIO_WritePin( GPIOA, LCD_u8DATA_6 , GET_BIT(u8CmdDataCpy , LOC_u8Loop++ ));
	HAL_GPIO_WritePin( GPIOA, LCD_u8DATA_7 , GET_BIT(u8CmdDataCpy , LOC_u8Loop++ ));

	/* Latch  */
	//DIO_voidSetPinValue(DIO_U8_PORTA, LCD_u8EN , DIO_U8_HIGH );
	HAL_GPIO_WritePin( GPIOA, LCD_u8EN , GPIO_PIN_SET );
	//_delay_ms(2);
	HAL_Delay(2);
	//DIO_voidSetPinValue(DIO_U8_PORTA, LCD_u8EN , DIO_U8_LOW );
	HAL_GPIO_WritePin( GPIOA, LCD_u8EN , GPIO_PIN_RESET );
	//_delay_ms(2);
	HAL_Delay(2);

}

void LCD_vidClear( void )
{
	/*  function set (Clear  Display) */
	LCD_vidSendCmd(0b00000001);
	/* wait 2 */
	//_delay_ms(2);
	HAL_Delay(2);
}


void LCD_vidGotoXY( uint8_t u8LineNbCpy , uint8_t u8CharNbCpy )
{
	/* switch case over line */
	if(  u8LineNbCpy == 1 )
	{
		/*  function set (Line1 position " x ") */
		LCD_vidSendCmd( (LCD_BEGINNING_FIRST_LINE + u8CharNbCpy) );

	}

	else if( u8LineNbCpy == 2 )
	{
		/*  function set (Line2 position " x ") */
		LCD_vidSendCmd( (LCD_BEGINNING_SECOND_LINE + u8CharNbCpy) );

	}


}


/* Write string */
void LCD_vidWriteString( uint8_t * pu8StringCpy )
{
	uint8_t Loc_u8Counter = 0 ;

	while(pu8StringCpy[Loc_u8Counter] !='\0')
	{
		LCD_vidWriteChar(pu8StringCpy[Loc_u8Counter]);
		Loc_u8Counter++;		
	}


}
