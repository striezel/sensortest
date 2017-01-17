#ifndef GPIO_HPP
#define GPIO_HPP

#include <string>

namespace test
{

enum class IODirection { In, Out};

enum class Edges { None, Rising, Falling, Both};

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


    /** \brief determines which edges should cause an interrupt
     *
     * \param e  enumeration to indicate the edges
     * \return Returns true, if the edge status was changed.
     *         Returns false, if an error occurred.
     */
    bool setEdge(const Edges e);


    /** \brief gets the current edge management status
     *
     * \return Returns the edge management status.
     */
    bool edge(Edges& e) const;


    /** \brief waits for edge on the pin
     *
     * \param value  stores the flank: 0 for no edge/timeout; -1 for falling and 1 for rising edge
     * \param timeoutMilliseconds timeout in milliseconds (maximum time to wait for an edge)
     * \return Returns true, if everything went fine - value is set in this case.
     * Returns false, if an error occurred - value is undefined in that case.
     */
    bool pollEdge(int& value, unsigned int timeoutMilliseconds) const;
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
