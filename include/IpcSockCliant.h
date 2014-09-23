#ifndef D_IPC_SOCK_CLIANT_H
#define D_IPC_SOCK_CLIANT_H

#include "IpcCliant.h"
#include "UtilTime.h"

#include <string>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>

class IpcSockCliant : public IpcCliant
{
private:
    /* config */
    const std::string path_;

    /* socket param */
    int sock_;
    struct sockaddr_un addr_;

public:
    IpcSockCliant(const std::string& path);
    virtual ~IpcSockCliant();

    bool init();
    bool start();
    bool stop();

    ssize_t send(const char buffer[], const size_t size);
};
#endif
