#include <stdio.h>

#include <lcm/lcm-cpp.hpp>
#include "keypress/keypress_t.hpp"
#include <common/timestamp.h>

class Handler 
{
  int total_time_system;
  int total_time_message;
  int total_message;
  int prev_utime;
  int prev_message_time;
public:
  Handler() : total_time_system(0), total_time_message(0) {
    prev_message_time = prev_utime = utime_now();
    total_message = 0;
  }
  ~Handler() {}

  void handleMessage(const lcm::ReceiveBuffer* rbuf,
		     const std::string& chan, 
		     const keypress::keypress_t* msg)
  {
    int64_t tmp = utime_now();
    total_time_system += tmp - prev_utime;
    prev_utime = tmp;

    total_time_message += msg->timestamp - prev_message_time;
    prev_message_time = msg->timestamp;
    total_message++;
    
    printf("average key typing per minute is %f (system estimate) \n", total_message / ((double)total_time_system / 1000000 / 60));
    printf("average key typing per minute is %f (message estimate) \n", total_message / ((double)total_time_message / 1000000 / 60));
  }
};

int main(int argc, char** argv)
{
  lcm::LCM lcm;

  if(!lcm.good())
    return 1;

  Handler handlerObject;
  lcm.subscribe("A0_KEY_PRESS", &Handler::handleMessage, &handlerObject);

  while (true) 
    while(0 == lcm.handle());

  return 0;
}
