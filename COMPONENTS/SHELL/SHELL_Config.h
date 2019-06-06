/*
 * RTOS_SHELL_Config.h
 *
 *  Created on: Sep 3, 2016
 *      Author: ehughes
 */

#ifndef SHELL_CONFIG_H_
#define SHELL_CONFIG_H_

/*!
 * @addtogroup SHELL
 * @{
 */


/*! @brief this macro sets the maximum working buffer for the shell printf function to ensure thread safety
 *         the working buffer is allocated on the stack of the task calling the function.    Any task
 *         using SHELL_printf needs enough extra stack space to cover this
 */
#ifndef     SHELL_PRINTF_MAX_LEN
#define   SHELL_PRINTF_MAX_LEN      (256U)
#endif

/*! @brief This macro sets the time the shell will waiting when checking mutexes and enqueuing data
 */

#ifndef     SHELL_QUEUE_WAIT_TIME_MS
#define     SHELL_QUEUE_WAIT_TIME_MS        (2000U)
#endif


/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*! @brief Macro to set on/off history feature. */
#ifndef SHELL_USE_HISTORY
#define SHELL_USE_HISTORY (0U)
#endif

/*! @brief Macro to set on/off history feature. */
#ifndef SHELL_SEARCH_IN_HIST
#define SHELL_SEARCH_IN_HIST (1U)
#endif

/*! @brief Macro to select method stream. */
#ifndef SHELL_USE_FILE_STREAM
#define SHELL_USE_FILE_STREAM (0U)
#endif

/*! @brief Macro to set on/off auto-complete feature. */
#ifndef SHELL_AUTO_COMPLETE
#define SHELL_AUTO_COMPLETE (1U)
#endif

/*! @brief Macro to set console buffer size. */
#ifndef SHELL_BUFFER_SIZE
#define SHELL_BUFFER_SIZE (256U)
#endif

/*! @brief Macro to set maximum arguments in command. */
#ifndef SHELL_MAX_ARGS
#define SHELL_MAX_ARGS (8U)
#endif

/*! @brief Macro to set maximum count of history commands. */
#ifndef SHELL_HIST_MAX
#define SHELL_HIST_MAX (4U)
#endif

/*! @brief Macro to set maximum count of commands. */
#ifndef SHELL_MAX_CMD
#define SHELL_MAX_CMD (64)
#endif

/* @} */

#endif /* SOURCE_SYSTEM_RTOS_SHELL_RTOS_SHELL_CONFIG_H_ */
