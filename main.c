#include <io.h>

#include "InputStream.h"
#include "OutputStream.h"
#include "CmdManager.h"
#include "UARTStream.h"

// Declare your global variables here

// Standard Input/Output functions
#include <stdio.h>

void main(void)
{
    const uint8_t CRLF[2] = {'\r', '\n'};

    // Global enable interrupts
    UARTStream_init();
    
    #asm("sei")                     
    
    OStream_writeBytes(Serial.Output, "Hello", 5);
    OStream_writeBytes(Serial.Output, " World\r\n", 8);
    OStream_flush(Serial.Output);

    while (1)
    {
        if (IStream_available(Serial.Input) > 0) {  
            Stream_LenType len = IStream_findPattern(Serial.Input, CRLF, sizeof(CRLF));
            if (len >= 0) {
                len += sizeof(CRLF);
                OStream_writeStream(Serial.Output, Serial.Input, len);
                OStream_flush(Serial.Output);
            }
        }        

    }
}
