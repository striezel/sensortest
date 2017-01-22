#include <cstring>
#include <iostream>
#include <thread>
#include "RCSwitch.h"

int main()
{
  std::cout << "Hello world!" << std::endl;

  wiringPiSetupGpio();

  RCSwitch rc = RCSwitch(1, -1);
  while(true)
  {
    if (rc.OokAvailable())
    {
      const int bufferSize = 256;
      char buffer[bufferSize];
      std::memset(buffer, 0, bufferSize);

      rc.getOokCode(buffer);
      std::cout << "Message: " << buffer << "\n";
    } //if
    std::this_thread::sleep_for(std::chrono::seconds(1));
  } //while

  return 0;
}
