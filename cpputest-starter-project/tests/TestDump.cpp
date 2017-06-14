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
static char buf[BUF_SIZE];
static const char *scratchFileName = "/tmp/TDD_test_file.txt";
static const char *scratchPrevFileName = "/tmp/prevTDD_test_file.txt";

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

/**
 * Testing strategy for dump().
 * 1) Divert output to a disk file.
 * 2) Retrieve contents using getOut() (to compare with a string.)
 * 3) getOut() closes and renames file to save for examination
 *    and to clear file contents for next operation.
 *    getOut() opens new file for nest output operation.
 * NOTE: file names will clash if more than one user runs this test
 * at the same time!
 */
static const char *getOut(void)
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
    STRCMP_EQUAL(   "00000000  00 01 02 03 04 05 06 07  08 09 0a 0b 0c 0d 0e 0f  |................|\n"
                    "00000010\n",
                    getOut())
    dump(buf, 17);
    STRCMP_EQUAL(   "00000000  00 01 02 03 04 05 06 07  08 09 0a 0b 0c 0d 0e 0f  |................|\n"
                    "00000010  10                                                |.|\n"
                    "00000011\n", getOut())
    dump(buf, 32);
    STRCMP_EQUAL(   "00000000  00 01 02 03 04 05 06 07  08 09 0a 0b 0c 0d 0e 0f  |................|\n"
                    "00000010  10 11 12 13 14 15 16 17  18 19 1a 1b 1c 1d 1e 1f  |................|\n"
                    "00000020\n", getOut())
    dump(buf, 128);
    STRCMP_EQUAL(   "00000000  00 01 02 03 04 05 06 07  08 09 0a 0b 0c 0d 0e 0f  |................|\n"
                    "00000010  10 11 12 13 14 15 16 17  18 19 1a 1b 1c 1d 1e 1f  |................|\n"
                    "00000020  20 21 22 23 24 25 26 27  28 29 2a 2b 2c 2d 2e 2f  | !\"#$%&'()*+,-./|\n"
                    "00000030  30 31 32 33 34 35 36 37  38 39 3a 3b 3c 3d 3e 3f  |0123456789:;<=>?|\n"
                    "00000040  40 41 42 43 44 45 46 47  48 49 4a 4b 4c 4d 4e 4f  |@ABCDEFGHIJKLMNO|\n"
                    "00000050  50 51 52 53 54 55 56 57  58 59 5a 5b 5c 5d 5e 5f  |PQRSTUVWXYZ[\\]^_|\n"
                    "00000060  60 61 62 63 64 65 66 67  68 69 6a 6b 6c 6d 6e 6f  |`abcdefghijklmno|\n"
                    "00000070  70 71 72 73 74 75 76 77  78 79 7a 7b 7c 7d 7e 7f  |pqrstuvwxyz{|}~.|\n"
                    "00000080\n", getOut())
    dump(buf, 15);
    STRCMP_EQUAL(   "00000000  00 01 02 03 04 05 06 07  08 09 0a 0b 0c 0d 0e     |...............|\n"
                    "0000000f\n",
                    getOut())
}
