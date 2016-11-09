/*
  Copyright (c) 2014 Arduino.  All right reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  See the GNU Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#ifndef _WIRING_CONSTANTS_
#define _WIRING_CONSTANTS_

#ifdef __cplusplus
extern "C"{
#endif // __cplusplus

// Defines for constants used in various places,
// Collected here for your viewing pleasure.

#define LOW             (0x0)
#define HIGH            (0x1)

//   //Interrupt related defines found in WInterrupts.h
//   //MSB used to define that it is related to INTerrupt
//   #define OFF 			0b10000000
//   #define RISING_DMA		0b10000001
//   #define FALLING_DMA		0b10000010
//   #define CHANGE_DMA		0b10000011
//   //		LOW				0b00000000 //already defined
//   #define RISING			0b10001001
//   #define FALLING			0b10001010
//   #define CHANGE			0b10001011
//   //		HIGH			0b00000001 //already defined

//	Pinmode related defines, look closely at wiring_digital.h for explanation
//   #define	INPUT 			0b00010000 
//   #define	INPUT_PULLDOWN 	0b00010010
//   #define	INPUT_PULLUP 	0b00010011
//   #define	OUTPUT 			0b00010100

#define PI 3.1415926535897932384626433832795
#define HALF_PI 1.5707963267948966192313216916398
#define TWO_PI 6.283185307179586476925286766559
#define DEG_TO_RAD 0.017453292519943295769236907684886
#define RAD_TO_DEG 57.295779513082320876798154814105
#define EULER 2.718281828459045235360287471352

#define SERIAL  0x0
#define DISPLAY 0x1

enum BitOrder {
	LSBFIRST = 0,
	MSBFIRST = 1
};


#ifdef __cplusplus
} // extern "C"
#endif // __cplusplus

#endif /* _WIRING_CONSTANTS_ */
