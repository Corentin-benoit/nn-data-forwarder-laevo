/* mbed Microcontroller Library
 * Copyright (c) 2019 ARM Limited
 * SPDX-License-Identifier: Apache-2.0
 * @brief: This is the code to use for edge impulse data forwarding, prints
 * 3 accelerometer and 3 gyroscopic readings
 * Inspired from the MBED OS Code from Edge Impulse Data forwarder:
 * https://github.com/edgeimpulse/example-dataforwarder-mbed/blob/master/source/main.cpp
 */

#include "mbed.h"
#include "../LSM6DSL/LSM6DSL_acc_gyro_driver.h"
#include "../LSM6DSL/LSM6DSLSensor.h"
#include "LSM6DSL/ST_INTERFACES/Sensors/PressureSensor.h"

#include "TouchSensor.hpp"
#include "StartButton.hpp"
#include "PotentiometerSensor.hpp"

/*
----------------------------------------------------------
--------------------INITIALISATION------------------------
----------------------------------------------------------
*/
////Initialise the startButton
StartButton startButton;

// Initialise the digital pin LED1 as an output
DigitalOut led1(LED1);

//Initialise the touch button on the A1 port
TouchSensor sensorButton(A5);

//Initialise the potentiometers 
PotentiometerSensor potentiometer_right(A1);
PotentiometerSensor potentiometer_left(A3);

// Set the sampling frequency in Hz
static int16_t sampling_freq = 100;
static int64_t time_between_samples_us = (1000000 / (sampling_freq));

// Measurements
float gyrOffX = 0; float gyrOffY = 0; float gyrOffZ = 0;

// Blinking rate in milliseconds
#define BLINKING_RATE    500ms
#define BUSY_LOOP_DURATON 9ms

// Check if this checks out for B-L4S5I board
static DevI2C devI2C(PB_11, PB_10);

static LSM6DSLSensor acc_gyro(&devI2C, LSM6DSL_ACC_GYRO_I2C_ADDRESS_LOW, PD_11);

////Initialise pressureSensor


/*
----------------------------------------------------------
--------------------FUNCTIIONS----------------------------
----------------------------------------------------------
*/
void calibrate_sensors(float accDiv, float gyrDiv, int N) {
    int startSkip = 200;
    int32_t gyro_val_buf[3];
    int gyrTotX = 0; 
    int gyrTotY = 0; 
    int gyrTotZ = 0;
    float pot_val_buf[2];
    float pot_right = 0;
    float pot_left = 0;

    // init initializes the component
    acc_gyro.init(NULL);
    // enables the accelero
    acc_gyro.enable_x();
    // enable gyro
    acc_gyro.enable_g();

    Timer t_ML;
    t_ML.start();

    for (int i = 0; i < N + startSkip; i++) {
        int64_t next_tick = t_ML.elapsed_time().count() + time_between_samples_us;
        acc_gyro.get_g_axes(gyro_val_buf);
        pot_val_buf[0] = potentiometer_right.displayRaw();
        pot_val_buf[1] = potentiometer_left.displayRaw();

        if (i >= startSkip) {
            gyrTotX += gyro_val_buf[0];
            gyrTotY += gyro_val_buf[1];
            gyrTotZ += gyro_val_buf[2];
            pot_right +=pot_val_buf[0];
            pot_left += pot_val_buf[1];
        }
        
        printf("%f\t%f\t%f\t%f\t%f\n",
            static_cast<float>(gyro_val_buf[0]) / gyrDiv,
            static_cast<float>(gyro_val_buf[1]) / gyrDiv,
            static_cast<float>(gyro_val_buf[2]) / gyrDiv,
            pot_val_buf[0],
            pot_val_buf[1]);

        if (i % 20 == 0) {
            led1 = !led1;
        }

        while (t_ML.elapsed_time().count() < next_tick){
            // busy loop
        } 

    }
    t_ML.stop();
    potentiometer_right.setOffset(pot_right/N);
    potentiometer_left.setOffset(pot_left/N);
    gyrOffX = gyrTotX/N;
    gyrOffY = gyrTotY/N;
    gyrOffZ = gyrTotZ/N;
    pot_right = pot_right/N;
    pot_left = pot_left/N;
    

    printf("Calibration Finished, offsets:\n");
    printf("%f\t%f\t%f\t%f\t%f\n\n\n",
            static_cast<float>(gyrOffX) / gyrDiv,
            static_cast<float>(gyrOffY) / gyrDiv,
            static_cast<float>(gyrOffZ) / gyrDiv,
            potentiometer_right.getOffset(),
            potentiometer_left.getOffset());
}

void initCalibration(float accDiv, float gyrDiv, int N)
{
    //user warning
    wait_us(10000);
    startButton.initDisplayIMU();
    wait_us(1500000);
    startButton.initDisplayPot();
    wait_us(10000);

    /* Calibration countdown */
    led1 = 1;
    printf("\nCalibration starting in 5 seconds, please keep the IMU horizontal and fully still.\n");
    ThisThread::sleep_for(1s);
    led1 = 0;
    printf("Calibration starting in 4 seconds.\n");
    ThisThread::sleep_for(1s);
    led1 = 1;
    printf("Calibration starting in 3 seconds.\n");
    ThisThread::sleep_for(1s);
    led1 = 0;
    printf("Calibration starting in 2 seconds.\n");
    ThisThread::sleep_for(1s);
    led1 = 1;
    printf("Calibration starting in 1 seconds.\n");
    ThisThread::sleep_for(1s);
    led1 = 0;
    printf("Calibration Started. This will take approximately %d seconds.\n", N/sampling_freq);

    calibrate_sensors(accDiv, gyrDiv, N);
    
}



int main(void)
{

    //Init
    float accDiv = 100.0f;
    float gyrDiv = 1000.0f;
    int N = 1010;

    startButton.displayWait();

    int32_t acc_val_buf[3];
    int32_t gyro_val_buf[3];
    // init initializes the component
    acc_gyro.init(NULL);
    // enables the accelero
    acc_gyro.enable_x();
    // enable gyro
    acc_gyro.enable_g();

    //Initialize timer
    Timer t;
    t.start();

    //Initialise the calibrations of all sensors
    initCalibration(accDiv, gyrDiv, N);
 

    ThisThread::sleep_for(3s);

    while (1) {
        int64_t next_tick = t.elapsed_time().count() + time_between_samples_us;
        acc_gyro.get_x_axes(acc_val_buf);
        acc_gyro.get_g_axes(gyro_val_buf);
        //numbers
        printf("%f\t%f\t%f\t%f\t%f\t%f\t%d\t%f\t%f\n",
            static_cast<float>(acc_val_buf[0]) / accDiv,            
            static_cast<float>(acc_val_buf[1]) / accDiv,
            static_cast<float>(acc_val_buf[2]) / accDiv,
            static_cast<float>(gyro_val_buf[0] - gyrOffX) / gyrDiv,
            static_cast<float>(gyro_val_buf[1] - gyrOffY) / gyrDiv,
            static_cast<float>(gyro_val_buf[2] - gyrOffZ) / gyrDiv,
            sensorButton.detection(),
            potentiometer_right.displayRawOffset(),
            potentiometer_left.displayRawOffset());
        
        wait_us(10000);

        while (t.elapsed_time().count() < next_tick){
            // busy loop
        } 
    }
    

}
