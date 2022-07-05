/**
 * @file PotentiometerSensor.cpp
 * @author Corentin BENOIT
 * @brief
 * @version 1.0
 * @date 2022-06-24
 *
 * @copyright Copyright (c) 2022
 */

#include "PotentiometerSensor.hpp"



using namespace std;

/*
----------------------------------------------------------
----------------CONSTRUCTOR------------------------------
----------------------------------------------------------
*/

PotentiometerSensor::PotentiometerSensor() : m_pin(ARDUINO_UNO_A2){
    printf("You need to connect the potentiometer sensor to the IMU's pin A2\n");
}

PotentiometerSensor::PotentiometerSensor(PinName pin) : m_pin(pin){}


/*
----------------------------------------------------------
----------------DESTRUCTOR------------------------------
----------------------------------------------------------
*/

PotentiometerSensor::~PotentiometerSensor(){}

/*
----------------------------------------------------------
-------------------ASSESSORS------------------------------
----------------------------------------------------------
*/

void PotentiometerSensor::setPin(PinName pin){
    m_pin = pin;
}

const PinName& PotentiometerSensor::getPin() const{
    return m_pin;
}

void PotentiometerSensor::setOffset(float offset){
    m_offset_standing = offset;
}

const float PotentiometerSensor::getOffset() const{
    return m_offset_standing;
}


/*
----------------------------------------------------------
----------------------------METHODS-----------------------
----------------------------------------------------------
*/

void PotentiometerSensor::display() const{
    AnalogIn pot(m_pin);
    float value = pot.read()*100.0f;
    printf("Percentage : %f\n", value);
}

float PotentiometerSensor::displayRaw() const
{
    AnalogIn pot(m_pin);
    return pot.read()*100.0f;
}

float PotentiometerSensor::displayRawOffset() const
{
    AnalogIn pot(m_pin);
    return (pot.read()*100.0f - m_offset_standing);
}








