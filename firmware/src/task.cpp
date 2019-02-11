#include "task.h"

#include <inttypes.h>

#include "stm32f3xx_hal.h"

Task::Task(const uint32_t hertz) : cycle_duration(72000000 / hertz) {};

auto Task::task() -> void {}

auto Task::process() -> void {
  auto cycle_cnt = DWT->CYCCNT;
  if (cycle_cnt - prev_cycle_cnt >= cycle_duration) {
    prev_cycle_cnt = cycle_cnt;
    task();
  }
}
