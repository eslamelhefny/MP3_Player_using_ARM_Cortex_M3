#include <avr/io.h>
#include"STD_TYPES.H"
#include "usart_driver.h"

void usart_init(unsigned short int baudrate)
{
	//Set baud rate
	unsigned short int UBRRVAL=(FCLK_SYSTEM/(baudrate*16UL))-1;
	UBRRL=UBRRVAL; 			//low byte
	UBRRH=(UBRRVAL>>8); 	//high byte

	//Set data frame format: asynchronous mode,no parity, 1 stop bit, 8 bit size
	UCSRC=(1<<URSEL)|(0<<UMSEL)|(0<<UPM1)|(0<<UPM0)|(0<<USBS)|(0<<UCSZ2)|(1<<UCSZ1)|(1<<UCSZ0); 

	//Enable Transmitter and Receiver
	UCSRB=(1<<RXEN)|(1<<TXEN);
}
unsigned char usart_getc( void )
{
	/* Wait for data to be received */
	while ( !(UCSRA & (1<<RXC)) );
	/* Get and return received data from buffer */
	return UDR;
}
void usart_putc( unsigned char data )
{
	/* Wait for empty transmit buffer */
	while ( !( UCSRA & (1<<UDRE)));
	/* Put data into buffer, sends the data */
	UDR = data;
}
void usart_puts( char* str )
{
	short int i=0;
	while(str[i])
	{
		usart_putc(str[i]);
		i++;
	}
}

void usart_putNumber(s16 num)
{
//	static u8 u8_fn_counter=0;
	s16 tmp = num;
	u8 u8DigitArr[10]={0};
	u8 u8flag = 0,u8counter =1;      //number is positive
	if (num == 0)
	{
		u8DigitArr[u8counter]=0;
		u8counter++;

	}
	else
	{

		while(tmp!=0)
		{
			if (num>0)   //negative
			{
				u8flag =1;
				u8DigitArr[u8counter] = -tmp%10;
				tmp/=10;
				u8counter++;

			}

			else if (num>0)
			{

				u8DigitArr[u8counter] = tmp%10;
				tmp/=10;
				u8counter++;

			}
			else
			{
				//do nothing
			}
		}
	}
	u8DigitArr[u8counter] ='\0';
	usart_puts(u8DigitArr);




}
