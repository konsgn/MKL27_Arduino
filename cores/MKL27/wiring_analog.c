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
//   #include "wiring_private.h"
#include "ser_print.h"
#include "wiring_analog.h"

#ifdef __cplusplus
extern "C" {
#endif

static int _readResolution = 10;
static int _ADCResolution  = 10;

//   // Wait for synchronization of registers between the clock domains
//   static __inline__ void syncADC() __attribute__((always_inline, unused));
//   static void syncADC() {
  //   while (ADC->STATUS.bit.SYNCBUSY == 1)
    //   ;
//   }

//   // Wait for synchronization of registers between the clock domains
//   static __inline__ void syncTC_8(Tc* TCx) __attribute__((always_inline, unused));
//   static void syncTC_8(Tc* TCx) {
  //   while (TCx->COUNT8.STATUS.bit.SYNCBUSY);
//   }

//   // Wait for synchronization of registers between the clock domains
//   static __inline__ void syncTCC(Tcc* TCCx) __attribute__((always_inline, unused));
//   static void syncTCC(Tcc* TCCx) {
  //   while (TCCx->SYNCBUSY.reg & TCC_SYNCBUSY_MASK);
//   }

void analogReadResolution(int res)
{
	_readResolution = res;
	if (res >= 16) {
		ADC0->CFG1 = (ADC0->CFG1&(~ADC_CFG1_MODE_MASK))|ADC_CFG1_MODE(_16BITRES);
		_ADCResolution = 16;
	} else if (res >= 12) {
		ADC0->CFG1 = (ADC0->CFG1&(~ADC_CFG1_MODE_MASK))|ADC_CFG1_MODE(_12BITRES);
		_ADCResolution = 12;
	} else if (res >= 10) {
		ADC0->CFG1 = (ADC0->CFG1&(~ADC_CFG1_MODE_MASK))|ADC_CFG1_MODE(_10BITRES);
		_ADCResolution = 10;
	} else {
		ADC0->CFG1 = (ADC0->CFG1&(~ADC_CFG1_MODE_MASK))|ADC_CFG1_MODE(_8BITRES);
		_ADCResolution = 8;
	}
	//   syncADC();
}

static inline uint32_t mapResolution(uint32_t value, uint32_t from, uint32_t to)
{
	if (from == to) {
		return value;
	}
	if (from > to) {
		return value >> (from-to);
	}
	return value << (to-from);
}

//   /*
 //   * Internal Reference is at 1.0v
 //   * External Reference should be between 1v and VDDANA-0.6v=2.7v
 //   *
 //   * Warning : On Arduino Zero board the input/output voltage for SAMD21G18 is 3.3 volts maximum
 //   */
//   void analogReference(eAnalogReference mode)
//   {
  //   syncADC();
  //   switch (mode)
  //   {
    //   case AR_INTERNAL:
    //   case AR_INTERNAL2V23:
      //   ADC->INPUTCTRL.bit.GAIN = ADC_INPUTCTRL_GAIN_1X_Val;      // Gain Factor Selection
      //   ADC->REFCTRL.bit.REFSEL = ADC_REFCTRL_REFSEL_INTVCC0_Val; // 1/1.48 VDDANA = 1/1.48* 3V3 = 2.2297
      //   break;

    //   case AR_EXTERNAL:
      //   ADC->INPUTCTRL.bit.GAIN = ADC_INPUTCTRL_GAIN_1X_Val;      // Gain Factor Selection
      //   ADC->REFCTRL.bit.REFSEL = ADC_REFCTRL_REFSEL_AREFA_Val;
      //   break;

    //   case AR_INTERNAL1V0:
      //   ADC->INPUTCTRL.bit.GAIN = ADC_INPUTCTRL_GAIN_1X_Val;      // Gain Factor Selection
      //   ADC->REFCTRL.bit.REFSEL = ADC_REFCTRL_REFSEL_INT1V_Val;   // 1.0V voltage reference
      //   break;

    //   case AR_INTERNAL1V65:
      //   ADC->INPUTCTRL.bit.GAIN = ADC_INPUTCTRL_GAIN_1X_Val;      // Gain Factor Selection
      //   ADC->REFCTRL.bit.REFSEL = ADC_REFCTRL_REFSEL_INTVCC1_Val; // 1/2 VDDANA = 0.5* 3V3 = 1.65V
      //   break;

    //   case AR_DEFAULT:
    //   default:
      //   ADC->INPUTCTRL.bit.GAIN = ADC_INPUTCTRL_GAIN_DIV2_Val;
      //   ADC->REFCTRL.bit.REFSEL = ADC_REFCTRL_REFSEL_INTVCC1_Val; // 1/2 VDDANA = 0.5* 3V3 = 1.65V
      //   break;
  //   }
//   }

//function to help with PWM:
void PWM_TPM(uint8_t tim,uint8_t ch,uint16_t value) {
	TPM_BASE_PTR[tim]->SC=0x0000; //disable TPM 
	TPM_BASE_PTR[tim]->CNT = 0; 
	TPM_BASE_PTR[tim]->MOD = ((0xFFFF)^((uint16_t)0x8000>>(A_Write_Resolution-1))); //allow for max value to be 100% pwm
	TPM_BASE_PTR[tim]->CONTROLS[ch].CnSC = 0xA8; // edge high PWM
	TPM_BASE_PTR[tim]->CONTROLS[ch].CnV  = (value << (16-A_Write_Resolution));
	TPM_BASE_PTR[tim]->SC |= 0x08; //enable TPM
};
				


uint32_t analogRead(uint32_t pin)
{
	uint32_t valueRead = 0;
	uint32_t portprev;
	// Handle the case the pin isn't usable as PIO
	if (!isPinAnalog(pin)) return 0;

	// Retrieve port information now that it's needed
	PORT_Type *Port= PORT_Type_Extract(pin);
	uint32_t pos = digitalPinToBit(pin);
	uint8_t analoginfo = AnalogChannel[digitalPinToAnalogNum(pin)];
	   //   ser_print_hex32(Port->PCR[pos]);
	
	portprev = Port->PCR[pos];
	Port->PCR[pos] = 0; // Selection for ADC input mux
	//   ser_print_hex32(ADC0->CFG1);

	if(analoginfo&0x20){ADC0->CFG2 |= ADC_CFG2_MUXSEL_MASK;}//if mux b
	else{ADC0->CFG2 &= (~ADC_CFG2_MUXSEL_MASK);} //if mux a
	//start reading in this manual mode no ping-ponging is done.
	ADC0->SC1[0] = analoginfo&0x1F; //mask out channel only
	
	// Store the value
	while (!(ADC0->SC1[0] & ADC_SC1_COCO_MASK)){ser_print(".");} // Waiting for conversion to complete
	valueRead = ADC0->R[0];

	//   // Disable ADC
	//   ADC0->SC1[0] = 0x1F;           
	
	Port->PCR[pos] = portprev;
		// analog, return mux to digital.

	return mapResolution(valueRead, _ADCResolution, _readResolution);
}

uint16_t analogReadInternal(uint8_t channel)
{
	uint32_t valueRead = 0;
	   //   ser_print_hex32(Port->PCR[pos]);

	ADC0->SC1[0] = channel & 0x1F; //mask out channel only just in case
	
	// Store the value
	while (!(ADC0->SC1[0] & ADC_SC1_COCO_MASK)){ser_print(".");} // Waiting for conversion to complete
	valueRead = ADC0->R[0];

	//   // Disable ADC
	ADC0->SC1[0] = 0x1F;           

	return valueRead;
}

// Right now, PWM output only works on the pins with
// hardware support.  
// For the rest of the pins, we default to digital output.
void analogWrite(uint32_t pin, uint32_t value)
{
	// Retrieve port and pin information 
	uint8_t prt= digitalPinToPort(pin);
	PORT_Type *Port = PORT_PTRS[prt];
	uint8_t bit= digitalPinToBit(pin);
	
	switch(prt){
		case PrtA:
			switch(bit){
				case 0:
					//alt3 tpm0 ch5
					//PWM_TPM(tim,ch)
					//TPM_BASE_PTRS[tim]->CONTROLS[ch].CnV  = (value << (16-A_Write_Resolution))
					PWM_TPM(0,5,value);
					Port->PCR[bit]&=0xFFFFF8FF;
					Port->PCR[bit]|=0x00000300;
					break;
				case 1:
					//alt3 tpm2 ch0
					PWM_TPM(2,0,value);
					Port->PCR[bit]&=0xFFFFF8FF;
					Port->PCR[bit]|=0x00000300;
					break;
				case 2:
					//alt3 tpm2 ch1
					PWM_TPM(2,1,value);
					Port->PCR[bit]&=0xFFFFF8FF;
					Port->PCR[bit]|=0x00000300;
					break;
				case 3:
					//alt3 tpm0 ch0
					PWM_TPM(0,0,value);
					Port->PCR[bit]&=0xFFFFF8FF;
					Port->PCR[bit]|=0x00000300;
					break;
				case 4:
					//alt3 tpm0 ch1
					PWM_TPM(0,1,value);
					Port->PCR[bit]&=0xFFFFF8FF;
					Port->PCR[bit]|=0x00000300;
					break;
				default:
					break;
			}
			break;
		case PrtB:
			switch(bit){
				case 0:
					//alt3 tpm1 ch0
					PWM_TPM(1,0,value);
					Port->PCR[bit]&=0xFFFFF8FF;
					Port->PCR[bit]|=0x00000300;
					break;
				case 1:
					//alt3 tpm1 ch1
					PWM_TPM(1,1,value);
					Port->PCR[bit]&=0xFFFFF8FF;
					Port->PCR[bit]|=0x00000300;
					break;
				default:
					break;
			}
			break;
		case PrtC:
			switch(bit){
				case 1:
					//alt4 tpm0 ch0
					PWM_TPM(0,0,value);
					Port->PCR[bit]&=0xFFFFF8FF;
					Port->PCR[bit]|=0x00000400;
					break;
				case 2:
					//alt4 tpm0 ch1
					PWM_TPM(0,1,value);
					Port->PCR[bit]&=0xFFFFF8FF;
					Port->PCR[bit]|=0x00000400;
					break;
				case 3:
					//alt4 tpm0 ch2
					PWM_TPM(0,2,value);
					Port->PCR[bit]&=0xFFFFF8FF;
					Port->PCR[bit]|=0x00000400;
					break;
				case 4:
					//alt4 tpm0 ch3
					PWM_TPM(0,3,value);
					Port->PCR[bit]&=0xFFFFF8FF;
					Port->PCR[bit]|=0x00000400;
					break;
				default:
					break;
			}
			break;
		case PrtD:
			switch(bit){
				case 4:
					//alt4 tpm0 ch4
					PWM_TPM(0,4,value);
					Port->PCR[bit]&=0xFFFFF8FF;
					Port->PCR[bit]|=0x00000400;
					break;
				case 5:
					//alt4 tpm0 ch5
					PWM_TPM(0,5,value);
					Port->PCR[bit]&=0xFFFFF8FF;
					Port->PCR[bit]|=0x00000400;
					break;
				default:
					break;
			}
			break;
		case PrtE:
			switch(bit){
				case 30:
					//alt3 tpm0 ch3
					PWM_TPM(0,3,value);
					Port->PCR[bit]&=0xFFFFF8FF;
					Port->PCR[bit]|=0x00000300;
					break;
				default:
					break;
			}
			break;
		default:
			break;
	}
}

//   #ifdef TimerX_Conflict
	//   #error TimerX_Conflict
//   #endif
//   #define TimerX_Conflict "Timer X is already used by PWM funtionality"
	
	
  //   PinDescription pinDesc = g_APinDescription[pin];
  //   uint32_t attr = pinDesc.ulPinAttribute;

  //   if ((attr & PIN_ATTR_ANALOG) == PIN_ATTR_ANALOG)
  //   {
    //   // DAC handling code

    //   if (pin != PIN_A0) { // Only 1 DAC on A0 (PA02)
      //   return;
    //   }

    //   value = mapResolution(value, _writeResolution, 10);

    //   syncDAC();
    //   DAC->DATA.reg = value & 0x3FF;  // DAC on 10 bits.
    //   syncDAC();
    //   DAC->CTRLA.bit.ENABLE = 0x01;     // Enable DAC
    //   syncDAC();
    //   return;
  //   }

  //   if ((attr & PIN_ATTR_PWM) == PIN_ATTR_PWM)
  //   {
    //   value = mapResolution(value, _writeResolution, 8);

    //   uint32_t tcNum = GetTCNumber(pinDesc.ulPWMChannel);
    //   uint8_t tcChannel = GetTCChannelNumber(pinDesc.ulPWMChannel);
    //   static bool tcEnabled[TCC_INST_NUM+TC_INST_NUM];

    //   if (!tcEnabled[tcNum]) {
      //   tcEnabled[tcNum] = true;

      //   if (attr & PIN_ATTR_TIMER) {
        //   #if !(ARDUINO_SAMD_VARIANT_COMPLIANCE >= 10603)
        //   // Compatibility for cores based on SAMD core <=1.6.2
        //   if (pinDesc.ulPinType == PIO_TIMER_ALT) {
          //   pinPeripheral(pin, PIO_TIMER_ALT);
        //   } else
        //   #endif
        //   {
          //   pinPeripheral(pin, PIO_TIMER);
        //   }
      //   } else {
        //   // We suppose that attr has PIN_ATTR_TIMER_ALT bit set...
        //   pinPeripheral(pin, PIO_TIMER_ALT);
      //   }

      //   uint16_t GCLK_CLKCTRL_IDs[] = {
        //   GCLK_CLKCTRL_ID(GCM_TCC0_TCC1), // TCC0
        //   GCLK_CLKCTRL_ID(GCM_TCC0_TCC1), // TCC1
        //   GCLK_CLKCTRL_ID(GCM_TCC2_TC3),  // TCC2
        //   GCLK_CLKCTRL_ID(GCM_TCC2_TC3),  // TC3
        //   GCLK_CLKCTRL_ID(GCM_TC4_TC5),   // TC4
        //   GCLK_CLKCTRL_ID(GCM_TC4_TC5),   // TC5
        //   GCLK_CLKCTRL_ID(GCM_TC6_TC7),   // TC6
        //   GCLK_CLKCTRL_ID(GCM_TC6_TC7),   // TC7
      //   };
      //   GCLK->CLKCTRL.reg = (uint16_t) (GCLK_CLKCTRL_CLKEN | GCLK_CLKCTRL_GEN_GCLK0 | GCLK_CLKCTRL_IDs[tcNum]);
      //   while (GCLK->STATUS.bit.SYNCBUSY == 1);

      //   // Set PORT
      //   if (tcNum >= TCC_INST_NUM) {
        //   // -- Configure TC
        //   Tc* TCx = (Tc*) GetTC(pinDesc.ulPWMChannel);
        //   // Disable TCx
        //   TCx->COUNT8.CTRLA.bit.ENABLE = 0;
        //   syncTC_8(TCx);
        //   // Set Timer counter Mode to 8 bits, normal PWM, prescaler 1/256
        //   TCx->COUNT8.CTRLA.reg |= TC_CTRLA_MODE_COUNT8 | TC_CTRLA_WAVEGEN_NPWM | TC_CTRLA_PRESCALER_DIV256;
        //   syncTC_8(TCx);
        //   // Set the initial value
        //   TCx->COUNT8.CC[tcChannel].reg = (uint8_t) value;
        //   syncTC_8(TCx);
        //   // Set PER to maximum counter value (resolution : 0xFF)
        //   TCx->COUNT8.PER.reg = 0xFF;
        //   syncTC_8(TCx);
        //   // Enable TCx
        //   TCx->COUNT8.CTRLA.bit.ENABLE = 1;
        //   syncTC_8(TCx);
      //   } else {
        //   // -- Configure TCC
        //   Tcc* TCCx = (Tcc*) GetTC(pinDesc.ulPWMChannel);
        //   // Disable TCCx
        //   TCCx->CTRLA.bit.ENABLE = 0;
        //   syncTCC(TCCx);
        //   // Set prescaler to 1/256
        //   TCCx->CTRLA.reg |= TCC_CTRLA_PRESCALER_DIV256;
        //   syncTCC(TCCx);
        //   // Set TCx as normal PWM
        //   TCCx->WAVE.reg |= TCC_WAVE_WAVEGEN_NPWM;
        //   syncTCC(TCCx);
        //   // Set the initial value
        //   TCCx->CC[tcChannel].reg = (uint32_t) value;
        //   syncTCC(TCCx);
        //   // Set PER to maximum counter value (resolution : 0xFF)
        //   TCCx->PER.reg = 0xFF;
        //   syncTCC(TCCx);
        //   // Enable TCCx
        //   TCCx->CTRLA.bit.ENABLE = 1;
        //   syncTCC(TCCx);
      //   }
    //   } else {
      //   if (tcNum >= TCC_INST_NUM) {
        //   Tc* TCx = (Tc*) GetTC(pinDesc.ulPWMChannel);
        //   TCx->COUNT8.CC[tcChannel].reg = (uint8_t) value;
        //   syncTC_8(TCx);
    //   } else {
        //   Tcc* TCCx = (Tcc*) GetTC(pinDesc.ulPWMChannel);
        //   TCCx->CTRLBSET.bit.LUPD = 1;
        //   syncTCC(TCCx);
        //   TCCx->CCB[tcChannel].reg = (uint32_t) value;
        //   syncTCC(TCCx);
        //   TCCx->CTRLBCLR.bit.LUPD = 1;
        //   syncTCC(TCCx);
      //   }
    //   }
    //   return;
  //   }

  //   // -- Defaults to digital write
  //   pinMode(pin, OUTPUT);
  //   value = mapResolution(value, _writeResolution, 8);
  //   if (value < 128) {
    //   digitalWrite(pin, LOW);
  //   } else {
    //   digitalWrite(pin, HIGH);
  //   }
//   }

#ifdef __cplusplus
}
#endif
