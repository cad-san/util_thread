#ifndef D_UTIL_THREAD_H
#define D_UTIL_THREAD_H

#include <pthread.h>

class UtilThread
{
private:
    bool ready_flag_;
    bool active_flag_;

    pthread_t main_thread_;
    pthread_mutex_t msg_guard_;

    pthread_cond_t active_req_;

    void requestStarting();
    void waitStarting();
    void notifyStarting();

    /* スレッドのメイン関数 */
    void main();

    /* スレッド起動用静的関数(pthreadの制約上必要) */
    static void* launcher(void *obj);

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
