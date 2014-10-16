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

template<typename MsgType>
bool send_wait_async( const std::shared_ptr< MessageQueue<MsgType> >& queue, MsgType message )
{
    return queue->send_wait(message);
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

TEST(MessageQueueThread, SendWait)
{
    /* 最大1つで初期化 */
    auto queue = std::make_shared< MessageQueue<int> >(1);

    /* キューに積む(FULL状態) */
    queue->send_wait(1);

    /* 非同期で送信処理を立ち上げる(無限長待機) */
    auto result = std::async(std::launch::async, send_wait_async<int>, queue, 2);

    /* キューを減らす(受信→送信走る→FULL状態) */
    queue->recv();

    /* join & 結果取得*/
    auto send_result = result.get();

    CHECK_EQUAL(true, send_result);
    CHECK_EQUAL(true, queue->full());
}
