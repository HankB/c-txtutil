/** @file logmsg.c - print output to console or file similarly to
 * printf() except that a priority is provided as the first argument
 * and the priority provided is comnpared to a preset level to determine
 * if the output is actually printed. The output file descriptor
 * defaults to stdout and can be changed to something else.
 *
 * Any message passed with a priority >= required priority 
 */

#include <stdio.h>
#include <stdarg.h>

#include "txtutil.h"

static int    reqdLogPri = 3;

/** @brief Retrieve present priority level
 * 
 * @retval Required priority
 */
int getLogPri(void)
{
    return reqdLogPri;
}

/** @brief Retrieve present priority level
 * 
 * @param New required priority
 * @retval Required priority
 */
int setLogPri(int newPri)
{
    int prevPri = reqdLogPri;
    reqdLogPri = newPri;
    return prevPri;
}