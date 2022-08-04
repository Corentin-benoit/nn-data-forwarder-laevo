/**
 * @file TouchSensor.hpp
 * @author Corentin BENOIT
 * @brief
 * @version 1.1
 * @date 2022-08-04
 *
 * @copyright Copyright (c) 2022
 *
 */

#ifndef DEF_TOUCHSENSOR
#define DEF_TOUCHSENSOR

#include <iostream>
#include "mbed.h"

// Detection level for the touch sensor
#define TRESHOLD 15 //In percentage


class TouchSensor
{
public:
    // Constructor
    TouchSensor();
    TouchSensor(PinName pin);

    // Destructor
    ~TouchSensor();


    // Assessors
    void setPin(PinName pin);
    const PinName& getPin() const;

    //Methods
    void display() const;
    int detection() const;



protected:
    PinName m_pin;

};
#endif