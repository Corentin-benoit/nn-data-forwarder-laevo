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
#include <math.h>
#include <cstdint>
#include "../LSM6DSL/LSM6DSL_acc_gyro_driver.h"
#include "../LSM6DSL/LSM6DSLSensor.h"

const int GRAPH_LENGTH = 100; // Length of line graph

//10 mm en linear 
//12.2 mm en circular
const int ACTIVE_LENGTH = 170-10; //Active Length of the membrane potentiometer
const int ACTIVE_ANGLE = 255;
const int RANGE = 75;
const int VOLTAGE_LIMITATION = 0;


class PotentiometerSensor
{
public:
    // Constructor
    PotentiometerSensor();
    PotentiometerSensor(PinName pin, short int id);

    // Destructor
    ~PotentiometerSensor();


    // Assessors
    void setPin(PinName pin);
    const PinName &getPin() const;

    void setId(short int id);
    const short int &getId() const;

    void setOffset(float offset);
    const float &getOffset() const;

    void setColor(char color);
    const char &getColor() const;

    //Methods
    float getRawDataPercentage() const;
    float getRawData0to1() const; 
    uint16_t getRawData_u16() const; 
    float getRawDataOffsetPercentage_u16() const; 
    float getRawDataOffset0to1() const; 
    uint16_t getRawDataOffset_u16() const; 

    void displayPercentage() const;
    void displayDistance(float position_soft_pot_offset) const;
    void displayAngle(float position_soft_pot_offset) const;
    void displayTorque(float torque) const;

    float calculateAngle() const;
    float calculateDistance() const;
    float calculateTorque(float angle, char& sens) const; //sens = 'i' to increase movement or 'd' to decrease movement
 
    void forceDirection(char& force_direction, uint16_t last_angle) const;
    void chooseColor();
    float mapData(float input) const;




protected:
    PinName m_pin;
    float m_offset_standing;
    char m_color = 'b';         //Need to be change by the user 
    short int m_id;           //Right: 0 or left: l 

};

double map(double x, double in_min, double in_max, double out_min, double out_max);  
float calculateGlobalTorque(float torque_right, float torque_left);

#endif