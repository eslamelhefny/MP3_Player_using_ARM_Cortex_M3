/*
 * UART_interface.h
 *
 *  Created on: Feb 15, 2021
 *      Author: Mohamed Allam
 */

#ifndef UART_HEADERS_UART_INTERFACE_H_
#define UART_HEADERS_UART_INTERFACE_H_


#include "STD_TYPES.h"
#include "BIT_MATH.h"

typedef enum {

	ID_UART1=1,
	ID_UART2,
	ID_UART3,
/* Number of UART units */
}UART_ID_type;
typedef enum {

	STRTBIT_8DATA_NSTOPBIT,
	STRTBIT_9DATA_NSTOPBIT,

}Word_length_type;

typedef enum {

	NO_PARITY=0,
	EVEN_PARITY=1,
	ODD_PARITY=3,

}ParityControl_Status_type;


typedef enum {

	PE,		/* Parity Error */
	TXE,	/*TX buffer is empty */
	CTS,    /* CTS flag		 */
	TC,     /* TX  compelete*/
	RXNE,   /* Recived data ready to be read */
	IDLE,   /*Idel line detected */
	LBD, /*Break Flag*/

	NE,      	/*Noise Error NE */	/*OverRun Error ORE*/  /*Framing Error FR      */
	ORE,		/*         Form multi comunication buffer DMAR			          */
	FR,         /*This bit generates interrupts for these status when Enable  EIEI*/
}Interrupt_type;

typedef enum {
	STOP_1_BIT,
	STOP__5_BIT, /*Smart card*/
	STOP_2_BIT,
	STOP_1_5_BIT, /*Smart Card*/

}StopBits_Conf_type;

/**************** Synchrounous mode***************/
typedef enum {
	CLOCK_DISABLE,
	CLOCK_ENABLE,

}Clock_Conf_type;

typedef enum {
	FIRSTCLOCK_FIRSTDATA,
	SECONDCLOCK_FIRSTDATA,

}ClockPhase_Conf_type;

//............... etc .............................


typedef enum {
	DMA_TX_DISABLE,
	DMA_TX_ENABLE,

}DMATransimitter_Conf_type;

typedef enum {
	DMA_RX_DISABLE,
	DMA_RX_ENABLE,

}DMAReciver_Conf_type;

typedef enum {

/* **********************************************************************
 * for FPU = 8 MHZ		9600 ==> 0d52.083333   fcpu/(16 *USART_BRR)		*
 * Mantissa = 0d52 =0x34												*
 * fraction = .0833 *16 = 0x1 											*
 * 	USART_BRR = 0x341   												*
 * **********************************************************************
 * 2400==>0d208.333														*
 * Matissa = 0d208 = 0xD0												*
 * Fraction= 0d5 =0x5													*
 * USART_BRR = 0xD05													*
 * **********************************************************************
 * 19200 ==> 26.0417													*
 * Matissa = 0d26 = 0x1A												*
 * Fraction= 0d7 =0x7													*
 * USART_BRR = 0xA17													*
 * **********************************************************************
 * 57600 ==>0d8.68														*
 * Mantissa= 0x8														*
 * Fraction=0d11=0xB													*
 *  USART_BRR = 0x8B													*
 *  *********************************************************************
 *  115200==>0d4.34														*
 *  Mantissa= 0x4														*
 * Fraction=0d5=0x5														*
 *  USART_BRR = 0x45													*
 *  *********************************************************************
 */
	RATE_2400=0xD05,
	RATE_9600=0x341,
	RATE_19200=0xA17,
	RATE_57600=0x8B,
	RATE_115200=045,
	//etc

}BoudRate_8MHZ_Conf_type;

/******************************************************************************************************************
 * SYNCHRONOUS MODE , LIN MODE , SMART CARD MODE , MULITPROCESSOR COMUNICATION ,SINGLE WIRE (HALF DUPLEX MODE)
 * 					 PINS  == > SW_RX ,RTS ,CTS
 * 					     Underconstruction
 */

/**********************************************************************************************/

typedef struct {

	UART_ID_type				  ID;
	Word_length_type	 		  word;
	ParityControl_Status_type	  parity;
	StopBits_Conf_type			  stop;
	DMATransimitter_Conf_type	  dmaTx;
	DMAReciver_Conf_type          dmaRx;
	BoudRate_8MHZ_Conf_type		  boudrate;
/* For more UART units init create more objects in main of this struct  and attach Its ID */
}UART_init_type;





void UART_voidInit(UART_init_type * uart);

void UART_voidSendChar(UART_init_type * uart , u8  data);
u8 UART_u8ReciveChar(UART_init_type * uart);
u8 UART_u8ReciveCharPeriodic(UART_init_type * uart , u8* data);

void UART_voidSendString(UART_init_type * uart ,u8 * str);
void UART_voidReciveString(UART_init_type * uart, u8 * str);
void UART_voidInterruptEnable(UART_init_type * uart , Interrupt_type interrupt);
void UART_voidInterruptDisable(UART_init_type * uart, Interrupt_type interrupt);
void UART_voidSetCallBack(UART_init_type * uart ,  void(*pf)(void));
			/*			Extra	API	       */
void UART_voidUartDisable(UART_init_type * uart);
void UART_voidUartEnable(UART_init_type * uart);
void UART_SendNumber(UART_init_type * uart ,u32 num);
u32 UART_RecivNumber(UART_init_type * uart );

void UART_voidSendStringCheckSum(UART_init_type * uart ,u8 *str); // my protocole
u8 UART_voidReciveStringCheckSum(UART_init_type * uart , u8 *str);

#endif /* UART_HEADERS_UART_INTERFACE_H_ */
