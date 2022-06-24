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
#include <cstdint>

#include "TouchSensor.hpp"

DigitalOut led1(LED1);

// Set the sampling frequency in Hz
static int16_t sampling_freq = 57;
static int64_t time_between_samples_us = (1000000 / (sampling_freq - 1));

// Measurements
float gyrOffX = 0; float gyrOffY = 0; float gyrOffZ = 0;

// Blinking rate in milliseconds
#define BLINKING_RATE    500ms
#define BUSY_LOOP_DURATON 9ms

// Check if this checks out for B-L4S5I board
static DevI2C devI2C(PB_11, PB_10);

static LSM6DSLSensor acc_gyro(&devI2C, LSM6DSL_ACC_GYRO_I2C_ADDRESS_LOW, PD_11);

void calibrate_acc_gyr(float accDiv, float gyrDiv, int N) {
    int startSkip = 200;
    
    /* Calibration countdown */
    led1 = 1;
    printf("\nCalibration starting in 5 seconds, please keep device horizontal and fully still.\n");
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

    int32_t gyro_val_buf[3];
    int gyrTotX = 0; int gyrTotY = 0; int gyrTotZ = 0;
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

        if (i >= startSkip) {
            gyrTotX += gyro_val_buf[0];
            gyrTotY += gyro_val_buf[1];
            gyrTotZ += gyro_val_buf[2];
        }
        
        printf("%f\t%f\t%f\n",
            static_cast<float>(gyro_val_buf[0]) / gyrDiv,
            static_cast<float>(gyro_val_buf[1]) / gyrDiv,
            static_cast<float>(gyro_val_buf[2]) / gyrDiv);

        if (i % 20 == 0) {
            led1 = !led1;
        }

        while (t_ML.elapsed_time().count() < next_tick){
            // busy loop
        } 

    }
    t_ML.stop();

    gyrOffX = gyrTotX/N;
    gyrOffY = gyrTotY/N;
    gyrOffZ = gyrTotZ/N;

    printf("Calibration Finished, offsets:\n");
    printf("%f\t%f\t%f\n\n\n",
            static_cast<float>(gyrOffX) / gyrDiv,
            static_cast<float>(gyrOffY) / gyrDiv,
            static_cast<float>(gyrOffZ) / gyrDiv);
}

void valToGraph(float n, float max, int ch) {
    float bin = 1/(float)ch;
    float p = (abs(n)/max);

    for (int vtg = 0; vtg <= ch; vtg++) {
        if (p > bin*vtg) {
            printf("|");
        } else {
            printf(" ");
        }
    }
}

int main(void)
{
    // Initialise the digital pin LED1 as an output
    DigitalOut led(LED1);

    //Initialise the touch button on the A1 port
    TouchSensor sensorButton(A1);
    
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

    float accDiv = 100.0f;
    float gyrDiv = 1000.0f;
    int N = 1010;
 
    calibrate_acc_gyr(accDiv, gyrDiv, N);

    ThisThread::sleep_for(3s);

    while (1) {
        int64_t next_tick = t.elapsed_time().count() + time_between_samples_us;
        acc_gyro.get_x_axes(acc_val_buf);
        acc_gyro.get_g_axes(gyro_val_buf);
        //numbers
        printf("%f\t%f\t%f\t%f\t%f\t%f\t%d\n",
            static_cast<float>(acc_val_buf[0]) / accDiv,            
            static_cast<float>(acc_val_buf[1]) / accDiv,
            static_cast<float>(acc_val_buf[2]) / accDiv,
            static_cast<float>(gyro_val_buf[0] - gyrOffX) / gyrDiv,
            static_cast<float>(gyro_val_buf[1] - gyrOffY) / gyrDiv,
            static_cast<float>(gyro_val_buf[2] - gyrOffZ) / gyrDiv,
            sensorButton.detection());

        while (t.elapsed_time().count() < next_tick){
            // busy loop
        } 
    }
}
