#include "UARTStream.h"
#include <ioavr.h>

#define NULL        ((void*) 0)

#define DATA_REGISTER_EMPTY (1 << 5)
#define RX_COMPLETE (1 << 7)
#define FRAMING_ERROR (1 << 4)
#define PARITY_ERROR (1 << 2)
#define DATA_OVERRUN (1 << 3)


UARTStream Serial;

static IStream uartInput;
static OStream uartOutput;

static uint8_t uartRxBuff[UART_RX_BUFF_SIZE];
static uint8_t uartTxBuff[UART_TX_BUFF_SIZE];

void UARTStream_transmit(OStream* stream, uint8_t* buff, Stream_LenType len);

void UARTStream_init(void) {
    // USART initialization
    // Communication Parameters: 8 Data, 1 Stop, No Parity
    // USART Receiver: On
    // USART Transmitter: On
    // USART Mode: Asynchronous
    // USART Baud Rate: 9600
    UCSRA = 0x00;
    UCSRB = (1<<7) | (1<<6) | (0<<5) | (1<<4) | (1<<3) | (0<<2) | (0<<1) | (0<<0);
    UCSRC = (1<<7) | (0<<6) | (0<<5) | (0<<4) | (0<<3) | (1<<2) | (1<<1) | (0<<0);
    UBRRH = 0x00;
    UBRRL = 0x33;
    
    // init streams         
    Serial.Input = &uartInput;
    Serial.Output = &uartOutput;
    IStream_init(Serial.Input, NULL, uartRxBuff, sizeof(uartRxBuff));
    OStream_init(Serial.Output, UARTStream_transmit, uartTxBuff, sizeof(uartTxBuff));    
}

void UARTStream_transmit(OStream* stream, uint8_t* buff, Stream_LenType len) {
    UDR = *buff;
}

#pragma vector = USART_RXC_vect
__interrupt void usart_rx_isr(void)
{
    char status;
    status = UCSRA;
    if ((status & (FRAMING_ERROR | PARITY_ERROR | DATA_OVERRUN))==0)
    {            
        IStream_receiveByte(Serial.Input, UDR);
    }
}


#pragma vector = USART_TXC_vect
__interrupt void usart_tx_isr(void)
{
    OStream_handle(Serial.Output, 1);
}
