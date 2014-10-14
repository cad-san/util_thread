#include "CppUTest/TestHarness.h"

#include "Errorable.h"
#include <string>

TEST_GROUP(Errorable)
{
    void setup()
    {
    }

    void teardown()
    {
    }
};

TEST(Errorable, ConstructtValue)
{
    Errorable<int> value(10);

    CHECK_EQUAL(false, value.isError());
    LONGS_EQUAL(10, value.getValue());
}

TEST(Errorable, ConstructError)
{
    Error<std::string> error("Invalid Type");
    Errorable<int> value(error);

    CHECK_EQUAL(true, value.isError());
    CHECK_EQUAL(error.get(), value.getError().get());
}