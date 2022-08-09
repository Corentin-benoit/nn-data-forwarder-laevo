![](https://www.laevo-exoskeletons.com/)
# Project of smart exoskeleton
# Eevee2

## Introduction

  This programme is a component of the second version of Eevee2, the second generation of the connected exoskeleton prototype developed by Lucas Ribeiro and Corentin Benoit.

## Objective

    This program pre-processes the raw sensor data to make it readable by edge-impulse.
    Allow :
    * Initialisation of sensors
    * Calibration of gyroscope
    * Data standardisation.

## Hardware 

  * Membrane potentiometer SoftPot [Datasheet](https://www.spectrasymbol.com/wp-content/uploads/2016/12/SOFTPOT-DATA-SHEET-Rev-F3.pdf).
  * Electronic board STM32L4S5I [Datasheet](https://www.st.com/en/evaluation-tools/b-l4s5i-iot01a.html).
  * Wiper for membrane potentiometer.
  * Fixing parts.
  * Laevo Flex.

## Software

### Be careful, you need, before to launch the code, enter the different membrane potentiometer characteristics.
  ```bash
  const int ACTIVE_LENGTH = 170-10; // SoftPot active length taking into account its (EET) Efective Electrical Travel
  const int ACTIVE_ANGLE = 255; // SoftPot active angle, conversion of active length in theory but may vary due to curved potentiometer
  const int VOLTAGE_LIMITATION = 0; //In the event that the input resistance reduces the current in the input pin too much, the new maximum should be measured and subtracted from 
  UINT16_T_MAX
  ```