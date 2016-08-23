/*
 * RuntimeConfig.h
 *
 *  Created on: May 15, 2014
 *      Author: emh203
 */

#include "stdint.h"

#ifndef RUNTIMECONFIG_H_
#define RUNTIMECONFIG_H_

#define CONFIG_FILE_NAME	"config.txt"

uint8_t LoadTimeMachineConfiguration();

#define MAX_SECTIONS             8

#define DATA_TYPE_STRING         0
#define DATA_TYPE_UINT_8         1
#define DATA_TYPE_UINT_16        2
#define DATA_TYPE_UINT_32        3
#define DATA_TYPE_INT_8          4
#define DATA_TYPE_INT_16         5
#define DATA_TYPE_INT_32         6
#define DATA_TYPE_FLOAT_32       7
#define DATA_TYPE_TIME	         8

typedef struct
{
    char *Name;
    void *Value;         //a pointer to where the config data will be dumped
    uint8_t DataType;   //Variable Type
    char *Minimum;      //Minimum Value.   Must be a string to handle wierd cases no minimum,  ints vs floats
    char *Maximum;      //Maximum Value.   Must be a string to handle wierd cases no maximum,  ints vs floats
    char *Default;      //Default Value.   Must be a string to handle wierd cases no minimum,  ints vs floats
    char *iniSection;  //Which section of the init file.

} ConfigurationItem;


void LoadRunTimeConfigurationDefaults();
int32_t ExportRunTimeConfiguration(char *FileName);
void LoadRunTimeConfiguration(char * FileName);
void Import_EEPROM_Settings();
void Export_EEPROM_Settings();

#endif /* RUNTIMECONFIG_H_ */



/* inih -- simple .INI file parser

inih is released under the New BSD license (see LICENSE.txt). Go to the project
home page for more info:

http://code.google.com/p/inih/

*/
#ifndef __INI_H__
#define __INI_H__


/* Parse given INI-style file. May have [section]s, name=value pairs
   (whitespace stripped), and comments starting with ';' (semicolon). Section
   is "" if name=value pair parsed before any section heading. name:value
   pairs are also supported as a concession to Python's ConfigParser.

   For each name=value pair parsed, call handler function with given user
   pointer as well as section, name, and value (data only valid for duration
   of handler call). Handler should return nonzero on success, zero on error.

   Returns 0 on success, line number of first error on parse error (doesn't
   stop on first error), -1 on file open error, or -2 on memory allocation
   error (only when INI_USE_STACK is zero).
*/
int ini_parse(const char *filename,
              int (*handler)(void *user, const char *section,
                             const char *name, const char *value),
              void *user);

/* Same as ini_parse(), but takes a FILE* instead of filename. This doesn't
   close the file when it's finished -- the caller must do that. */
int ini_parse_file(FIL *file,
                   int (*handler)(void *, const char *, const char *,
                                  const char *),
                   void *user);

/* Nonzero to allow multi-line value parsing, in the style of Python's
   ConfigParser. If allowed, ini_parse() will call the handler with the same
   name for each subsequent line parsed. */
#ifndef INI_ALLOW_MULTILINE
#define INI_ALLOW_MULTILINE 1
#endif

/* Nonzero to allow a UTF-8 BOM sequence (0xEF 0xBB 0xBF) at the start of
   the file. See http://code.google.com/p/inih/issues/detail?id=21 */
#ifndef INI_ALLOW_BOM
#define INI_ALLOW_BOM 1
#endif

/* Nonzero to use stack, zero to use heap (malloc/free). */
#ifndef INI_USE_STACK
#define INI_USE_STACK 1
#endif

/* Stop parsing on first error (default is to keep parsing). */
#ifndef INI_STOP_ON_FIRST_ERROR
#define INI_STOP_ON_FIRST_ERROR 0
#endif

/* Maximum line length for any line in INI file. */
#ifndef INI_MAX_LINE
#define INI_MAX_LINE 128
#endif

#endif /* __INI_H__ */

