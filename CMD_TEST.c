#include "CmdManager.h"
#include "UARTStream.h"
#include "Str.h"

static const char CMD_TEST_NAME[] = "test";

static void printCmdParam(Param* param);
static void parseAllParams(Param_Cursor* cursor);

static Cmd_Handled Test_onExe(CmdManager* manager, Cmd* cmd, Param_Cursor* cursor, Cmd_Type type);
static Cmd_Handled Test_onSet(CmdManager* manager, Cmd* cmd, Param_Cursor* cursor, Cmd_Type type);
static Cmd_Handled Test_onGet(CmdManager* manager, Cmd* cmd, Param_Cursor* cursor, Cmd_Type type);
static Cmd_Handled Test_onHelp(CmdManager* manager, Cmd* cmd, Param_Cursor* cursor, Cmd_Type type);
static Cmd_Handled Test_onResponse(CmdManager* manager, Cmd* cmd, Param_Cursor* cursor, Cmd_Type type);


//const Cmd CMD_TEST = CMD_INIT("test", Cmd_Type_Any, Test_onExe, Test_onSet, Test_onGet, Test_onHelp, Test_onResponse);

const Cmd CMD_TEST = {
    {Test_onExe, Test_onSet, Test_onGet, Test_onHelp, Test_onResponse},
    {CMD_TEST_NAME, sizeof(CMD_TEST_NAME) - 1},
    Cmd_Type_Any,        
};

Cmd_Handled Test_onExe(CmdManager* manager, Cmd* cmd, Param_Cursor* cursor, Cmd_Type type) {
    static const char TXT[] = "Test -> Execute\r\n";
    OStream_writeBytes(Serial.Output, (uint8_t*) TXT, sizeof(TXT) - 1);
    return Cmd_Done;
}
Cmd_Handled Test_onSet(CmdManager* manager, Cmd* cmd, Param_Cursor* cursor, Cmd_Type type) {
    static const char TXT[] = "Test -> Set\r\n";
    OStream_writeBytes(Serial.Output, (uint8_t*) TXT, sizeof(TXT) - 1);
    parseAllParams(cursor);
    return Cmd_Done;
}
Cmd_Handled Test_onGet(CmdManager* manager, Cmd* cmd, Param_Cursor* cursor, Cmd_Type type) {
    static const char TXT[] = "Test -> Get\r\n";
    OStream_writeBytes(Serial.Output, (uint8_t*) TXT, sizeof(TXT) - 1);
    return Cmd_Done;
}
Cmd_Handled Test_onHelp(CmdManager* manager, Cmd* cmd, Param_Cursor* cursor, Cmd_Type type) {
    static const char TXT[] = "Test -> Help\r\n";
    OStream_writeBytes(Serial.Output, (uint8_t*) TXT, sizeof(TXT) - 1);
    return Cmd_Done;
}
Cmd_Handled Test_onResponse(CmdManager* manager, Cmd* cmd, Param_Cursor* cursor, Cmd_Type type) {
    static const char TXT[] = "Test -> Response\r\n";
    OStream_writeBytes(Serial.Output, (uint8_t*) TXT, sizeof(TXT) - 1);
    return Cmd_Done;
}

static void parseAllParams(Param_Cursor* cursor) {
    Param param;
    while (CmdManager_nextParam(cursor, &param) != NULL) {
        printCmdParam(&param);
    }
}

static void printCmdParam(Param* param) {
    static const char* TYPES[] = {
        "Unknown",
        "Number",
        "NumberHex",
        "NumberBinary",
        "Float",
        "State",
        "StateKey",
        "Boolean",
        "String",
        "Null",
    };
    char temp[12];
    Str_LenType len;
    
    //printf("{%u, %s, ", param->Index, TYPES[(int) param->Value.Type]);
    OStream_writeChar(Serial.Output, '{');
    len = Str_parseNum(param->Index, Str_Decimal, STR_NORMAL_LEN, temp);
    OStream_writeBytes(Serial.Output, (uint8_t*) temp, len);
    OStream_flush(Serial.Output);
    OStream_writeBytes(Serial.Output, ", ", 2);
    OStream_writeBytes(Serial.Output, (uint8_t*) TYPES[(int) param->Value.Type], Str_len((char*) TYPES[(int) param->Value.Type]));
    OStream_flush(Serial.Output);
    OStream_writeBytes(Serial.Output, ", ", 2);
    switch (param->Value.Type) {
        case Param_ValueType_Unknown:
            //printf("\"%s\"", param->Value.Unknown);
            OStream_writeChar(Serial.Output, '"');
            OStream_writeBytes(Serial.Output, param->Value.Unknown, Str_len(param->Value.Unknown));
            OStream_writeChar(Serial.Output, '"');
            break;
        case Param_ValueType_Number:
            //printf("%d", param->Value.Number);
            len = Str_parseNum(param->Value.Number, Str_Decimal, STR_NORMAL_LEN, temp);
            OStream_writeBytes(Serial.Output, (uint8_t*) temp, len);
            break;
        case Param_ValueType_NumberHex:
            //printf("%X", param->Value.NumberHex);
            len = Str_parseUNum(param->Value.NumberHex, Str_Hex, STR_NORMAL_LEN, temp);
            OStream_writeBytes(Serial.Output, (uint8_t*) temp, len);
            break;
        case Param_ValueType_NumberBinary:
            //printf("%u", param->Value.NumberBinary);
            len = Str_parseUNum(param->Value.NumberBinary, Str_Binary, STR_NORMAL_LEN, temp);
            OStream_writeBytes(Serial.Output, (uint8_t*) temp, len);
            break;
        case Param_ValueType_Float:
            //printf("%g", param->Value.Float);
            len = Str_parseFloatFix(param->Value.Float, temp, 4);
            OStream_writeBytes(Serial.Output, (uint8_t*) temp, len);
            break;
        case Param_ValueType_State:
            //printf("%s", param->Value.State ? "HIGH" : "LOW");
            OStream_writeBytes(Serial.Output, param->Value.State ? "HIGH" : "LOW", 
                               param->Value.State ? 4 : 3);
            break;
        case Param_ValueType_StateKey:
            //printf("%s", param->Value.StateKey ? "ON" : "OFF");
            OStream_writeBytes(Serial.Output, param->Value.StateKey ? "ON" : "OFF", 
                               param->Value.StateKey ? 2 : 3);
            break;
        case Param_ValueType_Boolean:
            //printf("%s", param->Value.Boolean ? "true" : "false");
            OStream_writeBytes(Serial.Output, param->Value.Boolean ? "true" : "false", 
                               param->Value.StateKey ? 4 : 5);
            break;
        case Param_ValueType_String:
            //printf("\"%s\"", param->Value.String);
            OStream_writeChar(Serial.Output, '"');
            OStream_writeBytes(Serial.Output, (uint8_t*) param->Value.String, Str_len(param->Value.String));
            OStream_writeChar(Serial.Output, '"');
            break;
        case Param_ValueType_Null:
            //printf("\"%s\"", param->Value.Null);
            OStream_writeChar(Serial.Output, '"');
            OStream_writeBytes(Serial.Output, (uint8_t*) param->Value.Null, Str_len(param->Value.Null));
            OStream_writeChar(Serial.Output, '"');
            break;
    }
    //puts("}");
    OStream_writeBytes(Serial.Output, (uint8_t*) "}\r\n", 3);
    OStream_flush(Serial.Output);
}

