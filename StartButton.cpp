/**
 * @file StartButton.cpp
 * @author Corentin BENOIT
 * @brief
 * @version 1.1
 * @date 2022-08-04
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

/**
 * @brief Information for the user
 * 
 */

void StartButton::displayWait() const
{
    DigitalIn startButton(m_pin);
    cout<<"---------------------- click on the button to start the program -------------------"<<endl;
    while (detection(startButton) == false) {}
    ThisThread::sleep_for(500ms);
}

/**
 * @brief Information for the user
 * 
 */
void StartButton::initDisplayIMU() const
{
    DigitalIn startButton(m_pin);
    cout<<"--------------------- click on the button when the IMU is stable ------------------"<<endl;
    while (detection(startButton) == false) {}
    ThisThread::sleep_for(500ms);
}

/**
 * @brief Information for the user
 * 
 */
void StartButton::initDisplayPot() const
{
    DigitalIn startButton(m_pin);
    cout<<"--------------- click on the button when you are standing and stable --------------"<<endl;
    while (detection(startButton) == false) {}
    ThisThread::sleep_for(500ms);
}

/**
 * @brief Detects when the user clicks on the button (on the card or external button)
 * 
 * @param startButton 
 * @return true 
 * @return false 
 */
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





