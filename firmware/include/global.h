#if !defined(__GLOBAL__H__)
#define __GLOBAL__H__

#include <inttypes.h>
#include <memory>
#include <vector>

#include "task.h"

inline constexpr uint32_t FREQ = 72000000;

inline std::vector<std::unique_ptr<Task>> task_list;

inline float motor_power = 0; // -1 ~ 1

#endif // __GLOBAL__H__
