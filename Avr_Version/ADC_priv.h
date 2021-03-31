/*
 * ADC_priv.h
 *
 *  Created on: Mar 26, 2020
 *      Author: IOT scientist
 */

#ifndef ADC_PRIV_H_
#define ADC_PRIV_H_


/*
 * defining ADC registers
 */
#define ADC_ADMUX_REG    *((volatile u8*) 0x27)
#define ADC_ADCSRA_REG   *((volatile u8*) 0x26)
#define ADC_ADCH_REG     *((volatile u8*) 0x25)
#define ADC_ADCL_REG     *((volatile u8*) 0x24)
#define ADC_ADC_REG      *((volatile u16*) 0x24)
#define ADC_SFIOR_REG    *((volatile u8*) 0x50)
#define SREG_REG   *((volatile u8*) 0x5F)

#endif /* ADC_PRIV_H_ */
