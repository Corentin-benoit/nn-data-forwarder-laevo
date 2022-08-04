/**
 * @file PotentiometerSensor.cpp
 * @author Corentin BENOIT
 * @brief
 * @version 1.1
 * @date 2022-08-04
 *
 * @copyright Copyright (c) 2022
 */

#include "PotentiometerSensor.hpp"
#include <cmath>
#include <cstdint>
#include <ostream>
#include <stdint.h>
#include <string>




using namespace std;

/*
----------------------------------------------------------
----------------CONSTRUCTOR------------------------------
----------------------------------------------------------
*/

PotentiometerSensor::PotentiometerSensor() : m_pin(ARDUINO_UNO_A2), m_id(0){
        printf("You need to connect the right potentiometer sensor to the IMU's pin A2\n");
}

PotentiometerSensor::PotentiometerSensor(PinName pin, short int id) : m_pin(pin), m_id(id){}


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

void PotentiometerSensor::setId(short int id){
    m_id = id;
}

const short int &PotentiometerSensor::getId() const{
    return m_id;
}

void PotentiometerSensor::setOffset(float offset){
    m_offset_standing = offset;
}

const float& PotentiometerSensor::getOffset() const{
    return m_offset_standing;
}

void PotentiometerSensor::setColor(char color){
    m_color = color;
}

const char& PotentiometerSensor::getColor() const{
    return m_color;
}


/*
----------------------------------------------------------
----------------------------METHODS-----------------------
----------------------------------------------------------
*/

/**
 * @brief Retrieves the raw data of the potentiometer in percent [0; 100]%.
 * 
 * @return float 
 */
float PotentiometerSensor::getRawDataPercentage() const
{
    AnalogIn pot(m_pin);
    return pot.read()*100.0f;
}

/**
 * @brief Gets the raw data from [0; 1] float
 * 
 * @return float 
 */
float PotentiometerSensor::getRawData0to1() const
{
    AnalogIn pot(m_pin);
    return pot.read();
}

/**
 * @brief Retrieves the raw data, from [0; UINT16_MAX] uint16_t
 * 
 * @return uint16_t 
 */
uint16_t PotentiometerSensor::getRawData_u16() const
{
    AnalogIn pot(m_pin);
    return pot.read_u16();
}

/**
 * @brief Gets the raw data, from [0; UINT16_MAX] uint16_t and converts it to a percentage of [100; 0]% (assuming you want the positive increment direction of the potentiometer to be reversed
 * 
 * @return float 
 */
float PotentiometerSensor::getRawDataOffsetPercentage_u16() const
{
    AnalogIn pot(m_pin);
    return map(pot.read_u16(), 0.0f, m_offset_standing, 100.0f, 0.0f);
}

/**
 * @brief Gets the raw data from [0; 1] float after offset
 * You need to be the offsert with method read()
 * 
 * @return float 
 */
float PotentiometerSensor::getRawDataOffset0to1() const
{
    AnalogIn pot(m_pin);
    return (pot.read() - m_offset_standing);
}

/**
 * @brief Gets the raw data from [0; UINT16_MAX] uint16_t after offset
 * You need to be the offsert with method read_u16()
 * @return uint16_t 
 */
uint16_t PotentiometerSensor::getRawDataOffset_u16() const
{
    AnalogIn pot(m_pin);
    return (pot.read_u16() - m_offset_standing);
}

/**
 * @brief To display information to the user
 * 
 */
void PotentiometerSensor::displayPercentage() const{
    AnalogIn pot(m_pin);
    float value = pot.read()*100.0f;
    printf("Percentage : %f\n", value);
}

/**
 * @brief Gives the position of the wiper on the potentiometer in [cm]
 * 
 * @return float 
 */
float PotentiometerSensor::calculateDistance() const
{
    float position_soft_pot_offset = 0;
    float position_soft_pot = 0;

    // Initialise the soft pot
    AnalogIn pot(m_pin);
    uint16_t softPot = pot.read_u16();

 
                                        /* 
                                        Map the raw data after average,
                                        a Voltage limitation can occur due to the +-20% tolerance of the potentiometers.
                                        then we invert the values sens because of the angle increase when the distance increase and not vice-versa

                                        */
    position_soft_pot = map(softPot, 0, UINT16_MAX - VOLTAGE_LIMITATION, ACTIVE_LENGTH, 0); 

    //We apply the offset
    position_soft_pot_offset = position_soft_pot - m_offset_standing; //We have the Zero angle

    return position_soft_pot_offset;
}

/**
 * @brief Gives the position of the wiper on the potentiometer in [degree]
 * 
 * @return float 
 */
float PotentiometerSensor::calculateAngle() const
{
    float position_soft_pot_offset = 0;
    float position_soft_pot = 0;

    // Initialise the soft pot
    AnalogIn pot(m_pin);
    uint16_t softPot = pot.read_u16();

                                        /* 
                                        Map the raw data after average,
                                        a Voltage limitation can occur due to the +-20% tolerance of the potentiometers.
                                        then we invert the values sens because of the angle increase when the distance increase and not vice-versa

                                        */
    position_soft_pot = map(softPot, 0, UINT16_MAX - VOLTAGE_LIMITATION, ACTIVE_ANGLE, 0); 

    //We apply the offset
    position_soft_pot_offset = position_soft_pot - (m_offset_standing*ACTIVE_ANGLE)/ACTIVE_LENGTH; //We have the Zero angle

    return position_soft_pot_offset;
}

/**
 * @brief To display linear position of the wiper at the user
 * 
 * @param result_position 
 */
void PotentiometerSensor::displayDistance(float result_position) const
{
    if(m_id == 0)
        cout << "Right hip >  Distance : (" + to_string(result_position) + ") > Angle : (" + to_string((result_position*ACTIVE_ANGLE)/ACTIVE_LENGTH) +")" <<endl; // We have the distance 
    else
        cout << "Left hip >  Distance : (" + to_string(result_position) + ") > Angle : (" + to_string((result_position*ACTIVE_ANGLE)/ACTIVE_LENGTH) +")" <<endl; // We have the distance 
    wait_us(5E+4);
}

/**
 * @brief To display angulzr position of the wiper at the user
 * 
 * @param result_position 
 */
void PotentiometerSensor::displayAngle(float result_position) const
{
    if(m_id == 0)
        cout << "Right hip > Angle : (" + to_string(result_position) +")" <<endl; // We have the distance 
    else
        cout << "Left hip > Angle : (" + to_string(result_position) +")" <<endl; // We have the distance 
    wait_us(5E+4);
}


/**
 * @brief Determines the torque exerted by a rod of the exoskeleton
 * There are two types of polynomial approximation for the four colours present
 * 
 * @param angle 
 * @param sens 
 * @return float 
 */
float PotentiometerSensor::calculateTorque(float angle, char& sens) const 
{
    // Link for the polynomial solver
    //https://arachnoid.com/polysolve/

    // Type of gaz (blue, green, red, yellow) more strong to less strong 
    double terms[10] ={0};
    
    double t = 1;
    double torque = 0;

    switch (m_color) 
    {
        case 'b':
            if(sens == 'i')
            {   
                //Excel
                //torque = 0.000482886135895*pow(angle, 6) - 0.025761391549253*pow(angle, 5) + 0.534426185102348*pow(angle, 4) - 5.373631112830480*pow(angle, 3) + 25.860036109894300*pow(angle, 2) - 44.959960824227900*angle + 23.998076786112500;
                //y = 0.000482886135895x6 - 0.025761391549253x5 + 0.534426185102348x4 - 5.373631112830480x3 + 25.860036109894300x2 - 44.959960824227900x + 23.998076786112500

                //Polynomial solver
                double terms[] = {
                    6.0390762886100471e-002,
                    3.4485146154067359e-001,
                    -8.0223005018291674e-002,
                    1.0694637155283963e-002,
                    -4.3713446023477379e-004,
                    8.7944609802356418e-006,
                    -9.9303659941212642e-008,
                    6.4280352925394151e-010,
                    -2.2330077126102855e-012,
                    3.2331182714101534e-015
                };               
                for (double c : terms) {
                    torque += c * t;
                    t *= angle;
                }
            }
            else if(sens == 'd')
            {
                //torque = 0.000444616442161*pow(angle, 6) - 0.023418671065599*pow(angle, 5) + 0.479098858596444*pow(angle, 4) - 4.736468992021400*pow(angle, 3) + 22.039032338478100*pow(angle, 2) - 32.350948178442100*angle + 14.099999838566900;
                //y = 0,000444616442161x6 - 0,023418671065599x5 + 0,479098858596444x4 - 4,736468992021400x3 + 22,039032338478100x2 - 32,350948178442100x + 14,099999838566900
               
               //Polynomial solver
                double terms[] = {
                    -4.9226005826688679e-001,
                    -6.8032073413081040e-002,
                    1.0476701083793988e-001,
                    -3.0453679495789678e-003,
                    3.6867475121167207e-005,
                    -2.0750972489400584e-007,
                    4.4461644385231642e-010
                };
                for (double c : terms) {
                    torque += c * t;
                    t *= angle;
                }
            }
            else
            {
                cerr << "ERROR : The direction of the forces is not specified" << endl;
            }

            break;
        case 'g': 
            if(sens == 'i')
            {
                //Excel
                //torque = 0.000584795320571*pow(angle, 6) - 0.029226204597137*pow(angle, 5) + 0.568705180045981*pow(angle, 4) - 5.378646691402540*pow(angle, 3) + 24.472981947416000*pow(angle, 2) - 40.699176305672100*angle + 21.038461400661600;
                //y = 0.000584795320571x6 - 0.029226204597137x5 + 0.568705180045981x4 - 5.378646691402540x3 + 24.472981947416000x2 - 40.699176305672100x + 21.038461400661600

                //Polynomial solver
                double terms[] = {
                    1.4152323353517096e-001,
                    -8.8510073798745048e-001,
                    1.5308174517411952e-001,
                    -5.1579227494345020e-003,
                    8.3589098117597884e-005,
                    -7.6255260145990079e-007,
                    4.1077010330723478e-009,
                    -1.2860197026092484e-011,
                    1.9154590238220645e-014
                };
                for (double c : terms) {
                    torque += c * t;
                    t *= angle;
                }

            }
            else if(sens == 'd')
            {   
                //Excel
                //torque = 0.000531690745487*pow(angle, 6) - 0.026968871506028*pow(angle, 5) + 0.529723511205702*pow(angle, 4) - 5.006983548213610*pow(angle, 3) + 22.208000887942000*pow(angle, 2) - 31.70109453191980*angle + 13.518269079213500;
                //y = 0.000531690745487x6 - 0.026968871506028x5 + 0.529723511205702x4 - 5.006983548213610x3 + 22.208000887942000x2 - 31.701094531919800x + 13.518269079213500

                //Polynomial solver
                double terms[] = {
                    -4.8529411345834950e-001,
                    -3.3989047624638946e-002,
                    1.0158009135816432e-001,
                    -3.1741362085812453e-003,
                    4.0779864984530478e-005,
                    -2.4149103001697497e-007,
                    5.4136566996892976e-010
                };
                for (double c : terms) {
                    torque += c * t;
                    t *= angle;
                } 
            }
            else
            {
                cerr << "ERROR : The direction of the forces is not specified" << endl;
            }
            break;
        case 'r':
            if(sens == 'i')
            {
                //Excel
                //torque = 0.000740884071766*pow(angle, 6) - 0.034779610171597*pow(angle, 5) + 0.637323204316090*pow(angle, 4) - 5.693099494121270*pow(angle, 3) + 24.577364874276100*pow(angle, 2) -  39.389558979892200*angle + 19.839160895207800;
                //y = 0.000740884071766x6 - 0.034779610171597x5 + 0.637323204316090x4 - 5.693099494121270x3 + 24.577364874276100x2 - 39.389558979892200x + 19.839160895207800

                //Polynomial solver
                double terms[] = {
                    1.3639985396809645e-001,
                    -9.3425351689018665e-001,
                    1.6632246780315246e-001,
                    -6.0843833196909036e-003,
                    1.0459931738406873e-004,
                    -9.4685497199778344e-007,
                    4.3384200766486827e-009,
                    -7.9066725298070787e-012
                };
                for (double c : terms) {
                    torque += c * t;
                    t *= angle;
                }
            }
            else if(sens == 'd')
            {
                //Excel
                //torque = 0.000744324044238*pow(angle, 6) - 0.03498898541931*pow(angle, 5) + 0.640944767636029*pow(angle, 4) - 5.700002742916690*pow(angle, 3) + 24.139945027593100*pow(angle, 2) - 34.778478968422800*angle + 15.426573519653200;
                //y = 0.000744324044238x6 - 0.034988985419318x5 + 0.640944767636029x4 - 5.700002742916690x3 + 24.139945027593100x2 - 34.778478968422800x + 15.426573519653200

                //Polynomial solver
                double terms[] = {
                    -2.6625346143605733e-002,
                    -7.3699938708628498e-001,
                    1.8443799589857007e-001,
                    -7.1178127635252312e-003,
                    1.2763134144146265e-004,
                    -1.2051471769241495e-006,
                    5.7752844536866382e-009,
                    -1.1057055834412505e-011
                };
                for (double c : terms) {
                    torque += c * t;
                    t *= angle;
                }
            }
            else
            {
                cerr << "ERROR : The direction of the forces is not specified" << endl;
            }
            break;
        case 'y': 
            if(sens == 'i')
            {   
                //Excel
                //torque = 0.001689886477337*pow(angle, 6) - 0.069108716209712*pow(angle, 5) + 1.10140807085372*pow(angle, 4) - 8.561277646775120*pow(angle, 3) + 32.475335131632100*pow(angle, 2) - 48.462508941418500*angle + 23.398601277847800;
                //y = 0.001689886477337x6 - 0.069108716209712x5 + 1.101408070853720x4 - 8.561277646775120x3 + 32.475335131632100x2 - 48.462508941418500x + 23.398601277847800

                //Polynomial solver
                double terms[] = {
                    6.0890204193942542e-003,
                    -7.2363590049897608e-001,
                    1.4092279491048171e-001,
                    -3.9526577476453467e-003,
                    -1.8371920043055702e-005,
                    2.7447954859745584e-006,
                    -5.5813500684477708e-008,
                    5.3711622028302896e-010,
                    -2.5997107503181653e-012,
                    5.1181669926869705e-015
                };
                for (double c : terms) {
                    torque += c * t;
                    t *= angle;
                }
            }
            else if(sens == 'd')
            {
                //Excel
                //torque = 0.001152390778429*pow(angle, 6) - 0.047797094441989*pow(angle, 5) + 0.774612339833766*pow(angle, 4) - 6.125944474522840*pow(angle, 3) + 23.272374372289100*pow(angle, 2) - 30.701196036650800*angle + 12.552447444584700;
                //y = 0.001152390778429x6 - 0.047797094441989x5 + 0.774612339833766x4 - 6.125944474522840x3 + 23.272374372289100x2 - 30.701196036650800x + 12.552447444584700

                //Polynomial solver
                double terms[] = {
                    -2.6518218666320192e-001,
                    1.4370142680135034e-002,
                    9.3085141982699085e-002,
                    -3.5771779160659201e-003,
                    5.7052274327791571e-005,
                    -4.2378145573282467e-007,
                    1.1996904020126815e-009
                };
                for (double c : terms) {
                    torque += c * t;
                    t *= angle;
                }
            }
            else
            {
                cerr << "ERROR : The direction of the forces is not specified" << endl;
            }
            break;
        default:
            cerr << "ERROR : You didn't chose a type of gaz" << endl;
            break;
    }
    return torque;
}


//NEED TO BE IMPROVE WITH THE IMU
/**
 * @brief Determine on which curves to calculate the torque of one of the rods of the exo as a function of the displacement of the wiper at time t-1 and at time t
 * 
 * @param force_direction 
 * @param last_angle 
 */
void PotentiometerSensor::forceDirection(char& force_direction, uint16_t last_angle) const
{    
    int dead_zone = 200;
    // Initialise the soft pot
    AnalogIn pot(m_pin);
    wait_us(1E+4);
    uint16_t new_angle = pot.read_u16();

    //Stable zone, the user keep the position and it always a torque 
    if(new_angle - last_angle < dead_zone && new_angle - last_angle > -dead_zone)
    {
        //Continue
    }
    else if (new_angle-last_angle < 0) {
        force_direction = 'd';
    }
    else {
        force_direction = 'i';
    }
    

}

/**
 * @brief Torque information for user
 * 
 * @param torque 
 */
void PotentiometerSensor::displayTorque(float torque) const
{
    cout << "Global torque > [" + to_string(torque) + "] Nm "; // We have the distance 
    wait_us(5E+4);
}

/**
 * @brief determines the colour on the exoskeleton according to the user input
 * 
 */
void PotentiometerSensor::chooseColor()
{
   char color = ' ';
   bool condition = true;
   string displayColor;

   do {
        cout << "What colour bars your exoskeleton has ? (b/g/r/y) : " << endl;             //Display
        cin >> color ;                                                                      //User choose the color
        condition = !((color == 'r') || (color == 'g') || (color == 'b') || (color == 'y'));//Security

        if (condition) {                                                                    //If the condition is not respected, here we go again
            cerr << "You have not entered one of the requested colours"<<endl;
        }

   }while (condition);

   switch (color) 
   {
        case 'r':
            displayColor = "red";
            break;
        case 'g':
            displayColor = "green";
            break;
        case 'b':
            displayColor = "blue";
            break;
        case 'y':
            displayColor = "yellow";
            break;
        default:
        //Impossible
            break;
   }

    cout << "You choose the "+ displayColor + " color"<<endl;
    m_color = color;
}

/**
 * @brief Map the raw data after average,
a Voltage limitation can occur due to the +-20% tolerance of the potentiometers.
then we invert the values sens because of the angle increase when the distance increase and not vice-versa
 * 
 * @param input 
 * @return float 
 */
float PotentiometerSensor::mapData(float input) const
{
    return map(input, 0, UINT16_MAX - VOLTAGE_LIMITATION, ACTIVE_LENGTH, 0);
}

/**
 * @brief For mapping value
 * 
 * @param x 
 * @param in_min 
 * @param in_max 
 * @param out_min 
 * @param out_max 
 * @return double 
 */
double map(double x, double in_min, double in_max, double out_min, double out_max) {
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

/**
 * @brief Average torque 
 * 
 * @param torque_right 
 * @param torque_left 
 * @return float 
 */
float calculateGlobalTorque(float torque_right, float torque_left)
{
    return ((torque_left+torque_right)/2);
}








