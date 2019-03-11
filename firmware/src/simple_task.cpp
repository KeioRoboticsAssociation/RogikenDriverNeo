#include "simple_task.h"

#include <functional>
#include <inttypes.h>

#include "task.h"

SimpleTask::SimpleTask(const uint32_t hertz, std::function<void(void)> func)
  : Task(hertz), func(func) {}

auto SimpleTask::task() -> void {
  func();
}
