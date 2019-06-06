#include "DebugChannel.h"


uint32_t DebugChannelFlag = 0xFFFFFFFFFF;


const char *DebugChannelNames[32] = { "SYSTEM",//0 
                                    "MESH",    //1
                                    "4CH",     //2
                                    "SNS",     //3
                                    "CELL",     //4
                                    "MQTT",    //5
                                    "MQTT_NET",//6
                                    "BRIDGE",  //7
                                    "DNS", //8
                                    "NTP", //9
                                    (char *)0, //10
                                    (char *)0, //11
                                    (char *)0, //12
                                    (char *)0, //13
                                    (char *)0, //14
                                    (char *)0, //15
                                    (char *)0, //16
                                    (char *)0, //17
                                    (char *)0, //18
                                    (char *)0, //19
                                    (char *)0, //20
                                    (char *)0, //21
                                    (char *)0, //22
                                    (char *)0, //23
                                    (char *)0, //24
                                    (char *)0, //25
                                    (char *)0, //26
                                    (char *)0, //27
                                    (char *)0, //28
                                    (char *)0, //29
                                    (char *)0, //30
                                    (char *)0, //21
                                    };

const char *DebugChannelColors[32] = {
                                     DEBUG_WHITE,   //0 
                                     DEBUG_MAGENTA,    //1
                                     DEBUG_CYAN,     //2
                                     DEBUG_YELLOW,     //3
                                     DEBUG_GREEN,     //4
                                     DEBUG_YELLOW,    //5
                                     DEBUG_YELLOW,//6
                                     DEBUG_GREEN,  //7
                                     DEBUG_YELLOW, //8
                                     DEBUG_RED, //9
                                     (char *)0, //10
                                     (char *)0, //11
                                     (char *)0, //12
                                     (char *)0, //13
                                     (char *)0, //14
                                     (char *)0, //15
                                     (char *)0, //16
                                     (char *)0, //17
                                     (char *)0, //18
                                     (char *)0, //19
                                     (char *)0, //20
                                     (char *)0, //21
                                     (char *)0, //22
                                     (char *)0, //23
                                     (char *)0, //24
                                     (char *)0, //25
                                     (char *)0, //26
                                     (char *)0, //27
                                     (char *)0, //28
                                     (char *)0, //29
                                     (char *)0, //30
                                     (char *)0, //31
                                    };
