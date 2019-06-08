#include "pid_task.h"

#include <algorithm>
#include <inttypes.h>

#include "arm_math.h"

#include "global.h"
#include "task.h"

PidTask::PidTask(const uint32_t hertz) : Task(hertz) {
  pid.Kp = 0;
  pid.Ki = 0;
  pid.Kd = 0;
  arm_pid_init_f32(&pid, 1);
}

auto PidTask::task() -> void {
  auto prev = pid.state[2];
  arm_pid_f32(&pid, *set_point - *input);
  pid.state[2] = *set_point ? std::clamp(std::clamp(pid.state[2], prev - ACC_LIM, prev + ACC_LIM), -VEL_LIM, VEL_LIM) : 0;
  state_mgr.state.motor_power = pid.state[2];
}

auto PidTask::setGain(char type, float val) -> void {
  switch (type) {
    case 'P':
    case 'p':
      pid.Kp = val;
      break;
    case 'I':
    case 'i':
      pid.Ki = val;
      break;
    case 'D':
    case 'd':
      pid.Kd = val;
      break;
    default:
      break;
  }
  arm_pid_init_f32(&pid, 0);
}

auto PidTask::setTarget(const float* in, const float* sp) -> void {
  input = in;
  set_point = sp;
}

auto PidTask::resetState() -> void {
  pid.state[0] = 0;
  pid.state[1] = 0;
  pid.state[2] = state_mgr.state.motor_power;
}
