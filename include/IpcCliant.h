#ifndef D_IPC_CLIANT_H
#define D_IPC_CLIANT_H

#include <sys/types.h>

class IpcCliant {
public:
    virtual ~IpcCliant(){}

    virtual bool init() = 0;
    virtual bool start() = 0;
    virtual bool stop() = 0;

    virtual ssize_t send(const char buffer[], const size_t size) = 0;
};


#endif
