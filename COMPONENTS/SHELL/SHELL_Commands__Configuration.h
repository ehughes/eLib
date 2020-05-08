
#ifndef  SHELL_COMMANDS__CONFIGURATION_H_
#define  SHELL_COMMANDS__CONFIGURATION_H_

cmd_function_t load_config(p_shell_context_t Shell,int32_t argc, char **argv);
cmd_function_t get(p_shell_context_t Shell,int32_t argc, char **argv);
cmd_function_t set(p_shell_context_t Shell,int32_t argc, char **argv);
cmd_function_t save_config(p_shell_context_t Shell,int32_t argc, char **argv);
cmd_function_t ee(p_shell_context_t Shell,int32_t argc, char **argv);
cmd_function_t br(p_shell_context_t Shell,int32_t argc, char **argv);
cmd_function_t mget(p_shell_context_t Shell,int32_t argc, char **argv);
cmd_function_t mset(p_shell_context_t Shell,int32_t argc, char **argv);
cmd_function_t msave(p_shell_context_t Shell,int32_t argc, char **argv);

#endif /* SYSTEM_SHELL_RTOS_SHELL_COMMANDS__FILESYSTEM_H_ */
