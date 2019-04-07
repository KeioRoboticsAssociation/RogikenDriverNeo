#if !defined(__PID_TASK_H__)
#define __PID_TASK_H__

#include <inttypes.h>

#include "arm_math.h"

#include "task.h"

class PidTask : public Task {
  private:
    static constexpr float ZERO = 0;
    static constexpr float ACC_LIM = 1;
    static constexpr float VEL_LIM = 100;
    arm_pid_instance_f32 pid;
    const float* input = &ZERO;
    const float* set_point = &ZERO;
    auto task() -> void override;
  public:
    PidTask(const uint32_t);
    auto setGain(char, float) -> void;
    auto setTarget(const float*, const float*) -> void;
    auto resetState() -> void;
};

#endif // __PID_TASK_H__
