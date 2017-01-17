#include "Gpio.hpp"
#include <fstream>
#include <iostream>
#include <sstream>

namespace test
{

Gpio::Gpio(int pin, IODirection _direction)
: m_pin(pin),
  m_direction(_direction)
{
  std::stringstream stream;
  stream << pin;
  m_pinAsString = stream.str();
  if (!exportPin())
  {
    std::clog << "GPIO export failed!\n";
    return;
  }
  if (!setDirection())
  {
    std::clog << "GPIO direction could not be set!\n";
  }
}

Gpio::~Gpio()
{
  if (!unexportPin())
    std::clog << "GPIO unexport failed!\n";
}

int Gpio::pin() const
{
  return m_pin;
}

IODirection Gpio::direction() const
{
  return m_direction;
}

bool Gpio::exportPin()
{
  std::ofstream expo("/sys/class/gpio/export");
  if (!expo.good())
    return false;
  expo << m_pinAsString;
  expo.close();
  return expo.good();
}

bool Gpio::unexportPin()
{
  std::ofstream unexpo("/sys/class/gpio/unexport");
  if (!unexpo.good())
    return false;
  unexpo << m_pinAsString;
  unexpo.close();
  return unexpo.good();
}

bool Gpio::setDirection()
{
  const std::string fileName = std::string("/sys/class/gpio/gpio") + m_pinAsString + std::string("/direction");
  std::ofstream dir(fileName);
  if (!dir.good())
  {
    std::clog << "Could not open direction file " << fileName << "!\n";
    return false;
  }
  switch(m_direction)
  {
    case IODirection::In:
         dir << "in";
         break;
    case IODirection::Out:
    default:
         dir << "out";
         break;
  } //switch
  if (!dir.good())
  {
    std::clog << "Could not write to direction file!\n";
    return false;
  }
  dir.close();
  if (!dir.good())
  {
    std::clog << "Could not close direction file!\n";
    return false;
  }
  return dir.good();
}

bool Gpio::value(int& val) const
{
  std::ifstream valueStream(std::string("/sys/class/gpio/gpio") + m_pinAsString + std::string("/value"));
  if (!valueStream.good())
    return false;
  std::string str1;
  valueStream >> str1;
  valueStream.close();
  if (!valueStream.good())
    return false;
  if (str1 == "0")
    val = 0;
  else
    val = 1;
  return true;
}

} //namespace
