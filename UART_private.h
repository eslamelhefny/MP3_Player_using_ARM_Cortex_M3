/*
 * UART_private.h
 *
 *  Created on: Feb 15, 2021
 *      Author: Mohamed Allam
 */

#ifndef UART_HEADERS_UART_PRIVATE_H_
#define UART_HEADERS_UART_PRIVATE_H_

typedef struct {
	volatile u32  SR ;
	volatile u32  DR ;
	volatile u32  BRR ;
	volatile u32  CR1 ;
	volatile u32  CR2 ;
	volatile u32  CR3 ;
	volatile u32  GTPR ;


}UART_type;


#define   UART1 		  ((UART_type*)(0x40013800))
#define   UART2 		  ((UART_type*)(0x40004400))
#define   UART3			  ((UART_type*)(0x40004800))
#define   UART4			  ((UART_type*)(0x40004C00))
#define   UART5			  ((UART_type*)(0x40005000))

/**********************************************************************************
 * Important note :- the data Register (USART_DR) address is shifted 1 UART unit .*
 * ********************************************************************************
 *
 */







#endif /* UART_HEADERS_UART_PRIVATE_H_ */
