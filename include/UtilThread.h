#ifndef D_UTIL_THREAD_H
#define D_UTIL_THREAD_H

class UtilThread
{
private:
    bool ready_flag_;
    bool active_flag_;
public:
    UtilThread();
    ~UtilThread();

    bool init();
    bool start();
    bool stop();

    bool isReady() const;
    bool isActive() const;
};

#endif
