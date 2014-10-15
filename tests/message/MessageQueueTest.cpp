#include "CppUTest/TestHarness.h"

#include "MessageQueue.h"
#include <string>

TEST_GROUP(MessageQueue)
{
    void setup()
    {
    }

    void teardown()
    {
    }
};

TEST(MessageQueue, Init)
{
    MessageQueue<int> queue;

    LONGS_EQUAL(0, queue.size());
    CHECK_EQUAL(true, queue.empty());
}

TEST(MessageQueue, Send)
{
    MessageQueue<std::string> queue;
    std::string message = "message";

    CHECK_EQUAL(true, queue.send(message));

    LONGS_EQUAL(1, queue.size());
}

TEST(MessageQueue, Recv)
{
    MessageQueue<std::string> queue;
    std::string expect = "message";

    queue.send(expect);
    auto actual = queue.recv();

    CHECK_EQUAL(expect, actual.getValue());
    LONGS_EQUAL(0, queue.size());
}

TEST(MessageQueue, EmptyRecv)
{
    MessageQueue<std::string> queue;

    auto actual = queue.recv();

    CHECK_EQUAL(true, actual.isError());
    LONGS_EQUAL(0, queue.size());
}

TEST(MessageQueue, Full)
{
    MessageQueue<std::string> queue(1);

    CHECK_EQUAL(true, queue.send("message"));
    CHECK_EQUAL(true, queue.full());
}

TEST(MessageQueue, FullSend)
{
    MessageQueue<std::string> queue(1);

    CHECK_EQUAL(true, queue.send("become full"));

    CHECK_EQUAL(false, queue.send("not send"));
    CHECK_EQUAL(true, queue.full());
}
