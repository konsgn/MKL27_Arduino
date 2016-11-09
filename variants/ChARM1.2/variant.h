/*
  Copyright (c) 2016 Konstantin  All right reserved.

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

#ifndef __VARIANT_H_
#define __VARIANT_H_

/*----------------------------------------------------------------------------
 *        Definitions
 *----------------------------------------------------------------------------*/
//CPU Selection
#define CPU_MKL27Z64VFM4
#define CPU_MKL27_64



//Crystal And clock info 
#define Board_Crystal NULL
/** Frequency of the Core clock */
#define MKL_CORE_CLK		48000000
/** Frequency of the Platform clock */
#define MKL_PLATFORM_CLK	48000000
/** Frequency of the System Clock */
#define MKL_SYSTEM_CLK		48000000
/** Frequency of the Bus Clock */
#define MKL_BUS_CLK			24000000
/** Frequency of the Flash Clock */
#define MKL_FLASH_CLK		24000000

/*----------------------------------------------------------------------------
 *        Headers
 *----------------------------------------------------------------------------*/

#include "Arduino.h"
#include "pins_arduino.h"
#include <MKL27Z644.h>

//#ifdef __cplusplus
//#include "UARTClass.h"
//#include "USARTClass.h"
//#endif

//#ifdef __cplusplus
//extern "C"{
//#endif // __cplusplus

//**
 //* Libc porting layers
 //*/
//#if defined (  __GNUC__  ) /* GCC CS3 */
//#    include <syscalls.h> /** RedHat Newlib minimal stub */
//#endif

/*----------------------------------------------------------------------------
 *        Pins
 *----------------------------------------------------------------------------*/

// Number of pins defined in PinDescription array
#define PINS_COUNT           (22u)
#define NUM_DIGITAL_PINS     (22u)
#define NUM_ANALOG_INPUTS    (6u)

#define isPinAnalog(pin) ((22>pin)&(pin>15))
#define digitalPinToAnalogNum(pin) (pin-16) //defined channel array must
	// start from lowest A pin. in ChARM it is A5/16
extern const uint8_t AnalogChannel[];

#define A0 21
#define A1 20
#define A2 19
#define A3 18
#define A4 17
#define A5 16


#define PIN_0_BIT			4
#define PIN_1_BIT			5
#define PIN_2_BIT			6
#define PIN_3_BIT			7
#define PIN_4_BIT			0
#define PIN_5_BIT			3
#define PIN_6_BIT			0
#define PIN_7_BIT			18
#define PIN_8_BIT			19
#define PIN_9_BIT			1
#define PIN_10_BIT			2
#define PIN_11_BIT			3
#define PIN_12_BIT			4
#define PIN_13_BIT			5
#define PIN_14_BIT			6
#define PIN_15_BIT			7
#define PIN_16_BIT			1
#define PIN_17_BIT			2
#define PIN_18_BIT			1
#define PIN_19_BIT			0
#define PIN_20_BIT			30
#define PIN_21_BIT			16

#define PIN_0_BITMASK		(1<<(PIN_0_BIT))
#define PIN_1_BITMASK		(1<<(PIN_1_BIT))
#define PIN_2_BITMASK		(1<<(PIN_2_BIT))
#define PIN_3_BITMASK		(1<<(PIN_3_BIT))
#define PIN_4_BITMASK		(1<<(PIN_4_BIT))
#define PIN_5_BITMASK		(1<<(PIN_5_BIT))
#define PIN_6_BITMASK		(1<<(PIN_6_BIT))
#define PIN_7_BITMASK		(1<<(PIN_7_BIT))
#define PIN_8_BITMASK		(1<<(PIN_8_BIT))
#define PIN_9_BITMASK		(1<<(PIN_9_BIT))
#define PIN_10_BITMASK		(1<<(PIN_10_BIT))
#define PIN_11_BITMASK		(1<<(PIN_11_BIT))
#define PIN_12_BITMASK		(1<<(PIN_12_BIT))
#define PIN_13_BITMASK		(1<<(PIN_13_BIT))
#define PIN_14_BITMASK		(1<<(PIN_14_BIT))
#define PIN_15_BITMASK		(1<<(PIN_15_BIT))
#define PIN_16_BITMASK		(1<<(PIN_16_BIT))
#define PIN_17_BITMASK		(1<<(PIN_17_BIT))
#define PIN_18_BITMASK		(1<<(PIN_18_BIT))
#define PIN_19_BITMASK		(1<<(PIN_19_BIT))
#define PIN_20_BITMASK		(1<<(PIN_20_BIT))
#define PIN_21_BITMASK		(1<<(PIN_21_BIT))

#define PIN_0_PORT			PrtD
#define PIN_1_PORT			PrtD
#define PIN_2_PORT			PrtD
#define PIN_3_PORT			PrtD
#define PIN_4_PORT			PrtA
#define PIN_5_PORT			PrtA
#define PIN_6_PORT			PrtE
#define PIN_7_PORT			PrtA
#define PIN_8_PORT			PrtA
#define PIN_9_PORT			PrtA
#define PIN_10_PORT			PrtA
#define PIN_11_PORT			PrtC
#define PIN_12_PORT			PrtC
#define PIN_13_PORT			PrtC
#define PIN_14_PORT			PrtC
#define PIN_15_PORT			PrtC
#define PIN_16_PORT			PrtC
#define PIN_17_PORT			PrtC
#define PIN_18_PORT			PrtB
#define PIN_19_PORT			PrtB
#define PIN_20_PORT			PrtE
#define PIN_21_PORT			PrtE

#define PIN_0_CONFIG		PORTD_BASE
#define PIN_1_CONFIG		PORTD_BASE
#define PIN_2_CONFIG		PORTD_BASE
#define PIN_3_CONFIG		PORTD_BASE
#define PIN_4_CONFIG		PORTA_BASE
#define PIN_5_CONFIG		PORTA_BASE
#define PIN_6_CONFIG		PORTE_BASE
#define PIN_7_CONFIG		PORTA_BASE
#define PIN_8_CONFIG		PORTA_BASE
#define PIN_9_CONFIG		PORTA_BASE
#define PIN_10_CONFIG		PORTA_BASE
#define PIN_11_CONFIG		PORTC_BASE
#define PIN_12_CONFIG		PORTC_BASE
#define PIN_13_CONFIG		PORTC_BASE
#define PIN_14_CONFIG		PORTC_BASE
#define PIN_15_CONFIG		PORTC_BASE
#define PIN_16_CONFIG		PORTC_BASE
#define PIN_17_CONFIG		PORTC_BASE
#define PIN_18_CONFIG		PORTB_BASE
#define PIN_19_CONFIG		PORTB_BASE
#define PIN_20_CONFIG		PORTE_BASE
#define PIN_21_CONFIG		PORTE_BASE

//#define PIN_0_PORTREG		PIN_0_PORT.PDOR
//#define PIN_1_PORTREG		PIN_1_PORT.PDOR
//#define PIN_2_PORTREG		PIN_2_PORT.PDOR
//#define PIN_3_PORTREG		PIN_3_PORT.PDOR			
//#define PIN_4_PORTREG		PIN_4_PORT.PDOR
//#define PIN_5_PORTREG		PIN_5_PORT.PDOR			
//#define PIN_6_PORTREG		PIN_6_PORT.PDOR			
//#define PIN_7_PORTREG		PIN_7_PORT.PDOR			
//#define PIN_8_PORTREG		PIN_8_PORT.PDOR			
//#define PIN_9_PORTREG		PIN_9_PORT.PDOR			
//#define PIN_10_PORTREG		PIN_10_PORT.PDOR		
//#define PIN_11_PORTREG		PIN_11_PORT.PDOR			
//#define PIN_12_PORTREG		PIN_12_PORT.PDOR			
//#define PIN_13_PORTREG		PIN_13_PORT.PDOR			
//#define PIN_14_PORTREG		PIN_14_PORT.PDOR			
//#define PIN_15_PORTREG		PIN_15_PORT.PDOR			
//#define PIN_16_PORTREG		PIN_16_PORT.PDOR			
//#define PIN_17_PORTREG		PIN_17_PORT.PDOR			
//#define PIN_18_PORTREG		PIN_18_PORT.PDOR			
//#define PIN_19_PORTREG		PIN_19_PORT.PDOR			
//#define PIN_20_PORTREG		PIN_20_PORT.PDOR			
//#define PIN_21_PORTREG		PIN_21_PORT.PDOR




//Default Teensy2.0 compatible SPI: SPI1 D4-7
//Default Teensy2.0 compatible UART: LPUART1 A18 A19
//Default Teensy2.0 compatible I2C: I2C1  A3 E0



//---------------------------------End Custom


#endif
