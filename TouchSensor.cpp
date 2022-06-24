/**
 * @file TouchSensor.cpp
 * @author Martin CHASSARD
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

TouchSensor::TouchSensor(){
    printf("You need to connect the pressure sensor to the IMU's pin A1\n");
    analogin_init(pressureButton, A1);
}

TouchSensor::TouchSensor(PinName pin) : m_pin(pin){
    analogin_init(pressureButton, m_pin);
}


/*
----------------------------------------------------------
----------------DESTRUCTOR------------------------------
----------------------------------------------------------
*/

TouchSensor::~TouchSensor(){
    analogin_free(pressureButton);
}

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
    float value = analogin_read(pressureButton)*100.0f;
    if (value > 100)
    {
        cerr<<"The sensor returns an inconsistent value"<<endl;
        value = 100;
    }
    else if (value < 0) 
    {
        cerr<<"The sensor returns an inconsistent value"<<endl;
        value = 0;
    }
    else if(value > 0)
    {
        printf("Percentage of pressure : %d\n", static_cast<int>(value));
    }
}

bool TouchSensor::detection() const 
{
    if(static_cast<int>(analogin_read(pressureButton)*100.0f) > TRESHOLD){
        return true;
    }
    else{
        return false;
    }
}






