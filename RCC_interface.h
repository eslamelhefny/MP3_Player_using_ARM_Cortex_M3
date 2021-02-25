/************************************************************************/
/*	AUTHOR		:		AHMED ATTIA ATTIA 								*/
/*	VERSION		:		V01												*/
/*	DATE		:		10	NOV	2020									*/
/************************************************************************/

#ifndef			RCC_INTERFACE_H
#define			RCC_INTERFACE_H

#define			RCC_AHB				1
#define			RCC_APB1			3
#define			RCC_APB2			6

void RCC_voidInit(void);

void RCC_voidEnableClock(u8 Copy_u8BusID , u8 Copy_u8PeripheralID );
void RCC_voidDisableClock(u8 Copy_u8BusID , u8 Copy_u8PeripheralID );

#endif