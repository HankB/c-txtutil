/** @file Test output routines.
 *
 * The overall strategy is to redirect output to
 * a file and then seek/read back file contents to
 * compare with expected output.
 */
#include "CppUTest/TestHarness.h"

extern "C" {
#include <stdio.h>
#include "txtutil.h"
}

TEST_GROUP(logutil){
    void setup(){}

    void teardown(){}
};

TEST(logutil, test_logPri)
{
    CHECK(getLogPri() == 3)
    CHECK(setLogPri(4) == 3)
    CHECK(getLogPri() == 4)
}

// code for capturing output of the logmsg() API for testing purposes

static FILE *fp;
static FILE *oldFp;
#define BUF_SIZE (1024 * 16)
static char buf[BUF_SIZE];
static const char *scratchFileName = "/tmp/TDD_log_test_file.txt";
static const char *scratchPrevFileName = "/tmp/prevTDD_log_test_file.txt";

/**
 * Testing strategy for logmsg(). (cleverly copied from TestDump.cpp)
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


TEST_GROUP(log){
    void setup(){
        fp = fopen(scratchFileName, "w+");
        if (fp == NULL)
        {
            fprintf(stderr, "Cannot open \"%s\"\n", scratchFileName);
            exit(1);
        }
        oldFp = setLogout(fp);
    }

    void teardown()
    {
        setLogout(oldFp);
        fclose(fp);
    }
};

TEST(log, test_logOut)
{
    CHECK(oldFp == stdout)
    CHECK(logmsg(0, "Hello World") == 0)
    CHECK(logmsg(3, "Hello World") == 11)
    STRCMP_EQUAL("Hello World", getOut())
    CHECK(logmsg(3, "%s", "Goodbye World") == 13)
    STRCMP_EQUAL("Goodbye World", getOut())

}