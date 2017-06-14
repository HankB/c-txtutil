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