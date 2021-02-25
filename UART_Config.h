/*
 * UART_Config.h
 *
 *  Created on: Feb 15, 2021
 *      Author: Mohamed Allam
 */

#ifndef UART_HEADERS_UART_CONFIG_H_
#define UART_HEADERS_UART_CONFIG_H_

/*
 * After writing the last data into the USART_DR register, it is mandatory to wait for TC=1
 *	before disabling the USART or causing the microcontroller to enter the low-power mode
 */

/*
 * NUMBER_OF_UART_UNITS
 */

#define        NUMBER_OF_UART_UNITS				3

/*
 * MAX_STRING_SIZE in bytes
 */

#define       MAX_STRING_SIZE					1


#endif /* UART_HEADERS_UART_CONFIG_H_ */
