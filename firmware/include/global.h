#if !defined(__GLOBAL__H__)
#define __GLOBAL__H__

#include <inttypes.h>

#include "state_manager.h"
#include "task_list.h"

inline constexpr uint32_t FREQ = 72000000;

inline StateManager state_mgr;
inline TaskList task_list;

#endif // __GLOBAL__H__
