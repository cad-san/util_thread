#ifndef D_UTIL_THREAD_H
#define D_UTIL_THREAD_H

#include <pthread.h>

class UtilThread
{
private:
    bool ready_flag_;
    bool active_flag_;
    bool finish_flag_;

    pthread_t main_thread_;
    pthread_mutex_t status_guard_;

    pthread_cond_t active_req_;
    pthread_cond_t finish_req_;

    /* mutex制御 */
    void statusLock();
    void statusUnlock();

    /* フラグ制御 */
    void setReadyFlag(bool flag);
    void setActiveFlag(bool flag);
    void setFinishFlag(bool flag);

    bool needToFinish() const;

    /* スレッド開始処理 */
    void requestStarting();
    void waitStarting();
    void notifyStarting();

    /* スレッド終了処理 */
    void requestStopping();
    void waitStopping();
    void notifyStopping();

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
