#include "CppUTest/TestHarness.h"

#include "MessageQueue.h"
#include "UtilThread.h"
#include <string>
#include <future>

template<typename MsgType>
Errorable<MsgType> recv_wait_async( const std::shared_ptr< MessageQueue<MsgType> >& queue )
{
    return queue->recv_wait();
}

TEST_GROUP(MessageQueueThread)
{
    void setup()
    {
    }

    void teardown()
    {
    }
};

TEST(MessageQueueThread, RecvWait)
{
    auto queue = std::make_shared< MessageQueue<int> >();

    /* 非同期で受信処理を立ち上げる(無限長待機) */
    auto result = std::async(std::launch::async, recv_wait_async<int>, queue);

    /* キューに積む */
    queue->send(1);

    /* join & 結果取得*/
    auto value = result.get();

    CHECK_EQUAL(1, value.getValue());
}
