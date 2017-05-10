#include "CppUTest/TestHarness.h"

/** @file Test output routines.
 *
 * The overall strategy is to redirect output to
 * a file and then seek/read back file contents to
 * compare with expected output.
 */

// size of buffer used to capture output
// #define BUF_SIZE    4096
// static char buf[BUF_SIZE];

extern "C" {
/*
	 * You can add your c-only include files here
	 */
#include <stdio.h>
#include "txtutil.h"
}

TEST_GROUP(Dump){
    void setup(){}

    void teardown(){}};

TEST(Dump, test_endLine)
{
    // test endLine
    STRCMP_EQUAL("                 |hello world|", endLine("hello world"))
    STRCMP_EQUAL("                                             |hd|", endLine("hd"))
    STRCMP_EQUAL("                                                   ||", endLine(""))
    STRCMP_EQUAL("                           |hello wo|", endLine("hello wo"))
    STRCMP_EQUAL("                       |hello wor|", endLine("hello wor"))
    STRCMP_EQUAL("  |hello world agai|", endLine("hello world agai"))
    STRCMP_EQUAL("  |hello world agai|", endLine("hello world again")) // pathological case
}
TEST(Dump, test_formatLine)
{
    // test formatLine
    STRCMP_EQUAL("00000000  00 65 6c 6c 6f                                    |.ello|", formatLine(0, "\0ello", 5))
    STRCMP_EQUAL("00000000  68 65 6c 6c 6f 20 77 6f  72 6c 64                 |hello world|", formatLine(0, "hello world", 11))
    STRCMP_EQUAL("00000000  68 65 6c 6c 6f 20 77 6f  72 6c 64 20 61 67 61 69  |hello world agai|", formatLine(0, "hello world agai", 16))
    STRCMP_EQUAL("0000000a                                                    ||", formatLine(10, "", 0))

    // check all possible data values
    char    testStr[] = "hello";
    char    testOut[] = "00000000  00 65 6c 6c 6f                                    |.ello|";
    for(int i=0; i<256; i++) {
        testStr[0] = i;
        sprintf(testOut, "00000000  %2.2x 65 6c 6c 6f                                    |%cello|", (unsigned int)(i & 0xFF), isprint(i)?i:'.');
        fprintf(stderr, "%s\n", formatLine(0, testStr, 5)); // eyeball output
        STRCMP_EQUAL(testOut, formatLine(0, testStr, 5));
    }
}
