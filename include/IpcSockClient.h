#ifndef D_IPC_SOCK_CLIENT_H
#define D_IPC_SOCK_CLIENT_H

#include "IpcClient.h"
#include "UtilTime.h"

#include <string>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>

class IpcSockClient : public IpcClient
{
private:
    /* config */
    const std::string path_;

    /* socket param */
    int sock_;
    struct sockaddr_un addr_;

public:
    IpcSockClient(const std::string& path);
    virtual ~IpcSockClient();

    bool init();
    bool start();
    bool stop();

    ssize_t send(const char buffer[], const size_t size);
};
#endif
