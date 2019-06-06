/*
 * RTOS_SHELL_Commands_Core.h
 *
 *  Created on: Jan 7, 2018
 *      Author: Eli Hughes
 */

#ifndef __SHELL_APPLICATION
#define __SHELL_APPLICATION

cmd_function_t wipe(p_shell_context_t Shell,int32_t argc, char **argv);
cmd_function_t mstat(p_shell_context_t Shell,int32_t argc, char **argv);
cmd_function_t relay(p_shell_context_t Shell,int32_t argc, char **argv);
cmd_function_t rstat(p_shell_context_t Shell,int32_t argc, char **argv);
cmd_function_t ec_check(p_shell_context_t Shell,int32_t argc, char **argv);
cmd_function_t ec_start(p_shell_context_t Shell,int32_t argc, char **argv);
cmd_function_t ec_stop(p_shell_context_t Shell,int32_t argc, char **argv);
cmd_function_t ec_cal_clear(p_shell_context_t Shell,int32_t argc, char **argv);
cmd_function_t ec_cal_dry(p_shell_context_t Shell,int32_t argc, char **argv);
cmd_function_t ec_cal_single(p_shell_context_t Shell,int32_t argc, char **argv);
cmd_function_t ec_cal_low(p_shell_context_t Shell,int32_t argc, char **argv);
cmd_function_t ec_cal_high(p_shell_context_t Shell,int32_t argc, char **argv);
cmd_function_t ec_read(p_shell_context_t Shell,int32_t argc, char **argv);
cmd_function_t ec_temp(p_shell_context_t Shell,int32_t argc, char **argv);
cmd_function_t bme(p_shell_context_t Shell,int32_t argc, char **argv);
cmd_function_t rtd(p_shell_context_t Shell,int32_t argc, char **argv);

#endif /* SYSTEM_SHELL_RTOS_SHELL_COMMANDS__CORE_H_ */
