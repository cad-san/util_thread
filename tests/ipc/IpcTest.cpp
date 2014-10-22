#include "CppUTest/TestHarness.h"

#include "IpcSockServer.h"
#include "IpcSocKClient.h"
#include <future>

static const std::string dummy_path = "/tmp/test-socket";
static const int dummy_dueue_size = 128;
static const int buffer_size = 128;

TEST_GROUP(Ipc)
{
    void setup()
    {
    }

    void teardown()
    {
    }

    static ssize_t recv_async( IpcSockServer* server, char buffer[], size_t size )
    {
        return server->recv(buffer, size);
    }
};

TEST(Ipc, ServerSingle)
{
    UtilTime timeout = 0.5;
    IpcSockServer server(dummy_path, dummy_dueue_size, timeout);

    bool initted = server.init();
    bool started = server.start();
    bool stoped = server.stop();

    CHECK_EQUAL(true, initted);
    CHECK_EQUAL(true, started);
    CHECK_EQUAL(true, stoped);
}

TEST(Ipc, ClientSingle)
{
    IpcSockClient client(dummy_path);

    bool initted = client.init();
    bool started = client.start(); /* サーバ起動していないためfail */

    CHECK_EQUAL(true, initted);
    CHECK_EQUAL(false, started);
}


TEST(Ipc, SendRecv)
{
    UtilTime timeout = 0.5;
    IpcSockServer server(dummy_path, dummy_dueue_size, timeout);
    IpcSockClient client(dummy_path);

    /* サーバ起動 */
    server.init();
    server.start();

    char buffer[buffer_size];
    auto recv = std::async(std::launch::async, recv_async, &server, buffer, buffer_size);

    /* クライアント起動 */
    client.init();
    client.start();

    std::string message = "test";
    client.send(message.c_str(), message.size());

    /* 受信 */
    auto recv_size = recv.get();

    /* 終端文字が無いため追加 */
    if(0 < recv_size && recv_size < buffer_size)
        buffer[recv_size] = '\0';

    LONGS_EQUAL(message.size(), recv_size);
    STRCMP_EQUAL(message.c_str(), buffer);

    client.stop();
    server.stop();
}
