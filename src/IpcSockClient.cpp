#include "IpcSockClient.h"

#include <stdio.h>
#include <unistd.h>

IpcSockClient::IpcSockClient(const std::string& path) :
    path_(path),
    sock_(-1)
{
}

IpcSockClient::~IpcSockClient()
{
    stop();
}

bool IpcSockClient::init()
{
    struct sockaddr_un addr;
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, path_.c_str(), sizeof(addr.sun_path));
    addr_ = addr;

    return true;
}

bool IpcSockClient::start()
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

bool IpcSockClient::stop()
{
    if(!isActive()) {
        return false;
    }

    close(sock_);

    /* パラメータ上書き */
    this->sock_ = -1;

    return true;
}

ssize_t IpcSockClient::send(const char buffer[], const size_t size)
{
    ssize_t send_size = write(sock_, buffer, size);

    if(send_size == -1) {
        perror("write");
    }

    return send_size;
}

bool IpcSockClient::isActive() const
{
    return (this->sock_ != -1);
}
