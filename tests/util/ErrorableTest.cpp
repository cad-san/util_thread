#include "CppUTest/TestHarness.h"

#include "Errorable.h"
#include <string>

static const std::string ERR_INVALID_TYPE = "Invalid Type";

TEST_GROUP(Errorable)
{
    void setup()
    {
    }

    void teardown()
    {
    }
};

TEST(Errorable, ConstructValue)
{
    Errorable<int> value(10);

    CHECK_EQUAL(false, value.isError());
    LONGS_EQUAL(10, value.getValue());
}

TEST(Errorable, ConstructError)
{
    Error<std::string> error(ERR_INVALID_TYPE);
    Errorable<int> value(error);

    CHECK_EQUAL(true, value.isError());
    CHECK_EQUAL(ERR_INVALID_TYPE, value.getError().what());
}

TEST(Errorable, AssignValue)
{
    Errorable<int> value;

    value = 10;

    CHECK_EQUAL(false, value.isError());
    LONGS_EQUAL(10, value.getValue());
}

TEST(Errorable, AssignError)
{
    Errorable<int> value;

    value = Error<std::string>(ERR_INVALID_TYPE);

    CHECK_EQUAL(true, value.isError());
    CHECK_EQUAL(ERR_INVALID_TYPE, value.getError().what());
}

TEST(Errorable,ConstructAssignValue)
{
    Errorable<int> value = 10;

    CHECK_EQUAL(false, value.isError());
    LONGS_EQUAL(10, value.getValue());
}

TEST(Errorable,ConstructAssignError)
{
    Errorable<int> value = Error<std::string>(ERR_INVALID_TYPE);

    CHECK_EQUAL(true, value.isError());
    CHECK_EQUAL(ERR_INVALID_TYPE, value.getError().what());
}

TEST(Errorable, ExceptionGetValue)
{
    Errorable<int> value = Error<std::string>(ERR_INVALID_TYPE);

    CHECK_THROWS(std::logic_error, value.getValue());
}

TEST(Errorable, ExceptionGetError)
{
    Errorable<int> value = 10;

    CHECK_THROWS(std::logic_error, value.getError());
}
