/*
 * RTOS_SHELL_Commands_Core.h
 *
 *  Created on: Jan 7, 2018
 *      Author: Eli Hughes
 */

#ifndef SYSTEM_SHELL_RTOS_SHELL_COMMANDS__CORE_H_
#define SYSTEM_SHELL_RTOS_SHELL_COMMANDS__CORE_H_

cmd_function_t su(p_shell_context_t Shell, int32_t argc, char **argv);
cmd_function_t cow(p_shell_context_t Shell,int32_t argc, char **argv);
cmd_function_t shell_test(p_shell_context_t Shell,int32_t argc, char **argv);
cmd_function_t debug(p_shell_context_t Shell, int32_t argc, char **argv);
cmd_function_t reset(p_shell_context_t Shell,int32_t argc, char **argv);

cmd_function_t i(p_shell_context_t Shell,int32_t argc, char **argv);
cmd_function_t q(p_shell_context_t Shell,int32_t argc, char **argv);

#endif /* SYSTEM_SHELL_RTOS_SHELL_COMMANDS__CORE_H_ */
