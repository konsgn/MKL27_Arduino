//USB Common functions. 

/* ChARM USB core functionality
 * Copyright (c) 2016 Konsgn
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * 1. The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 * 
 * //adapted from https://github.com/payne92/bare-metal-arm
 */

#ifndef __USB_COMMON_H_
#define __USB_COMMON_H_


#ifndef USB_NONE


#ifdef __cplusplus
extern "C" {
#endif

//the Implementation specific header should be already called,
// it relies on this

#ifndef __USB_IMPLEMENTATION_
#error "IMPLEMENTATION HEADER NOT CALLED FIRST"
#endif

//   #include <MKL27Z644.h>
#include "Arduino.h"
//   #include <stdio.h>

//Device USB Stuff
typedef struct _USB_BD{
	union{
		volatile uint8_t LByte;
		struct {
			uint8_t NA 	:2;
			uint8_t PID :4;
			uint8_t DAT :1;
			uint8_t OWN :1;
		} BDReadByte;
		struct {
			uint8_t NA 	:2;
			uint8_t STALL 	:1; // when set, create stall condition if BD is read
			uint8_t DTS 	:1; // when set, allow usbfs to toggle Data bit
			uint8_t NINC 	:1; // when set, disable DMA add increment for FIFOs
			uint8_t KEEP 	:1; // when set, give usbfs full control of BD
			uint8_t DAT 	:1;
			uint8_t OWN 	:1;
		} BDWriteByte;
	}stat;
    uint8_t     _dummy;
    uint16_t    count;
    uint8_t     *addr; //this is a 32bit pointer to an address.. anywhere in mem  
}__attribute__((packed)) USB_BD;

typedef struct ep_info{
	union{
		volatile uint8_t booleans;
		struct {
			uint8_t BANK :1;
			uint8_t RSVD1:5;
			uint8_t DATA :1;
			uint8_t RSVD0:1;
		};
	}Bits;
	uint16_t	buff_size;
	uint8_t		*data;
	uint16_t	Len;
}__attribute__((packed)) ep_info;


//USB Protocol USB Stuff
typedef struct Setup_packet{
	uint8_t 	bmRequestType;
	uint8_t  	bRequest;
	union {
		uint16_t wValue;
		struct {
			uint8_t Descriptor_Index;
			uint8_t Descriptor_Type;
		};
	};
	//   uint16_t 	wValue;
	uint16_t	wIndex;
	uint16_t	wLength;
}__attribute__((packed)) Setup_packet;


typedef struct USB_Descriptor_Header{
	uint8_t bLength;
	uint8_t bDescriptorType;
}__attribute__((packed)) Descriptor_Header;

typedef struct Device_Descriptor{
	Descriptor_Header DHeader;
	//   uint8_t		bLength;
	//   uint8_t		bDescriptorType;
	uint16_t	bcdUSB;
	uint8_t		bDeviceClass;
	uint8_t		bDeviceSubClass;
	uint8_t		bDeviceProtocol;
	uint8_t		bMaxPacketSize;
	uint16_t	idVendor;
	uint16_t	idProduct;
	uint16_t	bcdDevice;
	uint8_t		iManufacturer;
	uint8_t		iProduct;
	uint8_t		iSerialNumber;
	uint8_t		bNumConfigurations;
}__attribute__((packed)) Device_Descriptor;

typedef struct Configuration_Header{
	Descriptor_Header DHeader;
	//   uint8_t		bLength;
	//   uint8_t		bDescriptorType;
	uint16_t	wTotalLength;
	uint8_t		bNumInterfaces;
	uint8_t		bConfigurationValue;
	uint8_t		iConfiguration;
	uint8_t		bmAttributes;
	uint8_t		bMaxPower;
}__attribute__((packed)) Configuration_Header;

typedef struct Interface_Header{
	Descriptor_Header DHeader;
	//   uint8_t		bLength
	//   uint8_t		bDescriptorType;
	uint8_t		bInterfaceNumber;
	uint8_t		bAlternateSetting;
	uint8_t		bNumEndpoints;
	uint8_t		bInterfaceClass;
	uint8_t		bInterfaceSubClass;
	uint8_t		bInterfaceProtocol;
	uint8_t		iInterface;
}__attribute__((packed)) Interface_Header;
	
typedef struct Endpoint_Descriptor{
	Descriptor_Header DHeader;
	//   uint8_t		bLength
	//   uint8_t		bDescriptorType;
	uint8_t		bEndpointAddress;
	uint8_t		bmAttributes;
	uint16_t	wMaxPacketSize;
	uint8_t		bInterval;
}__attribute__((packed)) Endpoint_Descriptor;


typedef struct USB_String_Desc{
	Descriptor_Header DHeader;
	uint16_t string[];
}USB_String_Desc;

#define String0_size 4
static USB_String_Desc String0 ={
	{0x04,0x03},
	{0x0409}
};

#define Empty_String_Desc_size 2
static USB_String_Desc Empty_String_Desc ={
	{0x02,0x03}
};
	


//PID type defines 
//Group Token
#define PID_OUT_TOKEN 		0x01
#define PID_IN_TOKEN		0x09
#define PID_SOF_TOKEN		0x05
#define PID_SETUP_TOKEN		0x0D
//Group Data
#define PID_DATA0_DATA 		0x03
#define PID_DATA1_DATA		0x0B
#define PID_DATA2_DATA		0x07
#define PID_MDATA_DATA		0x0F
//Group Handshake
#define PID_ACK_HSHK 		0x02
#define PID_NACK_HSHK		0x0A
#define PID_STALL_HSHK		0x0E
#define PID_NYET_HSHK		0x06
//Group Special
#define PID_PRE_AMBLE 		0x0C
#define PID_ERR				0x0C
#define PID_SPLIT			0x08
#define PID_PING			0x04


// bmRequestType masks & Definitions:
#define SETUP_DDIR_MASK 		0x80
#define SETUP_TYPE_MASK 		0x60
	#define SETUP_TYPE_STANDARD			0x00 
	#define SETUP_TYPE_CLASS			0x20 
	#define SETUP_TYPE_VENDOR			0x40 
	#define SETUP_TYPE_RESERVED			0x60 
#define SETUP_RECIPIENT_MASK 	0x1F
	#define SETUP_RECIPIENT_DEVICE 		0x00
	#define SETUP_RECIPIENT_INTERFACE 	0x01
	#define SETUP_RECIPIENT_ENDPOINT 	0x02
	#define SETUP_RECIPIENT_OTHER		0x03
	#define SETUP_RECIPIENT_RSVD_MASK 	0x1C //covers any reserved reciepients
	

// bRequest Common Values All/Device:
#define SETUP_REQUEST_GET_STATUS		0x00	
#define SETUP_REQUEST_CLEAR_FEATURE		0x01
#define SETUP_REQUEST_SET_FEATURE		0x03
#define SETUP_REQUEST_SET_ADDRESS		0x05
#define SETUP_REQUEST_GET_DESCRIPTOR	0x06
#define SETUP_REQUEST_SET_DESCRIPTOR	0x07
#define SETUP_REQUEST_GET_CONFIGURATION	0x08
#define SETUP_REQUEST_SET_CONFIGURATION	0x09

//bRequest Common Values Interface Unique:
#define SETUP_REQUEST_GET_INTERFACE		0x0A
#define SETUP_REQUEST_SET_INTERFACE		0x11

//bRequest Common Values Enpoint Unique:
#define SETUP_REQUEST_SYNC_FRAME		0x12

//Descriptor Types:
#define DESCRIPTOR_DEVICE				0x01
#define DESCRIPTOR_CONFIGURATION		0x02
#define DESCRIPTOR_STRING				0x03


// Buffer descriptor table


#define BD_PER_EP 4 //2 for odd/even ping pong buffs, and 2 for rx/tx
//   USB_BD bdt[ USED_ENDPOINTS * BD_PER_EP ] __attribute__ ((aligned(512), section("m_usb_bdt")));
//   static inline USB_BD *bd_rx(int num) { return &bdt[num * BD_PER_EP];}
//   static inline USB_BD *bd_tx(int num) { return bd_rx(num) + (BD_PER_EP / 2);}

#define ENDPOINT0_BUFF_SIZE 8
static uint8_t ep0_rx_buffs[2][ENDPOINT0_BUFF_SIZE]; //init even and odd rx buff
static uint8_t ep0_tx_buff[ENDPOINT0_BUFF_SIZE]; //init even and odd rx buff

//other endpoint sizes defined in device type included header.



// Initialize/enable an endpoint
void USB_Initialize(void);

//Implementation specific references:

//   extern static Device_Descriptor Dev_Desc;
//   extern Configuration_Descriptor Conf_Desc;
#define Conf_Desc Configuration_Descriptor

void Implementation_Setup_Handler();
//   void Implementation_SConfig_Handler(); //not implemented yet - not needed

void endpoint_handler1(USB_BD* bd_ptr);
void endpoint_handler2(USB_BD* bd_ptr);
void endpoint_handler3(USB_BD* bd_ptr);
void endpoint_handler4(USB_BD* bd_ptr);
void endpoint_handler5(USB_BD* bd_ptr);
void endpoint_handler6(USB_BD* bd_ptr);
void endpoint_handler7(USB_BD* bd_ptr);
void endpoint_handler8(USB_BD* bd_ptr);
void endpoint_handler9(USB_BD* bd_ptr);
void endpoint_handler10(USB_BD* bd_ptr);
void endpoint_handler11(USB_BD* bd_ptr);
void endpoint_handler12(USB_BD* bd_ptr);
void endpoint_handler13(USB_BD* bd_ptr);
void endpoint_handler14(USB_BD* bd_ptr);
void endpoint_handler15(USB_BD* bd_ptr);



#ifdef __cplusplus
}
#endif


#else  //USB_NONE

#ifdef __cplusplus
extern "C" {
#endif

void USB_Initialize(void);

#ifdef __cplusplus
}
#endif

#endif //USB_NONE


#endif //__USB_COMMON_H_
