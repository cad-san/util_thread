#ifndef D_UTIL_TIME_H
#define D_UTIL_TIME_H

#include <time.h>

struct UtilTime : timespec
{
private:
    typedef int util_sec;
    typedef int util_nsec;

    util_sec add_sec(const UtilTime& base, const UtilTime& addition) const;
    util_nsec add_nsec(const UtilTime& base, const UtilTime& addition) const;

    const UtilTime convert_time(const double& time) const;

public:
    static const int MSEC_BASE;
    static const int NSEC_BASE;

    UtilTime();
    virtual ~UtilTime();

    UtilTime operator+(const UtilTime& time) const;
    UtilTime& operator=(const UtilTime& time);
    UtilTime& operator+=(const UtilTime& time);

    UtilTime operator+(const double& time) const;
    UtilTime& operator=(const double& time);
    UtilTime& operator+=(const double& time);
};

#endif