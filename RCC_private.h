/************************************************************************/
/*	AUTHOR		:		AHMED ATTIA ATTIA 								*/
/*	VERSION		:		V01												*/
/*	DATE		:		10	NOV	2020									*/
/************************************************************************/

#ifndef			RCC_PRIVATE_H
#define			RCC_PRIVATE_H




#define		RCC_CR				*((volatile u32 * )0x40021000)
#define		RCC_CFGR			*((volatile u32 * )0x40021004)
#define		RCC_CIR				*((volatile u32 * )0x40021008)
#define		RCC_APB2RSTR		*((volatile u32 * )0x4002100C)
#define		RCC_APB1RSTR		*((volatile u32 * )0x40021010)
#define		RCC_AHBENR			*((volatile u32 * )0x40021014)
#define		RCC_APB2ENR			*((volatile u32 * )0x40021018)
#define		RCC_APB1ENR			*((volatile u32 * )0x4002101C)
#define		RCC_BDCR			*((volatile u32 * )0x40021020)
#define		RCC_CSR				*((volatile u32 * )0x40021024)


#define		RCC_HSI				0
#define		RCC_HSE				1
#define		RCC_PLL				2

#endif







