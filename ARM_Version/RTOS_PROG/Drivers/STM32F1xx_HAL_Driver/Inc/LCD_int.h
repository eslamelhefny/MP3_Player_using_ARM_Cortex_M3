/******************************************************/
/* Author  : Ahmed othman                         */
/* Date    : 4 FEB 2021                             */
/* Version : 1V                                       */
/******************************************************/

#ifndef _LCD_INT_H
#define _LCD_INT_H




void LCD_vidInit( void ); 

void LCD_vidSendCmd( uint8_t u8CmdCpy );

void LCD_vidWriteChar( uint8_t u8CharCpy );

void LCD_vidClear( void );

void LCD_vidGotoXY( uint8_t u8LineNbCpy , uint8_t u8CharNbCpy );

void LCD_vidWriteString( uint8_t * pu8StringCpy );

#endif 
