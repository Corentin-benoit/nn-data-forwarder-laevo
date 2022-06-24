/**
 * @file TouchSensor.hpp
 * @author Corentin BENOIT
 * @brief
 * @version 0.1
 * @date 2022-06-24
 *
 * @copyright Copyright (c) 2022
 *
 */

#ifndef DEF_TOUCHSENSOR
#define DEF_TOUCHSENSOR

#include <iostream>
#include <ostream>
#include "mbed.h"

// Detection level for the touch sensor
#define TRESHOLD 40


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
    bool detection() const;



protected:
    PinName m_pin;
    analogin_t *pressureButton;

};
#endif