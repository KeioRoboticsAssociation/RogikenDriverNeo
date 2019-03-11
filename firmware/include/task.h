#if !defined(__TASK_H__)
#define __TASK_H__

#include <inttypes.h>

class Task {
  private:
    uint32_t cycle_duration;
    uint32_t prev_cycle_cnt = 0;
    virtual auto task() -> void;
  public:
    Task(const uint32_t);
    virtual ~Task() = default;
    virtual auto process() -> void final;
};

#endif // __TASK_H__
