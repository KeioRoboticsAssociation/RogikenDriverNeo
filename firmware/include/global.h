#if !defined(__GLOBAL__H__)
#define __GLOBAL__H__

#include <inttypes.h>

#include "task_list.h"

inline constexpr uint32_t FREQ = 72000000;

inline TaskList task_list;

inline float motor_power = 0; // -1 ~ 1
inline float enc_speed = 0; // Hz

#endif // __GLOBAL__H__
