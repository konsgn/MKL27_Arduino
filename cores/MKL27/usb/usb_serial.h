

#ifndef __USB_SERIAL_H_
#define __USB_SERIAL_H_
#define __USB_IMPLEMENTATION_

#include "usb_common.h"

#include <MKL27Z644.h>
#include <stdio.h>

// USB Serial implementation, uses CDC-ACM.

#define USED_ENDPOINTS 3 //including USB EP0
#define VENDOR_ID  0x1209
#define PRODUCT_ID 0xB000
#define MANUFACTURER_STR 	{'K','o','n','s','g','n'}
#define PRODUCT_STR 		{'C','h','A','R','M',' ','S','e','r','i','a','l',}

//   #define ENDPOINT1_ALLOC_BUFFER //TX Buffer Type
//   #define ENDPOINT1_FIXED_BUFFER  
//   #define ENDPOINT2_FIXED_BUFFER  
//   #define ENDPOINT1_FIFO_BUFFER

#define CDC_ACM_SIZE 		16
#define CDC_RX_SIZE			64
#define CDC_TX_SIZE			64

#define Config_size sizeof(Configuration_Descriptor)

static ep_info ep_information[USED_ENDPOINTS];

static Device_Descriptor Dev_Desc= {
	(uint8_t)sizeof(Device_Descriptor),
	0x01, //constant device descriptor define
	0x0200,		// bcdUSB;
	0x02,		// bDeviceClass;
	0x00,		// bDeviceSubClass;
	0x00,		// bDeviceProtocol;
	ENDPOINT0_BUFF_SIZE,		// bMaxPacketSize;
	VENDOR_ID,		// idVendor;
	PRODUCT_ID,		// idProduct;
	0x0001,		// bcdDevice;
	0x01,		// iManufacturer;
	0x02,		// iProduct;
	0x03,		// iSerialNumber;
	0x01		// bNumConfigurations;
};

static struct {
	Configuration_Header C_header;
	Interface_Header I_ACM_header;
	uint8_t CDC_Header[5];
	uint8_t CDC_Call[5];
	uint8_t CDC_ACM[4];
	uint8_t Union_Desc[5];
	Endpoint_Descriptor E_ACM_Descriptor;
	Interface_Header I_DATA_header;
	Endpoint_Descriptor E_RX_Descriptor;
	Endpoint_Descriptor E_TX_Descriptor;
} __attribute__((packed)) Configuration_Descriptor = {
	.C_header.DHeader.bLength=sizeof(Configuration_Header),
	.C_header.DHeader.bDescriptorType= 0x02,
	.C_header.wTotalLength= Config_size,	// wTotalLength;
	.C_header.bNumInterfaces=0x02,	// bNumInterfaces;
	.C_header.bConfigurationValue=0x01,		// bConfigurationValue;
	.C_header.iConfiguration=0x00,		// iConfiguration;
	.C_header.bmAttributes=0xC0,		// bmAttributes;
	.C_header.bMaxPower=250,		// bMaxPower;
	
	.I_ACM_header.DHeader.bLength=sizeof(Interface_Header),
	.I_ACM_header.DHeader.bDescriptorType=0x04,
	.I_ACM_header.bInterfaceNumber=0x00,	// bInterfaceNumber;
	.I_ACM_header.bAlternateSetting=0x00,	// bAlternateSetting;
	.I_ACM_header.bNumEndpoints=0x01,	// bNumEndpoints;
	.I_ACM_header.bInterfaceClass=0x02,	// bInterfaceClass;
	.I_ACM_header.bInterfaceSubClass=0x02,	// bInterfaceSubClass;
	.I_ACM_header.bInterfaceProtocol=0x01,	// bInterfaceProtocol;
	.I_ACM_header.iInterface=0x00,	// iInterface;
		//Copied from Teensy, can't find CDC spec sheet.
	// CDC Header Functional Descriptor, CDC Spec 5.2.3.1, Table 26
	5,                                      // bFunctionLength
	0x24,                                   // bDescriptorType
	0x00,                                   // bDescriptorSubtype
	0x10,0x01,                              // bcdCDC //stored low byte first
	// Call Management Functional Descriptor, CDC Spec 5.2.3.2, Table 27
	5,                                      // bFunctionLength
	0x24,                                   // bDescriptorType
	0x01,                                   // bDescriptorSubtype
	0x01,                                   // bmCapabilities
	1,                                      // bDataInterface
	// Abstract Control Management Functional Descriptor, CDC Spec 5.2.3.3, Table 28
	4,                                      // bFunctionLength
	0x24,                                   // bDescriptorType
	0x02,                                   // bDescriptorSubtype
	0x06,                                   // bmCapabilities
	// Union Functional Descriptor, CDC Spec 5.2.3.8, Table 33
	5,                                      // bFunctionLength
	0x24,                                   // bDescriptorType
	0x06,                                   // bDescriptorSubtype
	0,                   					// bMasterInterface
	1,                     					// bSlaveInterface0
	
	.E_ACM_Descriptor.DHeader.bLength=sizeof(Endpoint_Descriptor),
	.E_ACM_Descriptor.DHeader.bDescriptorType=0x05,
	.E_ACM_Descriptor.bEndpointAddress=0x81,		// bEndpointAddress;
	.E_ACM_Descriptor.bmAttributes=0x03,		// bmAttributes;
	.E_ACM_Descriptor.wMaxPacketSize=CDC_ACM_SIZE,	// wMaxPacketSize;
	.E_ACM_Descriptor.bInterval=0x40,		// bInterval //64;
	
	.I_DATA_header.DHeader.bLength=sizeof(Interface_Header),
	.I_DATA_header.DHeader.bDescriptorType=0x04,
	.I_DATA_header.bInterfaceNumber=0x01,	// bInterfaceNumber;
	.I_DATA_header.bAlternateSetting=0x00,	// bAlternateSetting;
	.I_DATA_header.bNumEndpoints=0x02,	// bNumEndpoints; //should this be 1 if only ep 2 is used?
	.I_DATA_header.bInterfaceClass=0x0A,	// bInterfaceClass;
	.I_DATA_header.bInterfaceSubClass=0x00,	// bInterfaceSubClass;
	.I_DATA_header.bInterfaceProtocol=0x00,	// bInterfaceProtocol;
	.I_DATA_header.iInterface=0x00,	// iInterface;
	
	.E_RX_Descriptor.DHeader.bLength=sizeof(Endpoint_Descriptor),
	.E_RX_Descriptor.DHeader.bDescriptorType=0x05,
	.E_RX_Descriptor.bEndpointAddress=0x02,		// bEndpointAddress;
	.E_RX_Descriptor.bmAttributes=0x02,		// bmAttributes;
	.E_RX_Descriptor.wMaxPacketSize=CDC_RX_SIZE,	// wMaxPacketSize;
	.E_RX_Descriptor.bInterval=0x40,		// bInterval //64;
	
	.E_TX_Descriptor.DHeader.bLength=sizeof(Endpoint_Descriptor),
	.E_TX_Descriptor.DHeader.bDescriptorType=0x05,
	.E_TX_Descriptor.bEndpointAddress=0x82,		// bEndpointAddress;
	.E_TX_Descriptor.bmAttributes=0x02,		// bmAttributes;
	.E_TX_Descriptor.wMaxPacketSize=CDC_TX_SIZE,	// wMaxPacketSize;
	.E_TX_Descriptor.bInterval=0x40		// bInterval //64;
};

#define String1_size 14
static USB_String_Desc String1 ={
	{14,0x03},
	MANUFACTURER_STR
};


#define String2_size 26
static USB_String_Desc String2 ={
	{26,0x03},
	PRODUCT_STR
};

//
//Things to implement:
//extern void Implementation_Setup_Handler();
//extern void Implementation_SConfig_Handler();
//


#endif //USB_SERIAL

