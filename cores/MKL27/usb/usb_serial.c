

#ifndef USB_NONE

#include "usb_serial.h"
#include "usb_common.h"


static volatile LineInfo _usbLineInfo __attribute__((packed))= {
	115200, // dWDTERate
	0x00,   // bCharFormat
	0x00,   // bParityType
	0x08,   // bDataBits
	0x00    // lineState
};

void Implementation_Setup_Handler(){
	if(!(Setup->bmRequestType&0x80)){ //It's an incaming data transaction from pc
		switch(Setup->bRequest){
			case CDC_SET_LINE_CODING:
				break;
			case CDC_SET_CONTROL_LINE_STATE:
				break;
			case CDC_SEND_BREAK:
				//usb_tx(&bdt[0],0,0);
				//   USB0->ADDR=Setup->wValue;
				//   Setup=NULL;
				break;
			//   case SETUP_REQUEST_SET_CONFIGURATION:
				//   Implementation_SetConfig_Handler(Setup->wValue);
				//   usb_tx(&bdt[0],0,0);
				//   break;
		}
	}
	else{ //It's an outgoing transaction sent to pc
		switch(Setup->bRequest){
			case CDC_GET_LINE_CODING:
				ep0_tx_buff[0]= 0x00;
				ep0_tx_buff[1]= 0x00;
				usb_start_tx(0,ep0_tx_buff,2);
				break;
		}
	}
};

void Implementation_SetConfig_Handler(){}; //not implemented yet - not needed

void endpoint_handler1(USB_BD *bd_ptr){
};
void endpoint_handler2(USB_BD *bd_ptr){
};
void endpoint_handler3(USB_BD *bd_ptr){
};
void endpoint_handler4(USB_BD *bd_ptr){
};
void endpoint_handler5(USB_BD *bd_ptr){
};
void endpoint_handler6(USB_BD *bd_ptr){
};
void endpoint_handler7(USB_BD *bd_ptr){
};
void endpoint_handler8(USB_BD *bd_ptr){
};
void endpoint_handler9(USB_BD *bd_ptr){
};
void endpoint_handler10(USB_BD *bd_ptr){
};
void endpoint_handler11(USB_BD *bd_ptr){
};
void endpoint_handler12(USB_BD *bd_ptr){
};
void endpoint_handler13(USB_BD *bd_ptr){
};
void endpoint_handler14(USB_BD *bd_ptr){
};
void endpoint_handler15(USB_BD *bd_ptr){
};

#else  // ifndef USB_NONE
#endif // ifndef USB_NONE
