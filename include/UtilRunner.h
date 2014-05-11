#ifndef D_UTIL_RUNNER_H
#define D_UTIL_RUNNER_H

#include <memory>

class UtilRunner
{
public:
    virtual ~UtilRunner(){}
    virtual void init() = 0;
    virtual void step() = 0;
};
typedef std::shared_ptr<UtilRunner> RunnerPtr;

#endif
