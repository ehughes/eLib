#include "System.h"

int wolfcrypt_test(void* args);

cmd_function_t wolf_test(p_shell_context_t Shell,int32_t argc, char **argv)
{

    wolfcrypt_test(NULL);

    return 0;
     
}
