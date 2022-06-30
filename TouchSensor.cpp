/**
 * @file TouchSensor.cpp
 * @author Corentin BENOIT
 * @brief
 * @version 0.1
 * @date 2022-06-24
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

void TouchSensor::display() const{
    AnalogIn touchSensor(m_pin);
    int value = touchSensor.read()*100.0f;
    printf("Percentage of pressure : %d\n", value);
}

bool TouchSensor::detection() const 
{
    AnalogIn touchSensor(m_pin);
    int value = touchSensor.read()*100.0f;
    if(value > TRESHOLD)
    {
        return true;
    }
    else
    {
        return false;
    }
}


/*
Idées :
https://os.mbed.com/questions/6752/AnalogIn-read-function-is-giving-erroneo/


*/






