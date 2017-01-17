#ifndef GPIO_HPP
#define GPIO_HPP

#include <string>

namespace test
{

enum class IODirection { In, Out};

class Gpio
{
  public:
    /** \brief constructor
     *
     * \param pin the pin number
     * \param direction  the direction of the pin (in or out)
     */
    Gpio(int pin, IODirection _direction);


    /** \brief destructor
     */
    ~Gpio();

    Gpio(const Gpio& other) = delete;
    Gpio& operator=(const Gpio& other) = delete;
    Gpio(Gpio&& other) = delete;


    /** \brief get the pin number
     *
     * \return Returns the pin number.
     */
    int pin() const;


    /** \brief get current direction of the pin
     *
     * \return Returns an enum value indicating the direction.
     */
    IODirection direction() const;


    /** \brief gets the current value (input pin only)
     *
     * \param val  integer that will be used to store the pin value
     * \return Returns true on success, false on failure.
     */
    bool value(int& val) const;
  private:
    int m_pin; /**< pin number */
    std::string m_pinAsString; /**< same, but in string format */
    IODirection m_direction; /**< pin direction */

    bool exportPin();
    bool unexportPin();
    bool setDirection();
};

} //namespace

#endif // GPIO_HPP
