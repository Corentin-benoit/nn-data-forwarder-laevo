/**
 * @file PotentiometerSensor.hpp
 * @author Corentin BENOIT
 * @brief
 * @version 0.1
 * @date 2022-06-24
 *
 * @copyright Copyright (c) 2022
 *
 */

#ifndef DEF_POTENTIOMETERSENSOR
#define DEF_POTENTIOMETERSENSOR

#include <iostream>
#include "mbed.h"



class PotentiometerSensor
{
public:
    // Constructor
    PotentiometerSensor();
    PotentiometerSensor(PinName pin);

    // Destructor
    ~PotentiometerSensor();


    // Assessors
    void setPin(PinName pin);
    const PinName& getPin() const;

    //Methods
    void display() const;
    float displayRaw() const;



protected:
    PinName m_pin;

};
#endif