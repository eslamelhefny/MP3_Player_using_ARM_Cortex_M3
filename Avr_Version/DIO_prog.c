/*
 * DIO_prog.c
 *
 *  Created on: Jan 23, 2020
 *      Author: IOT scientist
 */
#include "STD_TYPES.H"
#include "DIO_int.h"
#include "DIO_priv.h"
#include "BIT_math.h"
/*************************************
 *
 */
volatile u8* u8PORT_arr[4] = { PORTA_REG, PORTB_REG, PORTC_REG, PORTD_REG };
volatile u8* u8DDR_arr[4]  = { DDRA_REG, DDRB_REG, DDRC_REG, DDRD_REG };
volatile u8* u8PIN_arr[4]  = { PINA_REG, PINB_REG, PINC_REG, PIND_REG};
/*****************************************************************************
 * this function set Pin direction input or output
 * take port number and portid (from PortA to PortD) and pinId (from Pin0 to Pin7) and direction (input or output)
 * return void
 * algorithm :
 * 		first check the input and it should be between porta to portd
 * 		and Pin0 to pin7
 * 		put the direction in DDR_Reg
 * 		***********************************************************************
 */
void DIO_vdSetPinDirection(u8 u8Portid, u8 u8PinId, u8 u8Dir)
{
	if ((u8Portid <= DIO_PORTD) && (u8PinId <= PIN7))
	{
		if (u8Dir == OUTPUT)
		{
			SET_BIT(*(u8DDR_arr[u8Portid]), u8PinId);

		}
		else if (u8Dir == INPUT)
		{
			CLEAR_BIT(*(u8DDR_arr[u8Portid]), u8PinId);

		}
		else
		{

			/*
			 * not valid
			 */
		}
	}
	else
	{
		//do nothing
	}
}
/*****************************************************************************
 * this function set Pin Value high or low
 * take port number and portid (from PortA to PortD) and pinId (from Pin0 to Pin7) and value(high or low)
 * return void
 * algorithm :
 * 		first check the input and it should be between porta to portd
 * 		and Pin0 to pin7
 * 		put the value in PORT_Reg
 * 		***********************************************************************
 */
void DIO_vdSetPinValue(u8 u8PortId, u8 u8PinId, u8 u8Val)
{
	if ((u8PortId <= DIO_PORTD) && (u8PinId <= PIN7))
	{
		if (u8Val == HIGH)
		{
			SET_BIT(*(u8PORT_arr[u8PortId]), u8PinId);
		}
		else if (u8Val == LOW)
		{
			CLEAR_BIT(*(u8PORT_arr[u8PortId]), u8PinId);
		}
		else
		{
			/*
			 * invalid input
			 */
		}

	}
	else
	{
		/*
		 * do nothing
		 */
	}

}
/*****************************************************************************
 * this function Toggle Pin Value if 0 make it 1 and vice versa..
 * take port number and portid (from PortA to PortD) and pinId (from Pin0 to Pin7)
 * return void
 * algorithm :
 * 		first check the input and it should be between porta to portd
 * 		and Pin0 to pin7
 * 		toggle pin by xoring it with one
 * 		***********************************************************************
 */
void DIO_vdTogglePin(u8 u8PortId,u8 u8PinId)
{
	if ((u8PortId <= DIO_PORTD) && (u8PinId <= PIN7))
		{
			TOGGLE_BIT(*(u8PORT_arr[u8PortId]),u8PinId);

		}
	else
	{
		/*
		 * do nothing
		 */
	}

}
/*****************************************************************************
 * this function Get Pin Value high or low
 * take port number and portid (from PortA to PortD) and pinId (from Pin0 to Pin7)
 * return u8 pin value
 * algorithm :
 * 		first check the input and it should be between porta to portd
 * 		and Pin0 to pin7
 * 		take pin value in variable and return it
 * 		***********************************************************************
 */
u8 DIO_u8GetPinVal(u8 u8PortId, u8 u8PinId)
{
	u8 u8PinVal =0;
	if ((u8PortId <= DIO_PORTD) && (u8PinId <= PIN7))
		{
			u8PinVal = GET_BIT(*(u8PIN_arr[u8PortId]),u8PinId);

		}
	else
	{
		/*
		 * do nothing
		 */
	}
	return u8PinVal;

}
/*****************************************************************************
 * this function set Port Direction input or output
 * take port number and portid (from PortA to PortD)
 * return void
 * algorithm :
 * 		first check the input and it should be between porta to portd
 * 		put the direction value in DDR_reg
 * 		***********************************************************************
 */
void DIO_vdSetPortDirection(u8 u8PortId, u8 u8Dir)
{
	if (u8PortId <= DIO_PORTD)
		{
			*(u8DDR_arr[u8PortId]) = u8Dir;

		}
	else
	{
		/*
		 * do nothing
		 */
	}

}
/*****************************************************************************
 * this function set Port Value High or Low
 * take port number and portid (from PortA to PortD)
 * return void
 * algorithm :
 * 		first check the input and it should be between porta to portd
 * 		put the value in PORT_Reg
 * 		***********************************************************************
 */
void DIO_vdSetPortValue(u8 u8PortId, u8 u8Val)
{
	if (u8PortId <= DIO_PORTD)
	{
		*(u8PORT_arr[u8PortId]) = u8Val;
	}
	else
	{
		/*
		 * do nothing
		 */
	}

}
/*****************************************************************************
 * this function set Pin Value input or output
 * take port number and portid (from PortA to PortD) and pinId (from Pin0 to Pin7)
 * return void
 * algorithm :
 * 		first check the input and it should be between porta to portd
 * 		and Pin0 to pin7
 * 		put the value in PORT_Reg
 * 		***********************************************************************
 */
void DIO_vdEnablePullUpRes(u8 u8PortId, u8 u8PinId)
{
	if ((u8PortId <= DIO_PORTD) && (u8PinId <= PIN7))
	{
		SET_BIT(*(u8PORT_arr[u8PortId]),u8PinId);
	}
	else
	{
		/*
		 * do nothing
		 */
	}

}

/*
 * DIO_vidSetPortMuxVal
 * func_arg : port number and mask and value
 * return val : void
 * Description : this function sets specific number of bits of port at a time
 */

void DIO_vidSetPortMuxVal(u8 u8Portid ,u8 u8mask, u8 u8val)
{
	if ( u8Portid >= DIO_PORTA )
	{
		*(u8PORT_arr[u8Portid])=((*(u8PORT_arr[u8Portid]) & (~u8mask ))| u8val);
	}
}
