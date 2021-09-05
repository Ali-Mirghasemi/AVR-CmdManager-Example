#include "UARTStream.h"
#include <io.h>

#define DATA_REGISTER_EMPTY (1<<UDRE)
#define RX_COMPLETE (1<<RXC)
#define FRAMING_ERROR (1<<FE)
#define PARITY_ERROR (1<<UPE)
#define DATA_OVERRUN (1<<DOR)


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
    UCSRA=(0<<RXC) | (0<<TXC) | (0<<UDRE) | (0<<FE) | (0<<DOR) | (0<<UPE) | (0<<U2X) | (0<<MPCM);
    UCSRB=(1<<RXCIE) | (1<<TXCIE) | (0<<UDRIE) | (1<<RXEN) | (1<<TXEN) | (0<<UCSZ2) | (0<<RXB8) | (0<<TXB8);
    UCSRC=(1<<URSEL) | (0<<UMSEL) | (0<<UPM1) | (0<<UPM0) | (0<<USBS) | (1<<UCSZ1) | (1<<UCSZ0) | (0<<UCPOL);
    UBRRH=0x00;
    UBRRL=0x33;
    
    // init streams         
    Serial.Input = &uartInput;
    Serial.Output = &uartOutput;
    IStream_init(Serial.Input, NULL, uartRxBuff, sizeof(uartRxBuff));
    OStream_init(Serial.Output, UARTStream_transmit, uartTxBuff, sizeof(uartTxBuff));    
}

void UARTStream_transmit(OStream* stream, uint8_t* buff, Stream_LenType len) {
    UDR = *buff;
}

interrupt [USART_RXC] void usart_rx_isr(void)
{
    char status;
    status = UCSRA;
    if ((status & (FRAMING_ERROR | PARITY_ERROR | DATA_OVERRUN))==0)
    {            
        IStream_receiveByte(Serial.Input, UDR);
    }
}

interrupt [USART_TXC] void usart_tx_isr(void)
{
    OStream_handle(Serial.Output, 1);
}
