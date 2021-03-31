/*
 * DIO_int.h
 *
 *  Created on: Jan 23, 2020
 *      Author: IOT scientist
 */

#ifndef DIO_INT_H_
#define DIO_INT_H_
/*
 * avr ports
 */
#define DIO_PORTA 0
#define DIO_PORTB 1
#define DIO_PORTC 2
#define DIO_PORTD 3
/*
 * avr pins
 */
#define PIN0 0
#define PIN1 1
#define PIN2 2
#define PIN3 3
#define PIN4 4
#define PIN5 5
#define PIN6 6
#define PIN7 7
/*
 * general values
 */
#define HIGH   0xff
#define LOW    0x00
#define INPUT  0X00
#define OUTPUT 0xFF

void DIO_vdSetPinDirection(u8 u8Portid, u8 u8PinId, u8 u8Dir);
void DIO_vdSetPinValue(u8 u8PortId, u8 u8PinId, u8 u8Val);
void DIO_vdTogglePin(u8 u8PortId,u8 u8PinId);
u8 DIO_u8GetPinVal(u8 u8PortId, u8 u8PinId);
void DIO_vdSetPortDirection(u8 u8PortId, u8 u8Dir);
void DIO_vdSetPortValue(u8 u8PortId, u8 u8Val);
void DIO_vdEnablePullUpRes(u8 u8PortId, u8 u8PinId);
void DIO_vidSetPortMuxVal(u8 u8Portid ,u8 u8mask, u8 u8val);
#endif /* DIO_INT_H_ */
