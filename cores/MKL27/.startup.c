/*
  Copyright (c) 2015 Arduino LLC.  All right reserved.

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

//   #include "sam.h"
//   #include "Arduino.h"
//   #include "variant.h"

//   #include <stdio.h>

/**   MKL27
 * \brief SystemInit() configures the needed clocks and according Flash Read Wait States.
 * At reset:
 * - IRC8M clock source is enabled with a divider by divs(1,1,2) which = clks(8MHz,4MHz)
 * - divs(x,y,z)= (x=FCRDIV),(y=OUTDIV1-Core),(z=OUTDIV4-bus/flash) 
 * - clks(x,y)  = (x=Core clk),(y=Bus/Flash clk) in MHz 
 * We need to:
 * 1) Enable IRC48M. Set up 48MHz Core and 24MHz Bus/Flash.
 *  a) Enable HIRC in all modes
 *  b) Set LIRC to 8mhz for peripherals?
 *  c) Setup main clock to 48MHz and enable LIRC in all modes
 * 2) Enable ADC Peripheral
 */


/**
 * \brief SystemInit() configures the needed clocks and according Flash Read Wait States.
 * At reset:
 * - OSC8M clock source is enabled with a divider by 1 (8MHz).
 * - Generic Clock Generator 0 (GCLKMAIN) is using OSC8M as source.
 * We need to:
 * 1) Enable XOSC32K clock (External on-board 32.768Hz oscillator), will be used as DFLL48M reference.
 * 2) Put XOSC32K as source of Generic Clock Generator 1
 * 3) Put Generic Clock Generator 1 as source for Generic Clock Multiplexer 0 (DFLL48M reference)
 * 4) Enable DFLL48M clock
 * 5) Switch Generic Clock Generator 0 to DFLL48M. CPU will run at 48MHz.
 * 6) Modify PRESCaler value of OSCM to have 8MHz
 * 7) Put OSC8M as source for Generic Clock Generator 3
 */
// Constants for Clock generators
//   #define GENERIC_CLOCK_GENERATOR_MAIN      (0u)
//   #define GENERIC_CLOCK_GENERATOR_XOSC32K   (1u)
//   #define GENERIC_CLOCK_GENERATOR_OSCULP32K (2u) /* Initialized at reset for WDT */
//   #define GENERIC_CLOCK_GENERATOR_OSC8M     (3u)
//   // Constants for Clock multiplexers
//   #define GENERIC_CLOCK_MULTIPLEXER_DFLL48M (0u)

//   void SystemInit( void )
//   {

  /* This Is not necessary as long as we are careful to set SIM to Safe values before MCG_lite change */
  // CLOCK_SetSimSafeDivs();

  // Set up SIM clock settings to work for 48/24MHz clocks divs= (x,1,2)
  // SIM->CLKDIV1 = 0x00010000U; //not needed, already default after reset

//moved to reset_handler
    //   // MCG->SC = 0x00u; //set FCRDIV to Div by 1 , which is default after reset
    //   MCG->MC = 0xC0u; //set HIRC to enabled in all modes, set LIRCDIV2 to Div by 1
    //   MCG->C2 = 0x05u; //set input to low feq,low-power crystal, set LIRC to 8MHz
    //   MCG->C1 = 0x03u; //set HIRC to be main clock source, set LIRC to be enabled in all modes

    //   SystemCoreClock= MKL_CORE_CLK; //update core clock

//   /* ----------------------------------------------------------------------------------------------
 //   * 8) Load ADC factory calibration values
 //   */
    //TODO






//   /* Set 1 Flash Wait State for 48MHz, cf tables 20.9 and 35.27 in SAMD21 Datasheet */
//   NVMCTRL->CTRLB.bit.RWS = NVMCTRL_CTRLB_RWS_HALF_Val ;

//   /* Turn on the digital interface clock */
//   PM->APBAMASK.reg |= PM_APBAMASK_GCLK ;

//   /* ----------------------------------------------------------------------------------------------
 //   * 1) Enable XOSC32K clock (External on-board 32.768Hz oscillator)
 //   */
//   SYSCTRL->XOSC32K.reg = SYSCTRL_XOSC32K_STARTUP( 0x6u ) | /* cf table 15.10 of product datasheet in chapter 15.8.6 */
                       //   SYSCTRL_XOSC32K_XTALEN | SYSCTRL_XOSC32K_EN32K ;
//   SYSCTRL->XOSC32K.bit.ENABLE = 1 ; /* separate call, as described in chapter 15.6.3 */

//   while ( (SYSCTRL->PCLKSR.reg & SYSCTRL_PCLKSR_XOSC32KRDY) == 0 )
//   {
  //   /* Wait for oscillator stabilization */
//   }

//   /* Software reset the module to ensure it is re-initialized correctly */
//   /* Note: Due to synchronization, there is a delay from writing CTRL.SWRST until the reset is complete.
 //   * CTRL.SWRST and STATUS.SYNCBUSY will both be cleared when the reset is complete, as described in chapter 13.8.1
 //   */
//   GCLK->CTRL.reg = GCLK_CTRL_SWRST ;

//   while ( (GCLK->CTRL.reg & GCLK_CTRL_SWRST) && (GCLK->STATUS.reg & GCLK_STATUS_SYNCBUSY) )
//   {
  //   /* Wait for reset to complete */
//   }

//   /* ----------------------------------------------------------------------------------------------
 //   * 2) Put XOSC32K as source of Generic Clock Generator 1
 //   */
//   GCLK->GENDIV.reg = GCLK_GENDIV_ID( GENERIC_CLOCK_GENERATOR_XOSC32K ) ; // Generic Clock Generator 1

//   while ( GCLK->STATUS.reg & GCLK_STATUS_SYNCBUSY )
//   {
  //   /* Wait for synchronization */
//   }

//   /* Write Generic Clock Generator 1 configuration */
//   GCLK->GENCTRL.reg = GCLK_GENCTRL_ID( GENERIC_CLOCK_GENERATOR_XOSC32K ) | // Generic Clock Generator 1
                    //   GCLK_GENCTRL_SRC_XOSC32K | // Selected source is External 32KHz Oscillator
// //                      GCLK_GENCTRL_OE | // Output clock to a pin for tests
                    //   GCLK_GENCTRL_GENEN ;

//   while ( GCLK->STATUS.reg & GCLK_STATUS_SYNCBUSY )
//   {
  //   /* Wait for synchronization */
//   }

//   /* ----------------------------------------------------------------------------------------------
 //   * 3) Put Generic Clock Generator 1 as source for Generic Clock Multiplexer 0 (DFLL48M reference)
 //   */
//   GCLK->CLKCTRL.reg = GCLK_CLKCTRL_ID( GENERIC_CLOCK_MULTIPLEXER_DFLL48M ) | // Generic Clock Multiplexer 0
                    //   GCLK_CLKCTRL_GEN_GCLK1 | // Generic Clock Generator 1 is source
                    //   GCLK_CLKCTRL_CLKEN ;

//   while ( GCLK->STATUS.reg & GCLK_STATUS_SYNCBUSY )
//   {
  //   /* Wait for synchronization */
//   }

//   /* ----------------------------------------------------------------------------------------------
 //   * 4) Enable DFLL48M clock
 //   */

//   /* DFLL Configuration in Closed Loop mode, cf product datasheet chapter 15.6.7.1 - Closed-Loop Operation */

//   /* Remove the OnDemand mode, Bug http://avr32.icgroup.norway.atmel.com/bugzilla/show_bug.cgi?id=9905 */
//   SYSCTRL->DFLLCTRL.bit.ONDEMAND = 0 ;

//   while ( (SYSCTRL->PCLKSR.reg & SYSCTRL_PCLKSR_DFLLRDY) == 0 )
//   {
  //   /* Wait for synchronization */
//   }

//   SYSCTRL->DFLLMUL.reg = SYSCTRL_DFLLMUL_CSTEP( 31 ) | // Coarse step is 31, half of the max value
                       //   SYSCTRL_DFLLMUL_FSTEP( 511 ) | // Fine step is 511, half of the max value
                       //   SYSCTRL_DFLLMUL_MUL( (VARIANT_MCK/VARIANT_MAINOSC) ) ; // External 32KHz is the reference

//   while ( (SYSCTRL->PCLKSR.reg & SYSCTRL_PCLKSR_DFLLRDY) == 0 )
//   {
  //   /* Wait for synchronization */
//   }

//   /* Write full configuration to DFLL control register */
//   SYSCTRL->DFLLCTRL.reg |= SYSCTRL_DFLLCTRL_MODE | /* Enable the closed loop mode */
                         //   SYSCTRL_DFLLCTRL_WAITLOCK |
                         //   SYSCTRL_DFLLCTRL_QLDIS ; /* Disable Quick lock */

//   while ( (SYSCTRL->PCLKSR.reg & SYSCTRL_PCLKSR_DFLLRDY) == 0 )
//   {
  //   /* Wait for synchronization */
//   }

//   /* Enable the DFLL */
//   SYSCTRL->DFLLCTRL.reg |= SYSCTRL_DFLLCTRL_ENABLE ;

//   while ( (SYSCTRL->PCLKSR.reg & SYSCTRL_PCLKSR_DFLLLCKC) == 0 ||
        //   (SYSCTRL->PCLKSR.reg & SYSCTRL_PCLKSR_DFLLLCKF) == 0 )
//   {
  //   /* Wait for locks flags */
//   }

//   while ( (SYSCTRL->PCLKSR.reg & SYSCTRL_PCLKSR_DFLLRDY) == 0 )
//   {
  //   /* Wait for synchronization */
//   }

//   /* ----------------------------------------------------------------------------------------------
 //   * 5) Switch Generic Clock Generator 0 to DFLL48M. CPU will run at 48MHz.
 //   */
//   GCLK->GENDIV.reg = GCLK_GENDIV_ID( GENERIC_CLOCK_GENERATOR_MAIN ) ; // Generic Clock Generator 0

//   while ( GCLK->STATUS.reg & GCLK_STATUS_SYNCBUSY )
//   {
  //   /* Wait for synchronization */
//   }

//   /* Write Generic Clock Generator 0 configuration */
//   GCLK->GENCTRL.reg = GCLK_GENCTRL_ID( GENERIC_CLOCK_GENERATOR_MAIN ) | // Generic Clock Generator 0
                    //   GCLK_GENCTRL_SRC_DFLL48M | // Selected source is DFLL 48MHz
// //                      GCLK_GENCTRL_OE | // Output clock to a pin for tests
                    //   GCLK_GENCTRL_IDC | // Set 50/50 duty cycle
                    //   GCLK_GENCTRL_GENEN ;

//   while ( GCLK->STATUS.reg & GCLK_STATUS_SYNCBUSY )
//   {
  //   /* Wait for synchronization */
//   }

//   /* ----------------------------------------------------------------------------------------------
 //   * 6) Modify PRESCaler value of OSC8M to have 8MHz
 //   */
//   SYSCTRL->OSC8M.bit.PRESC = SYSCTRL_OSC8M_PRESC_1_Val ;
//   SYSCTRL->OSC8M.bit.ONDEMAND = 0 ;

//   /* ----------------------------------------------------------------------------------------------
 //   * 7) Put OSC8M as source for Generic Clock Generator 3
 //   */
//   GCLK->GENDIV.reg = GCLK_GENDIV_ID( GENERIC_CLOCK_GENERATOR_OSC8M ) ; // Generic Clock Generator 3

//   /* Write Generic Clock Generator 3 configuration */
//   GCLK->GENCTRL.reg = GCLK_GENCTRL_ID( GENERIC_CLOCK_GENERATOR_OSC8M ) | // Generic Clock Generator 3
                    //   GCLK_GENCTRL_SRC_OSC8M | // Selected source is RC OSC 8MHz (already enabled at reset)
// //                      GCLK_GENCTRL_OE | // Output clock to a pin for tests
                    //   GCLK_GENCTRL_GENEN ;

//   while ( GCLK->STATUS.reg & GCLK_STATUS_SYNCBUSY )
//   {
  //   /* Wait for synchronization */
//   }

//   /*
 //   * Now that all system clocks are configured, we can set CPU and APBx BUS clocks.
 //   * There values are normally the one present after Reset.
 //   */
//   PM->CPUSEL.reg  = PM_CPUSEL_CPUDIV_DIV1 ;
//   PM->APBASEL.reg = PM_APBASEL_APBADIV_DIV1_Val ;
//   PM->APBBSEL.reg = PM_APBBSEL_APBBDIV_DIV1_Val ;
//   PM->APBCSEL.reg = PM_APBCSEL_APBCDIV_DIV1_Val ;

//   SystemCoreClock=MKL_CORE_CLK ;

//   /* ----------------------------------------------------------------------------------------------
 //   * 8) Load ADC factory calibration values
 //   */

//   // ADC Bias Calibration
//   uint32_t bias = (*((uint32_t *) ADC_FUSES_BIASCAL_ADDR) & ADC_FUSES_BIASCAL_Msk) >> ADC_FUSES_BIASCAL_Pos;

//   // ADC Linearity bits 4:0
//   uint32_t linearity = (*((uint32_t *) ADC_FUSES_LINEARITY_0_ADDR) & ADC_FUSES_LINEARITY_0_Msk) >> ADC_FUSES_LINEARITY_0_Pos;

//   // ADC Linearity bits 7:5
//   linearity |= ((*((uint32_t *) ADC_FUSES_LINEARITY_1_ADDR) & ADC_FUSES_LINEARITY_1_Msk) >> ADC_FUSES_LINEARITY_1_Pos) << 5;

//   ADC->CALIB.reg = ADC_CALIB_BIAS_CAL(bias) | ADC_CALIB_LINEARITY_CAL(linearity);

//   /*
 //   * 9) Disable automatic NVM write operations
 //   */
//   NVMCTRL->CTRLB.bit.MANW = 1;
//   }
