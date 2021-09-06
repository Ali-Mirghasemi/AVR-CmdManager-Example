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

static int8_t  Led_getLed(Cmd_Cursor* cursor);
static uint8_t Led_getState(Cmd_Cursor* cursor);

static Cmd_Handled Led_onSet(CmdManager* manager, Cmd* cmd, Cmd_Cursor* cursor, Cmd_Type type);
static Cmd_Handled Led_onGet(CmdManager* manager, Cmd* cmd, Cmd_Cursor* cursor, Cmd_Type type);
static Cmd_Handled Led_onHelp(CmdManager* manager, Cmd* cmd, Cmd_Cursor* cursor, Cmd_Type type);

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

static int8_t  Led_getLed(Cmd_Cursor* cursor) {
    Cmd_Param cmdParam;
    if (CmdManager_nextParam(cursor, &cmdParam) && cmdParam.ValueType == Cmd_ValueType_Number) {
        if (cmdParam.Value.Number < LED_NUM && cmdParam.Value.Number >= 0) {
            return (int8_t) cmdParam.Value.Number; 
        }        
    }  
    return -1;
}

static uint8_t Led_getState(Cmd_Cursor* cursor) {
    Cmd_Param cmdParam; 
    if (CmdManager_nextParam(cursor, &cmdParam)) {
        switch (cmdParam.ValueType) {
            case Cmd_ValueType_Number:
                if (cmdParam.Value.Number >= 0 && cmdParam.Value.Number <= 1) {
                    return cmdParam.Value.Number; 
                }
                break;
            case Cmd_ValueType_State:
                return cmdParam.Value.State;
            case Cmd_ValueType_StateKey:
                return cmdParam.Value.StateKey;
        }
    }     
                                         
    return -1;
}

static Cmd_Handled Led_onSet(CmdManager* manager, Cmd* cmd, Cmd_Cursor* cursor, Cmd_Type type) {                       
    int8_t led;
    int8_t state;      
    int8_t result = 0;                                  
                
    // get led
    if ((led = Led_getLed(cursor)) >= 0) {
        PORTC_Bit4 = 1;
        if ((state = Led_getState(cursor)) >= 0) {
            PORTC_Bit5 = 1;
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
            }                        
            result = 1;
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
static Cmd_Handled Led_onGet(CmdManager* manager, Cmd* cmd, Cmd_Cursor* cursor, Cmd_Type type) {
    PORTC_Bit6 = 1;
    return Cmd_Done;
}
static Cmd_Handled Led_onHelp(CmdManager* manager, Cmd* cmd, Cmd_Cursor* cursor, Cmd_Type type) {
    PORTC_Bit7 = 1;
    return Cmd_Done;
}
