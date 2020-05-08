
#ifndef SYSTEM_SHELL_RTOS_SHELL_COMMANDS__FILESYSTEM_H_
#define SYSTEM_SHELL_RTOS_SHELL_COMMANDS__FILESYSTEM_H_


cmd_function_t touch(p_shell_context_t Shell,int32_t argc, char **argv);
cmd_function_t format(p_shell_context_t Shell,int32_t argc, char **argv);
cmd_function_t ls(p_shell_context_t Shell,int32_t argc, char **argv);
cmd_function_t flash_sector_read(p_shell_context_t Shell,int32_t argc, char **argv);
cmd_function_t flash_info(p_shell_context_t Shell,int32_t argc, char **argv);

cmd_function_t test_flash(p_shell_context_t Shell,int32_t argc, char **argv);
cmd_function_t erase_flash(p_shell_context_t Shell,int32_t argc, char **argv);
cmd_function_t test_fs(p_shell_context_t Shell,int32_t argc, char **argv);
cmd_function_t more(p_shell_context_t Shell,int32_t argc, char **argv);
cmd_function_t rm(p_shell_context_t Shell,int32_t argc, char **argv);
cmd_function_t check_fs(p_shell_context_t Shell,int32_t argc, char **argv);

cmd_function_t Remote_fcreate(p_shell_context_t Shell,int32_t argc, char **argv);
cmd_function_t Remote_fwrite(p_shell_context_t Shell,int32_t argc, char **argv);
cmd_function_t Remote_fwriteline(p_shell_context_t Shell,int32_t argc, char **argv);
cmd_function_t md5check(p_shell_context_t Shell,int32_t argc, char **argv);
cmd_function_t file_rename(p_shell_context_t Shell,int32_t argc, char **argv);

cmd_function_t Remote_fwriteb64(p_shell_context_t Shell,int32_t argc, char **argv);

#endif /* SYSTEM_SHELL_RTOS_SHELL_COMMANDS__FILESYSTEM_H_ */
