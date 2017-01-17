#include <chrono>
#include <iostream>
#include "Gpio.hpp"

int main()
{
  std::cout << "Hello world!" << std::endl;
  const std::chrono::time_point<std::chrono::system_clock> start = std::chrono::system_clock::now();
  test::Gpio pin(17, test::IODirection::In);
  const std::chrono::time_point<std::chrono::system_clock> afterInit = std::chrono::system_clock::now();
  auto duration = afterInit - start;
  std::cout << "Time required for initialization: " <<
      std::chrono::duration_cast<std::chrono::microseconds>(duration).count() << " microseconds\n";
  int sumOnes = 0;
  int sumZeroes = 0;
  int sumFails = 0;
  const std::chrono::time_point<std::chrono::system_clock> queryStart = std::chrono::system_clock::now();
  for (int i = 0; i < 1024; ++i)
  {
    int val = -1;
    if (pin.value(val))
    {
      switch(val)
      {
        case 0:
             ++sumZeroes;
             break;
        case 1:
             ++sumOnes;
             break;
      } //switch
    } //if
    else
      ++sumFails;
  } //for i
  const std::chrono::time_point<std::chrono::system_clock> queryEnd = std::chrono::system_clock::now();
  duration = queryEnd - queryStart;
  std::cout << "Time required for 1024 queries: " << std::chrono::duration_cast<std::chrono::microseconds>(duration).count() << " microseconds\n";
  std::cout << "Ones:  " << sumOnes << "\nZeroes: " << sumZeroes << "\nFails: " << sumFails << "\n";
  return 0;
}
