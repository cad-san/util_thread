#include "CppUTest/TestHarness.h"

#include "UtilTime.h"

TEST_GROUP(UtilTime)
{
    void setup()
    {
    }
    void teardown()
    {
    }
};

TEST(UtilTime, InitFloat)
{
    UtilTime time = 1.1;

    LONGS_EQUAL(1, time.tv_sec);
    LONGS_EQUAL(100 * UtilTime::MSEC_BASE, time.tv_nsec);
}

TEST(UtilTime, SimpleAdd)
{
    UtilTime base;
    UtilTime addition;

    base.tv_sec = 1;
    base.tv_nsec = 1000;
    addition.tv_sec = 1;
    addition.tv_nsec = 1000;

    UtilTime result = base + addition;
    LONGS_EQUAL(2, result.tv_sec);
    LONGS_EQUAL(2000, result.tv_nsec);
}

TEST(UtilTime, AddEqual)
{
    UtilTime base;
    UtilTime addition;

    base.tv_sec = 1;
    base.tv_nsec = 1000;
    addition.tv_sec = 1;
    addition.tv_nsec = 1000;

    base += addition;
    LONGS_EQUAL(2, base.tv_sec);
    LONGS_EQUAL(2000, base.tv_nsec);
}

TEST(UtilTime, AddCarry)
{
    UtilTime base;
    UtilTime addition;

    base.tv_sec = 0;
    base.tv_nsec = 500 * UtilTime::MSEC_BASE;
    addition.tv_sec = 0;
    addition.tv_nsec = 500 * UtilTime::MSEC_BASE;

    base += addition;
    LONGS_EQUAL(1, base.tv_sec);
    LONGS_EQUAL(0, base.tv_nsec);
}

TEST(UtilTime, AddInteger)
{
    UtilTime base;
    UtilTime result;

    // 10.5 sec
    base.tv_sec = 10;
    base.tv_nsec = 500 * UtilTime::MSEC_BASE;

    result = base + 10;
    LONGS_EQUAL(20, result.tv_sec);
    LONGS_EQUAL(500 * UtilTime::MSEC_BASE, result.tv_nsec);
}

TEST(UtilTime, AddEqualWithInteger)
{
    UtilTime base;

    // 10.5 sec
    base.tv_sec = 10;
    base.tv_nsec = 500 * UtilTime::MSEC_BASE;

    base += 10;
    LONGS_EQUAL(20, base.tv_sec);
    LONGS_EQUAL(500 * UtilTime::MSEC_BASE, base.tv_nsec);
}

TEST(UtilTime, AddFloat)
{
    UtilTime base;
    UtilTime result;

    // 10.5 sec
    base.tv_sec = 10;
    base.tv_nsec = 500 * UtilTime::MSEC_BASE;

    result = base + 9.5;
    LONGS_EQUAL(20, result.tv_sec);
    LONGS_EQUAL(0 * UtilTime::MSEC_BASE, result.tv_nsec);
}

TEST(UtilTime, AddEqualFloat)
{
    UtilTime result;

    // 10.5 sec
    result.tv_sec = 10;
    result.tv_nsec = 500 * UtilTime::MSEC_BASE;

    result += 9.5;
    LONGS_EQUAL(20, result.tv_sec);
    LONGS_EQUAL(0 * UtilTime::MSEC_BASE, result.tv_nsec);
}

TEST(UtilTime, Equal)
{
    UtilTime actual;
    UtilTime expect;

    actual.tv_sec = 1;
    actual.tv_nsec = 5 * UtilTime::MSEC_BASE;

    expect.tv_sec = 5;
    expect.tv_nsec = 0 * UtilTime::MSEC_BASE;

    actual = expect;

    LONGS_EQUAL(expect.tv_sec, actual.tv_sec);
    LONGS_EQUAL(expect.tv_nsec, actual.tv_nsec);
}

TEST(UtilTime, EqualNumber)
{
    UtilTime actual;

    actual.tv_sec = 1;
    actual.tv_nsec = 5 * UtilTime::MSEC_BASE;

    actual = 5.0;

    LONGS_EQUAL(5, actual.tv_sec);
    LONGS_EQUAL(0, actual.tv_nsec);
}
