#include "IpcSockCliant.h"

#include <stdio.h>
#include <unistd.h>

IpcSockCliant::IpcSockCliant(const std::string& path) :
    path_(path)
{
}

IpcSockCliant::~IpcSockCliant()
{
}

bool IpcSockCliant::init()
{
    struct sockaddr_un addr;
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, path_.c_str(), UNIX_PATH_MAX);
    addr_ = addr;

    return true;
}

bool IpcSockCliant::start()
{
    /* ソケット作成 */
    int sock = socket(AF_UNIX, SOCK_STREAM, 0);

    if (sock == -1) {
        perror("socket");
        goto FAIL;
    }

    /* 接続 */
    if (connect(sock, (struct sockaddr*) &addr_, sizeof(struct sockaddr_un)) == -1) {
        perror("connect");
        goto FAIL;
    }

    /* 保存 */
    this->sock_ = sock;
    return true;

FAIL:
    close(sock);
    return false;
}

bool IpcSockCliant::stop()
{
    close(sock_);
    return false;
}

ssize_t IpcSockCliant::send(const char buffer[], const size_t size)
{
    ssize_t send_size = write(sock_, buffer, size);

    if(send_size == -1) {
        perror("write");
    }

    return send_size;
}
