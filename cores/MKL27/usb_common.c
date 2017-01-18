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

#ifndef USB_NONE

#ifdef USB_SERIAL
#include "usb/usb_serial.h"
#endif

#ifdef USB_SERIAL_HID
#include "usb/usb_serial_hid.h"
#endif

#ifdef USB_RAWHID
#include "usb/usb_rawhid.h"
#endif

#include "usb_common.h"
#include "ser_print.h"

USB_BD bdt[ USED_ENDPOINTS * BD_PER_EP ] __attribute__ ((aligned(512), section("m_usb_bdt")));

// Pointer to buffer of last rx'd setup...hopufully still the setup data when 
// referenced. only really needed for addr change.
static Setup_packet *Setup = NULL;

static inline ep_info* bdptr_to_epptr(USB_BD *bdptr){
	return (ep_info*)&ep_information[((uint32_t)bdptr>>5&0x000F)];
}

static inline USB_BD* base_bd(USB_BD *bd_ptr){
	return (USB_BD*)((uint32_t)bd_ptr&0xFFFFFFF8);
}

static inline void ep_flush_tx(uint8_t num){
	ep_info *ep = &ep_information[num];
	USB_BD *bdptr = &bdt[num*BD_PER_EP];
	bdptr[2].stat.LByte = bdptr[3].stat.LByte = 0;
    ep->Bits.booleans&=0x01; //clear all but bank pointer 
    ep->Len=0;
    ep->data=NULL;
}

// Initialize/enable an endpoint
static void usb_init_ep(uint8_t num, int buflen, uint8_t *rx_buf0, uint8_t *rx_buf1)
{
    ep_info *ep = &ep_information[num];
	USB_BD *bdptr = &bdt[num*BD_PER_EP];
	//bdt(0->3) point to rx and tx even and odd buffers 
	
	ep->buff_size = buflen;
    ep_flush_tx(num);
    ep->Bits.booleans=0x00; //clear all 
    
    // Configure BDT entries for receive
    bdptr[0].addr = rx_buf0;
    bdptr[1].addr = rx_buf1;
    bdptr[0].count = bdptr[1].count = buflen;
    bdptr[0].stat.LByte = bdptr[1].stat.LByte |= 0x80; //set own bit
    
    // Enable endpoint for transmit and receive
    USB0->ENDPOINT[num].ENDPT = USB_ENDPT_EPTXEN_MASK | USB_ENDPT_EPRXEN_MASK 
                        | USB_ENDPT_EPHSHK_MASK;
}

// Send a buffer for a given endpoint
// expects pointer to base BD (RX Even BD)
static uint16_t usb_tx(USB_BD *bdptr, uint8_t *data, uint16_t len)
{
	ep_info* ep = bdptr_to_epptr(bdptr);

	// convert base BD to point to TX EVEN BD. 
	bdptr = (uint32_t)bdptr & 0xFFFFFFE0; //mask out to point to base BD 
 	bdptr+=2;
 	// Then check if should point to odd BD
	if(ep->Bits.BANK)bdptr++;
    // Is the buffer owned by USBFNS or available to edit by our code? 
    if (bdptr->stat.LByte&0x80) //if OWN
        return 0;
    
    //   ser_print("\n bdptr:");
    //   ser_print_hex32(bdptr);
    //   ser_print("\n data:");
    //   ser_print_hex32(data);
    //   ser_print("\n len:");
    //   ser_print_hex32(len);
            
    bdptr->count = len;
    bdptr->addr = data;
    bdptr->stat.LByte = 0x80 | (ep->Bits.booleans&0x40);     // Give to USB controller
    
    //   ser_print("\n stat:");
    //   ser_print_hex32(bdptr->stat.LByte);
    
    //   ep->Bits.booleans ^= 1;                          // Alternate transmit buffers
    //   ep->Bits.booleans ^= 0x40;                       // Alternate DATA0/1
    ep->Bits.booleans^=0x41; // flip both data and txbuffer 
    
    //   ser_print("\n Bits:");
    //   ser_print_hex32(ep->Bits.booleans);
    return len;
}

static void usb_out_handler(USB_BD *bdptr){
	ep_info* ep = bdptr_to_epptr(bdptr);
	uint8_t transfer_size;
	while(ep->Len){
		transfer_size = (ep->Len < ep->buff_size)?ep->Len:ep->buff_size;
		if (!usb_tx(bdptr,ep->data,transfer_size))break;
		ep->Len-=transfer_size;
		ep->data+=transfer_size;
	}
		
}

static void usb_start_tx(uint8_t num, uint8_t *Data, uint16_t len){
	USB_BD *bdptr = &bdt[num*BD_PER_EP];
    ep_info *ep = &ep_information[num];
    ep->data = Data;
    ep->Len  = len;
	usb_out_handler(bdptr); //load up one buffer
}

static inline void ep0_standard_device(void){
	if(!(Setup->bmRequestType&0x80)){ //It's a outgoing transaction
		switch(Setup->bRequest){
			case SETUP_REQUEST_CLEAR_FEATURE:
				break;
			case SETUP_REQUEST_SET_FEATURE:
				break;
			case SETUP_REQUEST_SET_ADDRESS:
				usb_tx(&bdt[0],0,0);
				//   USB0->ADDR=Setup->wValue;
				//   Setup=NULL;
				break;
			case SETUP_REQUEST_SET_DESCRIPTOR:
				break;
			case SETUP_REQUEST_SET_CONFIGURATION:
				//   Implementation_SConfig_Handler(Setup->wValue);
				usb_tx(&bdt[0],0,0);
				break;
		}
	}
	else{ //It's an incoming transaction
		switch(Setup->bRequest){
			case SETUP_REQUEST_GET_STATUS:
				ep0_tx_buff[0]= 0x00;
				ep0_tx_buff[1]= 0x00;
				usb_start_tx(0,ep0_tx_buff,2);
				break;
			case SETUP_REQUEST_GET_DESCRIPTOR:
				switch (Setup->Descriptor_Type){
					case DESCRIPTOR_DEVICE:
						usb_start_tx(0,&Dev_Desc,(Setup->wLength < sizeof(Device_Descriptor))?Setup->wLength:sizeof(Device_Descriptor));
						break;
					case DESCRIPTOR_CONFIGURATION:
						usb_start_tx(0,&Conf_Desc,(Setup->wLength < sizeof(Configuration_Descriptor))?Setup->wLength:sizeof(Configuration_Descriptor));
						break;
					case DESCRIPTOR_STRING:
						switch(Setup->Descriptor_Index){
							case 0:
								usb_start_tx(0,&String0,(Setup->wLength < String0_size)?Setup->wLength:String0_size);
								break;
							#ifdef MANUFACTURER_STR
							case 1:
								usb_start_tx(0,&String1,(Setup->wLength < String1_size)?Setup->wLength:String1_size);
								break;
							#endif
							#ifdef PRODUCT_STR
							case 2:
								usb_start_tx(0,&String2,(Setup->wLength < String2_size)?Setup->wLength:String2_size);
								break;
							#endif
							default:
								usb_start_tx(0,&Empty_String_Desc,(Setup->wLength < Empty_String_Desc_size)?Setup->wLength:Empty_String_Desc_size);
								break;
							}
						break;
					default:
						break;
					}
				break;
			case SETUP_REQUEST_GET_CONFIGURATION:
				ep0_tx_buff[0]= 0x01;
				usb_start_tx(0,&ep0_tx_buff[0],1);
				break;
		}
	}
}

static inline void ep0_standard_interface(void){
	if(!(Setup->bmRequestType&0x80)){ //It's a outgoing transaction
		switch(Setup->bRequest){
			case SETUP_REQUEST_CLEAR_FEATURE:
				return;
			case SETUP_REQUEST_SET_FEATURE:
				break;
			case SETUP_REQUEST_SET_INTERFACE:
				break;
		}
	}
	else{ //It's an incoming transaction
		switch(Setup->bRequest){
			case SETUP_REQUEST_GET_STATUS:
				ep0_tx_buff[0]= 0x00;
				ep0_tx_buff[1]= 0x00;
				usb_start_tx(0,ep0_tx_buff,2);
				break;
			case SETUP_REQUEST_GET_INTERFACE:
				break;
		}
	}
}

static inline void ep0_standard_endpoint(void){
	if(!(Setup->bmRequestType&0x80)){ //It's a outgoing transaction
		switch(Setup->bRequest){
			case SETUP_REQUEST_CLEAR_FEATURE:
				USB0->ENDPOINT[Setup->wIndex&0x0F].ENDPT &= ~USB_ENDPT_EPSTALL_MASK;
				break;
			case SETUP_REQUEST_SET_FEATURE:
				break;
		}
	}
	else{ //It's an incoming transaction
		switch(Setup->bRequest){
			case SETUP_REQUEST_GET_STATUS:
				ep0_tx_buff[0]= 0x00;
				ep0_tx_buff[1]= 0x00;
				if(USB0->ENDPOINT[Setup->wIndex&0x0F].ENDPT&USB_ENDPT_EPSTALL_MASK) \
					ep0_tx_buff[0]= 0x01;
				usb_start_tx(0,ep0_tx_buff,2);
				break;
			case SETUP_REQUEST_SYNC_FRAME:
				break;
		}
	}
}


static inline void endpoint_handler0(USB_BD *bdptr){
	uint8_t PID = (bdptr->stat.LByte)>>2 & 0x0F;
	switch (PID){
		case PID_SETUP_TOKEN:
			Setup = (Setup_packet*)((bdptr->addr));			
			//   ser_print("\nSetup:");
			//   ser_print_hex32(bdptr->addr);		
			//   ser_print("\nRXd:");
			//   ser_print_hex16(bdptr->count);		
			//   ser_print("\nRXd:");
			//   ser_print_hex32(Setup->bmRequestType);		
			bdptr->count=ENDPOINT0_BUFF_SIZE;
			bdptr->stat.LByte|=0x40; //set Data1
			ep_info* ep = bdptr_to_epptr(bdptr);
			ep_flush_tx(0);
			ep->Bits.booleans|=0x40; //sent Data bit
			
			if (((Setup->bmRequestType)&SETUP_TYPE_MASK)==SETUP_TYPE_STANDARD){
	//   ser_print("SETUP:");
	//   ser_print_hex32(*(uint32_t*)Setup);	
				if (((Setup->bmRequestType)&SETUP_RECIPIENT_MASK)==SETUP_RECIPIENT_DEVICE)
					ep0_standard_device();
				else if (((Setup->bmRequestType)&SETUP_RECIPIENT_MASK)==SETUP_RECIPIENT_INTERFACE)
					ep0_standard_interface();
				else if (((Setup->bmRequestType)&SETUP_RECIPIENT_MASK)==SETUP_RECIPIENT_ENDPOINT)
					ep0_standard_endpoint();
			}
			else if(Implementation_Setup_Handler)(*Implementation_Setup_Handler);
			USB0->CTL = USB_CTL_USBENSOFEN_MASK;  
			break;
		case PID_OUT_TOKEN:
	//   ser_print("OUT:");
			
			//Shouldn't be sending data to endpoint 0...right?
			
			// give the buffer back
			bdptr->stat.LByte = 0x40; //setdata1
			bdptr->count = ENDPOINT0_BUFF_SIZE; //setdata1
			break;
		case PID_IN_TOKEN:
	//   ser_print("IN:");
    //   ser_print("\n bdptr:");
    //   ser_print_hex32(bdptr);
    //   ser_print("\n bdptrhi:");
    //   ser_print_hex32(*(uint32_t*)bdptr);
    //   ser_print("\n bdptradd:");
    //   ser_print_hex32(bdptr->addr);
			usb_out_handler(base_bd(bdptr));
			if(Setup->bRequest==SETUP_REQUEST_SET_ADDRESS){
				USB0->ADDR=Setup->wValue;
				Setup=NULL;
			}
			break;
		default:
	//   ser_print("Default:");
    //   ser_print_hex(PID);
			break;
	}
	//   USB0->CTL &= ~USB_CTL_TXSUSPENDTOKENBUSY_MASK;
	USB0->CTL = USB_CTL_USBENSOFEN_MASK; 
}

static void usb_packet_handler(uint8_t stat)
{
    uint8_t statinfo = stat >> 2;
    USB_BD* bd_ptr = &bdt[statinfo]; 
    uint8_t endpoint = ((stat>>4)&0x0F);
    ep_info *ep = &ep_information[endpoint];
    
	//   ser_print("\nEpt:");
	//   ser_print_hex(endpoint);
    switch (endpoint){
		case 0:
			endpoint_handler0(bd_ptr);
			break;
		case 1:
			endpoint_handler1(bd_ptr);
			break;
		case 2:
			endpoint_handler2(bd_ptr);
			break;
		case 3:
			endpoint_handler3(bd_ptr);
			break;
		case 4:
			endpoint_handler4(bd_ptr);
			break;
		case 5:
			endpoint_handler5(bd_ptr);
			break;
		case 6:
			endpoint_handler6(bd_ptr);
			break;
		case 7:
			endpoint_handler7(bd_ptr);
			break;
		case 8:
			endpoint_handler8(bd_ptr);
			break;
		case 9:
			endpoint_handler9(bd_ptr);
			break;
		case 10:
			endpoint_handler10(bd_ptr);
			break;
		case 11:
			endpoint_handler11(bd_ptr);
			break;
		case 12:
			endpoint_handler12(bd_ptr);
			break;
		case 13:
			endpoint_handler13(bd_ptr);
			break;
		case 14:
			endpoint_handler14(bd_ptr);
			break;
		case 15:
			endpoint_handler15(bd_ptr);
			break;
		default:
			break;
		}
    //   if(endpoint==0){
		//   //process endpoint 0 here, as it is common to all devices
		//   usb_
	//   }
	//   else{
		//   //if handler exists for that ep, call it with a pointer to the bd reference
		//   if(*endpoint_handler[endpoint-1])(*endpoint_handler[endpoint-1])(bd_ptr);
		//   else {
			//   //Error!
		//   }
	//   }
	
	    // For receive buffers, configure to receive next token
    int tx = stat & 0x08;
    if(!tx) {
        bd_ptr->count = ep->buff_size;               // XXX: generalize this
        bd_ptr->stat.LByte = 0x80; //set own
    }
	return;
}

void usb_int_handle(void){
	uint8_t status,error,it;
	
	//Pull in status register state at start of interrupt
	status = USB0->ISTAT; 
	//   ser_print("\nStatus:");
	//   ser_print_hex(status);
	//   do{
		if(status & USB_ISTAT_USBRST_MASK) {         
			//   ser_print("\nReset->");
			// Reset state seen, address was set to 0, ..maybe? doc is conflicting
			//following KQRUG.pdf
			//   for(it=0;it<16;it++){
				//   //disable each Endpoint
				//   USB0->ENDPOINT[it].ENDPT = 0x00;
			//   }
			
			//reset all ping pongs to 0 (even)
			USB0->CTL |= USB_CTL_ODDRST_MASK;

			// Configure endpoint 0 (the control endpoint)
			usb_init_ep(0, ENDPOINT0_BUFF_SIZE, ep0_rx_buffs[0], ep0_rx_buffs[1]);

			// Clear flags
			USB0->ERRSTAT = 0xFF;
			USB0->ISTAT = 0xFF;

			// Enable all error sources
			USB0->ERREN = 0x7E;

			// Enable USB interrupts
			USB0->INTEN = USB_INTEN_TOKDNEEN_MASK | USB_INTEN_ERROREN_MASK 
							| USB_INTEN_USBRSTEN_MASK | USB_INTEN_STALLEN_MASK;
			return;
		}
		// Process any pending token done interrupts (may be queued)
		//   else if (status & USB_ISTAT_TOKDNE_MASK) {    
		while(status & USB_ISTAT_TOKDNE_MASK) {
			//   ser_print("\nTKN-DNE->");
			usb_packet_handler(USB0->STAT);
			USB0->ISTAT = USB_ISTAT_TOKDNE_MASK;
			status = USB0->ISTAT;
		}
		if (status & USB_ISTAT_STALL_MASK) {
			//whaaaat? 
			USB0->ENDPOINT[0].ENDPT = USB_ENDPT_EPRXEN_MASK | USB_ENDPT_EPTXEN_MASK | USB_ENDPT_EPHSHK_MASK;
			USB0->ISTAT = USB_ISTAT_STALL_MASK;
		}
		if (status & USB_ISTAT_ERROR_MASK) {
			//   uint8_t err = USB0_ERRSTAT;
			//   USB0_ERRSTAT = err;
			//   //serial_print("err:");
			//   //serial_phex(err);
			//   //serial_print("\n");
			//   ser_print("\nErr:");
			ser_print_hex(USB0->ERRSTAT);
			USB0->ERRSTAT = 0xFF;
			USB0->ISTAT = USB_ISTAT_ERROR_MASK;
		}
		//   USB0->ISTAT =0xF0;
	//   }while(USB0->ISTAT);
}

void USB_Initialize(void){
	// Turn on 48MHz clock recovery from USB before initializing USB.
	USB0->CLK_RECOVER_IRC_EN = USB_CLK_RECOVER_IRC_EN_IRC_EN_MASK;
	USB0->CLK_RECOVER_CTRL   = USB_CLK_RECOVER_CTRL_CLOCK_RECOVER_EN_MASK;
	
	//Reset USB
	USB0->USBTRC0=0xC0; //pull reset pin high
	
	//   asm volatile("mov r0, r0");
	//   asm volatile("mov r0, r0"); //Datasheet says 2 cycle delay.
	while(USB0->USBTRC0 & USB_USBTRC0_USBRESET_MASK)continue;
	
	// Set the Buffer Descriptor Register address
	USB0->BDTPAGE1=(uint8_t)((uint32_t)bdt >> 8);
	USB0->BDTPAGE2=(uint8_t)((uint32_t)bdt >> 16);
	USB0->BDTPAGE3=(uint8_t)((uint32_t)bdt >> 24);
	
	// Clear Interrupts
	USB0->ISTAT   = 0xFF; //clear all
	USB0->ERRSTAT = 0xFF; //clear all
	//Enable Reset Interrupt
	USB0->INTEN = USB_INTEN_USBRSTEN_MASK;
	// enable from a default suspend state, and turn off weak pull downs
	USB0->USBCTRL = 0x00; 
	
	//enable the pull up on Dp for full speed. 
	USB0->CONTROL = USB_CONTROL_DPPULLUPNONOTG_MASK; 
	
	//enable the usb controller 
	USB0->CTL = USB_CTL_USBENSOFEN_MASK;
	
	USB_SetHandler(&usb_int_handle);
	NVIC_SetPriority(USB0_IRQn, 1);
	NVIC_EnableIRQ(USB0_IRQn);
}

#else  // USB_NONE
void USB_Initialize(void){
}

#endif //USB_NONE
