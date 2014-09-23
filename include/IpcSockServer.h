#ifndef D_IPC_SOCK_SERVER_H
#define D_IPC_SOCK_SERVER_H

#include "IpcServer.h"
#include "UtilTime.h"

#include <string>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>

class IpcSockServer : public IpcServer
{
public:
    static const int DEFAULT_QUEUE_SIZE;
    static const int DEFAULT_BUFFER_SIZE;

private:
    /* config */
    const std::string path_;
    const int queue_size_;

    /* timeout */
    const UtilTime timeout_;

    /* socket param */
    int sock_;
    struct sockaddr_un addr_;

public:
    IpcSockServer(const std::string& path, const int queue_size, const UtilTime& timeout);
    virtual ~IpcSockServer();

    bool init();
    bool start();
    bool stop();

    ssize_t recv(char buffer[], size_t size);
};
#endif
