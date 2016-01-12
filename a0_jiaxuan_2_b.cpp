#include <lcm/lcm-cpp.hpp>
#include "keypress/keypress_t.hpp"
#include <common/timestamp.h>
#include <iostream>

int main(int argc, char ** argv) {
  
  if (system("stty raw -echo") == -1) exit(1);
  lcm::LCM lcm;
  if (!lcm.good()) {
    if (system("stty sane") == -1) exit(1);
    return 1;
  }

  int8_t value;
  while (std::cin >> value) {
    keypress::keypress_t message;
    message.timestamp = utime_now();
    message.character = value;
    lcm.publish("A0_KEY_PRESS",&message);
  }
  
  if (system("stty sane") == -1) exit(1);
  return 0;
}
