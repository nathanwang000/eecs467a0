#include <lcm/lcm-cpp.hpp>
#include "keypress/keypress_t.hpp"
#include <common/timestamp.h>
#include <iostream>

int main(int argc, char ** argv) {
  
  system("stty raw -echo");
  lcm::LCM lcm;
  if (!lcm.good()) {
    system("stty sane");
    return 1;
  }

  int8_t value;
  while (std::cin >> value) {
    keypress::keypress_t message;
    message.timestamp = utime_now();
    message.character = value;
    lcm.publish("A0_KEY_PRESS",&message);
  }
  
  system("stty sane");
  return 0;
}
