#ifndef D_UTIL_TIME_H
#define D_UTIL_TIME_H

#include <time.h>

class UtilTime : public timespec
{
private:
    typedef time_t util_sec;
    typedef long util_nsec;

    util_sec add_sec(const UtilTime& base, const UtilTime& addition) const;
    util_nsec add_nsec(const UtilTime& base, const UtilTime& addition) const;

    const UtilTime convert_time(const double& time) const;

public:
    static const int MSEC_BASE;
    static const int NSEC_BASE;

    static UtilTime now();

public:
    UtilTime();
    UtilTime(const UtilTime& rhs);
    virtual ~UtilTime();

    UtilTime operator+(const UtilTime& time) const;
    UtilTime& operator=(const UtilTime& time);
    UtilTime& operator+=(const UtilTime& time);

    UtilTime operator+(const double& time) const;
    UtilTime& operator=(const double& time);
    UtilTime& operator+=(const double& time);
};

#endif
