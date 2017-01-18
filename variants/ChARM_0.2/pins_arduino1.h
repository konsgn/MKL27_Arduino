#ifndef PINS_ARDUINO_H_
#define PINS_ARDUINO_H_

#define MKL27ZXXVFM4

#define D0 0	//PTA_0 
#define D1 1	//PTA_1 
#define D2 2	//PTA_2 
#define D3 3	//PTA_3 
#define D4 4	//PTA_4 | Boot_pin 
#define D5 5	//PTA_18 
#define D6 6	//PTA_19 
#define D7 7	//PTA_20 

#define D8  8	//PTB_0
#define D9  9	//PTB_1

#define D10 10	//PTC_1
#define D11 11	//PTC_2
#define D12 12	//PTC_3
#define D13 13	//PTC_4
#define D14 14	//PTC_5
#define D15 15	//PTC_6
#define D16 16	//PTC_7

#define D17 17	//PTD_4
#define D18 18	//PTD_5
#define D19 19	//PTD_6
#define D20 20	//PTD_7

#define D21 21	//PTE_0
#define D22 22	//PTE_16
#define D23 23	//PTE_30

#define A0 D22	// SE1 
#define A1 D23  // SE23 
#define A2 D8 	// SE8
#define A3 D9	// SE9
#define A4 D10	// SE15 
#define A5 D11	// SE11 
#define A6 D18	// SE6b 
#define A7 D19	// SE7b 

//default SPI (SPI0) //TODO? add more spi?? why?
#define CS   D13 	//ALT-2 not first instance, but arranges well 
#define SCK  D14
#define MISO D15	//use ALT-5 to get matching miso-mosi to here
#define MOSI D16

//Define SPI1 channel anyway
#define CS1   D17 	//ALT-2 not first instance, but arranges well 
#define SCK1  D18
#define MISO1 D19	//use ALT-5 to get matching miso-mosi to here
#define MOSI1 D20   

//default I2C(I2C0)channel
#define SDA  D9 	//ALT-2
#define SCL  D8
// I2C1 channel
#define SDA1  D11 	//ALT-2
#define SCL1  D10

#define DEFAULT_RX_PIN D1  //TODO fix if issues arise  
#define DEFAULT_TX_PIN D2  //use LPUART0 on ALT-2

#define TWI_SDA SDA
#define TWI_SCL SCL

#define LED D13

#endif
