#if !defined(__SIMPLE_TASK_H__)
#define __SIMPLE_TASK_H__

#include <functional>
#include <inttypes.h>

#include "task.h"

class SimpleTask : public Task {
  private:
    std::function<void(void)> func;
    auto task() -> void override;
  public:
    SimpleTask(std::function<void(void)>, const uint32_t);
};

#endif // __SIMPLE_TASK_H__
