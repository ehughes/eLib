/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of Freescale Semiconductor, Inc. nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * POSIX getopt for Windows
 * Code given out at the 1985 UNIFORUM conference in Dallas.
 *
 * From std-unix@ut-sally.UUCP (Moderator, John Quarterman) Sun Nov  3 14:34:15 1985
 * Relay-Version: version B 2.10.3 4.3bsd-beta 6/6/85; site gatech.CSNET
 * Posting-Version: version B 2.10.2 9/18/84; site ut-sally.UUCP
 * Path: gatech!akgua!mhuxv!mhuxt!mhuxr!ulysses!allegra!mit-eddie!genrad!panda!talcott!harvard!seismo!ut-sally!std-unix
 * From: std-unix@ut-sally.UUCP (Moderator, John Quarterman)
 * Newsgroups: mod.std.unix
 * Subject: public domain AT&T getopt source
 * Message-ID: <3352@ut-sally.UUCP>
 * Date: 3 Nov 85 19:34:15 GMT
 * Date-Received: 4 Nov 85 12:25:09 GMT
 * Organization: IEEE/P1003 Portable Operating System Environment Committee
 * Lines: 91
 * Approved: jsq@ut-sally.UUC
 * Here's something you've all been waiting for:  the AT&T public domain
 * source for getopt(3).  It is the code which was given out at the 1985
 * UNIFORUM conference in Dallas.  I obtained it by electronic mail
 * directly from AT&T.  The people there assure me that it is indeed
 * in the public domain
 * There is no manual page.  That is because the one they gave out at
 * UNIFORUM was slightly different from the current System V Release 2
 * manual page.  The difference apparently involved a note about the
 * famous rules 5 and 6, recommending using white space between an option
 * and its first argument, and not grouping options that have arguments.
 * Getopt itself is currently lenient about both of these things White
 * space is allowed, but not mandatory, and the last option in a group can
 * have an argument.  That particular version of the man page evidently
 * has no official existence, and my source at AT&T did not send a copy.
 * The current SVR2 man page reflects the actual behavor of this getopt.
 * However, I am not about to post a copy of anything licensed by AT&T.
 */

#include <assert.h>
#include "SHELL.h"
#include <stdio.h>
#include <stdarg.h>
#include "string.h"
#include "stdbool.h"


/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define KEY_ESC (0x1BU)
#define KET_DEL (0x7FU)

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

static int32_t ParseLine(p_shell_context_t ThisShellContext, const char *cmd, uint32_t len, char *argv[SHELL_MAX_ARGS]); /*!< parse line command */

static int32_t StrCompare(const char *str1, const char *str2, int32_t count); /*!< compare string command */

static void ProcessCommand(p_shell_context_t ThisShellContext, const char *cmd); /*!< process a command */

static void GetHistoryCommand(p_shell_context_t ThisShellContext, uint8_t hist_pos); /*!< get commands history */

static void AutoComplete(p_shell_context_t ThisShellContext); /*!< auto complete command */

static int32_t StrLen(const char *str); /*!< get string length */

static char *StrCopy(char *dest, const char *src, int32_t count); /*!< string copy */

/*******************************************************************************
 * Variables
 ******************************************************************************/




int32_t SHELL_printf(const shell_context_struct * Shell,const char *FormatString,...)
{

    char StringBuffer[SHELL_PRINTF_MAX_LEN];

    va_list argptr;
    va_start(argptr,FormatString);
    vsnprintf((char *)StringBuffer,SHELL_PRINTF_MAX_LEN,FormatString,argptr);
    va_end(argptr);

    for(int i=0;i<strnlen(StringBuffer,SHELL_PRINTF_MAX_LEN);i++)
    {
        ByteEnqueue(Shell->ShellOutQueue, StringBuffer[i]);
    }

    return 0;
}

int32_t SHELL_SendBuffer(const shell_context_struct * Shell,uint8_t *Buffer,uint32_t BufferLength)
{

    for(int i=0;i<BufferLength;i++)
    {
        ByteEnqueue(Shell->ShellOutQueue, Buffer[i]);
    }

    return 0;
}

int32_t SHELL_SendByte(const shell_context_struct * Shell,uint8_t Byte)
{
    ByteEnqueue(Shell->ShellOutQueue, Byte);
    return 0;
}

uint32_t Shell_CheckBreakCharacter(const shell_context_struct * Shell)
{
    if(BytesInQueue(Shell->ShellInQueue) > 0)
    {
        uint8_t tmp;

        ByteDequeue(Shell->ShellInQueue,&tmp);

        return true;
    }
    else
    {
        return false;
    }
}

void SHELL_Main(p_shell_context_t Shell)
{
    uint8_t ch;
    int32_t i;

    if (!Shell)
    {
        return;
    }

        while(BytesInQueue(Shell->ShellInQueue))
        {
                ByteDequeue(Shell->ShellInQueue,&ch);

                /* Special key */
                if (ch == KEY_ESC)
                {
                    Shell->stat = kSHELL_Special;
                    continue;
                }
                else if (Shell->stat == kSHELL_Special)
                {
                    /* Function key */
                    if (ch == '[')
                    {
                        Shell->stat = kSHELL_Function;
                        continue;
                    }
                    Shell->stat = kSHELL_Normal;
                }
                else if (Shell->stat == kSHELL_Function)
                {
                    Shell->stat = kSHELL_Normal;

                    switch ((uint8_t)ch)
                    {
                        /* History operation here */
                        case 'A': /* Up key */
                            GetHistoryCommand(Shell, Shell->hist_current);
                            if (Shell->hist_current < (Shell->hist_count - 1))
                            {
                                Shell->hist_current++;
                            }
                            break;
                        case 'B': /* Down key */
                            GetHistoryCommand(Shell, Shell->hist_current);
                            if (Shell->hist_current > 0)
                            {
                                Shell->hist_current--;
                            }
                            break;
                        case 'D': /* Left key */
                            if (Shell->c_pos)
                            {
                                SHELL_printf(Shell,"\b");
                                Shell->c_pos--;
                            }
                            break;
                        case 'C': /* Right key */
                            if (Shell->c_pos < Shell->l_pos)
                            {
                                if(Shell->echo){SHELL_printf(Shell,"%c", Shell->line[Shell->c_pos]);};
                                Shell->c_pos++;
                            }
                            break;
                        default:
                            break;
                    }
                    continue;
                }
                /* Handle tab key */
                else if (ch == '\t')
                {
        #if SHELL_AUTO_COMPLETE
                    /* Move the cursor to the beginning of line */
                    for (i = 0; i < Shell->c_pos; i++)
                    {
                        SHELL_printf(Shell,"\b");
                    }
                    /* Do auto complete */
                    AutoComplete(Shell);
                    /* Move position to end */
                    Shell->c_pos = Shell->l_pos = StrLen(Shell->line);
        #endif
                    continue;
                }
        #if SHELL_SEARCH_IN_HIST
                /* Search command in history */
                else if ((ch == '`') && (Shell->l_pos == 0) && (Shell->line[0] == 0x00))
                {
                }
        #endif
                /* Handle backspace key */
                else if ((ch == KET_DEL) || (ch == '\b'))
                {
                    /* There must be at last one char */
                    if (Shell->c_pos == 0)
                    {
                        continue;
                    }

                    Shell->l_pos--;
                    Shell->c_pos--;

                    if (Shell->l_pos > Shell->c_pos)
                    {
                        memmove(&Shell->line[Shell->c_pos], &Shell->line[Shell->c_pos + 1],
                                Shell->l_pos - Shell->c_pos);
                        Shell->line[Shell->l_pos] = 0;

                        if(Shell->echo){SHELL_printf(Shell,"\b%s  \b", &Shell->line[Shell->c_pos]);};

                        /* Reset position */
                        for (i = Shell->c_pos; i <= Shell->l_pos; i++)
                        {
                            if(Shell->echo){SHELL_printf(Shell,"\b");};

                        }
                    }
                    else /* Normal backspace operation */
                    {
                        if(Shell->echo){SHELL_printf(Shell,"\b \b");};
                        Shell->line[Shell->l_pos] = 0;
                    }
                    continue;
                }
                else
                {
                }

                /* Input too long */
                if (Shell->l_pos >= (SHELL_BUFFER_SIZE - 1))
                {
                    Shell->l_pos = 0;
                }

                /* Handle end of line, break */
                if ((ch == '\r') || (ch == '\n'))
                {
                    if(Shell->echo){SHELL_printf(Shell,"\r\n");};
                    ProcessCommand(Shell, Shell->line);
                    /* Reset all params */
                    Shell->c_pos = Shell->l_pos = 0;
                    Shell->hist_current = 0;
                    if(Shell->echo){SHELL_printf(Shell,Shell->prompt);};
                    memset(Shell->line, 0, sizeof(Shell->line));
                    continue;
                }

                /* Normal character */
                if (Shell->c_pos < Shell->l_pos)
                {
                    memmove(&Shell->line[Shell->c_pos + 1], &Shell->line[Shell->c_pos],
                            Shell->l_pos - Shell->c_pos);
                    Shell->line[Shell->c_pos] = ch;

                    if(Shell->echo){SHELL_printf(Shell,"%s", &Shell->line[Shell->c_pos]);};
                    /* Move the cursor to new position */
                    for (i = Shell->c_pos; i < Shell->l_pos; i++)
                    {
                        if(Shell->echo){SHELL_printf(Shell,"\b");};
                    }
                }
                else
                {
                    Shell->line[Shell->l_pos] = ch;
                    if(Shell->echo){SHELL_printf(Shell,"%c", ch);}
                }

                ch = 0;
                Shell->l_pos++;
                Shell->c_pos++;
        }

}


static void ProcessCommand(p_shell_context_t ThisShellContext, const char *cmd)
{
    static const shell_command_context_t *tmpCommand = NULL;
    static const char *tmpCommandString;
    int32_t argc;
    char *argv[SHELL_BUFFER_SIZE];
    uint8_t flag = 0;
    uint8_t tmpCommandLen;
    uint8_t tmpLen;
    uint8_t i = 0;

    tmpLen = StrLen(cmd);
    argc = ParseLine(ThisShellContext,cmd, tmpLen, argv);

    if ((tmpCommand == NULL) && (argc > 0))
    {
        for (i = 0; i < ThisShellContext->ShellCommands.numberOfCommandInList; i++)
        {
            tmpCommand = &(ThisShellContext->ShellCommands.CommandList[i]);
            tmpCommandString = tmpCommand->pcCommand;
            tmpCommandLen = StrLen(tmpCommandString);


            /* Compare with space or end of string */
            if ((cmd[tmpCommandLen] == ' ') || (cmd[tmpCommandLen] == 0x00))
            {
                if (StrCompare(tmpCommandString, argv[0], tmpCommandLen) == 0)
                {
                    if (argc > 0)
                    {
                        flag = 1;
                        break;
                    }
                }
            }
        }
    }
    else
    {
        return;  //no command
    }

    if(flag == 0)
    {
        SHELL_printf(ThisShellContext,
                "\r\n\"%s\" Command not recognized.  Enter 'help' to view a list of available commands.\r\n\r\n",cmd);
            tmpCommand = NULL;
     }

    if(tmpCommand != NULL)
    {
        tmpLen = StrLen(cmd);
        /* Compare with last command. Push back to history buffer if different */
        if (tmpLen != StrCompare(cmd, ThisShellContext->hist_buf[0], StrLen(cmd)))
        {
            for (i = SHELL_HIST_MAX - 1; i > 0; i--)
            {
                memset(ThisShellContext->hist_buf[i], '\0', SHELL_BUFFER_SIZE);
                tmpLen = StrLen(ThisShellContext->hist_buf[i - 1]);
                StrCopy(ThisShellContext->hist_buf[i], ThisShellContext->hist_buf[i - 1], tmpLen);
            }
            memset(ThisShellContext->hist_buf[0], '\0', SHELL_BUFFER_SIZE);
            tmpLen = StrLen(cmd);
            StrCopy(ThisShellContext->hist_buf[0], cmd, tmpLen);
            if (ThisShellContext->hist_count < SHELL_HIST_MAX)
            {
                ThisShellContext->hist_count++;
            }
        }

        if(tmpCommand->pFuncCallBack == 0)
        {
            SHELL_printf(ThisShellContext,"\r\nCommand %s has a null callback\r\n\r\n",tmpCommand->pcCommand);
        }
        else
        {

                if(ThisShellContext->CurrentPrivilegeLevel >= tmpCommand->MinPrivilegeLevel)
                tmpCommand->pFuncCallBack((void *)ThisShellContext, argc, argv);
            else
                        SHELL_printf(ThisShellContext,"\r\nYou do not have privilege to run %s\r\n\r\n",tmpCommand->pcCommand);

            
        }

        tmpCommand = NULL;
    }

}

static void GetHistoryCommand(p_shell_context_t ThisShellContext, uint8_t hist_pos)
{
    uint8_t i;
    uint32_t tmp;

    if (ThisShellContext->hist_buf[0][0] == '\0')
    {
        ThisShellContext->hist_current = 0;
        return;
    }
    if (hist_pos > SHELL_HIST_MAX)
    {
        hist_pos = SHELL_HIST_MAX - 1;
    }
    tmp = StrLen(ThisShellContext->line);
    /* Clear current if have */
    if (tmp > 0)
    {
        memset(ThisShellContext->line, '\0', tmp);
        for (i = 0; i < tmp; i++)
        {
            SHELL_printf(ThisShellContext,"\b \b");
        }
    }

    ThisShellContext->l_pos = StrLen(ThisShellContext->hist_buf[hist_pos]);
    ThisShellContext->c_pos = ThisShellContext->l_pos;
    StrCopy(ThisShellContext->line, ThisShellContext->hist_buf[hist_pos], ThisShellContext->l_pos);

    if(ThisShellContext->echo){SHELL_printf(ThisShellContext,ThisShellContext->hist_buf[hist_pos]);};
}

static void AutoComplete(p_shell_context_t ThisShellContext)
{
    int32_t len;
    int32_t minLen;
    uint8_t i = 0;
    const shell_command_context_t *tmpCommand = NULL;
    const char *namePtr;
    const char *cmdName;

    minLen = 0;
    namePtr = NULL;

    if (!StrLen(ThisShellContext->line))
    {
        return;
    }
    if(ThisShellContext->echo){SHELL_printf(ThisShellContext,"\r\n");};
    /* Empty tab, list all commands */
    if (ThisShellContext->line[0] == '\0')
    {
        HelpCommand(ThisShellContext, 0, NULL);
        return;
    }
    /* Do auto complete */
    for (i = 0; i < ThisShellContext->ShellCommands.numberOfCommandInList; i++)
    {
        tmpCommand = &ThisShellContext->ShellCommands.CommandList[i];
        cmdName = tmpCommand->pcCommand;
        if (StrCompare(ThisShellContext->line, cmdName, StrLen(ThisShellContext->line)) == 0)
        {
            if (minLen == 0)
            {
                namePtr = cmdName;
                minLen = StrLen(namePtr);
                /* Show possible matches */
                if(ThisShellContext->echo){SHELL_printf(ThisShellContext,"%s\r\n", cmdName);};
                continue;
            }
            len = StrCompare(namePtr, cmdName, StrLen(namePtr));
            if (len < 0)
            {
                len = len * (-1);
            }
            if (len < minLen)
            {
                minLen = len;
            }
        }
    }
    /* Auto complete string */
    if (namePtr)
    {
        StrCopy(ThisShellContext->line, namePtr, minLen);
    }
    if(ThisShellContext->echo){SHELL_printf(ThisShellContext,"%s%s", ThisShellContext->prompt, ThisShellContext->line);};
    return;
}

static char *StrCopy(char *dest, const char *src, int32_t count)
{
    char *ret = dest;
    int32_t i = 0;

    for (i = 0; i < count; i++)
    {
        dest[i] = src[i];
    }

    return ret;
}

static int32_t StrLen(const char *str)
{
    int32_t i = 0;

    while (*str)
    {
        str++;
        i++;
    }
    return i;
}

static int32_t StrCompare(const char *str1, const char *str2, int32_t count)
{
    while (count--)
    {
        if (*str1++ != *str2++)
        {
            return *(unsigned char *)(str1 - 1) - *(unsigned char *)(str2 - 1);
        }
    }
    return 0;
}

static int32_t ParseLine(p_shell_context_t ThisShellContext,const char *cmd, uint32_t len, char *argv[SHELL_MAX_ARGS])
{
    uint32_t argc;
    char *p;
    uint32_t position;

    /* Init params */
    memset(ThisShellContext->g_paramBuffer, '\0', len + 1);
    StrCopy(ThisShellContext->g_paramBuffer, cmd, len);

    p = ThisShellContext->g_paramBuffer;
    position = 0;
    argc = 0;

    while (position < len)
    {
        /* Skip all blanks */
        while (((char)(*p) == ' ') && (position < len))
        {
            *p = '\0';
            p++;
            position++;
        }
        /* Process begin of a string */
        if (*p == '"')
        {
            p++;
            position++;
            argv[argc] = p;
            argc++;
            /* Skip this string */
            while ((*p != '"') && (position < len))
            {
                p++;
                position++;
            }
            /* Skip '"' */
            *p = '\0';
            p++;
            position++;
        }
        else /* Normal char */
        {
            argv[argc] = p;
            argc++;
            while (((char)*p != ' ') && ((char)*p != '\t') && (position < len))
            {
                p++;
                position++;
            }
        }
    }
    return argc;
}

int32_t SHELL_RegisterCommand(p_shell_context_t ThisShellContext,
                              char *pcCommand, 
                  char *pcHelpString,   
                              cmd_function_t  pFuncCallBack, 
                              uint32_t MinPrivilegeLevel    
                       )
{
    int32_t result = 0;

    /* If have room  in command list */
    if (ThisShellContext->ShellCommands.numberOfCommandInList < SHELL_MAX_CMD)
    {
        ThisShellContext->ShellCommands.CommandList[ThisShellContext->ShellCommands.numberOfCommandInList].pcCommand = pcCommand;
        ThisShellContext->ShellCommands.CommandList[ThisShellContext->ShellCommands.numberOfCommandInList].pcHelpString = pcHelpString;
        ThisShellContext->ShellCommands.CommandList[ThisShellContext->ShellCommands.numberOfCommandInList].pFuncCallBack = pFuncCallBack;
        ThisShellContext->ShellCommands.CommandList[ThisShellContext->ShellCommands.numberOfCommandInList].MinPrivilegeLevel = MinPrivilegeLevel;
        ThisShellContext->ShellCommands.numberOfCommandInList++;
    }
    else
    {
        result = -1;
    }
    return result;
}


cmd_function_t HelpCommand(p_shell_context_t ThisShellContext, int32_t argc, char **argv)
{
    uint32_t i = 0;

    SHELL_printf(ThisShellContext,"\r\nCommands:");
    SHELL_printf(ThisShellContext,"\r\n------------------------\r\n\r\n");

    for (i = 0; i < ThisShellContext->ShellCommands.numberOfCommandInList; i++)
    {
        Delay_mS(10);
        SHELL_printf(ThisShellContext,VT100_YELLOW);

        
        SHELL_printf(ThisShellContext,"%s",
                    ThisShellContext->ShellCommands.CommandList[i].pcCommand);


        SHELL_printf(ThisShellContext,VT100_WHITE);

        SHELL_printf(ThisShellContext," > ");

        SHELL_printf(ThisShellContext,VT100_CYAN);

        SHELL_printf(ThisShellContext,"%s",
                        ThisShellContext->ShellCommands.CommandList[i].pcHelpString);

        SHELL_printf(ThisShellContext,"\r\n");
    }

    SHELL_printf(ThisShellContext,VT100_DEFAULT);
    SHELL_printf(ThisShellContext,"\r\n");
    return 0;
}

