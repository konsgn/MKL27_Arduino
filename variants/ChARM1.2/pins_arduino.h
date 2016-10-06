#ifndef __MKL_VARIANT_CHARM_V1.2_H_
#define __MKL_VARIANT_CHARM_V1.2_H_

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

const GPIO_Type GPIO_PTRS[] = GPIO_BASE_PTRS;
const PORT_Type PORT_PTRS[] = PORT_BASE_PTRS;

const enum portpoint{
	PORTA=0;
	PORTB,
	PORTC,
	PORTD,
	PORTE
}

struct pin_and_config_table{
	volatile uint8_t regandpinmask; //Upper 3 bits represent port as ordered in GPIO_PTRS, lower 5 bits show which pin 0-31
	volatile uint32_t *config;
}

const struct pin_and_config_table digital_pin_to_info_PGM[] = {
	{((volatile uint8_t)((PIN_0_PORT)<<5)  + (PIN_0_BIT)),  &PIN_0_CONFIG},
	{((volatile uint8_t)((PIN_1_PORT)<<5)  + (PIN_1_BIT)),  &PIN_1_CONFIG},
	{((volatile uint8_t)((PIN_2_PORT)<<5)  + (PIN_2_BIT)),  &PIN_2_CONFIG},
	{((volatile uint8_t)((PIN_3_PORT)<<5)  + (PIN_3_BIT)),  &PIN_3_CONFIG},
	{((volatile uint8_t)((PIN_4_PORT)<<5)  + (PIN_4_BIT)),  &PIN_4_CONFIG},
	{((volatile uint8_t)((PIN_5_PORT)<<5)  + (PIN_5_BIT)),  &PIN_5_CONFIG},
	{((volatile uint8_t)((PIN_6_PORT)<<5)  + (PIN_6_BIT)),  &PIN_6_CONFIG},
	{((volatile uint8_t)((PIN_7_PORT)<<5)  + (PIN_7_BIT)),  &PIN_7_CONFIG},
	{((volatile uint8_t)((PIN_8_PORT)<<5)  + (PIN_8_BIT)),  &PIN_8_CONFIG},
	{((volatile uint8_t)((PIN_9_PORT)<<5)  + (PIN_9_BIT)),  &PIN_9_CONFIG},
	{((volatile uint8_t)((PIN_10_PORT)<<5) + (PIN_10_BIT)), &PIN_10_CONFIG},
	{((volatile uint8_t)((PIN_11_PORT)<<5) + (PIN_11_BIT)), &PIN_11_CONFIG},
	{((volatile uint8_t)((PIN_12_PORT)<<5) + (PIN_12_BIT)), &PIN_12_CONFIG},
	{((volatile uint8_t)((PIN_13_PORT)<<5) + (PIN_13_BIT)), &PIN_13_CONFIG},
	{((volatile uint8_t)((PIN_14_PORT)<<5) + (PIN_14_BIT)), &PIN_14_CONFIG},
	{((volatile uint8_t)((PIN_15_PORT)<<5) + (PIN_15_BIT)), &PIN_15_CONFIG},
	{((volatile uint8_t)((PIN_16_PORT)<<5) + (PIN_16_BIT)), &PIN_16_CONFIG},
	{((volatile uint8_t)((PIN_17_PORT)<<5) + (PIN_17_BIT)), &PIN_17_CONFIG},
	{((volatile uint8_t)((PIN_18_PORT)<<5) + (PIN_18_BIT)), &PIN_18_CONFIG},
	{((volatile uint8_t)((PIN_19_PORT)<<5) + (PIN_19_BIT)), &PIN_19_CONFIG},
	{((volatile uint8_t)((PIN_20_PORT)<<5) + (PIN_20_BIT)), &PIN_20_CONFIG},
	{((volatile uint8_t)((PIN_21_PORT)<<5) + (PIN_21_BIT)), &PIN_21_CONFIG}
};

#define GPIO_Type_Extract(pin) (GPIO_Type*)GPIO_PTRS[(digital_pin_to_info_PGM[pin].regandpinmask>>5)]
#define PORT_Type_Extract(pin) (PORT_Type*)PORT_PTRS[(digital_pin_to_info_PGM[pin].regandpinmask>>5)]

enum 


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

#define PIN_0_PORT			PORTD
#define PIN_1_PORT			PORTD
#define PIN_2_PORT			PORTD
#define PIN_3_PORT			PORTD
#define PIN_4_PORT			PORTA
#define PIN_5_PORT			PORTA
#define PIN_6_PORT			PORTE
#define PIN_7_PORT			PORTA
#define PIN_8_PORT			PORTA
#define PIN_9_PORT			PORTA
#define PIN_10_PORT			PORTA
#define PIN_11_PORT			PORTC
#define PIN_12_PORT			PORTC
#define PIN_13_PORT			PORTC
#define PIN_14_PORT			PORTC
#define PIN_15_PORT			PORTC
#define PIN_16_PORT			PORTC
#define PIN_17_PORT			PORTC
#define PIN_18_PORT			PORTB
#define PIN_19_PORT			PORTB
#define PIN_20_PORT			PORTE
#define PIN_21_PORT			PORTE

#define PIN_0_CONFIG		PORTD
#define PIN_1_CONFIG		PORTD
#define PIN_2_CONFIG		PORTD
#define PIN_3_CONFIG		PORTD
#define PIN_4_CONFIG		PORTA
#define PIN_5_CONFIG		PORTA
#define PIN_6_CONFIG		PORTE
#define PIN_7_CONFIG		PORTA
#define PIN_8_CONFIG		PORTA
#define PIN_9_CONFIG		PORTA
#define PIN_10_CONFIG		PORTA
#define PIN_11_CONFIG		PORTC
#define PIN_12_CONFIG		PORTC
#define PIN_13_CONFIG		PORTC
#define PIN_14_CONFIG		PORTC
#define PIN_15_CONFIG		PORTC
#define PIN_16_CONFIG		PORTC
#define PIN_17_CONFIG		PORTC
#define PIN_18_CONFIG		PORTB
#define PIN_19_CONFIG		PORTB
#define PIN_20_CONFIG		PORTE
#define PIN_21_CONFIG		PORTE
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

#define digitalPinToPort(pin)		( digital_pin_to_info_PGM[pin].regandpinmask>>5 )
#define digitalPinToBit(pin)		(digital_pin_to_info_PGM[pin].regandpinmask && 0x1F)
#define digitalPinToBitMask(pin)	( 1<<(digital_pin_to_info_PGM[pin].regandpinmask && 0x1F) )
#define portOutputRegister(pin) 	((volatile uint32_t *)GPIO_Type_Extract(pin).PDOR) 
#define portSetRegister(pin)    	((volatile uint32_t *)GPIO_Type_Extract(pin).PSOR)
#define portClearRegister(pin)  	((volatile uint32_t *)GPIO_Type_Extract(pin).PCOR)
#define portToggleRegister(pin) 	((volatile uint32_t *)GPIO_Type_Extract(pin).PTOR)
#define portInputRegister(pin)  	((volatile uint32_t *)GPIO_Type_Extract(pin).PDIR)
#define portModeRegister(pin)   	((volatile uint32_t *)GPIO_Type_Extract(pin).PDDR)
#define portConfigRegister(pin) 	((volatile uint32_t *)(digital_pin_to_info_PGM[(pin)].config))
#define digitalPinToPortReg(pin) 	(portOutputRegister(pin))
#define digitalPinToBit(pin) (1)

//Default Teensy2.0 compatible SPI: SPI1 D4-7
//Default Teensy2.0 compatible UART: LPUART1 A18 A19
//Default Teensy2.0 compatible I2C: I2C1  A3 E0



//---------------------------------End Custom


#endif
