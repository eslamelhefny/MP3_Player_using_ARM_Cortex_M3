/*
 * LCD_int.h
 *
 *  Created on: Jan 27, 2020
 *      Author: IOT scientist
 */

#ifndef LCD_INT_H_
#define LCD_INT_H_
/*
 * Define LCD commands
 */
#define LCD_ClearDisplay                0x01
#define LCD_ReturnHome                  0x02
#define LCD_ShiftLift                   0x05
#define LCD_ShiftRight                  0x07
#define LCD_WriteFromLeftToRight        0x06
#define LCD_WriteFromRightToLeft        0x04
#define LCD_DisplayOnCursorOnBlinkOn    0x0F
#define LCD_DisplayOnCursorOnBlinkOff   0x0E
#define LCD_DisplayOnCursorOffBlinkOff  0x0E
#define LCD_DisplayOffCursorOffBlinkOff 0x08
#define LCD_MoveCursorLeft              0x10
#define LCD_MoveCursorRight             0x14
#define LCD_MoveCursorRightEnd          0x1C
#define LCD_MoveCursorLeftEnd           0x18
#define LCD_8Bit_1_Line_5_8_Dot_Mode    0x30
#define LCD_8Bit_2_Line_5_8_Dot_Mode    0x38
#define LCD_4Bit_1_Line_5_8_Dot_Mode    0x20
#define LCD_4Bit_2_Line_5_8_Dot_Mode    0x28
#define LCD_8Bit_1_Line_5_11_Dot_Mode   0x34
#define LCD_4Bit_1_Line_5_11_Dot_Mode   0x24

/*
 * LCD modes
 */
#define LCD_8BIT_MODE                1
#define LCD_4BIT_MODE                0
#define LCD_2LINE_MODE               1
#define LCD_1LINE_MODE               0
#define LCD_5_8_DOT_MODE             1
#define LCD_5_11_DOT_MODE            0
/*
 * Define LCD raws and colums
 */
#define ROW0  0x00
#define ROW1  0x01

#define COL0  0x00
#define COL1  0x01
#define COL2  0x02
#define COL3  0x03
#define COL4  0x04
#define COL5  0x05
#define COL6  0x06
#define COL7  0x07
#define COL8  0x08
#define COL9  0x09
#define COL10 0x0A
#define COL11 0x0B
#define COL12 0x0C
#define COL13 0x0D
#define COL14 0x0E
#define COL15 0x0F
/*
 * Define functions
 */
void LCD_vdInit();
void LCD_vdWriteData(u8 u8data);
void LCD_vdClearDisplay();
void LCD_vdGoToXY(u8 u8Row, u8 u8Col);
void LCD_vdShiftLeft(void);
void LCD_vdShiftRight(void);
void LCD_vdWriteNumber(s16 s16num);
void LCD_vdWriteString(u8* pu8Data);
void vdWriteMyNameArabic(void);

#endif /* LCD_INT_H_ */
