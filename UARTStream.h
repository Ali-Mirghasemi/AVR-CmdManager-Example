#ifndef _UART_STREAM_H_
#define _UART_STREAM_H_

#include "InputStream.h"
#include "OutputStream.h"

#define UART_RX_BUFF_SIZE       64
#define UART_TX_BUFF_SIZE       64

typedef struct {
    IStream*     Input;
    OStream*     Output;
} UARTStream;

extern UARTStream Serial;

void UARTStream_init(void);

#endif /* _UART_STREAM_H_ */
