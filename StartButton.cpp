/**
 * @file StartButton.cpp
 * @author Corentin BENOIT
 * @brief
 * @version 0.1
 * @date 2022-06-24
 *
 * @copyright Copyright (c) 2022
 */

#include "StartButton.hpp"



using namespace std;

/*
----------------------------------------------------------
----------------CONSTRUCTOR------------------------------
----------------------------------------------------------
*/

StartButton::StartButton() : m_pin(BUTTON1){
}

StartButton::StartButton(PinName pin) : m_pin(pin){
}


/*
----------------------------------------------------------
----------------DESTRUCTOR------------------------------
----------------------------------------------------------
*/

StartButton::~StartButton()
{
}

/*
----------------------------------------------------------
-------------------ASSESSORS------------------------------
----------------------------------------------------------
*/

void StartButton::setPin(PinName pin){
    m_pin = pin;
}

const PinName& StartButton::getPin() const{
    return m_pin;
}


/*
----------------------------------------------------------
----------------------------METHODS-----------------------
----------------------------------------------------------
*/


void StartButton::displayWait() const
{
    DigitalIn startButton(m_pin);
    cout<<"------------- click on the button to start the program -------------"<<endl;
    while (detection(startButton) == false) 
    {
    }
}

bool StartButton::detection(DigitalIn startButton) const 
{
    //Active the PullUp mode of the card for an extern button
    if(m_pin != BUTTON1)
    {
        DigitalIn(startButton).mode(PullUp);
    }

    //error handling
    if(DigitalIn(startButton).is_connected() == 0)
    {
        cerr<<"No button is connected"<<endl;
    }

    if(DigitalIn(startButton).read() == 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}





