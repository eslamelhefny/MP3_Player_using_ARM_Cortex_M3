/*
 * STD_TYPS.H
 *
 *  Created on: Jan 23, 2020
 *      Author: eslamelhefny
 *
 */

/*#############################STD_TYPES####################
 * this file contain standard types
 * for embedded c
 *
 */

#ifndef STD_TYPES_H_
#define STD_TYPES_H_
/*
 * type for 8 bit unsigned
 * its range from 0 to 255
 */
typedef unsigned char u8;
/*
 * type for 16 bit unsigned
 * its range from 0 to 65535
 */
typedef unsigned short u16;
/*
 * type for 32 bit unsigned
 * its range from 0 to 2^32
 */
typedef unsigned int u32;
/*
 * type for 8 bit signed
 * its range from -128 to 127
 */
typedef signed char s8;
/*
 * type for 16 bit signed
 * its range from -32768 to 32767
 */
typedef signed short s16;
/*
 * type for 32 bit signed
 * its range from -2^(31) to 2^(31)-1
 */
typedef signed int s32;
/*
 * type for floating points numbers
 */
typedef float f32;
typedef double f64;

/*
 * bool
 */
#ifndef bool
typedef unsigned char  bool;
#define TRUE  1
#define FALSE 0
#endif
#define ON 1
#define OFF 0
#endif /* STD_TYPES_H_ */
