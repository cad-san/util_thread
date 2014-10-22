#include "IpcSockServer.h"

#include <stdio.h>
#include <unistd.h>

IpcSockServer::IpcSockServer(const std::string& path,
                             const int queue_size, const UtilTime& timeout) :
    path_(path),
    queue_size_(queue_size),
    timeout_(timeout),
    sock_(-1)
{
}

IpcSockServer::~IpcSockServer()
{
    stop();
}

bool IpcSockServer::init()
{
    struct sockaddr_un addr;
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, path_.c_str(), sizeof(addr.sun_path));
    addr_ = addr;

    remove(path_.c_str());
    return true;
}

bool IpcSockServer::start()
{
    int sock;

    /* ソケット取得 */
    sock = socket(AF_UNIX, SOCK_STREAM, 0);

    if(sock == -1) {
        perror("socket");
        return false;
    }

    /* バインド */
    if (bind(sock, (struct sockaddr*) &addr_, sizeof(struct sockaddr_un)) == -1) {
        perror("bind");
        goto FAIL;
    }

    /* リッスン */
    if (listen(sock, queue_size_) == -1) {
        perror("listen");
        goto FAIL;
    }

    /* パラメータを保存 */
    sock_ = sock;

    return true;

FAIL:
    close(sock);
    return false;
}

bool IpcSockServer::stop()
{
    if(!isActive()) {
        return false;
    }

    close(sock_);
    remove(path_.c_str());

    /* パラメータ上書き */
    this->sock_ = -1;

    return true;
}

ssize_t IpcSockServer::recv(char buffer[], size_t size)
{
    fd_set fd_s;

    FD_ZERO(&fd_s);
    FD_SET(sock_, &fd_s);

    /* 接続を監視 */
    int retval = pselect(sock_+1, &fd_s, NULL, NULL, &timeout_, NULL);

    if(retval <= 0) {
        return retval;
    }

    /* 接続確立 */
    int sock_fd = accept(sock_, NULL, NULL );

    if( sock_fd <= 0 ) {
        perror("accept");
        return -1;
    }

    /* 受信 */
    ssize_t recv_size = read(sock_fd, buffer, size);

    if(recv_size == -1) {
        perror("read");
    }

    close(sock_fd);
    return recv_size;
}

bool IpcSockServer::isActive() const
{
    return (this->sock_ != -1);
}
