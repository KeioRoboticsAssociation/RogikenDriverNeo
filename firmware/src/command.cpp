#include <stdio.h>
#include <string>

#include <global.h>

auto command(std::string data) -> void {
  switch (data[0]) {  
    case 'R': {
      float val = 0;
      sscanf(data.c_str() + 2, "%f", &val);
      motor_power = val > 1 ? 1 : val < -1 ? -1 : val;
      break;
    }
    default:
      printf("E Command not found\n%s", data.c_str());
      break;
  }
}
