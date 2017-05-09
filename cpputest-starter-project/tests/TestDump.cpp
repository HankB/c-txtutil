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

extern "C"
{
	/*
	 * You can add your c-only include files here
	 */
    #include <stdio.h>
    #include "txtutil.h"
}

TEST_GROUP(Dump)
{
    void setup()
    {
    }

    void teardown()
    {
    }
};

TEST(Dump, test1)
{
    /*
     * Instantiate your class, or call the function, you want to test
     */
     FAIL("fail")
 
}

