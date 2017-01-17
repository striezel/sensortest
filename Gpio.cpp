#include "Gpio.hpp"
#include <fstream>
#include <iostream>
#include <sstream>
//most includes below are C-style stuff
#include <fcntl.h> //for poll() and non-blocking I/O
#include <poll.h> //for poll()
#include <cstring> //for memset()
#include <unistd.h> //for close()

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

bool Gpio::pollEdge(int& value, unsigned int timeoutMilliseconds) const
{
  const std::string fileName = std::string("/sys/class/gpio/gpio") + m_pinAsString + std::string("/value");
  int fd = open(fileName.c_str(), O_RDONLY | O_NONBLOCK);
  if (fd < 0)
  {
    std::clog << "Could not open " << fileName << ".\n";
    return false;
  }
  struct pollfd fds[1];
  std::memset(&fds, '\0', sizeof(fds));
  fds[0].fd = fd;
  fds[0].events = POLLPRI;
  int pollResult = poll(fds, 1, timeoutMilliseconds);
  if (pollResult < 0)
  {
    std::clog << "Polling of file descriptor failed!\n";
    close(fd);
    return false;
  }
  else if (pollResult == 0)
  {
    //timeout, but that is OK
    value = 0;
  } //else (timeout)
  else if (pollResult > 0)
  {
    const unsigned int bufferSize = 4;
    char buffer[bufferSize];
    std::memset(buffer, 0, bufferSize);
    const ssize_t readBytes = read(fd, buffer, bufferSize);
    if (readBytes < 0)
    {
      std::clog << "Error while reading from file!\n";
      close(fd);
      return false;
    }
    if (readBytes == 0)
    {
      //should not happen, because there ought to be data
      std::clog << "Error: Zero bytes read from file after polling.\n";
      close(fd);
      return false;
    }
    std::string data = std::string(buffer, readBytes);
    if (("0\n" == data) || ("0" == data))
      value = -1; //falling flank
    else if (("1\n" == data) || ("1" == data))
      value = 1; //rising flank
    else
    {
      //unknown value
      std::clog << "Unexpected value \"" << data << "\" was encountered!\n";
      value = 0;
      close(fd);
      return false;
    }
  } //else (poll() found something)
  close(fd);
  return true;
}

} //namespace
