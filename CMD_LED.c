#include "CmdManager.h"
#include "UARTStream.h"
#include <ioavr.h>

#define LED_NUM     3

#define LED_0       PORTC_Bit0
#define LED_0_DDR   DDRC_Bit0
#define LED_1       PORTC_Bit1
#define LED_1_DDR   DDRC_Bit1
#define LED_2       PORTC_Bit2
#define LED_2_DDR   DDRC_Bit2

static const char CMD_LED_NAME[] = "led";  

static int8_t  Led_getLed(Param_Cursor* cursor);
static uint8_t Led_getState(Param_Cursor* cursor);

static void printLedState(uint8_t led, uint8_t state);

static Cmd_Handled Led_onSet(CmdManager* manager, Cmd* cmd, Param_Cursor* cursor, Cmd_Type type);
static Cmd_Handled Led_onGet(CmdManager* manager, Cmd* cmd, Param_Cursor* cursor, Cmd_Type type);
static Cmd_Handled Led_onHelp(CmdManager* manager, Cmd* cmd, Param_Cursor* cursor, Cmd_Type type);

//const Cmd CMD_LED = CMD_INIT(CMD_LED_NAME, Cmd_Type_Set | Cmd_Type_Get | Cmd_Type_Help, NULL, Led_onSet, Led_onGet, Led_onHelp, NULL;

const Cmd CMD_LED = {
    {NULL, Led_onSet, Led_onGet, Led_onHelp, NULL},
    {CMD_LED_NAME, sizeof(CMD_LED_NAME) - 1},
    (Cmd_Type_Set | Cmd_Type_Get | Cmd_Type_Help),        
};

void Led_init(void) {
    LED_0_DDR = 1;
    LED_1_DDR = 1;
    LED_2_DDR = 1;
}

extern void printCmdParam(Param* param);

static int8_t  Led_getLed(Param_Cursor* cursor) {
    Param cmdParam;
    if (CmdManager_nextParam(cursor, &cmdParam)) {
      if (cmdParam.ValueType == Param_ValueType_Number) {
        return (uint8_t) cmdParam.Value.Number & 0x07;
      }
    }  
    return -1;
}

static uint8_t Led_getState(Param_Cursor* cursor) {
    Param cmdParam; 
    if (CmdManager_nextParam(cursor, &cmdParam)) {
        switch (cmdParam.ValueType) {
            case Param_ValueType_Number:
                return (uint8_t) cmdParam.Value.Number & 0x01;
            case Param_ValueType_State:
                return cmdParam.Value.State;
            case Param_ValueType_StateKey:
                return cmdParam.Value.StateKey;
            case Param_ValueType_Boolean:
                return cmdParam.Value.Boolean;
        }
    }     
                                         
    return -1;
}

static Cmd_Handled Led_onSet(CmdManager* manager, Cmd* cmd, Param_Cursor* cursor, Cmd_Type type) {                       
    int8_t led;
    int8_t state;      
    int8_t result = 0;                                  
                
    // get led
    if ((led = Led_getLed(cursor)) >= 0) {
        if ((state = Led_getState(cursor)) >= 0) {
            result = 1;
            switch (led) {
                case 0:
                    LED_0 = state;
                    break;
                case 1:
                    LED_1 = state;
                    break;
                case 2:  
                    LED_2 = state;
                    break;
                default:
                    result = 0;
                    break;
            }                        
        }        
    }   
                                 
    if (result) {
        OStream_writeBytes(Serial.Output, "\r\nOK\r\n", 6);
        OStream_flush(Serial.Output);
    }   
    else {
        OStream_writeBytes(Serial.Output, "\r\nERROR\r\n", 9);
        OStream_flush(Serial.Output);
    }
    
    return Cmd_Done;
}
static Cmd_Handled Led_onGet(CmdManager* manager, Cmd* cmd, Param_Cursor* cursor, Cmd_Type type) {
    printLedState(0, LED_0);
    printLedState(1, LED_1);
    printLedState(2, LED_2);
    return Cmd_Done;
}
static Cmd_Handled Led_onHelp(CmdManager* manager, Cmd* cmd, Param_Cursor* cursor, Cmd_Type type) {
    static const char TXT[] = "\r\n+LED: <led>,<state>\r\n\r\nOK\r\n";
    OStream_writeBytes(Serial.Output, (uint8_t*) TXT, sizeof(TXT) - 1);
    OStream_flush(Serial.Output);
    return Cmd_Done;
}

static void printLedState(uint8_t led, uint8_t state) {
  static const char TXT[] = "\r\n+LED: ";
  char temp[4];
  Str_LenType len;
  OStream_writeBytes(Serial.Output, (uint8_t*) TXT, sizeof(TXT) - 1);
  OStream_flush(Serial.Output);
  len = Str_parseUNum(led, Str_Decimal, STR_NORMAL_LEN, temp);
  OStream_writeBytes(Serial.Output, (uint8_t*) temp, len);
  OStream_flush(Serial.Output);
  OStream_writeChar(Serial.Output, ',');
  OStream_writeBytes(Serial.Output, (uint8_t*) (state ? "ON" : "OFF"), state ? 2 : 3);
  OStream_writeBytes(Serial.Output, (uint8_t*) "\r\n", 2);
  OStream_flush(Serial.Output);
}
