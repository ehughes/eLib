#include "System.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "ctype.h"
#include "RuntimeConfig.h"
#include "Components/SHELL/SHELL.h"

#ifndef TERMINAL_PRINTF
        #define TERMINAL_PRINTF(...)   SHELL_printf(&MySerialShell,__VA_ARGS__)
#endif

#ifndef FALSE
        #define FALSE   0
#endif

#ifndef TRUE
        #define TRUE    1
#endif

EEPROM_Settings MyEEPROM_Settings;

int32_t EEPROM_Load()
{
/*
        memcpy(&MyEEPROM_Settings,(void *)FSL_FEATURE_EEPROM_BASE_ADDRESS,sizeof(MyEEPROM_Settings));

        if(MyEEPROM_Settings.EEPROM_MagicKey != EEPROM_MAGIC_KEY)
        {
                strncpy(MyEEPROM_Settings.DoB,"N/A",sizeof(MyEEPROM_Settings.DoB));
                strncpy(MyEEPROM_Settings.HW_ID,"N/A",sizeof(MyEEPROM_Settings.HW_ID));
                strncpy(MyEEPROM_Settings.SN,"N/A",sizeof(MyEEPROM_Settings.SN));


                return EEPROM_INVALID;
        }
        else
        {
                return EEPROM_OK;
        }
        */

        return EEPROM_OK;
}

int32_t EEPROM_Save()
{
    //MyEEPROM_Settings.EEPROM_MagicKey = EEPROM_MAGIC_KEY;
//      EEPROM_WritePage(EEPROM, 0, (uint32_t *)&MyEEPROM_Settings);
     return EEPROM_OK;
}

uint32_t ConfigFile_EnableRelay;
char ConfigFile_Subnet[CONFIG_MAX_VALUE_STRING_LENGTH];
char ConfigFile_Application[CONFIG_MAX_VALUE_STRING_LENGTH];

uint16_t ConfigFile_UnicastAddrBase4Ch;

uint16_t ConfigFile_SensorPubAddr;
uint16_t ConfigFile_SubAddr;

uint8_t  ConfigFile_EnableBeep;

#if USE_BRIDGE == 1
     
     uint32_t ConfigFile_CustomerID;
     uint32_t ConfigFile_BridgeID;
   
     char ConfigFile_AzureHost[CONFIG_MAX_VALUE_STRING_LENGTH];
     char ConfigFile_AzureKey[CONFIG_MAX_VALUE_STRING_LENGTH];
     char ConfigFile_AzureDeviceID[CONFIG_MAX_VALUE_STRING_LENGTH];
    
     char ConfigFile_TimeServer0[64];
     char ConfigFile_TimeServer1[64];
     char ConfigFile_TimeServer2[64];
     char ConfigFile_TimeServer3[64];

     uint32_t ConfigFile_JackID[4];

#endif

const ConfigurationItem  MyConfigurationItems[] =
{

        {
                "EnableBeep",
                "Enable Beep Feedback",
                (void *)(&ConfigFile_EnableBeep),
                DATA_TYPE_UINT_8,
                "0","255","1",
                "Bridge"
        },
        {
                "TimeServer0",
                "NTP Server Option 0",
                (void *)(&ConfigFile_TimeServer0),
                DATA_TYPE_STRING,
                "","","time.nist.gov",
                "Bridge"
        },
        {
                "TimeServer1",
                "NTP Server Option 1",
                (void *)(&ConfigFile_TimeServer1),
                DATA_TYPE_STRING,
                "","","time.google.com",
                "Bridge"
        },
        {
                "TimeServer2",
                "NTP Server Option 2",
                (void *)(&ConfigFile_TimeServer2),
                DATA_TYPE_STRING,
                "","","time.windows.com",
                "Bridge"
        },
        {
                "TimeServer3",
                "NTP Server Option 3",
                (void *)(&ConfigFile_TimeServer3),
                DATA_TYPE_STRING,
                "","","pool.ntp.org",
                "Bridge"
        },



     #if USE_BRIDGE == 1
  
       {
                "CustomerID",
                "CustomerID For this bridge",
                (void *)(&ConfigFile_CustomerID),
                DATA_TYPE_UINT_32,
                "0","4294967295","1",
                "Bridge"
        },

        

         {
                "BridgeID",
                "Bridge ID for this bridge",
                (void *)(&ConfigFile_BridgeID),
                DATA_TYPE_UINT_32,
                "0","4294967295","1",
                "Bridge"
        },

        {
                "JackID-A",
                "Physical JackID for Channel Aon on the box",
                (void *)(&ConfigFile_JackID[0]),
                DATA_TYPE_UINT_32,
                "0","4294967295","1",
                "Bridge"
        },
        {
                "JackID-B",
                "Physical JackID for Channel B on on the box",
                (void *)(&ConfigFile_JackID[1]),
                DATA_TYPE_UINT_32,
                "0","4294967295","2",
                "Bridge"
        },
        {
                "JackID-C",
                "Physical JackID for Channel C on on the box",
                (void *)(&ConfigFile_JackID[2]),
                DATA_TYPE_UINT_32,
                "0","4294967295","3",
                "Bridge"
        },
        {
                "JackID-D",
                "Physical JackID for Channel D on on the box",
                (void *)(&ConfigFile_JackID[3]),
                DATA_TYPE_UINT_32,
                "0","4294967295","4",
                "Bridge"
        },


        {
                "AzureHost",
                "Address of the IOT Hub",
                (void *)(&ConfigFile_AzureHost),
                DATA_TYPE_STRING,
                "","","tz-brew.azure-devices.net",
                "Bridge"
        },

        {
                "AzureKey",
                "Key for IOT Hub",
                (void *)(&ConfigFile_AzureKey),
                DATA_TYPE_STRING,
                "","","jVRejSEFpliuv2q7XTcO9G4qmC0GSyNqvPYFWmnZap8=",
                "Bridge"
        },

        {
                "AzureDeviceID",
                "IOT Hub Device Name",
                (void *)(&ConfigFile_AzureDeviceID),
                DATA_TYPE_STRING,
                "","","ThePrancingPony",
                "Bridge"
        },

     #endif

        {
                "EnableRelay",
                "Make this node a relay",
                (void *)(&ConfigFile_EnableRelay),
                DATA_TYPE_UINT_32,
                #if RELAY_IS_ON_BY_DEFAULT == 1
                    "0","1","1",
                #else
                    "0","1","0",
                #endif
                "Mesh"
        },
        {
                "NetKey",
                "Subnet Key for this node",
                (void *)(&ConfigFile_Subnet),
                DATA_TYPE_STRING,
                "","","AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA",
                "Mesh"
        },
        {
                "AppKey",
                "Application Key for this node",
                (void *)(&ConfigFile_Application),
                DATA_TYPE_STRING,
                "","","BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB",
                "Mesh"
        },
        {
                "UnicastAddrBase4Ch",
                "Base Local Unicast Address for the 4Channel function (Decimal)",
                (void *)(&ConfigFile_UnicastAddrBase4Ch),
                DATA_TYPE_UINT_16,
                "1","32763",32763,
                "Mesh"
        },
        {
                "SensorPubAddr",
                "Publication Address for the Sensor Server",
                (void *)(&ConfigFile_SensorPubAddr),
                DATA_TYPE_UINT_16,
                "1","65536","49152",
                "Mesh"
        },
        {
                "SubAddr",
                "Subscription Address",
                (void *)(&ConfigFile_SubAddr),
                DATA_TYPE_UINT_16,
                "1","65536","49152",
                "Mesh"
        },


};

#define NUM_CONFIGURATION_ITEMS sizeof(MyConfigurationItems)/sizeof(ConfigurationItem)

char *SectionName[MAX_SECTIONS];

typedef union
{
    uint8_t  uint8_t_Value;
    uint16_t uint16_t_Value;
    uint32_t uint32_t_Value;
    int8_t   int8_t_Value;
    int16_t  int16_t_Value;
    int32_t  int32_t_Value;
    float   float_Value;

} ValueHolder;


void Configuration_LoadWithValue(ConfigurationItem *NextConfigurationItem,void *value, uint8_t PostToTerminal);


/*
 * Finds a configuration item by index
 * returns CONFIGURATION_ITEM_NOT_FOUND (-1) if it does not exist
 */

int32_t Configuration_GetIndex(char * Name)
{
        for (int i=0;i<NUM_CONFIGURATION_ITEMS;i++)
        {
                if(strcmp(Name,MyConfigurationItems[i].Name) == 0)
                {
                        return i;
                }
        }

        return CONFIGURATION_ITEM_NOT_FOUND;
}

int32_t Configuration_SetByName(char * Name,void * Value)
{
        int32_t Index = -1;

        if( (Index = Configuration_GetIndex(Name)) == CONFIGURATION_ITEM_NOT_FOUND)
        {
                return CONFIGURATION_ITEM_NOT_FOUND;
        }
        else
        {
                Configuration_LoadWithValue((ConfigurationItem *)&MyConfigurationItems[Index],Value, FALSE);
        }

        return Index;
}

char ValueString[CONFIG_MAX_VALUE_STRING_LENGTH];

char * Configuration_GetValueString(char * Name)
{
        int32_t Index = -1;
    ValueHolder V;

        if( (Index = Configuration_GetIndex(Name)) == CONFIGURATION_ITEM_NOT_FOUND)
        {
                return NULL;
        }
        else
        {
                  switch(MyConfigurationItems[Index].DataType)
                  {
                      default:
                          break;

                      case DATA_TYPE_STRING:
                          snprintf(&ValueString[0],CONFIG_MAX_VALUE_STRING_LENGTH,"%s",(char *)MyConfigurationItems[Index].Value);
                          break;

                      case DATA_TYPE_UINT_8:
                          V.uint32_t_Value = *((uint8_t *)MyConfigurationItems[Index].Value);
                          snprintf(&ValueString[0],CONFIG_MAX_VALUE_STRING_LENGTH,"%u", (unsigned int)V.uint32_t_Value);
                          break;

                      case DATA_TYPE_UINT_16:
                          V.uint32_t_Value = *((uint16_t *)MyConfigurationItems[Index].Value);
                          snprintf(&ValueString[0],CONFIG_MAX_VALUE_STRING_LENGTH,"%u",(unsigned int)V.uint32_t_Value);
                          break;

                      case DATA_TYPE_UINT_32:
                          V.uint32_t_Value = *((uint32_t *)MyConfigurationItems[Index].Value);
                          snprintf(&ValueString[0],CONFIG_MAX_VALUE_STRING_LENGTH,"%u",(unsigned int)V.uint32_t_Value);
                          break;

                      case DATA_TYPE_INT_8:
                          V.int32_t_Value = *((int8_t *)MyConfigurationItems[Index].Value);
                          snprintf(&ValueString[0],CONFIG_MAX_VALUE_STRING_LENGTH,"%d",(int)V.int32_t_Value);
                          break;

                      case DATA_TYPE_INT_16:
                          V.int32_t_Value = *((int16_t *)MyConfigurationItems[Index].Value);
                          snprintf(&ValueString[0],CONFIG_MAX_VALUE_STRING_LENGTH,"%d",(int)V.int32_t_Value);
                          break;

                      case DATA_TYPE_INT_32:
                          V.int32_t_Value = *((int32_t *)MyConfigurationItems[Index].Value);
                          snprintf(&ValueString[0],CONFIG_MAX_VALUE_STRING_LENGTH,"%d",(int)V.int32_t_Value);
                          break;

                      case DATA_TYPE_FLOAT_32:
                          V.float_Value = *((float *)MyConfigurationItems[Index].Value);
                          snprintf(&ValueString[0],CONFIG_MAX_VALUE_STRING_LENGTH,"%f",V.float_Value);
                          break;
                  }
        }

        return &ValueString[0];
}


uint32_t Configuration_GetNumItems()
{
        return  sizeof(MyConfigurationItems)/sizeof(ConfigurationItem);
}

void Configuration_GetSectionNames()
{
    uint32_t i,j,CurrentSection = 0;
    uint8_t MatchFound = 0;

    //Reset all of our names
    for(i=0; i<MAX_SECTIONS; i++)
        {
            SectionName[i] = NULL;
        }

    //Loop through each Config item and see if there is a section in the table

    for(i=0; i<NUM_CONFIGURATION_ITEMS; i++)
        {
            MatchFound = 0;

            for(j=0; j<CurrentSection; j++)
                {
                    if(strcmp(MyConfigurationItems[i].iniSection,SectionName[j]) == 0)
                        {
                            MatchFound = 1;
                            break;
                        }
                }

            if(MatchFound == 0)
                {
                    SectionName[CurrentSection++] = MyConfigurationItems[i].iniSection;
                }

            if(CurrentSection == MAX_SECTIONS)
                break;
        }
}


int iniLineHandler(void *user, const char *section, const char *name,const char *value);

void Configuration_LoadDefaults()
{
    uint32_t i;

    for(i=0; i<NUM_CONFIGURATION_ITEMS; i++)
        {
            Configuration_LoadWithValue((ConfigurationItem *)&MyConfigurationItems[i],MyConfigurationItems[i].Default, FALSE);
        }
}

char sBuf[64];

int32_t Configuration_Export(char *FileName)
{
    uint32_t i = 0,j;
    FIL   F;
    ValueHolder V;

    if (f_open(&F, FileName,FA_CREATE_ALWAYS| FA_WRITE) != FR_OK)
        {
            TERMINAL_PRINTF("Could not open file for writing\r\n");
            return -1;
        }

        i = 0;
    //handle the generic structure
    Configuration_GetSectionNames();
    while((SectionName[i] != NULL) && (i<MAX_SECTIONS))
        {
            f_printf(&F,"[%s]\r\n\r\n",SectionName[i]);

            for(j=0; j<NUM_CONFIGURATION_ITEMS; j++)
                {
                    if(strcmp(SectionName[i],MyConfigurationItems[j].iniSection) == 0)
                        {
                            switch(MyConfigurationItems[j].DataType)
                                {
                                    default:
                                        break;

                                    case DATA_TYPE_STRING:
                                        f_printf(&F,"%s = %s\r\n",(char *)MyConfigurationItems[j].Name,(char *)MyConfigurationItems[j].Value);
                                        break;

                                    case DATA_TYPE_UINT_8:
                                        V.uint32_t_Value = *((uint8_t *)MyConfigurationItems[j].Value);
                                        f_printf(&F,"%s = %d\r\n",(char *)MyConfigurationItems[j].Name, V.uint32_t_Value);
                                        break;

                                    case DATA_TYPE_UINT_16:
                                        V.uint32_t_Value = *((uint16_t *)MyConfigurationItems[j].Value);
                                        f_printf(&F,"%s = %d\r\n",(char *)MyConfigurationItems[j].Name,V.uint32_t_Value);
                                        break;

                                    case DATA_TYPE_UINT_32:
                                        V.uint32_t_Value = *((uint32_t *)MyConfigurationItems[j].Value);
                                        f_printf(&F,"%s = %d\r\n",(char *)MyConfigurationItems[j].Name,V.uint32_t_Value);
                                        break;

                                    case DATA_TYPE_INT_8:
                                        V.int32_t_Value = *((int8_t *)MyConfigurationItems[j].Value);
                                        f_printf(&F,"%s = %d\r\n",(char *)MyConfigurationItems[j].Name,V.int32_t_Value);
                                        break;

                                    case DATA_TYPE_INT_16:
                                        V.int32_t_Value = *((int16_t *)MyConfigurationItems[j].Value);
                                        f_printf(&F,"%s = %d\r\n",(char *)MyConfigurationItems[j].Name,V.int32_t_Value);
                                        break;

                                    case DATA_TYPE_INT_32:
                                        V.int32_t_Value = *((int32_t *)MyConfigurationItems[j].Value);
                                        f_printf(&F,"%s = %d\r\n",(char *)MyConfigurationItems[j].Name,V.int32_t_Value);
                                        break;

                                    case DATA_TYPE_FLOAT_32:

                                        V.float_Value = *((float *)MyConfigurationItems[j].Value);
                                        sprintf(sBuf,"%f",V.float_Value);
                                        f_printf(&F,"%s = %s\r\n",(char *)MyConfigurationItems[j].Name,(char *)sBuf);
                                        break;
                                }
                        }


                }

            i++;
        }

    f_close(&F);

    return 0;
}

int32_t  Configuration_Load(char * FileName)
{
    Configuration_LoadDefaults();

    if (ini_parse(FileName,iniLineHandler, (void *)&MyConfigurationItems) < 0)
        {
                return -1;

        }
    else
        {
            return CONFIG_LOAD_OK;
        }
}

int iniLineHandler(void *user, const char *section, const char *name,const char *value)
{
    uint32_t i;
    ConfigurationItem *NextCI= (ConfigurationItem *)user;


 #define MATCH_SECTION_AND_NAME(s, n) strcmp(section, s) == 0 && strcmp(name, n) == 0

 for(i=0; i<NUM_CONFIGURATION_ITEMS; i++)
        {
                if(MATCH_SECTION_AND_NAME(NextCI[i].iniSection,NextCI[i].Name))
                        {
                                //IF we are here we have a match on the section
                                //Load the data
                                Configuration_LoadWithValue(&NextCI[i],(void *)value,false);
                        }
        }

    return 0;
}

void Configuration_LoadWithValue(ConfigurationItem *NextConfigurationItem,void *value, uint8_t PostToTerminal)
{
    ValueHolder MyValueHolder;
    ValueHolder Max;
    ValueHolder Min;

    switch(NextConfigurationItem->DataType)
        {
            default:
                if(PostToTerminal == TRUE)
                    TERMINAL_PRINTF("Unknown Configuration Data type of %i\r\n",NextConfigurationItem->DataType);

                break;

            case  DATA_TYPE_STRING:
                strcpy((char *)NextConfigurationItem->Value,(const char *)value);

                if(PostToTerminal == TRUE)
                    TERMINAL_PRINTF("%s loaded with value of %s\r\n",NextConfigurationItem->Name,value);

                break;

            case DATA_TYPE_UINT_8:
            case DATA_TYPE_UINT_16:
            case DATA_TYPE_UINT_32:

                //We will first load in the value as if it was a 32bit unsigned data type and then wack it down to size if needed
                MyValueHolder.uint32_t_Value = atoi((const char *)value);
                Max.uint32_t_Value = atoi(NextConfigurationItem->Maximum);
                Min.uint32_t_Value = atoi(NextConfigurationItem->Minimum);

                //see if value exceeds Maximum or minimum
                if(MyValueHolder.uint32_t_Value >= Max.uint32_t_Value)
                    {
                        MyValueHolder.uint32_t_Value = Max.uint32_t_Value;
                    }

                if(MyValueHolder.uint32_t_Value <= Min.uint32_t_Value)
                    {
                        MyValueHolder.uint32_t_Value = Min.uint32_t_Value;
                    }

                //Copy in the value.  It is possible that the value will be outside of the bounds so we will cast to use C default rules
                //We will also look at the current data type to get the casting correct

                if(NextConfigurationItem->DataType == DATA_TYPE_UINT_8)
                    {
                        *((uint8_t *)NextConfigurationItem->Value) = (uint8_t)MyValueHolder.uint32_t_Value;

                        if(PostToTerminal == TRUE)
                            TERMINAL_PRINTF("%s loaded with value of %i\r\n",NextConfigurationItem->Name, *((uint8_t *)NextConfigurationItem->Value));
                    }
                else  if(NextConfigurationItem->DataType == DATA_TYPE_UINT_16)
                    {
                        *((uint16_t *)NextConfigurationItem->Value) = (uint16_t)MyValueHolder.uint32_t_Value;

                        if(PostToTerminal == TRUE)
                            TERMINAL_PRINTF("%s loaded with value of %i\r\n",NextConfigurationItem->Name, *((uint16_t *)NextConfigurationItem->Value));
                    }
                else  if(NextConfigurationItem->DataType == DATA_TYPE_UINT_32)
                    {
                        *((uint32_t *)NextConfigurationItem->Value) = (uint32_t)MyValueHolder.uint32_t_Value;

                        if(PostToTerminal == TRUE)
                            TERMINAL_PRINTF("%s loaded with value of %i\r\n",NextConfigurationItem->Name, *((uint32_t *)NextConfigurationItem->Value));
                    }

                break;

            case DATA_TYPE_INT_8:
            case DATA_TYPE_INT_16:
            case DATA_TYPE_INT_32:
                //We will first load in the value as if it was a 32bit signed data type and then wack it down to size if needed
                MyValueHolder.int32_t_Value = atoi((const char *)value);
                Max.int32_t_Value = atoi(NextConfigurationItem->Maximum);
                Min.int32_t_Value = atoi(NextConfigurationItem->Minimum);

                //see if value exceeds Maximum or minimum
                if(MyValueHolder.int32_t_Value > Max.int32_t_Value)
                    {
                        MyValueHolder.int32_t_Value = Max.int32_t_Value;
                    }

                if(MyValueHolder.int32_t_Value < Min.int32_t_Value)
                    {
                        MyValueHolder.int32_t_Value = Min.int32_t_Value;
                    }

                //Copy in the value.  It is possible that the value will be outside of the bounds so we will cast to use C default rules
                //We will also look at the current data type to get the casting correct

                if(NextConfigurationItem->DataType == DATA_TYPE_INT_8)
                    {
                        *((int8_t *)NextConfigurationItem->Value) = (int8_t)MyValueHolder.int32_t_Value;

                        if(PostToTerminal == TRUE)
                            TERMINAL_PRINTF("%s loaded with value of %i\r\n",NextConfigurationItem->Name, *((int8_t *)NextConfigurationItem->Value));
                    }
                else  if(NextConfigurationItem->DataType == DATA_TYPE_INT_16)
                    {
                        *((int16_t *)NextConfigurationItem->Value) = (int16_t)MyValueHolder.int32_t_Value;

                        if(PostToTerminal == TRUE)
                            TERMINAL_PRINTF("%s loaded with value of %i\r\n",NextConfigurationItem->Name, *((int16_t *)NextConfigurationItem->Value));
                    }
                else  if(NextConfigurationItem->DataType == DATA_TYPE_INT_32)
                    {
                        *((int32_t *)NextConfigurationItem->Value) = (int32_t)MyValueHolder.int32_t_Value;

                        if(PostToTerminal == TRUE)
                            TERMINAL_PRINTF("%s loaded with value of %i\r\n",NextConfigurationItem->Name, *((int32_t *)NextConfigurationItem->Value));
                    }

                break;

            case DATA_TYPE_FLOAT_32:
                //We will first load in the value as if it was a 32bit signed data type and then wack it down to size if needed
                MyValueHolder.float_Value= atof((const char *)value);
                Max.float_Value = atof(NextConfigurationItem->Maximum);
                Min.float_Value = atof(NextConfigurationItem->Minimum);

                //see if value exceeds Maximum or minimum
                if(MyValueHolder.float_Value > Max.float_Value)
                    {
                        MyValueHolder.float_Value = Max.float_Value;
                    }

                if(MyValueHolder.float_Value < Min.float_Value)
                    {
                        MyValueHolder.float_Value = Min.float_Value;
                    }

                //Copy in the value.  It is possible that the value will be outside of the bounds so we will cast to use C default rules
                //We will also look at the current data type to get the casting correct
                *((float *)NextConfigurationItem->Value) = (float)MyValueHolder.float_Value;

                if(PostToTerminal == TRUE)
                    TERMINAL_PRINTF("%s loaded with value of %f\r\n",NextConfigurationItem->Name, *((float *)NextConfigurationItem->Value));

                break;
        }

}





#if !INI_USE_STACK
#include "stdlib.h"
#endif

#define MAX_SECTION 50
#define MAX_NAME 128

/* Strip whitespace chars off end of given string, in place. Return s. */
static char *rstrip(char *s)
{
    char *p = s + strlen(s);

    while (p > s && isspace((unsigned char)(*--p)))
        *p = '\0';

    return s;
}

/* Return pointer to first non-whitespace char in given string. */
static char *lskip(const char *s)
{
    while (*s && isspace((unsigned char)(*s)))
        s++;

    return (char *)s;
}

/* Return pointer to first char c or ';' comment in given string, or pointer to
   null at end of string if neither found. ';' must be prefixed by a whitespace
   character to register as a comment. */
static char *find_char_or_comment(const char *s, char c)
{
    int was_whitespace = 0;

    while (*s && *s != c && !(was_whitespace && *s == ';'))
        {
            was_whitespace = isspace((unsigned char)(*s));
            s++;
        }

    return (char *)s;
}




/* Version of strncpy that ensures dest (size bytes) is null-terminated. */
static char *strncpy0(char *dest, const char *src, size_t size)
{
    strncpy(dest, src, size);
    dest[size - 1] = '\0';
    return dest;
}


/* See documentation in header file. */
int ini_parse_file(FIL *file,
                   int (*handler)(void *, const char *, const char *,
                                  const char *),
                   void *user)
{
    /* Uses a fair bit of stack (use heap instead if you need to) */
#if INI_USE_STACK
    char line[INI_MAX_LINE];
#else
    char *line;
#endif
    char section[MAX_SECTION] = "";
    char prev_name[MAX_NAME] = "";
    char *start;
    char *end;
    char *name;
    char *value;
    int lineno = 0;
    int error = 0;
#if !INI_USE_STACK
    line = (char *)malloc(INI_MAX_LINE);

    if (!line)
        {
            return -2;
        }

#endif

    while (f_gets(line,INI_MAX_LINE,file) != 0)
        {
            lineno++;
            start = line;
#if INI_ALLOW_BOM

            if (lineno == 1 && (unsigned char)start[0] == 0xEF &&
                    (unsigned char)start[1] == 0xBB &&
                    (unsigned char)start[2] == 0xBF)
                {
                    start += 3;
                }

#endif
            start = lskip(rstrip(start));

            if (*start == ';' || *start == '#')
                {
                    /* Per Python ConfigParser, allow '#' comments at start of line */
                }

#if INI_ALLOW_MULTILINE
            else if (*prev_name && *start && start > line)
                {
                    /* Non-black line with leading whitespace, treat as continuation
                       of previous name's value (as per Python ConfigParser). */
                    if (!handler(user, section, prev_name, start) && !error)
                        error = lineno;
                }

#endif
            else if (*start == '[')
                {
                    /* A "[section]" line */
                    end = find_char_or_comment(start + 1, ']');

                    if (*end == ']')
                        {
                            *end = '\0';
                            strncpy0(section, start + 1, sizeof(section));
                            *prev_name = '\0';
                        }
                    else if (!error)
                        {
                            /* No ']' found on section line */
                            error = lineno;
                        }
                }
            else if (*start && *start != ';')
                {
                    /* Not a comment, must be a name[=:]value pair */
                    end = find_char_or_comment(start, '=');

                    if (*end != '=')
                        {
                            end = find_char_or_comment(start, ':');
                        }

                    if (*end == '=' || *end == ':')
                        {
                            *end = '\0';
                            name = rstrip(start);
                            value = lskip(end + 1);
                            end = find_char_or_comment(value, '\0');

                            if (*end == ';')
                                *end = '\0';

                            rstrip(value);
                            /* Valid name[=:]value pair found, call handler */
                            strncpy0(prev_name, name, sizeof(prev_name));

                            if (!handler(user, section, name, value) && !error)
                                error = lineno;
                        }
                    else if (!error)
                        {
                            /* No '=' or ':' found on name[=:]value line */
                            error = lineno;
                        }
                }

#if INI_STOP_ON_FIRST_ERROR

            if (error)
                break;

#endif
        }

#if !INI_USE_STACK
    free(line);
#endif
    return error;
}

/* See documentation in header file. */
int ini_parse(const char *filename,
              int (*handler)(void *, const char *, const char *, const char *),
              void *user)
{
    FIL file;

    int error;


    if (f_open(&file,filename, FA_OPEN_EXISTING | FA_READ) != FR_OK)
    {
        f_close(&file);
        return -1;
    }

    error = ini_parse_file(&file, handler, user);

    f_close(&file);

    return error;
}


