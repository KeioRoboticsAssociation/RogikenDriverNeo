#include "simple_task.h"

#include <functional>
#include <inttypes.h>

#include "task.h"

SimpleTask::SimpleTask(std::function<void(void)> func, const uint32_t hertz)
  : Task(hertz), func(func) {};

auto SimpleTask::task() -> void {
  func();
}
