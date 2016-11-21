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

#include <MKL27Z644.h>
#include <stdio.h>
#include "Arduino.h"
#include "variant.h"
#include "ser_print.h"

//create a vector table in ram
#define __ram_vector_table__

/* RTOS Hooks */
//   extern void svcHook(void);
//   extern void pendSVHook(void);
extern int sysTickHook(void);

/* Default empty handler */
void Dummy_Handler(void)
{
//   #if defined DEBUG
	//   __BKPT(3);
//   #endif
	//   for (;;) { }
}

// reset button tied weakly here .... do reset
void Call_Reset (void) 
{
	banzai();
}


/* Cortex-M0+ core handlers */
void HardFault_Handler(void) __attribute__ ((weak, alias("Dummy_Handler")));
void Reset_Handler    (void);
//   void NMI_Handler      (void) __attribute__ ((weak, alias("Dummy_Handler")));
void NMI_Handler      (void) __attribute__ ((weak, alias("Call_Reset"))); //in case user overrides..
void SVC_Handler      (void) __attribute__ ((weak, alias("Dummy_Handler")));
void PendSV_Handler   (void) __attribute__ ((weak, alias("Dummy_Handler")));
void SysTick_Handler  (void);

/* Peripherals handlers */
void DMA0_Handler			(void) __attribute__ ((weak, alias("Dummy_Handler")));
void DMA1_Handler			(void) __attribute__ ((weak, alias("Dummy_Handler")));
void DMA2_Handler			(void) __attribute__ ((weak, alias("Dummy_Handler")));
void DMA3_Handler			(void) __attribute__ ((weak, alias("Dummy_Handler")));
void FTFA_Handler			(void) __attribute__ ((weak, alias("Dummy_Handler")));
void PMC_Handler			(void) __attribute__ ((weak, alias("Dummy_Handler")));
void LLWU_Handler			(void) __attribute__ ((weak, alias("Dummy_Handler")));
void I2C0_Handler			(void) __attribute__ ((weak, alias("Dummy_Handler")));
void I2C1_Handler			(void) __attribute__ ((weak, alias("Dummy_Handler")));
void SPI0_Handler			(void) __attribute__ ((weak, alias("Dummy_Handler")));
void SPI1_Handler			(void) __attribute__ ((weak, alias("Dummy_Handler")));
void LPUART0_Handler		(void) __attribute__ ((weak, alias("Dummy_Handler")));
void LPUART1_Handler		(void) __attribute__ ((weak, alias("Dummy_Handler")));
void UART2_FLEXIO_Handler	(void) __attribute__ ((weak, alias("Dummy_Handler")));
void ADC0_Handler			(void) __attribute__ ((weak, alias("Dummy_Handler")));
void CMP0_Handler			(void) __attribute__ ((weak, alias("Dummy_Handler")));
void TPM0_Handler			(void) __attribute__ ((weak, alias("Dummy_Handler")));
void TPM1_Handler			(void) __attribute__ ((weak, alias("Dummy_Handler")));
void TPM2_Handler			(void) __attribute__ ((weak, alias("Dummy_Handler")));
void RTC_Handler			(void) __attribute__ ((weak, alias("Dummy_Handler")));
void RTC_Seconds_Handler	(void) __attribute__ ((weak, alias("Dummy_Handler"))); 
void PIT_Handler			(void) __attribute__ ((weak, alias("Dummy_Handler")));
void USB0_Handler			(void) __attribute__ ((weak));
void LPTMR0_Handler			(void) __attribute__ ((weak, alias("Dummy_Handler")));
void PORTA_IRQ_Handler		(void) __attribute__ ((weak, alias("Dummy_Handler")));
void PORTBCDE_IRQ_Handler	(void) __attribute__ ((weak, alias("Dummy_Handler")));

/* Initialize segments */
extern uint32_t __etext;
extern uint32_t __data_start__;
extern uint32_t __data_end__;
extern uint32_t __bss_start__;
extern uint32_t __bss_end__;
extern uint32_t __StackTop;

__attribute__ ((section(".m_interrupts_ram"), used))
void (* _VectorsRam[NUMBER_OF_INT_VECTORS])(void);

/* Exception Table */
__attribute__ ((section(".isr_vector"), used)) void (* const _VectorsFlash[NUMBER_OF_INT_VECTORS])(void) =
{
	/* Configure Initial Stack Pointer, using linker-generated symbols */
	(void*) (&__StackTop),
	(void*) Reset_Handler,
	(void*) NMI_Handler,
	(void*) HardFault_Handler,
	(void*) (0UL), /* Reserved */
	(void*) (0UL), /* Reserved */
	(void*) (0UL), /* Reserved */
	(void*) (0UL), /* Reserved */
	(void*) (0UL), /* Reserved */
	(void*) (0UL), /* Reserved */
	(void*) (0UL), /* Reserved */
	(void*) SVC_Handler,
	(void*) (0UL), /* Reserved */
	(void*) (0UL), /* Reserved */
	(void*) PendSV_Handler,
	(void*) SysTick_Handler,

	/* Configurable interrupts */
	(void*) DMA0_Handler,
	(void*) DMA1_Handler,
	(void*) DMA2_Handler,
	(void*) DMA3_Handler,
	(void*) (0UL), /* Reserved */
	(void*) FTFA_Handler,
	(void*) PMC_Handler,
	(void*) LLWU_Handler,
	(void*) I2C0_Handler,
	(void*) I2C1_Handler,
	(void*) SPI0_Handler,
	(void*) SPI1_Handler,
	(void*) LPUART0_Handler,
	(void*) LPUART1_Handler,
	(void*) UART2_FLEXIO_Handler,
	(void*) ADC0_Handler,
	(void*) CMP0_Handler,
	(void*) TPM0_Handler,
	(void*) TPM1_Handler,
	(void*) TPM2_Handler,
	(void*) RTC_Handler,
	(void*) RTC_Seconds_Handler, 
	(void*) PIT_Handler,
	(void*) (0UL), /* Reserved */
	(void*) USB0_Handler,
	(void*) (0UL), /* Reserved */
	(void*) (0UL), /* Reserved */
	(void*) (0UL), /* Reserved */
	(void*) LPTMR0_Handler,
	(void*) (0UL), /* Reserved */
	(void*) PORTA_IRQ_Handler,
	(void*) PORTBCDE_IRQ_Handler
};

extern int main(void);
extern void __libc_init_array(void);


const struct {
	uint8_t  bkey7;
	uint8_t  bkey6;
	uint8_t  bkey5;
	uint8_t  bkey4;
	uint8_t  bkey3;
	uint8_t  bkey2;
	uint8_t  bkey1;
	uint8_t  bkey0;
	uint32_t FPROT; //all unprotect each bit if (F_size/32)
	uint8_t  FSEC; //disable flash secure
	uint8_t  FOPT;
	uint8_t  RESV1;
	uint8_t  RESV2;
} Flash_Config __attribute__ ((section(".FlashConfig"), used))={
	#ifdef FSEC_PROTECT
	#define time __TIME__
	//   time[0],
	//   time[1],
	//   time[2],
	//   time[3],
	//   time[4],
	//   time[5],
	//   time[6],
	//   time[7],
	0xFF, //backdoor key isn't implemented yet. should be tied to compiler 
	0xFF, //unique value... why not?
	0xFF,
	0xFF,
	0xFF,
	0xFF,
	0xFF,
	0xFF,
	
	0xFFFFFFFF,
	0xFF, // lock all  from read //FSEC
	0x3D, // set fast but hi power init, boot to Flash unless button//FOPT
	0xFF,
	0xFF
	#else // if the flash isn't being protected.
	0xFF,
	0xFF,
	0xFF,
	0xFF,
	0xFF,
	0xFF,
	0xFF,
	0xFF,
	0xFFFFFFFF,
	0xBE, // unlock all including flash //FSEC
	0x3D, // set fast but hi power init, boot to flash unless button//FOPT
	0xFF,
	0xFF
	#endif
};

/* This is called on processor reset to initialize the device and call main() */
__attribute__ ((section(".startup") ))//,optimize("-Os")))
void Reset_Handler(void)
{
	//Moving a bunch of thing to here... follow similar startup to teensy.
	
	uint32_t *pSrc, *pDest;
	unsigned int i;
	
	// Disable all interrupts
	__disable_irq();
	SIM->COPC = 0u;  // disable the watchdog
	PMC->REGSC = 0x59u; // ack iso for vllp wakeup and bandgap disable.
	//thanks paul!
#if 0
	// testing only, enable ser_print
	SIM->CLKDIV1 = SIM_CLKDIV1_OUTDIV1(0) | SIM_CLKDIV1_OUTDIV4(1);
	MCG->SC = 0x00u; //set FCRDIV to Div by 1 , which is default after reset
    MCG->MC = 0xC0u; //set HIRC to enabled in all modes, set LIRCDIV2 to Div by 1
    MCG->C2 = 0x05u; //set input to low feq,low-power crystal, set LIRC to 8MHz
    MCG->C1 = 0x03u; //set HIRC to be main clock source, set LIRC to be enabled in all modes
	//   SIM->SOPT2 = SIM_SOPT2_UART0SRC(1) | SIM_SOPT2_TPMSRC(1);
	SIM->SOPT2 = 0xFF;
	SIM->SCGC4 |= 0xFFFFFFFF;
	SIM->SCGC5 |= 0xFFFFFFFF;
	UART2->BDH = 0;
	UART2->BDL = 13; // 115108 from 24MHz with SBR=13 & SBR+BRFD=13.03125
	UART2->C4 |= 0x01; // 115108 from 24MHz with BRFA=00001 aka .03125
	UART2->C2 |= UART_C2_TE_MASK;
	PORTE->PCR[16] =  PORT_PCR_MUX(3);
#endif
	
	//SIM settings to 
	SIM->SOPT2=0x15440060u; // All clk sources set to 48MHZ USB set to internal 
							// 48 clockout is 1khz rtc_clockout is 1Hz  
	//SIM SOPT4,5,7 dont need changing from default..not using any fancy options

	SIM->SCGC4=0xFFFFFFFFu; //enable all 
	SIM->SCGC5=0xFFFFFFFFu; //enable all 
	SIM->SCGC6=0xFFFFFFFFu; //enable all 
	SIM->SCGC7=0xFFFFFFFFu; //enable all... DMA 
	//   ser_print("clocks enabled for all\n");
	//   while (1){ser_write(0xaa);delay(10);}
	

	// initialize MCG 
    // MCG->SC = 0x00u; //set FCRDIV to Div by 1 , which is default after reset
    MCG->MC = 0xC0u; //set HIRC to enabled in all modes, set LIRCDIV2 to Div by 1
    MCG->C2 = 0x05u; //set input to low feq,low-power crystal, set LIRC to 8MHz
    MCG->C1 = 0x03u; //set HIRC to be main clock source, set LIRC to be enabled in all modes

    SystemCoreClock= MKL_CORE_CLK; //update core clock
	//   ser_print("Clocks all to 48MHz\n");
	
	//enough room here to init analog?
	//Calibrate Process start:
	//
	ADC0->CFG1 = 0x7C;//3MHZ ADCK long sample 16bit
	ADC0->SC2  = 0x01;//sets averaging to 32 and starts calibration
	ADC0->SC3  = 0xC7;//sets averaging to 32 and starts calibration
	
	/* Initialize the initialized data section while waiting for cal*/
	pSrc = &__etext;
	pDest = &__data_start__;

	if ((&__data_start__ != &__data_end__) && (pSrc != pDest)) {
		for (; pDest < &__data_end__; pDest++, pSrc++)
			*pDest = *pSrc;
	}

	/* Clear the zero section */
	if ((&__data_start__ != &__data_end__) && (pSrc != pDest)) {
		for (pDest = &__bss_start__; pDest < &__bss_end__; pDest++)
			*pDest = 0;
	}
	
	//   ser_print("Data init & cleared\n");
	
	// default all interrupts to medium priority level
	for (i=0; i < NUMBER_OF_INT_VECTORS; i++) _VectorsRam[i] = _VectorsFlash[i];
	for (i=0; i < NUMBER_OF_INT_VECTORS-16; i++) NVIC_SetPriority(i, 128);
	SCB->VTOR = (uint32_t)&_VectorsRam[0];	// use vector table in RAM
	
	//   ser_print("Vectors Mapped\n");	
	
	// initialize the SysTick counter... only starts with enable irq
	while(SysTick_Config(SystemCoreClock/1000));

	// trap for completed calibration:
	while(ADC0->SC3&0x80);//{ser_print(".");} //check CAL bit for running status
	if(ADC0->SC3&0x40){ //check for fail.
		//  ser_print("Failed\n\n");
	}
	else {
		//  ser_print("Success\n\n");
		i=0x0000;
		i+=ADC0->CLP0;
		i+=ADC0->CLP1;
		i+=ADC0->CLP2;
		i+=ADC0->CLP3;
		i+=ADC0->CLP4;
		i+=ADC0->CLPS;
		i = i>>1;
		i |= 0x8000;
		ADC0->PG = i;
		i=0x0000;
		i+=ADC0->CLM0;
		i+=ADC0->CLM1;
		i+=ADC0->CLM2;
		i+=ADC0->CLM3;
		i+=ADC0->CLM4;
		i+=ADC0->CLMS;
		i = i>>1;
		i |= 0x8000;
		ADC0->MG = i;
		// TODO use asm optimize
	}	
		
	__enable_irq();	
	
	/* Initialize the C library */
	__libc_init_array();

	//   SystemInit(); //currently empty

	main();

	while (1);
}

/* Default Arduino systick handler */
extern void SysTick_DefaultHandler(void);

void SysTick_Handler(void)
{
	if (sysTickHook())
		return;
	SysTick_DefaultHandler();
}

static void (*usb_isr)(void) = NULL;

void USB0_Handler(void)
{
	if (usb_isr)
		usb_isr();
}

extern void USB_SetHandler(void (*new_usb_isr)(void))
{
	usb_isr = new_usb_isr;
}
