#ifndef D_UTIL_THREAD_H
#define D_UTIL_THREAD_H

class UtilThread
{
public:
    UtilThread();
    ~UtilThread();

    bool isReady() const;
    bool isActive() const;
};

#endif
