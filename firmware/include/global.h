#if !defined(__GLOBAL__H)
#define __GLOBAL__H

#include <memory>
#include <vector>

#include "task.h"

inline std::vector<std::unique_ptr<Task>> task_list;

#endif // __GLOBAL__H
