#include "UtilThread.h"

UtilThread::UtilThread() :
    ready_flag_(false),
    active_flag_(false),
    finish_flag_(false)
{
    pthread_mutex_init(&status_guard_, NULL);
}

UtilThread::~UtilThread()
{
}

bool UtilThread::init()
{
    pthread_cond_init(&active_req_, NULL);
    pthread_cond_init(&finish_req_, NULL);

    this->ready_flag_ = true;
    this->active_flag_ = false;
    this->finish_flag_ = false;
    return true;
}

bool UtilThread::start()
{
    requestStarting();
    waitStarting();
    return true;
}

bool UtilThread::stop()
{
    requestStopping();
    waitStopping();
    return true;
}

void UtilThread::main()
{
    /* スレッド開始 */
    notifyStarting();
    for(;;)
    {
        if(finish_flag_)
            break;

        const struct timespec wait_time = {0, 100 * 1000};
        nanosleep(&wait_time, NULL);
    }

    /* スレッド終了 */
    notifyStopping();
    return;
}

void UtilThread::statusLock()
{
    pthread_mutex_lock(&status_guard_);
}

void UtilThread::statusUnlock()
{
    pthread_mutex_unlock(&status_guard_);
}

void UtilThread::requestStarting()
{
    pthread_create(&this->main_thread_, NULL, UtilThread::launcher, this);
}

void UtilThread::waitStarting()
{
    statusLock();
    while(!isActive())
        pthread_cond_wait(&active_req_, &status_guard_);
    statusUnlock();
}

void UtilThread::notifyStarting()
{
    statusLock();
    this->active_flag_ = true;
    statusUnlock();

    pthread_cond_signal(&this->active_req_);
}

void UtilThread::requestStopping()
{
    statusLock();
    this->finish_flag_ = true;
    statusUnlock();

    pthread_cond_signal(&this->finish_req_);
}

void UtilThread::waitStopping()
{
    pthread_join(this->main_thread_, NULL);
}

void UtilThread::notifyStopping()
{
    statusLock();
    this->active_flag_ = false;
    statusUnlock();

    pthread_cond_signal(&this->active_req_);
}

bool UtilThread::isReady() const
{
    return this->ready_flag_;
}

bool UtilThread::isActive() const
{
    return this->active_flag_;
}

void* UtilThread::launcher(void* obj)
{
    UtilThread* thread = reinterpret_cast<UtilThread *>(obj);
    thread->main();
    return NULL;
}
