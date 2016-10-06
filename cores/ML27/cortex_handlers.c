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

/* RTOS Hooks */
extern void svcHook(void);
extern void pendSVHook(void);
extern int sysTickHook(void);

/* Default empty handler */
void Dummy_Handler(void)
{
#if defined DEBUG
  __BKPT(3);
#endif
  for (;;) { }
}

/* Cortex-M0+ core handlers */
void HardFault_Handler(void) __attribute__ ((weak, alias("Dummy_Handler")));
void Reset_Handler    (void);
void NMI_Handler      (void) __attribute__ ((weak, alias("Dummy_Handler")));
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
void PORTA_IRQ_Handler			(void) __attribute__ ((weak, alias("Dummy_Handler")));
void PORTBCDE_IRQ_Handler		(void) __attribute__ ((weak, alias("Dummy_Handler")));

/* Initialize segments */
extern uint32_t __etext;
extern uint32_t __data_start__;
extern uint32_t __data_end__;
extern uint32_t __bss_start__;
extern uint32_t __bss_end__;
extern uint32_t __StackTop;

/* Exception Table */
__attribute__ ((section(".isr_vector"))) const DeviceVectors exception_table =
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

/* This is called on processor reset to initialize the device and call main() */
void Reset_Handler(void)
{
  uint32_t *pSrc, *pDest;

  /* Initialize the initialized data section */
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

  /* Initialize the C library */
  __libc_init_array();

  SystemInit();

  main();

  while (1)
    ;
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

void USB_SetHandler(void (*new_usb_isr)(void))
{
  usb_isr = new_usb_isr;
}
