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
#include <ctype.h>

#include "txtutil.h"

static FILE *fp = NULL;
static int count = 16;
#define COUNT 16

#define LINE_BUF_LEN 100

// min() macro from http://stackoverflow.com/questions/3437404/min-and-max-in-c
#define min(x, y) ( \
    { __auto_type __x = (x); __auto_type __y = (y); \
      __x < __y ? __x : __y; })

/** @brief change output FILE *
 *
 * @param New FILE* to use for output
 * @retval previous FILE*
 */
FILE *setStdout(FILE *newFp)
{
    FILE *prevFp = fp;
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
 * (TODO: Add buffer to parameter to save a memory copy)
 */
const char *endLine(const char *str)
{
    static char line[LINE_BUF_LEN];
    int len = strlen(str); // length of incoming string
    int lenSoFar = 0;

    // adding spaces so ASCII representation lines up with previous lines
    line[0] = '\0'; // init buffer
    for (int i = len; i < count; i++)
    {
        strncat(line + lenSoFar, "   ", LINE_BUF_LEN - lenSoFar);
        lenSoFar += 3;
    }

    if (len <= count / 2)
    { // need extra midline ' '?
        strncat(line + lenSoFar, " ", LINE_BUF_LEN - lenSoFar);
        lenSoFar++;
    }

    strncat(line + lenSoFar, "  |", LINE_BUF_LEN - lenSoFar);
    lenSoFar += 2;
    strncat(line + lenSoFar, str, min(LINE_BUF_LEN - lenSoFar, count));
    lenSoFar += min(len, count);
    strncat(line + lenSoFar, "|", LINE_BUF_LEN - lenSoFar);

    return line;
}

/** @brief format a line of the 'hexdump' output
 * @param index of the first byte in the data.
 * @param pointer to the data
 * @param number of bytes of valid data in data
 * @retval formated line.
 *
 * @note data can contain null and non-printable values.
 * 
 */
const char *formatLine(int byteIndex, const char *data, int dataLen)
{
    char asciiRep[COUNT + 1];
    static char line[LINE_BUF_LEN];
    int bytes = min(dataLen, count); // ignore surplus data
    int stringLen;

    asciiRep[0] = '\0'; // init the string
    snprintf(line, LINE_BUF_LEN, "%8.8x ", byteIndex);

    for (int i = 0; i < bytes; i++)
    {
        stringLen = strlen(line);
        snprintf(line + stringLen, LINE_BUF_LEN - stringLen, " %2.2x", (unsigned int)(data[i] & 0xFF));
        if (i == count / 2 - 1)
        {
            line[stringLen + 3] = ' ';
            line[stringLen + 4] = '\0';
        }
        if (isprint(data[i])) // add character or '.'
            asciiRep[i] = data[i];
        else
            asciiRep[i] = '.';
        asciiRep[i + 1] = '\0'; // terminate string
    }

    if(dataLen > 0)
        strncat(line, endLine(asciiRep), LINE_BUF_LEN);
    else
        line[8] = '\0'; // truncate trailing ' '

    return line;
}

void dump(const char* data, size_t dataLen)
{
    int     i=0;

    // print full lines if there is more than one line of data
    if(dataLen >= count)
        for(i=0; i<dataLen-count; i += count)
            fprintf(fp, "%s\n", formatLine(i, data+i, count));

    // fprintf(stderr, "mod %ld, i %d\n", dataLen%count, i);

    // print partial line when lest than a line's worth of data is left
    if(dataLen%count != 0) {
        fprintf(fp, "%s\n", formatLine(i, data+i, dataLen%count));
        i += dataLen%count;
    }

    // print trailing line
    fprintf(fp, "%s\n", formatLine(i, data+i, 0));
    return;
}