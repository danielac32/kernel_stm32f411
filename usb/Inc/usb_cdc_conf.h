#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <usb.h>
#include <stm32.h>
#include <usb_cdc.h>

extern usbd_device udev;
extern uint32_t	ubuf[];
extern uint8_t     fifo[];
extern uint32_t    fpos;
extern int len;

void cdc_init_usbd(void);

#define UART_CDC_RX0_BUFFER_SIZE 64

#define UART_CDC_RX0_BUFFER_MASK (UART_CDC_RX0_BUFFER_SIZE - 1)
#define UART_CDC_FRAME_ERROR      0x0800             
#define UART_CDC_OVERRUN_ERROR    0x0400            
#define UART_CDC_BUFFER_OVERFLOW  0x0200           
#define UART_CDC_NO_DATA          0x0100   

extern uint8_t UART_CDC_RxBuf[];
extern uint8_t UART_CDC_TxHead;
extern uint8_t UART_CDC_TxTail;
extern uint8_t UART_CDC_RxHead;
extern uint8_t UART_CDC_RxTail;
extern uint8_t UART_CDC_LastRxError;
extern uint16_t usb_available(void);
extern uint16_t usb_getc(void);
extern uint8_t readBytes(char *buffer,uint8_t length);



