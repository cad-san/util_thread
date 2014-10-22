#include "CppUTest/TestHarness.h"

#include "IpcSockServer.h"
#include "IpcSocKClient.h"
#include <future>

static const std::string dummy_path = "/tmp/test-socket";
static const int dummy_dueue_size = 128;

TEST_GROUP(Ipc)
{
    void setup()
    {
    }

    void teardown()
    {
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
