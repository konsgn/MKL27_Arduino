/*
  Arduino.h - Main include file for the Arduino SDK
  Copyright (c) 2005-2013 Arduino Team.  All right reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#ifndef Arduino_h
#define Arduino_h

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

typedef bool boolean;
typedef uint8_t byte;
typedef uint16_t word;

// some libraries and sketches depend on this
// AVR stuff, assuming Arduino.h or WProgram.h
// automatically includes it...
#include <avr/pgmspace.h>
#include <avr/interrupt.h>

#include "binary.h"
#include "itoa.h"

#ifdef __cplusplus
extern "C"{
#endif // __cplusplus

// Includes Freescale CMSIS
#include <MKL27Z644>
#include <fsl_device_registers.h>
#include <fsl_device_headers.h>
#include "wiring_constants.h"

#define clockCyclesPerMicrosecond() ( SystemCoreClock / 1000000L )
#define clockCyclesToMicroseconds(a) ( ((a) * 1000L) / (SystemCoreClock / 1000L) )
#define microsecondsToClockCycles(a) ( (a) * (SystemCoreClock / 1000000L) )

void yield(void);

/* sketch */
extern void setup( void ) ;
extern void loop( void ) ;

/* Define attribute */
#if defined   ( __CC_ARM   ) /* Keil uVision 4 */
    #define WEAK (__attribute__ ((weak)))
#elif defined ( __ICCARM__ ) /* IAR Ewarm 5.41+ */
    #define WEAK __weak
#elif defined (  __GNUC__  ) /* GCC CS */
    #define WEAK __attribute__ ((weak))
#endif

///* Definitions and types for pins */
//typedef enum _EAnalogChannel
//{
  //NO_ADC=-1,
  //ADC0=0,
  //ADC1,
  //ADC2,
  //ADC3,
  //ADC4,
  //ADC5,
  //ADC6,
  //ADC7,
  //ADC8,
  //ADC9,
  //ADC10,
  //ADC11,
  //ADC12,
  //ADC13,
  //ADC14,
  //ADC15,
  //DA0
//} EAnalogChannel ;

//#define ADC_CHANNEL_NUMBER_NONE 0xffffffff

//// Definitions for PWM channels
//typedef enum _EPWMChannel
//{
  //NOT_ON_PWM=-1,
  //PWM_CH0=0,
  //PWM_CH1,
  //PWM_CH2,
  //PWM_CH3,
  //PWM_CH4,
  //PWM_CH5
//} EPWMChannel ;

//// Definitions for TC channels
//typedef enum _ETCChannel
//{
  //NOT_ON_TIMER=-1,
  //TPM0_CHA0=0,
  //TPM0_CHA1,
  //TPM0_CHA2,
  //TPM0_CHA3,
  //TPM0_CHA4,
  //TPM0_CHA5,
  //TPM0_CHA6,
  //TPM0_CHA7,
  //TPM1_CHA0,
  //TPM1_CHA1,
  //TPM2_CHA0,
  //TPM2_CHA1,
//} ETCChannel ;

///**
 //* Pin Attributes to be OR-ed
 //*/
//#define PIN_ATTR_COMBO         (1UL<<0)
//#define PIN_ATTR_ANALOG        (1UL<<1)
//#define PIN_ATTR_DIGITAL       (1UL<<2)
//#define PIN_ATTR_PWM           (1UL<<3)
//#define PIN_ATTR_TIMER         (1UL<<4)

//#define PIN_STATUS_DIGITAL_INPUT_PULLUP  (0x01)
//#define PIN_STATUS_DIGITAL_INPUT         (0x02)
//#define PIN_STATUS_DIGITAL_OUTPUT        (0x03)
//#define PIN_STATUS_ANALOG                (0x04)
//#define PIN_STATUS_PWM                   (0x05)
//#define PIN_STATUS_TIMER                 (0x06)
//#define PIN_STATUS_SERIAL                (0x07)
//#define PIN_STATUS_DW_LOW                (0x10)
//#define PIN_STATUS_DW_HIGH               (0x11)

///* Types used for the tables below */
//typedef struct _PinDescription
//{
  //Pio* pPort ;
  //uint32_t ulPin ;
  //uint32_t ulPeripheralId ;
  //EPioType ulPinType ;
  //uint32_t ulPinConfiguration ;
  //uint32_t ulPinAttribute ;
  //EAnalogChannel ulAnalogChannel ; /* Analog pin in the Arduino context (label on the board) */
  //EAnalogChannel ulADCChannelNumber ; /* ADC Channel number in the SAM device */
  //EPWMChannel ulPWMChannel ;
  //ETCChannel ulTCChannel ;
//} PinDescription ;

//extern uint8_t g_pinStatus[];

/* Pins table to be instanciated into variant.cpp */
extern const PinDescription g_APinDescription[] ;

// undefine stdlib's abs if encountered
#ifdef abs
#undef abs
#endif // abs

#define min(a,b) ((a)<(b)?(a):(b))
#define max(a,b) ((a)>(b)?(a):(b))
#define abs(x) ((x)>0?(x):-(x))
#define constrain(amt,low,high) ((amt)<(low)?(low):((amt)>(high)?(high):(amt)))
#define round(x)     ((x)>=0?(long)((x)+0.5):(long)((x)-0.5))
#define radians(deg) ((deg)*DEG_TO_RAD)
#define degrees(rad) ((rad)*RAD_TO_DEG)
#define sq(x) ((x)*(x))

#define interrupts() __enable_irq()
#define noInterrupts() __disable_irq()

#define lowByte(w) ((uint8_t) ((w) & 0xff))
#define highByte(w) ((uint8_t) ((w) >> 8))

#define bitRead(value, bit) (((value) >> (bit)) & 0x01)
#define bitSet(value, bit) ((value) |= (1UL << (bit)))
#define bitClear(value, bit) ((value) &= ~(1UL << (bit)))
#define bitWrite(value, bit, bitvalue) (bitvalue ? bitSet(value, bit) : bitClear(value, bit))

#define bit(b) (1UL << (b))

#ifdef __cplusplus
} // extern "C"

#include "WCharacter.h"
#include "WString.h"
#include "Tone.h"
#include "WMath.h"
#include "HardwareSerial.h"
#include "wiring_pulse.h"

#endif // __cplusplus

// Include board variant
#include "variant.h"

#include "wiring.h"
#include "wiring_digital.h"
#include "wiring_analog.h"
#include "wiring_shift.h"
#include "WInterrupts.h"

#include "watchdog.h"

// USB Device
#define USB_VID            0x1209 // arduino LLC vid
#define USB_PID_CHARM      0xC7A0 // ChArm PID C 7=H =Cheap A=arm 0=v1

#include "USB/USBDesc.h"
#include "USB/USBCore.h"
#include "USB/USBAPI.h"

#endif // Arduino_h
