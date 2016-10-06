/*
	Copyright (c) 2016 Konstantin.  All right reserved.

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

#include "Arduino.h"

#ifdef __cplusplus
 extern "C" {
#endif 

void pinMode( uint8_t pin, uint8_t ulMode )
{
	
	// Handle the case the pin isn't usable as PIO
	if ( pin>= PINS_COUNT ) return ;
	
	// Retrieve port information now that it's needed
	PORT_Type *Port= PORT_Type_Extract(pin);
	uint32_t pos = digitalPinToBit(pin);
	
	//PinMode is packed in the following manner,bits= 00abc0xyz 
	// abc = MUX Settings , MUX ALT1 = Set as pin
	// x= Port Data Direction 1= output 0=input
	// y= Pull Enabled 
	// z= Pull Select  0=pulldown 1=pullup 
	
	uint32_t Pctrlbitsmask = 0x00000703; //mask the mux & pullup related bits
	uint32_t Pctrlbits = (((uint32_t)ulMode<<4)&((uint32_t)ulMode)&Pctrlbitsmask);
	Port->PCR[pos] |=  Pctrlbits;
	Port->PCR[pos] &=  (Pctrlbits |~ Pctrlbitsmask);
	
	if(((ulMode>>4)&0x07)!=1){
		return; //if selecting special functionality, drop out to save time
	}
	
	//Get GPIO info now that it's needed
	GPIO_Type *Gpio= GPIO_Type_Extract(pin);
	uint32_t mask = digitalPinToBitMask(pin);
	
	//TODO improve this, there must be a better way.
	if (ulMode&0b00000100){//if set output
		Gpio->PDDR |= mask; //set pin bit direction to output
	}
	else{
		Gpio->PDDR &= ~mask; // set direction to input by setting to 0
	}
	
	//Pullups already set with the pin mux selection, so we can return now.
}

void digitalWrite( uint8_t pin, uint8_t ulVal )
{
	// Handle the case the pin isn't usable as PIO
	if ( pin>= PINS_COUNT ) return;

	//Get pin info
	PORT_Type *Port= PORT_Type_Extract(pin);
	uint32_t pos = digitalPinToBit(pin);
	GPIO_Type *Gpio= GPIO_Type_Extract(pin);
	uint32_t mask = digitalPinToBitMask(pin);
	
	switch ( ulVal )
	{
		case LOW:
			Gpio->PCOR = mask;
			Port->PCR[pos] &= ~0x00000003;
		break ;

		default:
			Gpio->PSOR = mask;
			Port->PCR[pos] |= 0x00000003;
		break ;
	}

	return ;
}

int digitalRead( uint8_t pin )
{
	// Handle the case the pin isn't usable as PIO
	if ( pin>= PINS_COUNT ) return;

	GPIO_Type *Gpio= GPIO_Type_Extract(pin);
	uint32_t mask = digitalPinToBitMask(pin);

	if ( Gpio->PDIR & mask )
	{
		return HIGH ;
	}

	return LOW ;
}

#ifdef __cplusplus
}
#endif

