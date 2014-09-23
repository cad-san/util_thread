#ifndef D_IPC_SERVER_H
#define D_IPC_SERVER_H

#include <sys/types.h>

class IpcServer {
public:
    virtual ~IpcServer(){}

    virtual bool init() = 0;
    virtual bool start() = 0;
    virtual bool stop() = 0;

    virtual ssize_t recv(char buffer[], size_t size) = 0;
};

#endif
