#include "task.h"

#include <inttypes.h>

#include "stm32f3xx_hal.h"

#include "global.h"

Task::Task(const uint32_t hertz) {
  setHertz(hertz);
}

auto Task::task() -> void {}

auto Task::process() -> void {
  if (enable && DWT->CYCCNT - prev_cycle_cnt >= cycle_duration) {
    prev_cycle_cnt += cycle_duration;
    task();
  }
}

auto Task::setHertz(const uint32_t hertz) -> void {
  enable = hertz;
  cycle_duration = hertz ? FREQ / hertz : 0;
}
