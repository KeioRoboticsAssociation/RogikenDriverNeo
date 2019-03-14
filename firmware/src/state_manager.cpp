#include "state_manager.h"

#include "global.h"
#include "pid_task.h"

auto StateManager::parseFloat(const char* str) -> float {
  auto is_minus = str[0] == '-';
  auto val1 = 0;
  auto val2 = 0;
  sscanf(str + (is_minus ? 1 : 0), "%i.%i", &val1, &val2);
  auto pow = 10;
  auto val2_copy = val2;
  while ((val2_copy /= 10) != 0) {
    pow *= 10;
  }
  return (val1 + static_cast<float>(val2) / pow) * (is_minus ? -1 : 1);
}

auto StateManager::executeTextCommand(const std::string command) -> void {
  switch (command[0]) {
    case 'R': {
      if (state.mode != Mode::RAW) { // TODO: Refactor param updating
        state.mode = Mode::RAW;
        task_list.at("pid_vel")->setHertz(0);
      }
      auto val = parseFloat(command.c_str() + 2);
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
      state.target_vel = parseFloat(command.c_str() + 2);
      printf("O Target velocity set to %dHz\n", static_cast<int>(state.target_vel));
      break;
    }
    default:
      printf("E Command not found\n%s", command.c_str());
      break;
  }
}
