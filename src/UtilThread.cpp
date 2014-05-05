#include "UtilThread.h"

UtilThread::UtilThread() :
    ready_flag_(false),
    active_flag_(false),
    finish_flag_(false)
{
    pthread_mutex_init(&msg_guard_, NULL);
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

void UtilThread::requestStarting()
{
    pthread_create(&this->main_thread_, NULL, UtilThread::launcher, this);
}

void UtilThread::waitStarting()
{
    pthread_mutex_lock(&msg_guard_);
    while(!isActive())
        pthread_cond_wait(&active_req_, &msg_guard_);
    pthread_mutex_unlock(&msg_guard_);
}

void UtilThread::notifyStarting()
{
    pthread_mutex_lock(&msg_guard_);
    this->active_flag_ = true;
    pthread_mutex_unlock(&msg_guard_);

    pthread_cond_signal(&this->active_req_);
}

void UtilThread::requestStopping()
{
    pthread_mutex_lock(&msg_guard_);
    this->finish_flag_ = true;
    pthread_mutex_unlock(&msg_guard_);

    pthread_cond_signal(&this->finish_req_);
}

void UtilThread::waitStopping()
{
    pthread_join(this->main_thread_, NULL);
}

void UtilThread::notifyStopping()
{
    pthread_mutex_lock(&msg_guard_);
    this->active_flag_ = false;
    pthread_mutex_unlock(&msg_guard_);

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
