#include "state_manager.h"

#include <stddef.h>
#include <stdio.h>
#include <string>

#include "global.h"
#include "pid_task.h"

auto StateManager::parseFloat(const std::string& str) -> float {
  const auto size = str.size();
  size_t cur = 0;
  bool is_minus = false;
  for (; cur < size; cur++) {
    const auto& c = str[cur];
    if (c == '-') {
      is_minus = true;
    } else if (c >= '0' && c <= '9') {
      break;
    }
  }
  int val1 = 0;
  float val2 = 0;
  for (; cur < size; cur++) {
    const auto& c = str[cur];
    if (c >= '0' && c <= '9') {
      val1 = val1 * 10 + c - '0';
    } else if (c == '.') {
      cur++;
      unsigned int val2_i = 0;
      unsigned int pow = 1;
      for (; cur < size; cur++) {
        const auto& chr = str[cur];
        if (chr >= '0' && chr <= '9') {
          val2_i = val2_i * 10 + chr - '0';
          pow *= 10;
        }
      }
      val2 = static_cast<float>(val2_i) / pow;
      break;
    }
  }
  return (val1 + val2) * (is_minus ? -1 : 1);
}

auto StateManager::executeTextCommand(const std::string& command) -> void {
  switch (command[0]) {
    case 'R': {
      if (state.mode != Mode::RAW) { // TODO: Refactor param updating
        state.mode = Mode::RAW;
        task_list.at("pid_vel")->setHertz(0);
      }
      auto val = parseFloat(command.substr(2));
      state.motor_power = val > 100 ? 100 : val < -100 ? -100 : val;
      printf("O Motor power set to %d%%\n", static_cast<int>(state.motor_power));
      break;
    }
    case 'V': {
      if (state.mode != Mode::VEL_PID) {
        state.mode = Mode::VEL_PID;
        auto pid_task = static_cast<PidTask*>(task_list.at("pid_vel").get());
        pid_task->resetState();
        pid_task->setTarget(&state.enc_vel, &state.target_vel);
        pid_task->setHertz(1000);
      }
      state.target_vel = parseFloat(command.substr(2));
      printf("O Target velocity set to %dHz\n", static_cast<int>(state.target_vel));
      break;
    }
    default:
      printf("E Command not found\n%s", command.c_str());
      break;
  }
}
