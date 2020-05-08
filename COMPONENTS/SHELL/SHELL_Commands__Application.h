/*
 * RTOS_SHELL_Commands_Core.h
 *
 *  Created on: Jan 7, 2018
 *      Author: Eli Hughes
 */

#ifndef __SHELL_APPLICATION
#define __SHELL_APPLICATION


cmd_function_t probe(p_shell_context_t Shell,int32_t argc, char **argv);
cmd_function_t pp(p_shell_context_t Shell,int32_t argc, char **argv);
cmd_function_t rt(p_shell_context_t Shell,int32_t argc, char **argv);

#endif /* SYSTEM_SHELL_RTOS_SHELL_COMMANDS__CORE_H_ */
