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

#include "Arduino.h"
#include "WInterrupts.h"

typedef void (*interruptCB)(void);

static interruptCB callbacksPioA[32];
static interruptCB callbacksPioB[32];
static interruptCB callbacksPioC[32];
static interruptCB callbacksPioD[32];
static interruptCB callbacksPioE[32];

/* Configure PIO interrupt sources */
static void __initialize() {
	int i;
	for (i=0; i<32; i++) {
		callbacksPioA[i] = NULL;
		callbacksPioB[i] = NULL;
		callbacksPioC[i] = NULL;
		callbacksPioD[i] = NULL;
		callbacksPioE[i] = NULL;
	}

	//   CLOCK_EnableClock(kCLOCK_PortA);
	NVIC_DisableIRQ(PORTA_IRQn);
	NVIC_ClearPendingIRQ(PORTA_IRQn);
	NVIC_SetPriority(PORTA_IRQn, 0);
	NVIC_EnableIRQ(PORTA_IRQn);

	//   CLOCK_EnableClock(kCLOCK_PortB); //already done in wiring.c
	//   CLOCK_EnableClock(kCLOCK_PortC);
	//   CLOCK_EnableClock(kCLOCK_PortD);
	//   CLOCK_EnableClock(kCLOCK_PortE);
	NVIC_DisableIRQ(PORTB_PORTC_PORTD_PORTE_IRQn);
	NVIC_ClearPendingIRQ(PORTB_PORTC_PORTD_PORTE_IRQn);
	NVIC_SetPriority(PORTB_PORTC_PORTD_PORTE_IRQn, 0);
	NVIC_EnableIRQ(PORTB_PORTC_PORTD_PORTE_IRQn);
}


void attachInterrupt(uint32_t pin, void (*callback)(void), INT_Type mode)
{
	static int enabled = 0;
	if (!enabled) {
		__initialize();
		enabled = 1;
	}

	// Retrieve pin information
	PORT_Type *Port= PORT_Type_Extract(pin);
	portpoint *pio = digitalPinToPort(pin);
	uint32_t mask = digitalPinToBitMask(pin);
	uint32_t pos = digitalPinToBit(pin);

	// Set callback function
	if (pio == PORTA)
		callbacksPioA[pos] = callback;
	if (pio == PORTB)
		callbacksPioB[pos] = callback;
	if (pio == PORTC)
		callbacksPioC[pos] = callback;
	if (pio == PORTD)
		callbacksPioD[pos] = callback;
	if (pio == PORTE)
		callbacksPioE[pos] = callback;

	// Configure the interrupt mode
	Port.PCR[pos] &= !PORT_PCR_IRQC_MASK; //clear ircq
	Port.PCR[pos] |= PORT_PCR_IRQC(mode);


//TODO NEXT
	//// Configure the interrupt mode
	//if (mode == CHANGE) {
		//// Disable additional interrupt mode (detects both rising and falling edges)
		//pio->PIO_AIMDR = mask;
	//} else {
		//// Enable additional interrupt mode
		//pio->PIO_AIMER = mask;

		//// Select mode of operation
		//if (mode == LOW) {
			//pio->PIO_LSR = mask;    // "Level" Select Register
			//pio->PIO_FELLSR = mask; // "Falling Edge / Low Level" Select Register
		//}
		//if (mode == HIGH) {
			//pio->PIO_LSR = mask;    // "Level" Select Register
			//pio->PIO_REHLSR = mask; // "Rising Edge / High Level" Select Register
		//}
		//if (mode == FALLING) {
			//pio->PIO_ESR = mask;    // "Edge" Select Register
			//pio->PIO_FELLSR = mask; // "Falling Edge / Low Level" Select Register
		//}
		//if (mode == RISING) {
			//pio->PIO_ESR = mask;    // "Edge" Select Register
			//pio->PIO_REHLSR = mask; // "Rising Edge / High Level" Select Register
		//}
	//}

	//// Enable interrupt
	//pio->PIO_IER = mask;
}

void detachInterrupt(uint32_t pin)
{
	// Retrieve pin information
	PORT_Type *Port= PORT_Type_Extract(pin);
	portpoint *pio = digitalPinToPort(pin);
	uint32_t pos = digitalPinToBit(pin);
	
	
	// Set callback function
	if (pio == PORTA)
		callbacksPioA[pos] = NULL;
	if (pio == PORTB)
		callbacksPioB[pos] = NULL;
	if (pio == PORTC)
		callbacksPioC[pos] = NULL;
	if (pio == PORTD)
		callbacksPioD[pos] = NULL;
	if (pio == PORTE)
		callbacksPioE[pos] = NULL;

	// Disable interrupt
	Port.PCR[pos] &= !PORT_PCR_IRQC_MASK; //clear IRQ mode config
	Port.PCR[pos] != !PORT_PCR_ISF_MASK;  //clear ISF if set
}

#ifdef __cplusplus
extern "C" {
#endif

void PORTA_IRQ_Handler(void) {
  uint32_t isr = PORTA->ISFR;
  uint8_t leading_zeros;
  while((leading_zeros=__CLZ(isr))<32)
  {
    uint8_t pin=32-leading_zeros-1;
    if(callbacksPioA[pin]) callbacksPioA[pin]();
    isr=isr&(~(1<<pin));
  }
}

void PORTBCDE_IRQ_Handler(void) {
  uint32_t isr = PORTB->ISFR;
  uint8_t leading_zeros;
  while((leading_zeros=__CLZ(isr))<32)
  {
    uint8_t pin=32-leading_zeros-1;
    if(callbacksPioB[pin]) callbacksPioB[pin]();
    isr=isr&(~(1<<pin));
  }
  isr = PORTC->ISFR;
  while((leading_zeros=__CLZ(isr))<32)
  {
    uint8_t pin=32-leading_zeros-1;
    if(callbacksPioB[pin]) callbacksPioC[pin]();
    isr=isr&(~(1<<pin));
  }
  isr = PORTD->ISFR;
  while((leading_zeros=__CLZ(isr))<32)
  {
    uint8_t pin=32-leading_zeros-1;
    if(callbacksPioB[pin]) callbacksPioD[pin]();
    isr=isr&(~(1<<pin));
  }
  isr = PORTE->ISFR;
  while((leading_zeros=__CLZ(isr))<32)
  {
    uint8_t pin=32-leading_zeros-1;
    if(callbacksPioB[pin]) callbacksPioE[pin]();
    isr=isr&(~(1<<pin));
  }
}

#ifdef __cplusplus
}
#endif
