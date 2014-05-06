#include "UtilThread.h"

#include <time.h>

static const int INTERVAL_TIME = 500;

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

    setReadyFlag(true);
    setActiveFlag(false);
    setFinishFlag(false);
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
    UtilTime expect_time = getBaseTime();

    /* スレッド開始 */
    notifyStarting();
    for(;;)
    {
        /* 終了判定 */
        if(needToFinish())
            break;

        statusLock();

        /* Intervalに基づき待機 */
        expect_time = getNextTime(expect_time);
        pthread_cond_timedwait(&finish_req_, &status_guard_, &expect_time);

        statusUnlock();
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
    setActiveFlag(true);
    pthread_cond_signal(&this->active_req_);
}

void UtilThread::requestStopping()
{
    setFinishFlag(true);
    pthread_cond_signal(&this->finish_req_);
}

void UtilThread::waitStopping()
{
    pthread_join(this->main_thread_, NULL);
}

void UtilThread::notifyStopping()
{
    setActiveFlag(false);
    pthread_cond_signal(&this->active_req_);
}

void UtilThread::setReadyFlag(bool flag)
{
    statusLock();
    this->ready_flag_ = flag;
    statusUnlock();
}

void UtilThread::setActiveFlag(bool flag)
{
    statusLock();
    this->active_flag_ = flag;
    statusUnlock();
}

void UtilThread::setFinishFlag(bool flag)
{
    statusLock();
    this->finish_flag_ = flag;
    statusUnlock();
}

bool UtilThread::isReady() const
{
    return this->ready_flag_;
}

bool UtilThread::isActive() const
{
    return this->active_flag_;
}

bool UtilThread::needToFinish() const
{
    return this->finish_flag_;
}

void* UtilThread::launcher(void* obj)
{
    UtilThread* thread = reinterpret_cast<UtilThread *>(obj);
    thread->main();
    return NULL;
}

void UtilThread::setIntervalMiliSec(const int interval_msec)
{
    interval_ = static_cast<double>(interval_msec) / 1000;
}

const UtilTime UtilThread::getIntervalTime() const
{
    return interval_;
}

const UtilTime UtilThread::getBaseTime() const
{
    UtilTime base;
    clock_gettime(CLOCK_REALTIME, &base);
    return base;
}

const UtilTime UtilThread::getNextTime(const UtilTime& base) const
{
    return base + interval_;
}
