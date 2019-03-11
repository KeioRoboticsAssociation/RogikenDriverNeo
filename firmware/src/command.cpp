#include "command.h"

#include <stdio.h>
#include <string>

#include <global.h>

auto command(std::string data) -> void {
  switch (data[0]) {  
    case 'R': {
      auto is_minus = data[2] == '-';
      auto val1 = 0;
      auto val2 = 0;
      sscanf(data.c_str() + (is_minus ? 3 : 2), "%i.%i", &val1, &val2);
      auto pow = 10;
      auto val2_copy = val2;
      while ((val2_copy /= 10) != 0) {
        pow *= 10;
      }
      float val = (val1 + static_cast<float>(val2) / pow) * (is_minus ? -1 : 1);
      motor_power = val > 1 ? 1 : val < -1 ? -1 : val;
      printf("O Motor power set to %d%%\n", static_cast<int>(motor_power * 100));
      break;
    }
    default:
      printf("E Command not found\n%s", data.c_str());
      break;
  }
}
