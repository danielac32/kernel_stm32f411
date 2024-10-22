#include <interrupt.h>
#include <usb_cdc_conf.h>
#include <gpio.h>
/* Device descriptor */
static const struct usb_device_descriptor device_desc = {
    .bLength            = sizeof(struct usb_device_descriptor),
    .bDescriptorType    = USB_DTYPE_DEVICE,
    .bcdUSB             = VERSION_BCD(2,0,0),
    .bDeviceClass       = USB_CLASS_IAD,
    .bDeviceSubClass    = USB_SUBCLASS_IAD,
    .bDeviceProtocol    = USB_PROTO_IAD,
    .bMaxPacketSize0    = CDC_EP0_SIZE,
    .idVendor           = 0x0483,
    .idProduct          = 0x5740,
    .bcdDevice          = VERSION_BCD(1,0,0),
    .iManufacturer      = 1,
    .iProduct           = 2,
    .iSerialNumber      = INTSERIALNO_DESCRIPTOR,
    .bNumConfigurations = 1,
};

/* Device configuration descriptor */
static const struct cdc_config config_desc = {
    .config = {
        .bLength                = sizeof(struct usb_config_descriptor),
        .bDescriptorType        = USB_DTYPE_CONFIGURATION,
        .wTotalLength           = sizeof(struct cdc_config),
        .bNumInterfaces         = 2,
        .bConfigurationValue    = 1,
        .iConfiguration         = NO_DESCRIPTOR,
        .bmAttributes           = USB_CFG_ATTR_RESERVED | USB_CFG_ATTR_SELFPOWERED,
        .bMaxPower              = USB_CFG_POWER_MA(100),
    },
    .comm_iad = {
        .bLength = sizeof(struct usb_iad_descriptor),
        .bDescriptorType        = USB_DTYPE_INTERFASEASSOC,
        .bFirstInterface        = 0,
        .bInterfaceCount        = 2,
        .bFunctionClass         = USB_CLASS_CDC,
        .bFunctionSubClass      = USB_CDC_SUBCLASS_ACM,
        .bFunctionProtocol      = CDC_PROTOCOL,
        .iFunction              = NO_DESCRIPTOR,
    },
    .comm = {
        .bLength                = sizeof(struct usb_interface_descriptor),
        .bDescriptorType        = USB_DTYPE_INTERFACE,
        .bInterfaceNumber       = 0,
        .bAlternateSetting      = 0,
        .bNumEndpoints          = 1,
        .bInterfaceClass        = USB_CLASS_CDC,
        .bInterfaceSubClass     = USB_CDC_SUBCLASS_ACM,
        .bInterfaceProtocol     = CDC_PROTOCOL,
        .iInterface             = NO_DESCRIPTOR,
    },
    .cdc_hdr = {
        .bFunctionLength        = sizeof(struct usb_cdc_header_desc),
        .bDescriptorType        = USB_DTYPE_CS_INTERFACE,
        .bDescriptorSubType     = USB_DTYPE_CDC_HEADER,
        .bcdCDC                 = VERSION_BCD(1,1,0),
    },
    .cdc_mgmt = {
        .bFunctionLength        = sizeof(struct usb_cdc_call_mgmt_desc),
        .bDescriptorType        = USB_DTYPE_CS_INTERFACE,
        .bDescriptorSubType     = USB_DTYPE_CDC_CALL_MANAGEMENT,
        .bmCapabilities         = 0,
        .bDataInterface         = 1,

    },
    .cdc_acm = {
        .bFunctionLength        = sizeof(struct usb_cdc_acm_desc),
        .bDescriptorType        = USB_DTYPE_CS_INTERFACE,
        .bDescriptorSubType     = USB_DTYPE_CDC_ACM,
        .bmCapabilities         = 0,
    },
    .cdc_union = {
        .bFunctionLength        = sizeof(struct usb_cdc_union_desc),
        .bDescriptorType        = USB_DTYPE_CS_INTERFACE,
        .bDescriptorSubType     = USB_DTYPE_CDC_UNION,
        .bMasterInterface0      = 0,
        .bSlaveInterface0       = 1,
    },
    .comm_ep = {
        .bLength                = sizeof(struct usb_endpoint_descriptor),
        .bDescriptorType        = USB_DTYPE_ENDPOINT,
        .bEndpointAddress       = CDC_NTF_EP,
        .bmAttributes           = USB_EPTYPE_INTERRUPT,
        .wMaxPacketSize         = CDC_NTF_SZ,
        .bInterval              = 0xFF,
    },
    .data = {
        .bLength                = sizeof(struct usb_interface_descriptor),
        .bDescriptorType        = USB_DTYPE_INTERFACE,
        .bInterfaceNumber       = 1,
        .bAlternateSetting      = 0,
        .bNumEndpoints          = 2,
        .bInterfaceClass        = USB_CLASS_CDC_DATA,
        .bInterfaceSubClass     = USB_SUBCLASS_NONE,
        .bInterfaceProtocol     = USB_PROTO_NONE,
        .iInterface             = NO_DESCRIPTOR,
    },
    .data_eprx = {
        .bLength                = sizeof(struct usb_endpoint_descriptor),
        .bDescriptorType        = USB_DTYPE_ENDPOINT,
        .bEndpointAddress       = CDC_RXD_EP,
        .bmAttributes           = USB_EPTYPE_BULK,
        .wMaxPacketSize         = CDC_DATA_SZ,
        .bInterval              = 0x01,
    },
    .data_eptx = {
        .bLength                = sizeof(struct usb_endpoint_descriptor),
        .bDescriptorType        = USB_DTYPE_ENDPOINT,
        .bEndpointAddress       = CDC_TXD_EP,
        .bmAttributes           = USB_EPTYPE_BULK,
        .wMaxPacketSize         = CDC_DATA_SZ,
        .bInterval              = 0x01,
    },

};

static const struct usb_string_descriptor lang_desc     = USB_ARRAY_DESC(USB_LANGID_ENG_US);
static const struct usb_string_descriptor manuf_desc_en = USB_STRING_DESC("Open source USB stack for STM32");
static const struct usb_string_descriptor prod_desc_en  = USB_STRING_DESC("CDC Loopback demo");
static const struct usb_string_descriptor *const dtable[] = {
    &lang_desc,
    &manuf_desc_en,
    &prod_desc_en,
};

usbd_device udev;
uint32_t    ubuf[0x20];
uint8_t     fifo[0x200];
uint32_t    fpos=0;

static struct usb_cdc_line_coding cdc_line = {
    .dwDTERate          = 38400,
    .bCharFormat        = USB_CDC_1_STOP_BITS,
    .bParityType        = USB_CDC_NO_PARITY,
    .bDataBits          = 8,
};

static usbd_respond cdc_getdesc (usbd_ctlreq *req, void **address, uint16_t *length) {
    const uint8_t dtype = req->wValue >> 8;
    const uint8_t dnumber = req->wValue & 0xFF;
    const void* desc;
    uint16_t len = 0;
    switch (dtype) {
    case USB_DTYPE_DEVICE:
        desc = &device_desc;
        break;
    case USB_DTYPE_CONFIGURATION:
        desc = &config_desc;
        len = sizeof(config_desc);
        break;
    case USB_DTYPE_STRING:
        if (dnumber < 3) {
            desc = dtable[dnumber];
        } else {
            return usbd_fail;
        }
        break;
    default:
        return usbd_fail;
    }
    if (len == 0) {
        len = ((struct usb_header_descriptor*)desc)->bLength;
    }
    *address = (void*)desc;
    *length = len;
    return usbd_ack;
}


static usbd_respond cdc_control(usbd_device *dev, usbd_ctlreq *req, usbd_rqc_callback *callback) {
    if (((USB_REQ_RECIPIENT | USB_REQ_TYPE) & req->bmRequestType) == (USB_REQ_INTERFACE | USB_REQ_CLASS)
        && req->wIndex == 0 ) {
        switch (req->bRequest) {
        case USB_CDC_SET_CONTROL_LINE_STATE:
            return usbd_ack;
        case USB_CDC_SET_LINE_CODING:
            memcpy(&cdc_line, req->data, sizeof(cdc_line));
            return usbd_ack;
        case USB_CDC_GET_LINE_CODING:
            dev->status.data_ptr = &cdc_line;
            dev->status.data_count = sizeof(cdc_line);
            return usbd_ack;
        default:
            return usbd_fail;
        }
    }
    return usbd_fail;
}


static void cdc_rxonly (usbd_device *dev, uint8_t event, uint8_t ep) {
   usbd_ep_read(dev, ep, fifo, CDC_DATA_SZ);
}

static void cdc_txonly(usbd_device *dev, uint8_t event, uint8_t ep) {
    uint8_t _t = dev->driver->frame_no();
    memset(fifo, _t, CDC_DATA_SZ);
    usbd_ep_write(dev, ep, fifo, CDC_DATA_SZ);
}

static void cdc_rxtx(usbd_device *dev, uint8_t event, uint8_t ep) {
    if (event == usbd_evt_eptx) {
        cdc_txonly(dev, event, ep);
    } else {
        cdc_rxonly(dev, event, ep);
    }
}


int len;
 

uint8_t UART_CDC_RxBuf[UART_CDC_RX0_BUFFER_SIZE];

uint8_t UART_CDC_TxHead;
uint8_t UART_CDC_TxTail;
uint8_t UART_CDC_RxHead;
uint8_t UART_CDC_RxTail;
uint8_t UART_CDC_LastRxError;


uint16_t usb_available(void){
    uint16_t ret;
    ret = (UART_CDC_RX0_BUFFER_SIZE + UART_CDC_RxHead - UART_CDC_RxTail) & UART_CDC_RX0_BUFFER_MASK;
    return ret;
}


uint16_t usb_getc(void){
  uint16_t tmptail;
  uint8_t data;
 
    if (UART_CDC_RxHead == UART_CDC_RxTail) {
      return UART_CDC_NO_DATA;   // no data available 
    }
  // calculate / store buffer index 
    tmptail = (UART_CDC_RxTail + 1) & UART_CDC_RX0_BUFFER_MASK;

    UART_CDC_RxTail = tmptail;

 // get data from receive buffer 
    data = UART_CDC_RxBuf[tmptail];

    return (UART_CDC_LastRxError << 8) + data;
} 


uint8_t readBytes(char *buffer,uint8_t length){
  uint8_t count = 0;
  while (count < length) {
    int c = usb_getc();
    if (c < 0) break;
    *buffer++ = (char)c;
    count++;
    delay(5);
  }
  return count;
}


extern  void    ttyhandler(uint32, char c, int tipo);

/* CDC loop callback. Both for the Data IN and Data OUT endpoint */
static void cdc_loopback(usbd_device *dev, uint8_t event, uint8_t ep) {
    

    uint16_t tmphead;
    uint8_t data;
   
    uint8_t lastRxError;

    #if 0
    
    len=usbd_ep_read(dev, CDC_RXD_EP, &fifo, CDC_DATA_SZ);
    
    if(len>0){
        hw_toggle_pin(GPIOx(GPIO_C),13);
    }//kprintf("interrupt: %s\n",fifo);

    //uint32 q=disable(); 
    //for (int i = 0; i < len; ++i){
        //ttyhandler (1, fifo[i], 0);
    //}
    //restore(q); 
    
    #else
    uint32 q=disable();
    len=usbd_ep_read(dev, CDC_RXD_EP, &fifo, CDC_DATA_SZ);

    for (int i = 0; i < len; ++i)
    {
        tmphead = (UART_CDC_RxHead + 1) & UART_CDC_RX0_BUFFER_MASK;

        if (tmphead == UART_CDC_RxTail) {
            //error: receive buffer overflow 
            lastRxError = UART_CDC_BUFFER_OVERFLOW >> 8;
        } else {
            // store new index 
            UART_CDC_RxHead = tmphead;
            // store received data in buffer 
            UART_CDC_RxBuf[tmphead] = fifo[i];
            //memcpy(UART_CDC_RxBuf,&fifo[0],len);
        }
        UART_CDC_LastRxError = lastRxError;
        //hw_toggle_pin(GPIOx(GPIO_C),13);
    }

    
    
    restore(q); 
    #endif
    

     
}
 

static usbd_respond cdc_setconf (usbd_device *dev, uint8_t cfg) {
    switch (cfg) {
    case 0:
        /* deconfiguring device */
        usbd_ep_deconfig(dev, CDC_NTF_EP);
        usbd_ep_deconfig(dev, CDC_TXD_EP);
        usbd_ep_deconfig(dev, CDC_RXD_EP);
        usbd_reg_endpoint(dev, CDC_RXD_EP, 0);
        usbd_reg_endpoint(dev, CDC_TXD_EP, 0);
        return usbd_ack;
    case 1:
        /* configuring device */
        usbd_ep_config(dev, CDC_RXD_EP, USB_EPTYPE_BULK /*| USB_EPTYPE_DBLBUF*/, CDC_DATA_SZ);
        usbd_ep_config(dev, CDC_TXD_EP, USB_EPTYPE_BULK /*| USB_EPTYPE_DBLBUF*/, CDC_DATA_SZ);
        usbd_ep_config(dev, CDC_NTF_EP, USB_EPTYPE_INTERRUPT, CDC_NTF_SZ);

        usbd_reg_endpoint(dev, CDC_RXD_EP, cdc_loopback);
        //usbd_reg_endpoint(dev, CDC_TXD_EP, getUSBdata);
        //usbd_reg_endpoint(dev, CDC_TXD_EP, cdc_loopback);

//        usbd_ep_write(dev, CDC_TXD_EP, 0, 0);
        return usbd_ack;
    default:
        return usbd_fail;
    }
}

void cdc_init_usbd(void) {
    usbd_init(&udev, &usbd_hw, CDC_EP0_SIZE, ubuf, sizeof(ubuf));
    usbd_reg_config(&udev, cdc_setconf);
    usbd_reg_control(&udev, cdc_control);
    usbd_reg_descr(&udev, cdc_getdesc);
    UART_CDC_TxHead = 0;
    UART_CDC_TxTail = 0;
    UART_CDC_RxHead = 0;
    UART_CDC_RxTail = 0;
}

extern  syscall yield(void);
/*int usbTask(){
    while(1){
       if(usb_available()){
             printf("->%c\n",usb_getc() );
            //kputc(usb_getc());
            //uint32 q = disable();
            //ttyhandler(1,usb_getc(),0);
            //restore(q);
        }
        //printf("usbTask\n");
        //yield();
    }
    return 0;
}
*/

void OTG_FS_Handler(void) {
   //asm volatile("cpsid i");
   usbd_poll(&udev);
   //asm volatile("cpsie i");
}