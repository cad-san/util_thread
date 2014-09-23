#include "IpcSockServer.h"
#include "UtilThread.h"
#include "UtilRunner.h"

#include "sock_config.h"

#include <cstdio>

class SockRunner : public UtilRunner
{
private:
    IpcSockServer sock_;

public:
    SockRunner(const std::string& path, int queue_size, const UtilTime& timeout) :
        sock_(path, queue_size, timeout)
    {
    }

    ~SockRunner()
    {
        sock_.stop();
    }

    void init()
    {
        sock_.init();

        sock_.start();
    }

    void step()
    {
        char buffer[2048];
        size_t recv_size = sock_.recv(buffer, 2048);
        if(recv_size > 0 && recv_size < 2048)
        {
            buffer[recv_size] = '\0';
            printf("receive message : %s\n",buffer);
        }
    }
};

typedef std::shared_ptr<SockRunner> SockRunnerPtr;

int main()
{
    UtilTime timeout;
    timeout.tv_sec = 0;
    timeout.tv_nsec = 500 * UtilTime::MSEC_BASE;

    SockRunnerPtr sock = std::make_shared<SockRunner>(SOCK_PATH, 128, timeout);
    UtilThread sock_thread(sock);

    sock_thread.setIntervalMiliSec(500);

    printf("start server\n");

    sock_thread.init();
    sock_thread.start();

    printf("hit key to exit...\n");
    getchar();

    sock_thread.stop();

    return 0;
}
