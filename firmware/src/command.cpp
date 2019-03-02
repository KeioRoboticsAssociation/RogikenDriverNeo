#include <stdio.h>
#include <string>

auto execute_command(std::string data) -> void {
  switch (data[0]) {  
    default:
      printf("frame detected\n%s", data.c_str());
      break;
  }
}
