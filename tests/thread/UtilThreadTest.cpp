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
        thread = new UtilThread(runner);
    }

    void teardown()
    {
        delete thread;
    }

    void destroyThead()
    {
        if(thread == NULL)
            return;
        delete thread;
        thread = NULL;
    }

    void waitFirstStep()
    {
        UtilTime expect_time = thread->getNextTime(thread->getBaseTime());
        runner->waitPerforming(expect_time);
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
    CHECK_EQUAL(true, runner->initialized());
}

TEST(UtilThread, Start)
{
    bool ret_init = thread->init();
    bool ret_start = thread->start();

    CHECK_EQUAL(true, ret_init);
    CHECK_EQUAL(true, ret_start);
    CHECK_EQUAL(true, thread->isActive());

    waitFirstStep(); // step()が実行されるまで待つ
    CHECK_EQUAL(true, runner->performed());
}

TEST(UtilThread, Stop)
{
    bool ret_init = thread->init();
    bool ret_start = thread->start();
    waitFirstStep();
    bool ret_stop = thread->stop();

    CHECK_EQUAL(true, ret_init);
    CHECK_EQUAL(true, ret_start);
    CHECK_EQUAL(true, ret_stop);
    CHECK_EQUAL(false, thread->isActive());
    CHECK_EQUAL(true, runner->performed());
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

TEST(UtilThread, StartWithoutInit)
{
    bool ret_start = thread->start();

    CHECK_EQUAL(false, ret_start);
    CHECK_EQUAL(false, thread->isActive());

    bool ret_stop = thread->stop();
    CHECK_EQUAL(false, ret_stop);
    CHECK_EQUAL(false, thread->isActive());

    CHECK_EQUAL(false, runner->initialized());
    CHECK_EQUAL(false, runner->performed());
}

TEST(UtilThread, StartUntilActive)
{
    bool ret_init, ret_start;

    ret_init = thread->init();
    CHECK_EQUAL(true, ret_init);
    CHECK_EQUAL(true, thread->isReady());

    ret_start = thread->start();
    CHECK_EQUAL(true, ret_start);
    CHECK_EQUAL(true, thread->isActive());

    ret_start = thread->start();
    CHECK_EQUAL(false, ret_start);
    CHECK_EQUAL(true, thread->isActive());
}

TEST(UtilThread, StopWithoutStart)
{
    bool ret_init, ret_stop;

    ret_init = thread->init();
    CHECK_EQUAL(true, ret_init);
    CHECK_EQUAL(true, thread->isReady());

    ret_stop = thread->stop();
    CHECK_EQUAL(false, ret_stop);
    CHECK_EQUAL(false, thread->isActive());

    CHECK_EQUAL(true, runner->initialized());
    CHECK_EQUAL(false, runner->performed());
}

TEST(UtilThread, WithoutStop)
{
    bool ret_init, ret_start;
    ret_init = thread->init();
    CHECK_EQUAL(true, ret_init);
    CHECK_EQUAL(true, thread->isReady());

    ret_start = thread->start();
    CHECK_EQUAL(true, ret_start);
    CHECK_EQUAL(true, thread->isActive());

    waitFirstStep(); // step()が実行されるまで待つ

    destroyThead(); // stop()を呼ばずにdelete

    CHECK_EQUAL(true, runner->initialized());
    CHECK_EQUAL(true, runner->performed());
}

TEST(UtilThread, InitAfterStarted)
{
    bool ret_init, ret_start, ret_stop;

    ret_init = thread->init();
    ret_start = thread->start();

    CHECK_EQUAL(true, ret_init);
    CHECK_EQUAL(true, ret_start);

    ret_init = thread->init();
    CHECK_EQUAL(false, ret_init);

    ret_stop = thread->stop();
    CHECK_EQUAL(true, ret_stop);
}

TEST(UtilThread, Start2ndTime)
{
    bool ret_init, ret_start, ret_stop;

    ret_init = thread->init();
    ret_start = thread->start();
    ret_stop = thread->stop();

    CHECK_EQUAL(true, ret_init);
    CHECK_EQUAL(true, ret_start);
    CHECK_EQUAL(true, ret_stop);

    ret_start = thread->start();
    CHECK_EQUAL(true, ret_start);
    CHECK_EQUAL(true, thread->isActive());

    ret_stop = thread->stop();
    CHECK_EQUAL(true, ret_stop);
    CHECK_EQUAL(false, thread->isActive());
}
