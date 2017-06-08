#include "CppUTest/TestHarness.h"

/** @file Test output routines.
 *
 * The overall strategy is to redirect output to
 * a file and then seek/read back file contents to
 * compare with expected output.
 */

extern "C" {
/*
	 * You can add your c-only include files here
	 */
#include <stdio.h>
#include "txtutil.h"
}

TEST_GROUP(txtutil){
    void setup(){}

    void teardown(){}};

TEST(txtutil, test_endLine)
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
TEST(txtutil, test_formatLine)
{
    // test formatLine
    STRCMP_EQUAL("00000000  00 65 6c 6c 6f                                    |.ello|", formatLine(0, "\0ello", 5))
    STRCMP_EQUAL("00000000  68 65 6c 6c 6f 20 77 6f  72 6c 64                 |hello world|", formatLine(0, "hello world", 11))
    STRCMP_EQUAL("00000000  68 65 6c 6c 6f 20 77 6f  72 6c 64 20 61 67 61 69  |hello world agai|", formatLine(0, "hello world agai", 16))
    STRCMP_EQUAL("0000000a", formatLine(10, "", 0))

    // check all possible data values
    char testStr[] = "hello";
    char testOut[] = "00000000  00 65 6c 6c 6f                                    |.ello|";
    for (int i = 0; i < 256; i++)
    {
        testStr[0] = i;
        sprintf(testOut, "00000000  %2.2x 65 6c 6c 6f                                    |%cello|", (unsigned int)(i & 0xFF), isprint(i) ? i : '.');
        // fprintf(stderr, "%s\n", formatLine(0, testStr, 5)); // eyeball output
        STRCMP_EQUAL(testOut, formatLine(0, testStr, 5));
    }
}

// code for capturing output of the dump() API for testing purposes
static FILE *fp;
static FILE *oldFp;
#define BUF_SIZE (1024*16)
char buf[BUF_SIZE];
const char *scratchFileName = "/tmp/TDD_test_file.txt";

TEST_GROUP(dump){

    void setup(){
        fp = fopen(scratchFileName, "w+");
        if (fp == NULL)
        {
            fprintf(stderr, "Cannot open \"%s\"\n", scratchFileName);
            exit(1);
        }
        oldFp = setStdout(fp);
    }

    void teardown()
    {
        setStdout(oldFp);
        fclose(fp);
    }
};

const char *getOut(void) {
    fseek(fp, 0, SEEK_SET);
    fread(buf, BUF_SIZE, 1, fp);
    fseek(fp, 0, SEEK_SET);
    return buf;
}

TEST(dump, test_dump)
{
    // test dump
    dump("hello", 5);
    STRCMP_EQUAL("00000000  68 65 6c 6c 6f                                    |hello|\n00000005\n", getOut())
    // dump("\00ello", 5);
    // STRCMP_EQUAL("00000000  00 65 6c 6c 6f                                    |.ello|", getOut())
    // STRCMP_EQUAL("00000000  68 65 6c 6c 6f 20 77 6f  72 6c 64                 |hello world|", getOut())
    // // STRCMP_EQUAL("00000000  68 65 6c 6c 6f 20 77 6f  72 6c 64 20 61 67 61 69  |hello world agai|", dump("hello world agai", 16))
    // // STRCMP_EQUAL("0000000a                                                    ||", dump("", 0))

}
