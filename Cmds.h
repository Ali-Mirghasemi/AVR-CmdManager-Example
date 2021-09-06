#ifndef _CMDS_H_
#define _CMDS_H_

#include "CmdManager.h"

// "LED"
extern const Cmd CMD_TEST;
// "LED"
extern const Cmd CMD_LED;


static Cmd_Array CMDS[] = {
    (Cmd*) &CMD_LED,
    (Cmd*) &CMD_TEST,
};
static const Cmd_LenType CMDS_LEN = CMD_ARR_LEN(CMDS);

void Led_init(void);


#endif /* _CMDS_H_ */ 
