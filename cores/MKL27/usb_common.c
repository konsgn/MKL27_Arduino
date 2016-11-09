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




// Initialize/enable an endpoint
static void usb_init_ep(uint8_t num, int buflen, uint8_t *rx_buf0, uint8_t *rx_buf1)
{
    endpoint_t *ep = &endpoints[num];

    ep->num = num;
    ep_clear_tx(ep, 1);
    
    // Configure BDT entries for receive
    USB_BD *bdptr = bdt_rx(num);
    bdtptr[0].addr = rx_buf0;
    bdtptr[1].addr = rx_buf1;
    bdtptr[0].count = bdtptr[1].count = buflen;
    bdtptr[0].stat._byte = bdtptr[1].stat._byte = _OWN;
    
    // Enable endpoint for transmit and receive
    USB0_ENDPT(num) = USB_ENDPT_EPTXEN_MASK | USB_ENDPT_EPRXEN_MASK 
                        | USB_ENDPT_EPHSHK_MASK;
}


static inline void endpoint_handler0(USB_BD *bd_ptr){
	uint8_t PID = (bd_ptr->BDReadByte)>>2 & 0x0F;
	//   Setup_packet *setup = malloc(sizeof(Setup_packet)); //should exist
	switch (PID){
		case PID_SETUP_TOKEN:
			if ((setup->bmRequestType)&SETUP_TYPE_MASK==SETUP_TYPE_STANDARD){
				
			}
			else if(Implementation_Setup_Handler)(*Implementation_Setup_Handler);
			
			break;
		case PID_OUT_TOKEN: 
			//Shouldn't be sending data to endpoint 0...right?
			break;
		case PID_IN_TOKEN:
			break;
		default:
			break;
	}
	
}

static void usb_packet_handler(uint8_t stat)
{
    uint8_t statinfo = stat >> 2;
    USB_BD *bd_ptr = &bdt[statinfo]; 
    uint8_t endpoint = ((stat>>4)&0x0F);
    
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
	return;
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
	
	USB_SetHandler(usb_int_handle);
	NVIC_SET_PRIORITY(USB0_IRQn, 1);
	NVIC_EnableIRQ(USB0_IRQn);
}


void usb_int_handle(void){
	uint8_t status,error,it;
	
	//Pull in status register state at start of interrupt
	status = USB0->ISTAT; 
	
	if(status & USB_ISTAT_USBRST_MASK) {         
		// Reset state seen, address was set to 0, ..maybe? doc is conflicting
		//following KQRUG.pdf
		for(it=0;it<16;it++){
			//disable each Endpoint
			USB0->ENDPOINT[it] = 0x00;
		}
		
		//reset all ping pongs to 0 (even)
		USB0->CTL |= USB_CTL_ODDRST_MASK;

		// Configure endpoint 0 (the control endpoint)
		usb_init_ep(0, EP0_BUFSIZE, ep0_rx_buffers[0], ep0_rx_buffers[1]);

		// Clear flags
		USB0_ERRSTAT = 0xFF;
		USB0_ISTAT = 0xFF;

		// Enable all error sources
		USB0_ERREN = 0x7E;

		// Enable USB interrupts
		USB0_INTEN = USB_INTEN_TOKDNEEN_MASK | USB_INTEN_ERROREN_MASK 
						| USB_INTEN_USBRSTEN_MASK | USB_INTEN_STALLEN_MASK;
        return;
    }
    // Process any pending token done interrupts (may be queued)
    else if (status & USB_ISTAT_TOKDNE_MASK) {
    //   while(status & USB_ISTAT_TOKDNE_MASK) {
        usb_packet_handler(status);
        USB0->ISTAT = USB_ISTAT_TOKDNE_MASK;
        //   status = USB0_ISTAT;
    }
    else if (status & USB_ISTAT_STALL_MASK) {
		//whaaaat? 
		USB0->ENDPT0 = USB_ENDPT_EPRXEN | USB_ENDPT_EPTXEN | USB_ENDPT_EPHSHK;
		USB0->ISTAT = USB_ISTAT_STALL;
	}
    else if (status & USB_ISTAT_ERROR_MASK) {
		//   uint8_t err = USB0_ERRSTAT;
		//   USB0_ERRSTAT = err;
		//   //serial_print("err:");
		//   //serial_phex(err);
		//   //serial_print("\n");
		USB0_ISTAT = USB_ISTAT_ERROR;
	}
    
}


#else
void USB_Initialize(void){
}

#endif
