#include "state_manager.h"

#include <cmath>
#include <limits>
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
      for (; cur < size && pow < 100000000; cur++) { // UINT32_MAX = 4294967295
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

auto StateManager::printFloat(const float val) -> std::string {
  const auto is_minus = val < 0;
  auto val1 = static_cast<int>(val);
  auto val2 = (val - val1) * (is_minus ? -1 : 1);
  auto digit = 1;
  auto pow = 10;
  while (
    std::abs(val2 * pow - std::trunc(val2 * pow)) > std::numeric_limits<float>::epsilon() &&
    digit < 9 // UINT32_MAX = 4294967295
  ){
    digit++;
    pow *= 10;
  }
  std::string fmt = "%d.%0" + std::to_string(digit) + "u";
  char str[64];
  snprintf(str, 64, fmt.c_str(), val1, static_cast<unsigned int>(val2 * pow));
  return std::string(str);
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
      printf("O Motor power set to %s%%\n", printFloat(state.motor_power).c_str());
      break;
    }
    case 'V': {
      if (state.mode != Mode::VEL_PID) {
        state.mode = Mode::VEL_PID;
        auto pid_task = static_cast<PidTask*>(task_list.at("pid_vel").get());
        pid_task->setTarget(&state.enc_vel, &state.target_vel);
        pid_task->resetState();
        pid_task->setHertz(1000);
      }
      state.target_vel = parseFloat(command.substr(2));
      printf("O Target velocity set to %sHz\n", printFloat(state.target_vel).c_str());
      break;
    }
    case 'S': {
      switch (command[2]) {
        case 'k':
        case 'K': {
          auto gain = parseFloat(command.substr(5));
          static_cast<PidTask*>(task_list.at("pid_vel").get())->setGain(command[3], gain);
          printf("O PID gain K%c set to %s\n", command[3], printFloat(gain).c_str());
          break;
        }
        case 'D':
          state.debug = !state.debug;
          printf("O Debug output %d\n", state.debug);
          break;
        default:
          printf("E Config prop name %s not found\n", command.substr(2).c_str());
          break;
      }
      break;
    }
    default:
      printf("E Command not found\n%s", command.c_str());
      break;
  }
}
