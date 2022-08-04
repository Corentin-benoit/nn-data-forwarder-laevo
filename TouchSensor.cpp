/**
 * @file TouchSensor.cpp
 * @author Corentin BENOIT
 * @brief
 * @version 1.1
 * @date 2022-08-04
 *
 * @copyright Copyright (c) 2022
 */

#include "TouchSensor.hpp"





using namespace std;

/*
----------------------------------------------------------
----------------CONSTRUCTOR------------------------------
----------------------------------------------------------
*/

TouchSensor::TouchSensor() : m_pin(ARDUINO_UNO_A1){
    printf("You need to connect the pressure sensor to the IMU's pin A1\n");
}

TouchSensor::TouchSensor(PinName pin) : m_pin(pin){}


/*
----------------------------------------------------------
----------------DESTRUCTOR------------------------------
----------------------------------------------------------
*/

TouchSensor::~TouchSensor(){}

/*
----------------------------------------------------------
-------------------ASSESSORS------------------------------
----------------------------------------------------------
*/

void TouchSensor::setPin(PinName pin){
    m_pin = pin;
}

const PinName& TouchSensor::getPin() const{
    return m_pin;
}


/*
----------------------------------------------------------
----------------------------METHODS-----------------------
----------------------------------------------------------
*/

/**
 * @brief For display the pressure percentage on the pressure sensor
 * 
 */
void TouchSensor::display() const{
    AnalogIn touchSensor(m_pin);
    int value = touchSensor.read()*100.0f;
    printf("Percentage of pressure : %d\n", value);
}

/**
 * @brief Finds a Boolean value (0 or 100) the scale is essential. It allows you to give importance to the data when it is sent to EdgeImpulse
 * 
 * @return int 
 */

int TouchSensor::detection() const 
{
    AnalogIn touchSensor(m_pin);
    int value = touchSensor.read()*100.0f;
    if(value > TRESHOLD)
    {
        return 100;
    }
    else
    {
        return 0;
    }
}







