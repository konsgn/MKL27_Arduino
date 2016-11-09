/*
  Copyright (c) 2011-2012 Arduino.  All right reserved.

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

#ifndef _WIRING_INTERRUPTS_
#define _WIRING_INTERRUPTS_

#include "Arduino.h"

#ifdef __cplusplus
extern "C" {
#endif

//Interrupt related defines, use MSB to signify that they are related to INT
#define OFF 			0b10000000
#define RISING_DMA		0b10000001
#define FALLING_DMA		0b10000010
#define CHANGE_DMA		0b10000011
//		LOW				0b00000000 //already defined
#define RISING			0b10001001
#define FALLING			0b10001010
#define CHANGE			0b10001011
//		HIGH			0b00000001 //already defined

void attachInterrupt(uint32_t pin, void (*callback)(void), uint8_t mode);

void detachInterrupt(uint8_t pin);

#ifdef __cplusplus
}
#endif

#endif /* _WIRING_INTERRUPTS_ */
