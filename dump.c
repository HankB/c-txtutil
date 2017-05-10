/** @file dump.c - dump binary or ASCII data to the console. Output
 * format inspired by the BSD 'hexdump -C' output format]
 * For example
 * <pre>
hbarta@swanky:~/Documents/go-wks/src/github.com/HankB/playground$ hexdump -C ~/go/bin/go|tail -3
0097e1f0  5f 73 74 61 72 74 00 63  72 6f 73 73 63 61 6c 6c  |_start.crosscall|
0097e200  5f 61 6d 64 36 34 00                              |_amd64.|
0097e207
hbarta@swanky:~/Documents/go-wks/src/github.com/HankB/playground$
 * </pre>
*/

#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "txtutil.h"

static FILE* fp = NULL;
static int count = 16;


#define LINE_BUF_LEN        100

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

/** @brief finish line formatting and output
 * endLine has two jobs. First pad remainder of line if <16
 * bytes have been printed and then pad and add the ascii
 * representation (and '\n') This is part of the code that will print
 * binary strings using a format like 'hexdump -C'
 * @param string with 'ascii representation' of line 
 * (one character per byte)
 * @retval formatted string
 * 
 * @note A static buffer is used to hold the returned string
 * which will be overwritten by subsequent calls.
 */
const char * endLine(const char *str) {
    int len = strlen(str);  // length of incoming string
    static char line[LINE_BUF_LEN];

    // adding spaces so ASCII representation lines up with previous lines
    int lenSoFar = 0;
    for(int i=len; i<count; i++) {
        strncat(line+lenSoFar, "   ", LINE_BUF_LEN-lenSoFar);
        lenSoFar += 3;  
    }

    if(len < count/2)  {    // need extra midline ' '?
        strncat(line+lenSoFar, " ", LINE_BUF_LEN-lenSoFar);\
        lenSoFar++;
    }

    strncat(line+lenSoFar, "  |", LINE_BUF_LEN-lenSoFar);
    lenSoFar += 3;  
    strncat(line+lenSoFar, str, LINE_BUF_LEN-lenSoFar);
    lenSoFar += len;  
    strncat(line+lenSoFar, "|", LINE_BUF_LEN-lenSoFar);
    
    return line;
}
