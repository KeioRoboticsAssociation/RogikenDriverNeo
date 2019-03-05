#include "simple_task.h"

#include <functional>
#include <inttypes.h>
#include <string>

#include "task.h"

SimpleTask::SimpleTask(const std::string id, const uint32_t hertz, std::function<void(void)> func)
  : Task(id, hertz), func(func) {};

auto SimpleTask::task() -> void {
  func();
}
