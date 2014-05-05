#include "UtilThread.h"

UtilThread::UtilThread() :
    ready_flag_(false),
    active_flag_(false)
{
}

UtilThread::~UtilThread()
{
}

bool UtilThread::init()
{
    this->ready_flag_ = true;
    return true;
}

bool UtilThread::start()
{
    this->active_flag_ = true;
    return true;
}

bool UtilThread::isReady() const
{
    return this->ready_flag_;
}

bool UtilThread::isActive() const
{
    return this->active_flag_;
}
