#include "state_manager.h"

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
      float val = parseFloat(command.c_str() + 2);
      state.motor_power = val > 1 ? 1 : val < -1 ? -1 : val;
      printf("O Motor power set to %d%%\n", static_cast<int>(state.motor_power * 100));
      break;
    }
    default:
      printf("E Command not found\n%s", command.c_str());
      break;
  }
}
