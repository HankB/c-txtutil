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
static FILE *fp = NULL;

/** @brief change output FILE *
 *
 * @param New FILE* to use for output
 * @retval previous FILE*
 */
FILE *setLogout(FILE *newFp)
{
    // init fp if needed
    if(fp == NULL)
        fp = stdout;
    FILE *prevFp = fp;
    fp = newFp;
    return prevFp;
}

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

/** @brief send logging output if priority met
 * 
 * @param Assigned priority for this message
 * @param Format string (as with printf())
 * @retval Required priority
 */
int logmsg(int pri, const char *format, ...)
{
    if(pri >= reqdLogPri)
    {
        va_list argp;
        va_start(argp, format);
        int retval = vfprintf(fp, format, argp);
        va_end(argp);
        return retval;
    }
    else
    {
        return 0;
    }
}
