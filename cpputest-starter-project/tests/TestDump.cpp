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

TEST(txtutil, test_endLine){
    // test endLine
    STRCMP_EQUAL("                 |hello world|", endLine("hello world"));
    STRCMP_EQUAL("                                             |hd|", endLine("hd"));
    STRCMP_EQUAL("                                                   ||", endLine(""));
    STRCMP_EQUAL("                           |hello wo|", endLine("hello wo"));
    STRCMP_EQUAL("                       |hello wor|", endLine("hello wor"));
    STRCMP_EQUAL("  |hello world agai|", endLine("hello world agai"));
    STRCMP_EQUAL("  |hello world agai|", endLine("hello world again")); // pathological case
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
#define BUF_SIZE (1024 * 16)
char buf[BUF_SIZE];
const char *scratchFileName = "/tmp/TDD_test_file.txt";
const char *scratchPrevFileName = "/tmp/prevTDD_test_file.txt";

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
}
;

const char *getOut(void)
{
    size_t bytesRead;
    fseek(fp, 0, SEEK_SET);                  // sek to start of file
    bytesRead = fread(buf, 1, BUF_SIZE, fp); // read contents
    buf[bytesRead] = '\0';                   // null terminate the input
    fclose(fp);                              // close the file
    if (rename(scratchFileName, scratchPrevFileName))
    { // rename so we can examine contents (if necessary)
        fprintf(stderr, "file rename error\n");
        exit(1);
    }
    fp = fopen(scratchFileName, "w+"); // open the file again
    if (fp == NULL)
    {
        fprintf(stderr, "Cannot open \"%s\"\n", scratchFileName);
        exit(1);
    }
    return buf;
}

TEST(dump, test_dump)
{
    // test dump
    dump("hello", 5);
    STRCMP_EQUAL("00000000  68 65 6c 6c 6f                                    |hello|\n00000005\n", getOut())
    dump("\00ello", 5);
    STRCMP_EQUAL("00000000  00 65 6c 6c 6f                                    |.ello|\n00000005\n", getOut())
    dump("hello world", 11);
    STRCMP_EQUAL("00000000  68 65 6c 6c 6f 20 77 6f  72 6c 64                 |hello world|\n0000000b\n", getOut())
    dump("hello world aga", 15);
    STRCMP_EQUAL("00000000  68 65 6c 6c 6f 20 77 6f  72 6c 64 20 61 67 61     |hello world aga|\n0000000f\n", getOut())
    dump("hello world agai", 16);
    STRCMP_EQUAL("00000000  68 65 6c 6c 6f 20 77 6f  72 6c 64 20 61 67 61 69  |hello world agai|\n00000010\n", getOut())
    dump("hello world again", 17);
    STRCMP_EQUAL("00000000  68 65 6c 6c 6f 20 77 6f  72 6c 64 20 61 67 61 69  |hello world agai|\n00000010  6e                                                |n|\n00000011\n", getOut())
    dump("hello world agai", 16);
    STRCMP_EQUAL("00000000  68 65 6c 6c 6f 20 77 6f  72 6c 64 20 61 67 61 69  |hello world agai|\n00000010\n", getOut())

// Now a much bigger buffer
#define BUFLEN 256
    char buf[BUFLEN];
    for (int i = 0; i < BUFLEN; i++)
        buf[i] = i;
        
    dump(buf, 16);
    STRCMP_EQUAL("00000000  00 01 02 03 04 05 06 07  08 09 0a 0b 0c 0d 0e 0f  |................|\n00000010\n", getOut())
}
