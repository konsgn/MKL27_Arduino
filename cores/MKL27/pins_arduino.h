#ifndef __PINS_ARDUINO_H_
#define __PINS_ARDUINO_H_


#include "Arduino.h"
#include <MKL27Z644.h>
#include "variant.h"


static GPIO_Type* GPIO_PTRS[] = GPIO_BASE_PTRS;
static PORT_Type* PORT_PTRS[] = PORT_BASE_PTRS;

//portpoint defines to ease pointing to a particular port

//   const typedef enum _portpoint{
#define PrtA 0
#define PrtB 1
#define PrtC 2
#define PrtD 3
#define PrtE 4
//   }

struct pin_and_config_table {
	uint8_t regandpinmask; //Upper 3 bits represent port as ordered in GPIO_PTRS, lower 5 bits show which pin 0-31
	volatile uint32_t * config;
};

extern const struct pin_and_config_table digital_pin_to_info_PGM[];

#define GPIO_Type_Extract(pin)  GPIO_PTRS[(digital_pin_to_info_PGM[pin].regandpinmask>>5)]
#define PORT_Type_Extract(pin)  PORT_PTRS[(digital_pin_to_info_PGM[pin].regandpinmask>>5)]

#define digitalPinToPort(pin)		(digital_pin_to_info_PGM[pin].regandpinmask>>5)
#define digitalPinToBit(pin)		(digital_pin_to_info_PGM[pin].regandpinmask & 0x1F)
#define digitalPinToBitMask(pin)	(1<<(digital_pin_to_info_PGM[pin].regandpinmask & 0x1F))
#define portOutputRegister(pin) 	((volatile uint32_t *)&(GPIO_Type_Extract(pin)->PDOR)) 
#define portSetRegister(pin)    	((volatile uint32_t *)&(GPIO_Type_Extract(pin)->PSOR))
#define portClearRegister(pin)  	((volatile uint32_t *)&(GPIO_Type_Extract(pin)->PCOR))
#define portToggleRegister(pin) 	((volatile uint32_t *)&(GPIO_Type_Extract(pin)->PTOR))
#define portInputRegister(pin)  	((volatile uint32_t *)&(GPIO_Type_Extract(pin)->PDIR))
#define portModeRegister(pin)   	((volatile uint32_t *)&(GPIO_Type_Extract(pin)->PDDR))
//   #define portConfigRegister(pin) 	((volatile uint32_t *)(digital_pin_to_info_PGM[(pin)].config))
#define digitalPinToPortReg(pin) 	(portOutputRegister(pin))


#endif
