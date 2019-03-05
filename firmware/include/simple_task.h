#if !defined(__SIMPLE_TASK_H__)
#define __SIMPLE_TASK_H__

#include <functional>
#include <inttypes.h>
#include <string>

#include "task.h"

class SimpleTask : public Task {
  private:
    std::function<void(void)> func;
    auto task() -> void override;
  public:
    SimpleTask(const std::string, const uint32_t, std::function<void(void)>);
};

#endif // __SIMPLE_TASK_H__
