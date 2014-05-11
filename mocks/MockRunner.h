#ifndef D_MOCK_RUNNER_H
#define D_MOCK_RUNNER_H

#include <memory>
#include <pthread.h>

#include "UtilRunner.h"

class MockRunner : public UtilRunner
{
private:
    pthread_mutex_t status_guard_;
    pthread_cond_t performed_req_;

    bool initialized_flag_;
    bool performed_flag_;

    void statusLock()
    {
        pthread_mutex_lock(&status_guard_);
    }

    void statusUnlock()
    {
        pthread_mutex_unlock(&status_guard_);
    }

public:
    MockRunner() :
        initialized_flag_(false),
        performed_flag_(false)
    {
        pthread_mutex_init(&status_guard_, NULL);
        pthread_cond_init(&performed_req_, NULL);
    }

    void init()
    {
        statusLock();
        initialized_flag_ = true;
        statusUnlock();
    }

    void step()
    {
        statusLock();
        performed_flag_ = true;
        pthread_cond_signal(&performed_req_);
        statusUnlock();
    }

    bool initialized()
    {
        return initialized_flag_;
    }

    bool performed()
    {
        return performed_flag_;
    }

    void waitPerforming(UtilTime until)
    {
        if(performed_flag_)
            return;

        statusLock();
        pthread_cond_timedwait(&performed_req_, &status_guard_, &until);
        statusUnlock();
    }
};
typedef std::shared_ptr<MockRunner> MockRunnerPtr;

#endif
