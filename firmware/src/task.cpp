#include "task.h"

#include <inttypes.h>
#include <string>

#include "stm32f3xx_hal.h"

#include "global.h"

Task::Task(const std::string id, const uint32_t hertz) : id(id), cycle_duration(FREQ / hertz) {}

auto Task::task() -> void {}

auto Task::process() -> void {
  if (DWT->CYCCNT - prev_cycle_cnt >= cycle_duration) {
    prev_cycle_cnt += cycle_duration;
    task();
  }
}
