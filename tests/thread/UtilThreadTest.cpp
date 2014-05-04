#include "CppUTest/TestHarness.h"

#include "UtilThread.h"

TEST_GROUP(UtilThread)
{
    UtilThread* thread;

    void setup()
    {
        thread = new UtilThread();
    }

    void terdown()
    {
        delete thread;
    }
};
