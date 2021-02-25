#ifndef		_STK_PRIVATE_H
#define		_STK_PRIVATE_H

typedef struct
{
	volatile u32 CTRL	;
	volatile u32 LOAD	;
	volatile u32 VAL	;
	
}MSTK_Type;

#define			MSTK			((volatile MSTK_Type *)(0xE000E010))

#define			MSTK_SRC_AHB				1
#define			MSTK_SRC_AHB_8				0

#define			MSTK_SINGLE_INTERVAL		0
#define			MSTK_PERIOD_INTERVAL		1

#endif
