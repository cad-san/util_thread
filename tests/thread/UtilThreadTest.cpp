#include "CppUTest/TestHarness.h"

#include "UtilThread.h"
#include "MockRunner.h"

TEST_GROUP(UtilThread)
{
    MockRunnerPtr runner;
    UtilThread* thread;

    void setup()
    {
        runner = std::make_shared<MockRunner>();
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

TEST(UtilThread, Stop)
{
    bool ret_init = thread->init();
    bool ret_start = thread->start();
    bool ret_stop = thread->stop();

    CHECK_EQUAL(true, ret_init);
    CHECK_EQUAL(true, ret_start);
    CHECK_EQUAL(true, ret_stop);
    CHECK_EQUAL(false, thread->isActive());
}

TEST(UtilThread, Interval)
{
    UtilTime result;
    thread->setIntervalMiliSec( 500 );
    result = thread->getIntervalTime();

    LONGS_EQUAL(0, result.tv_sec);
    LONGS_EQUAL(500 * UtilTime::MSEC_BASE, result.tv_nsec);
}

TEST(UtilThread, NextTime)
{
    thread->setIntervalMiliSec( 500 );
    UtilTime base = thread->getBaseTime();
    UtilTime expect = base + 0.5;
    UtilTime actual = thread->getNextTime(base);

    LONGS_EQUAL(expect.tv_sec, actual.tv_sec);
    LONGS_EQUAL(expect.tv_nsec, actual.tv_nsec);
}
