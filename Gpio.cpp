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

bool Gpio::setEdge(const Edges e)
{
  const std::string fileName = std::string("/sys/class/gpio/gpio") + m_pinAsString + std::string("/edge");
  std::ofstream edge(fileName.c_str());
  if (!edge.good())
  {
    std::clog << "Could not open edge file " << fileName << "!\n";
    return false;
  }
  switch(e)
  {

    case Edges::Rising:
         edge << "rising";
         break;
    case Edges::Falling:
         edge << "falling";
         break;
    case Edges::Both:
         edge << "both";
         break;
    case Edges::None:
    default:
         edge << "none";
         break;
  } //switch
  if (!edge.good())
  {
    std::clog << "Could not write to edge file!\n";
    return false;
  }
  edge.close();
  if (!edge.good())
  {
    std::clog << "Could not close edge file!\n";
    return false;
  }
  return edge.good();
}

bool Gpio::edge(Edges& val) const
{
  std::ifstream valueStream(std::string("/sys/class/gpio/gpio") + m_pinAsString + std::string("/edge"));
  if (!valueStream.good())
    return false;
  std::string str1;
  valueStream >> str1;
  valueStream.close();
  if (!valueStream.good())
    return false;
  if (str1 == "none")
    val = Edges::None;
  else if (str1 == "both")
    val = Edges::Both;
  else if (str1 == "rising")
    val = Edges::Rising;
  else if (str1 == "falling")
    val = Edges::Falling;
  else
  {
    std::clog << "Unknown edge value \"" << str1 << "\"!\n";
    return false;
  }
  return true;
}

} //namespace
