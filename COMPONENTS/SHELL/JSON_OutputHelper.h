/*
 * JSON_OutputHelper.h
 *
 *  Created on: Jan 10, 2018
 *      Author: Eli Hughes
 */

#ifndef JSON_OUTPUTHELPER_H_
#define JSON_OUTPUTHELPER_H_


#define ASCII_STX	0x02
#define ASCII_ETX	0x03

#define TABIN	for(int t=0;t<TabInCount;t++){OutputTab(Shell);}

void StartJSON(p_shell_context_t Shell);

void StopJSON(p_shell_context_t Shell);

void OutputTab(p_shell_context_t Shell);

void OutputTabs(p_shell_context_t Shell,uint32_t NumTabs);

void OutputObjID(p_shell_context_t Shell, char * ID);

void OutputNewLine(p_shell_context_t Shell);

void OutputNewJSON_Line(p_shell_context_t Shell);

void OutputJSON_StringVariable(p_shell_context_t Shell,char * Name, char * Value);

void OutputUID(p_shell_context_t Shell);

void OutputJSON_IntegerVariable(p_shell_context_t Shell,char * Name, int32_t Value);

void OutputJSON_FloatVariable(p_shell_context_t Shell,char * Name, float Value,uint32_t DecimalPlaces);

void OutputJSON_BooleanVariable(p_shell_context_t Shell,char * Name, bool Value);

void OutputJSON_UnsignedIntegerVariable(p_shell_context_t Shell,char * Name, uint32_t Value);


#endif /* SYSTEM_SHELL_JSON_OUTPUTHELPER_H_ */
