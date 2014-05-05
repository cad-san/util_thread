#include "CppUTest/TestHarness.h"

#include "UtilThread.h"

TEST_GROUP(UtilThread)
{
    UtilThread* thread;

    void setup()
    {
        thread = new UtilThread();
    }

    void teardown()
    {
        delete thread;
    }
};

TEST(UtilThread, Create)
{
    CHECK_EQUAL(false, thread->isReady());
    CHECK_EQUAL(false, thread->isActive());
}

TEST(UtilThread, Init)
{
    bool result = thread->init();

    CHECK_EQUAL(true, result);
    CHECK_EQUAL(true, thread->isReady());
    CHECK_EQUAL(false, thread->isActive());
}

TEST(UtilThread, Start)
{
    bool ret_init = thread->init();
    bool ret_start = thread->start();

    CHECK_EQUAL(true, ret_init);
    CHECK_EQUAL(true, ret_start);
    CHECK_EQUAL(true, thread->isActive());
}
