#include "UtilTime.h"
#include <math.h>

const int UtilTime::MSEC_BASE = 1000 * 1000;
const int UtilTime::NSEC_BASE = 1000 * 1000 * 1000;

UtilTime::UtilTime()
{
    this->tv_sec = 0;
    this->tv_nsec = 0;
}

UtilTime::~UtilTime()
{
}

UtilTime::util_sec
UtilTime::add_sec(const UtilTime& base, const UtilTime& addition) const
{
    UtilTime::util_sec result = static_cast<UtilTime::util_sec>(base.tv_sec + addition.tv_sec);
    result += static_cast<UtilTime::util_sec>((base.tv_nsec + addition.tv_nsec) / NSEC_BASE);
    return result;
}

UtilTime::util_nsec
UtilTime::add_nsec(const UtilTime& base, const UtilTime& addition) const
{
    return static_cast<UtilTime::util_nsec>((base.tv_nsec + addition.tv_nsec) % NSEC_BASE);
}

const UtilTime
UtilTime::convert_time(const double& time) const
{
    double integral;
    double decimal;
    decimal = modf(time, &integral);

    UtilTime result;
    result.tv_sec = static_cast<util_sec>(integral);
    result.tv_nsec = static_cast<util_nsec>(decimal * NSEC_BASE);

    return result;
}

UtilTime UtilTime::operator+(const UtilTime& time) const
{
    UtilTime result;

    result.tv_sec = add_sec(*this, time);
    result.tv_nsec = add_nsec(*this, time);

    return result;
}

UtilTime& UtilTime::operator=(const UtilTime& time)
{
    tv_sec = time.tv_sec;
    tv_nsec = time.tv_nsec;

    return *this;
}

UtilTime& UtilTime::operator+=(const UtilTime& time)
{
    tv_sec = add_sec(*this, time);
    tv_nsec = add_nsec(*this, time);

    return *this;
}

UtilTime UtilTime::operator+(const double& time) const
{
    UtilTime addition = convert_time(time);
    return *this + addition;
}

UtilTime& UtilTime::operator=(const double& time)
{
    *this = convert_time(time);
    return *this;
}

UtilTime& UtilTime::operator+=(const double& time)
{
    UtilTime addition = convert_time(time);
    *this += addition;
    return *this;
}
