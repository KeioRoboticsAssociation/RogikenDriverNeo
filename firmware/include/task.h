#if !defined(__TASK_H__)
#define __TASK_H__

#include <inttypes.h>

#include "stm32f3xx_hal.h"

class Task {
  private:
    bool enable;
    uint32_t cycle_duration;
    uint32_t prev_cycle_cnt = DWT->CYCCNT;
    virtual auto task() -> void;
  public:
    Task(const uint32_t);
    virtual ~Task() = default;
    virtual auto process() -> void final;
    virtual auto setHertz(const uint32_t) -> void final;
};

#endif // __TASK_H__
