#include "pid_task.h"

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
  arm_pid_f32(&pid, *set_point - *input);
  pid.state[2] = pid.state[2] > 100 ? 100 : pid.state[2] < -100 ? -100 : pid.state[2];
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
  arm_pid_reset_f32(&pid);
}
