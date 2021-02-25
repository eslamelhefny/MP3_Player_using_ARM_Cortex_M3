/************************************************************************/
/*	AUTHOR		:		AHMED ATTIA ATTIA 								*/
/*	VERSION		:		V01												*/
/*	DATE		:		10	NOV	2020									*/
/************************************************************************/

#ifndef			RCC_CONFIG_H
#define			RCC_CONFIG_H
/*	Options		:		RCC_HSI
						RCC_HSE
						RCC_PLL						*/
#define			RCC_CLOCK_TYPE			RCC_HSE	


/*	Note: Select this value only if clock type PLL	*/
/*	Options		:		RCC_PLL_HSE_DIV_2
						RCC_PLL_HSE
						RCC_PLL_HSI_DIV_2				*/
#if				RCC_CLOCK_TYPE	==		RCC_PLL
#define			RCC_PLL_INPUT			RCC_PLL_HSE			
#endif

/*	Options		:		may be 2,3,4,5,6,7,..16			*/
#if				RCC_CLOCK_TYPE	==		RCC_PLL
#define			RCC_PLL_MUL_VAL			4			
#endif


#endif