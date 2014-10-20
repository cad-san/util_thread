#include "CppUTest/TestHarness.h"

#include "MessageQueue.h"
#include "UtilThread.h"
#include <string>
#include <future>

TEST_GROUP(MessageQueueThread)
{
    typedef int MsgType;
    typedef MessageQueue<MsgType> MsgQueue;
    typedef std::shared_ptr< MsgQueue > MsgQueuePtr;

    void setup()
    {
    }

    void teardown()
    {
    }

    static Errorable<MsgType> recv_wait_async( const MsgQueuePtr& queue )
    {
        return queue->recv_wait();
    }

    static Errorable<MsgType> recv_time_wait_async( const MsgQueuePtr& queue, const UtilTime& time )
    {
        return queue->recv_wait(time);
    }

    static bool send_wait_async( const MsgQueuePtr& queue, MsgType message )
    {
        return queue->send_wait(message);
    }
};

TEST(MessageQueueThread, RecvWait)
{
    auto queue = std::make_shared< MsgQueue >();

    /* 非同期で受信処理を立ち上げる(無限長待機) */
    auto result = std::async(std::launch::async, recv_wait_async, queue);

    /* キューに積む */
    queue->send(1);

    /* join & 結果取得*/
    auto value = result.get();

    CHECK_EQUAL(1, value.getValue());
}

TEST(MessageQueueThread, SendWait)
{
    /* 最大1つで初期化 */
    auto queue = std::make_shared< MsgQueue >(1);

    /* キューに積む(FULL状態) */
    queue->send_wait(1);

    /* 非同期で送信処理を立ち上げる(無限長待機) */
    auto result = std::async(std::launch::async, send_wait_async, queue, 2);

    /* キューを減らす(受信→送信走る→FULL状態) */
    queue->recv();

    /* join & 結果取得*/
    auto send_result = result.get();

    CHECK_EQUAL(true, send_result);
    CHECK_EQUAL(true, queue->full());
}

TEST(MessageQueueThread, RecvTimedWait)
{
    auto queue = std::make_shared< MsgQueue >();

    UtilTime time_out = 0.5; /* sec */

    /* 非同期で受信処理を立ち上げる(無限長待機) */
    auto result = std::async(std::launch::async, recv_time_wait_async, queue, time_out);

    /* キューに積む */
    queue->send(1);

    /* join & 結果取得*/
    auto value = result.get();

    CHECK_EQUAL(1, value.getValue());
}

TEST(MessageQueueThread, RecvTimeOut)
{
    auto queue = std::make_shared< MsgQueue >();

    UtilTime time_out = 0.01; /* sec */

    /* タイムアウトするまで待つ */
    auto result = queue->recv_wait(time_out);

    CHECK_EQUAL(1, result.isError());
}
