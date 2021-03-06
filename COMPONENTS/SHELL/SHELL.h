#ifndef _FSL_SHELL_H_
#define _FSL_SHELL_H_


#include "SHELL_Config.h"
#include "Components/DATA_STRUCT/QUEUE/eQueue.h"


#define VT100_RED     "\033[31;40m"
#define VT100_GREEN   "\033[32;40m"
#define VT100_YELLOW  "\033[33;40m"
#define VT100_BLUE    "\033[34;40m"
#define VT100_MAGENTA "\033[35;40m"
#define VT100_CYAN    "\033[36;40m"
#define VT100_WHITE   "\033[37;40m"

#define VT100_DEFAULT  VT100_WHITE
/*!
 * @addtogroup SHELL
 * @{
 */

/*! @brief A type for the handle special key. */
typedef enum _fun_key_status
{
    kSHELL_Normal = 0U,   /*!< Normal key */
    kSHELL_Special = 1U,  /*!< Special key */
    kSHELL_Function = 2U, /*!< Function key */
} fun_key_status_t;

/*! @brief User command function prototype. */
typedef int32_t (*cmd_function_t)(const void * shell_context, int32_t argc, char **argv);

/*! @brief User command data structure. */
typedef struct _shell_command_context
{
    char *pcCommand; /*!< The command that is executed.  For example "help".  It must be all lower case. */
    char *pcHelpString;    /*!< String that describes how to use the command.  It should start with the command itself,
                                    and end with "\r\n".  For example "help: Returns a list of all the commands\r\n". */
    cmd_function_t  pFuncCallBack; /*!< A pointer to the callback function that returns the output generated by the command. */
    
    uint32_t MinPrivilegeLevel;


} shell_command_context_t;

/*! @brief Structure list command. */
typedef struct _shell_command_context_list
{
    shell_command_context_t CommandList[SHELL_MAX_CMD]; /*!< The command table list */
    uint8_t numberOfCommandInList;                             /*!< The total command in list */
} shell_command_context_list_t;


/*! @brief Data structure for Shell environment. */
typedef struct _shell_context_struct
{
    char *prompt;                 /*!< Prompt string */
    enum _fun_key_status stat;    /*!< Special key status */
    char *Description;            /*!< A textual description of the shell*/
    char line[SHELL_BUFFER_SIZE]; /*!< Consult buffer */
    uint32_t                    CurrentPrivilegeLevel;
    uint8_t cmd_num;              /*!< Number of user commands */
    uint8_t l_pos;                /*!< Total line position */
    uint8_t c_pos;                /*!< Current line position */
    uint8_t echo;                                 /*Determines if characters are echoed*/
    uint8_t QuietOnBadCommand;                                 /*Determines if characters are echoed*/
    

    ByteQueue * ShellInQueue;   /*!<This input queue is checked from shell input*/
    ByteQueue * ShellOutQueue; /*!<This output queue is what the shell writes to*/

    uint16_t hist_current;                            /*!< Current history command in hist buff*/
    uint16_t hist_count;                              /*!< Total history command in hist buff*/
    char hist_buf[SHELL_HIST_MAX][SHELL_BUFFER_SIZE]; /*!< History buffer*/

    char  g_paramBuffer[SHELL_BUFFER_SIZE];



    shell_command_context_list_t        ShellCommands;

} shell_context_struct, *p_shell_context_t;

cmd_function_t HelpCommand(p_shell_context_t ThisShellContext, int32_t argc, char **argv); /*!< help command */


/***
 *      _______  _______ _____ ____  _   _    _    _
 *     | ____\ \/ /_   _| ____|  _ \| \ | |  / \  | |
 *     |  _|  \  /  | | |  _| | |_) |  \| | / _ \ | |
 *     | |___ /  \  | | | |___|  _ <| |\  |/ ___ \| |___
 *     |_____/_/\_\ |_|_|_____|_| \_\_| \_/_/ __\_\_____|_____ _______  _______
 *     / ___|| | | | ____| |   | |      / ___/ _ \| \ | |_   _| ____\ \/ /_   _|
 *     \___ \| |_| |  _| | |   | |     | |  | | | |  \| | | | |  _|  \  /  | |
 *      ___) |  _  | |___| |___| |___  | |__| |_| | |\  | | | | |___ /  \  | |
 *     |____/|_|_|_|_____|_____|_____|__\____\___/|_| \_| |_| |_____/_/\_\ |_|
 *        / \  / ___/ ___| ____/ ___/ ___|
 *       / _ \| |  | |   |  _| \___ \___ \
 *      / ___ \ |__| |___| |___ ___) |__) |
 *     /_/   \_\____\____|_____|____/____/
 *
 */


extern shell_context_struct MySerialShell;
extern shell_context_struct MyCloudShell;

/*******************************************************************************
 * API
 ******************************************************************************/

#if defined(__cplusplus)
extern "C" {
#endif /* _cplusplus */



int32_t SHELL_RegisterCommand(p_shell_context_t ThisShellContext,
                              char *pcCommand, 
                              char *pcHelpString,   
                              cmd_function_t  pFuncCallBack, 
                              uint32_t MinPrivilegeLevel        
                              );

int32_t SHELL_printf(const shell_context_struct * Shell,const char *FormatString,...);

int32_t SHELL_SendBuffer(const shell_context_struct * Shell,uint8_t *Buffer,uint32_t BufferLength);

int32_t SHELL_SendByte(const shell_context_struct * Shell,uint8_t Byte);

uint32_t Shell_CheckBreakCharacter(const shell_context_struct * Shell);

void SHELL_Main(p_shell_context_t Shell);


/* @} */

void Init_Shell();

void Init_Shell_IO();

void Shell_MoveQueues();

void DeInit_Shell();


#if defined(__cplusplus)
}
#endif

/*! @}*/

#endif /* _FSL_SHELL_H_ */
