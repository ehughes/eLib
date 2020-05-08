#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "ctype.h"
#include "RuntimeConfiguration.h"
#include "Components/SHELL/SHELL.h"

#ifndef RT_CONFIG_DEBUG
        #define RT_CONFIG_DEBUG(...)   SHELL_printf(&MySerialShell,__VA_ARGS__)
#endif

#ifndef FALSE
        #define FALSE   0
#endif

#ifndef TRUE
        #define TRUE    1
#endif


char *SectionName[MAX_SECTIONS];

char ValueString[CONFIG_MAX_VALUE_STRING_LENGTH];

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


void Configuration_LoadWithValue(ConfigurationItem *NextConfigurationItem,void *value);


int32_t Configuration_GetCount(ConfigurationItem *ConfigList)
{
    int i =0;

    while(
          ((void *)(ConfigList[i].Name) != (void *)(0)) && 
          (i<CONFIG_MAX_LIST_LENGTH)
          )
    {
        i++; 
    }

  return i;
}

/*
 * Finds a configuration item by index
 * returns CONFIGURATION_ITEM_NOT_FOUND (-1) if it does not exist
 */

int32_t Configuration_GetIndex(ConfigurationItem *ConfigList, char * Name)
{
        for (int i=0;i<Configuration_GetCount(ConfigList);i++)
        {
                if(strcmp(Name,ConfigList[i].Name) == 0)
                {
                        return i;
                }
        }

        return CONFIGURATION_ITEM_NOT_FOUND;
}

int32_t Configuration_SetByName(ConfigurationItem *ConfigList,char * Name,void * Value)
{
        int32_t Index = CONFIGURATION_ITEM_NOT_FOUND;

        if( (Index = Configuration_GetIndex(ConfigList,Name)) > 0)
        {

                Configuration_LoadWithValue((ConfigurationItem *)&ConfigList[Index],Value);
        }

        return Index;
}



char * Configuration_GetValueString(ConfigurationItem *ConfigList,char * Name)
{
      int32_t Index = CONFIGURATION_ITEM_NOT_FOUND;
  
      ValueHolder V;

        if( (Index = Configuration_GetIndex(ConfigList,Name)) == CONFIGURATION_ITEM_NOT_FOUND)
        {
                return NULL;
        }
        else
        {
                  switch(ConfigList[Index].DataType)
                  {
                      default:
                          break;

                      case DATA_TYPE_STRING:
                          snprintf(&ValueString[0],CONFIG_MAX_VALUE_STRING_LENGTH,"%s",(char *)ConfigList[Index].Value);
                          break;

                      case DATA_TYPE_UINT_8:
                          V.uint32_t_Value = *((uint8_t *)ConfigList[Index].Value);
                          snprintf(&ValueString[0],CONFIG_MAX_VALUE_STRING_LENGTH,"%u", (unsigned int)V.uint32_t_Value);
                          break;

                      case DATA_TYPE_UINT_16:
                          V.uint32_t_Value = *((uint16_t *)ConfigList[Index].Value);
                          snprintf(&ValueString[0],CONFIG_MAX_VALUE_STRING_LENGTH,"%u",(unsigned int)V.uint32_t_Value);
                          break;

                      case DATA_TYPE_UINT_32:
                          V.uint32_t_Value = *((uint32_t *)ConfigList[Index].Value);
                          snprintf(&ValueString[0],CONFIG_MAX_VALUE_STRING_LENGTH,"%u",(unsigned int)V.uint32_t_Value);
                          break;

                      case DATA_TYPE_INT_8:
                          V.int32_t_Value = *((int8_t *)ConfigList[Index].Value);
                          snprintf(&ValueString[0],CONFIG_MAX_VALUE_STRING_LENGTH,"%d",(int)V.int32_t_Value);
                          break;

                      case DATA_TYPE_INT_16:
                          V.int32_t_Value = *((int16_t *)ConfigList[Index].Value);
                          snprintf(&ValueString[0],CONFIG_MAX_VALUE_STRING_LENGTH,"%d",(int)V.int32_t_Value);
                          break;

                      case DATA_TYPE_INT_32:
                          V.int32_t_Value = *((int32_t *)ConfigList[Index].Value);
                          snprintf(&ValueString[0],CONFIG_MAX_VALUE_STRING_LENGTH,"%d",(int)V.int32_t_Value);
                          break;

                      case DATA_TYPE_FLOAT_32:
                          V.float_Value = *((float *)ConfigList[Index].Value);
                          snprintf(&ValueString[0],CONFIG_MAX_VALUE_STRING_LENGTH,"%f",V.float_Value);
                          break;
                  }
        }

        return &ValueString[0];
}


void Configuration_GetSectionNames(ConfigurationItem *ConfigList)
{
    uint32_t i,j,CurrentSection = 0;
    uint8_t MatchFound = 0;

    //Reset all of our names
    for(i=0; i<MAX_SECTIONS; i++)
        {
            SectionName[i] = NULL;
        }

    //Loop through each Config item and see if there is a section in the table

    for(i=0; i<Configuration_GetCount(ConfigList); i++)
        {
            MatchFound = 0;

            for(j=0; j<CurrentSection; j++)
                {
                    if(strcmp(ConfigList[i].iniSection,SectionName[j]) == 0)
                        {
                            MatchFound = 1;
                            break;
                        }
                }

            if(MatchFound == 0)
                {
                    SectionName[CurrentSection++] = ConfigList[i].iniSection;
                }

            if(CurrentSection == MAX_SECTIONS)
                break;
        }
}


int iniLineHandler(void *user, const char *section, const char *name,const char *value);

void Configuration_LoadDefaults(ConfigurationItem *ConfigList)
{
    uint32_t i;

    for(i=0; i<Configuration_GetCount(ConfigList); i++)
        {
            Configuration_LoadWithValue((ConfigurationItem *)&ConfigList[i],ConfigList[i].Default);
        }
}

char sBuf[64];

int32_t Configuration_Export(ConfigurationItem *ConfigList,char *FileName)
{
    uint32_t i = 0,j;
    FIL   F;
    ValueHolder V;

    if (f_open(&F, FileName,FA_CREATE_ALWAYS| FA_WRITE) != FR_OK)
        {
            RT_CONFIG_DEBUG("Could not open file for writing\r\n");
            return -1;
        }

        i = 0;
    //handle the generic structure
    Configuration_GetSectionNames(ConfigList);

    while((SectionName[i] != NULL) && (i<MAX_SECTIONS))
        {
            f_printf(&F,"[%s]\r\n\r\n",SectionName[i]);

            for(j=0; j<Configuration_GetCount(ConfigList); j++)
                {
                    if(strcmp(SectionName[i],ConfigList[j].iniSection) == 0)
                        {
                            switch(ConfigList[j].DataType)
                                {
                                    default:
                                        break;

                                    case DATA_TYPE_STRING:
                                        f_printf(&F,"%s = %s\r\n",(char *)ConfigList[j].Name,(char *)ConfigList[j].Value);
                                        break;

                                    case DATA_TYPE_UINT_8:
                                        V.uint32_t_Value = *((uint8_t *)ConfigList[j].Value);
                                        f_printf(&F,"%s = %d\r\n",(char *)ConfigList[j].Name, V.uint32_t_Value);
                                        break;

                                    case DATA_TYPE_UINT_16:
                                        V.uint32_t_Value = *((uint16_t *)ConfigList[j].Value);
                                        f_printf(&F,"%s = %d\r\n",(char *)ConfigList[j].Name,V.uint32_t_Value);
                                        break;

                                    case DATA_TYPE_UINT_32:
                                        V.uint32_t_Value = *((uint32_t *)ConfigList[j].Value);
                                        f_printf(&F,"%s = %d\r\n",(char *)ConfigList[j].Name,V.uint32_t_Value);
                                        break;

                                    case DATA_TYPE_INT_8:
                                        V.int32_t_Value = *((int8_t *)ConfigList[j].Value);
                                        f_printf(&F,"%s = %d\r\n",(char *)ConfigList[j].Name,V.int32_t_Value);
                                        break;

                                    case DATA_TYPE_INT_16:
                                        V.int32_t_Value = *((int16_t *)ConfigList[j].Value);
                                        f_printf(&F,"%s = %d\r\n",(char *)ConfigList[j].Name,V.int32_t_Value);
                                        break;

                                    case DATA_TYPE_INT_32:
                                        V.int32_t_Value = *((int32_t *)ConfigList[j].Value);
                                        f_printf(&F,"%s = %d\r\n",(char *)ConfigList[j].Name,V.int32_t_Value);
                                        break;

                                    case DATA_TYPE_FLOAT_32:

                                        V.float_Value = *((float *)ConfigList[j].Value);
                                        sprintf(sBuf,"%f",V.float_Value);
                                        f_printf(&F,"%s = %s\r\n",(char *)ConfigList[j].Name,(char *)sBuf);
                                        break;
                                }
                        }


                }

            i++;
        }

    f_close(&F);

    return 0;
}

int32_t  Configuration_Load(ConfigurationItem *ConfigList, char * FileName)
{
    Configuration_LoadDefaults(ConfigList);

    if (ini_parse(FileName,iniLineHandler, (void *)ConfigList) < 0)
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

       for(i=0; i<Configuration_GetCount(NextCI); i++)
        {
             if(MATCH_SECTION_AND_NAME(NextCI[i].iniSection,NextCI[i].Name))
             {
                 //IF we are here we have a match on the section
                 //Load the data
                 Configuration_LoadWithValue(&NextCI[i],(void *)value);
             }
        }

    return 0;
}

void Configuration_LoadWithValue(ConfigurationItem *NextConfigurationItem,void *value)
{
    ValueHolder MyValueHolder;
    ValueHolder Max;
    ValueHolder Min;

    switch(NextConfigurationItem->DataType)
        {
            default:
              
                RT_CONFIG_DEBUG("Unknown Configuration Data type of %i\r\n",NextConfigurationItem->DataType);

                break;

            case  DATA_TYPE_STRING:
                strcpy((char *)NextConfigurationItem->Value,(const char *)value);

                RT_CONFIG_DEBUG("%s loaded with value of %s\r\n",NextConfigurationItem->Name,value);

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
                        RT_CONFIG_DEBUG("%s loaded with value of %i\r\n",NextConfigurationItem->Name, *((uint8_t *)NextConfigurationItem->Value));
                    }
                else  if(NextConfigurationItem->DataType == DATA_TYPE_UINT_16)
                    {
                        *((uint16_t *)NextConfigurationItem->Value) = (uint16_t)MyValueHolder.uint32_t_Value;
                        RT_CONFIG_DEBUG("%s loaded with value of %i\r\n",NextConfigurationItem->Name, *((uint16_t *)NextConfigurationItem->Value));
                    }
                else  if(NextConfigurationItem->DataType == DATA_TYPE_UINT_32)
                    {
                        *((uint32_t *)NextConfigurationItem->Value) = (uint32_t)MyValueHolder.uint32_t_Value;
                         RT_CONFIG_DEBUG("%s loaded with value of %i\r\n",NextConfigurationItem->Name, *((uint32_t *)NextConfigurationItem->Value));
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
                         RT_CONFIG_DEBUG("%s loaded with value of %i\r\n",NextConfigurationItem->Name, *((int8_t *)NextConfigurationItem->Value));
                    }
                else  if(NextConfigurationItem->DataType == DATA_TYPE_INT_16)
                    {
                        *((int16_t *)NextConfigurationItem->Value) = (int16_t)MyValueHolder.int32_t_Value;

                        RT_CONFIG_DEBUG("%s loaded with value of %i\r\n",NextConfigurationItem->Name, *((int16_t *)NextConfigurationItem->Value));
                    }
                else  if(NextConfigurationItem->DataType == DATA_TYPE_INT_32)
                    {
                        *((int32_t *)NextConfigurationItem->Value) = (int32_t)MyValueHolder.int32_t_Value;

                        RT_CONFIG_DEBUG("%s loaded with value of %i\r\n",NextConfigurationItem->Name, *((int32_t *)NextConfigurationItem->Value));
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

                RT_CONFIG_DEBUG("%s loaded with value of %f\r\n",NextConfigurationItem->Name, *((float *)NextConfigurationItem->Value));

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


