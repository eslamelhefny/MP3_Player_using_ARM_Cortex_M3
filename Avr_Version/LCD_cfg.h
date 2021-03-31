/*
 * LCD_cfg.h
 *
 *  Created on: Jan 27, 2020
 *      Author: IOT scientist
 */

#ifndef LCD_CFG_H_
#define LCD_CFG_H_

#define LCD_CTRL_PORT                DIO_PORTC
#define LCD_DATA_PORT                DIO_PORTB

#define LCD_RS_PIN                   0
#define LCD_RW_PIN                   1
#define LCD_E_PIN                    2

/*
 * incase 4bit mode
 */
#define LCD_DATA_PIN0                0
#define LCD_DATA_PIN1                1
#define LCD_DATA_PIN2                2
#define LCD_DATA_PIN3                3


/*
 * change your configuration here
 */

#define LCD_DATA_BUS_MODE            LCD_8BIT_MODE    //LCD_8BIT_MODE or LCD_4BIT_MODE
#define LCD_NUM_OF_LINES             LCD_2LINE_MODE   //LCD_2LINE_MODE or LCD_1LINE_MODE
#define LCD_CHAR_SIZE                LCD_5_8_DOT_MODE //LCD_5_8_DOT_MODE or LCD_5_11_DOT_MODE
#define LCD_CURSOR_STATUS            ON               //ON or off
#define LCD_CURSOR_BLINK_STATUS      ON               //ON or off

#endif /* LCD_CFG_H_ */
