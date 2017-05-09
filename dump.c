/** @file dump.c - dump binary or ASCII data to the console. Output
    format inspired by the BSD 'hexdump -C' output format]

*/

#include <stdint.h>
#include <stdio.h>

#include "txtutil.h"

static FILE* fp = NULL;
/** @brief change output FILE *
*
* @param New FILE* to use for output
* @retval previous FILE*
*/
FILE* setStdout(FILE* newFp) {
    FILE* prevFp = fp;
    fp = newFp;
    return prevFp;
}

int endLine(const char *str, unsigned int len) {
    if(fp == NULL) fp = stdout; // set default value for fp
    return fprintf(fp, "abc\n");
}
