#include <ioavr.h>

#include "InputStream.h"
#include "OutputStream.h"
#include "CmdManager.h"
#include "UARTStream.h"
#include "Cmds.h"

static const char TITLE_CRLF[2]              = "\r\n";
static const char TITLE_WELCOME[]            = "AVR CmdManger";
static const char TITLE_AVAILABLE_CMDS[]     = "Available Cmds";
static const char TITLE_NOT_FOUND[]          = "NotFound:";

void printCmds(Cmd_Array* cmds, Cmd_LenType len);
void printInfo(void);

void Manager_onNotFound(CmdManager* manager, char* str);
void Manager_onOverflow(CmdManager* manager);

void main(void)
{
    CmdManager manager;
    DDRC = 0xFF;
    // Global enable interrupts
    UARTStream_init();   
    Led_init();
    // enable global interrupt
    SREG_I = 1;
                              
    printInfo();
        
    CmdManager_init(&manager, CMDS, CMDS_LEN);  
    CmdManager_onNotFound(&manager, Manager_onNotFound);
    CmdManager_onOverflow(&manager, Manager_onOverflow);
    
    while (1)
    {
        CmdManager_handle(&manager, Serial.Input); 
    }
}

void Manager_onNotFound(CmdManager* manager, char* str) {
    OStream_writeBytes(Serial.Output, (uint8_t*) TITLE_NOT_FOUND, sizeof(TITLE_NOT_FOUND) - 1);
    OStream_writeBytes(Serial.Output, (uint8_t*) str, strlen(str));
}
void Manager_onOverflow(CmdManager* manager) {
  
}
    
void printCmds(Cmd_Array* cmds, Cmd_LenType len) {
    char temp[5];
    Str_LenType strLen;
    
    while (len-- > 0) {                     
        OStream_writeChar(Serial.Output, '{');
        OStream_writeBytes(Serial.Output, (uint8_t*) (*cmds)->CmdName.Name, (*cmds)->CmdName.Len);
        OStream_writeChar(Serial.Output, ',');
        OStream_flush(Serial.Output);
        strLen = Str_parseNum((*cmds)->CmdName.Len, Str_Decimal, STR_NORMAL_LEN, temp);
        OStream_writeBytes(Serial.Output, (uint8_t*) temp, strLen);
        OStream_writeChar(Serial.Output, ',');
        OStream_flush(Serial.Output);
        strLen = Str_parseNum((*cmds)->Types.Flags, Str_Binary, 5, temp);
        OStream_writeBytes(Serial.Output, (uint8_t*) temp, strLen);        
        OStream_writeChar(Serial.Output, '}');
        OStream_writeBytes(Serial.Output, (uint8_t*) TITLE_CRLF, sizeof(TITLE_CRLF));
        OStream_flush(Serial.Output);
        cmds++;     
    }
}
void printInfo(void) {
    OStream_writeBytes(Serial.Output, (uint8_t*) TITLE_WELCOME, sizeof(TITLE_WELCOME) - 1);
    OStream_writeBytes(Serial.Output, (uint8_t*) TITLE_CRLF, sizeof(TITLE_CRLF));
    OStream_flush(Serial.Output);   
    OStream_writeBytes(Serial.Output, (uint8_t*) TITLE_AVAILABLE_CMDS, sizeof(TITLE_AVAILABLE_CMDS) - 1);
    OStream_writeBytes(Serial.Output, (uint8_t*) TITLE_CRLF, sizeof(TITLE_CRLF));
    OStream_flush(Serial.Output);
    printCmds(CMDS, CMDS_LEN);
}
