/**
 * @file StartButton.hpp
 * @author Corentin BENOIT
 * @brief
 * @version 0.1
 * @date 2022-06-28
 *
 * @copyright Copyright (c) 2022
 *
 */


#ifndef DEF_STARTBUTTON
#define DEF_STARTBUTTON

#include <iostream>
#include <ostream>
#include "mbed.h"


class StartButton
{
public:
    // Constructor
    StartButton(); //Intern Button
    StartButton(PinName pin); //extern Button

    // Destructor
    ~StartButton();


    // Assessors
    void setPin(PinName pin);
    const PinName& getPin() const;

    //Methods
    //void init() const;
    void displayWait() const;
    void initDisplayIMU() const;
    void initDisplayPot() const;
    bool detection(DigitalIn startButton) const;



protected:
    PinName m_pin;
};
#endif