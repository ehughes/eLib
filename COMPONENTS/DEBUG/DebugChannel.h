#include "stdint.h"

#ifndef _DEBUG_H
#define _DEBUG_H


#define DEBUG_RED           "\033[31;40m"
#define DEBUG_GREEN         "\033[32;40m"
#define DEBUG_YELLOW        "\033[33;40m"
#define DEBUG_BLUE          "\033[34;40m"
#define DEBUG_MAGENTA       "\033[35;40m"
#define DEBUG_CYAN          "\033[36;40m"
#define DEBUG_WHITE         "\033[37;40m"
#define DEBUG_COLOR_DEFAULT DEBUG_WHITE

extern uint32_t DebugChannelFlag;

extern const char *DebugChannelNames[32];
extern const char *DebugChannelColors[32];



#define DEBUG_CHANNEL_SYSTEM    0
#define DEBUG_CHANNEL_MESH      1
#define DEBUG_CHANNEL_4CH       2
#define DEBUG_CHANNEL_SNS       3
#define DEBUG_CHANNEL_CELL      4
#define DEBUG_CHANNEL_MQTT      5
#define DEBUG_CHANNEL_MQTT_NET  6
#define DEBUG_CHANNEL_BRIDGE    7
#define DEBUG_CHANNEL_DNS       8
#define DEBUG_CHANNEL_NTP       9



#define DEBUG_CHANNEL(DebugChannel,...)            if(DebugChannelFlag&(1<<DebugChannel))\
                                                   {\
                                                       SHELL_printf(&MySerialShell,"[%d] %s[%s] "DEBUG_COLOR_DEFAULT,\
                                                       System__SecondsSinceBoot,DebugChannelColors[DebugChannel],DebugChannelNames[DebugChannel] );\
                                                       SHELL_printf(&MySerialShell,__VA_ARGS__);\
                                                   }

#define DEBUG_NO_MARKER(DebugChannel,...)           if(DebugChannelFlag&&(1<<DebugChannel))\
                                                    {\
                                                      SHELL_printf(&MySerialShell,__VA_ARGS__);\
                                                    }


#endif

