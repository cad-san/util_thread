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
    std::string actual = queue.recv();

    CHECK_EQUAL(expect, actual);
    LONGS_EQUAL(0, queue.size());
}