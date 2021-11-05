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
#include "hello.h"
#include <cstdint>


DigitalOut led1(LED1);

// Set the sampling frequency in Hz
static int16_t sampling_freq = 101;
static int64_t time_between_samples_us = (1000000 / (sampling_freq - 1));

// Measurements
// int accTotX = 0; int accTotY = 0; int accTotZ = 0;
// int gyrTotX = 0; int gyrTotY = 0; int gyrTotZ = 0;
// float accAvgX = 0; float accAvgY = 0; float accAvgZ = 0;
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
        // int64_t next_tick = t.read_us() + time_between_samples_us;
        int64_t next_tick = t_ML.elapsed_time().count() + time_between_samples_us;
        // printf("gyro enable val: %d\n", check_ena_g);
        acc_gyro.get_g_axes(gyro_val_buf);
        // return is 0 for success else its a fail
        // printf("ACC reading: x=%d, y=%d, z=%d\n", acc_val_buf[0], acc_val_buf[1], acc_val_buf[2]);
        // printf("GYRO reading: x=%d, y=%d, z=%d\n", gyro_val_buf[0], gyro_val_buf[1], gyro_val_buf[2]);

        if (i >= startSkip) {
            gyrTotX += gyro_val_buf[0];
            gyrTotY += gyro_val_buf[1];
            gyrTotZ += gyro_val_buf[2];
        }
        
        printf("%f\t%f\t%f\n",
            static_cast<float>(gyro_val_buf[0]) / gyrDiv,
            static_cast<float>(gyro_val_buf[1]) / gyrDiv,
            static_cast<float>(gyro_val_buf[2]) / gyrDiv);
        // led = !led;
        // ThisThread::sleep_for(BUSY_LOOP_DURATON);

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
    
    int32_t acc_val_buf[3];
    int32_t gyro_val_buf[3];
    // init initializes the component
    acc_gyro.init(NULL);
    // enables the accelero
    acc_gyro.enable_x();
    // enable gyro
    acc_gyro.enable_g();

    // int y = bar(20);
    Timer t;
    t.start();

    /* ORIGINAL
    while (true) {
        // int64_t next_tick = t.read_us() + time_between_samples_us;
        int64_t next_tick = t.elapsed_time().count() + time_between_samples_us;
        // printf("gyro enable val: %d\n", check_ena_g);
        acc_gyro.get_x_axes(acc_val_buf);
        acc_gyro.get_g_axes(gyro_val_buf);
        // return is 0 for success else its a fail
        // printf("ACC reading: x=%d, y=%d, z=%d\n", acc_val_buf[0], acc_val_buf[1], acc_val_buf[2]);
        // printf("GYRO reading: x=%d, y=%d, z=%d\n", gyro_val_buf[0], gyro_val_buf[1], gyro_val_buf[2]);
        printf("%f\t%f\t%f\t%f\t%f\t%f\n",
            static_cast<float>(acc_val_buf[0]) / 100.0f,
            static_cast<float>(acc_val_buf[1]) / 100.0f,
            static_cast<float>(acc_val_buf[2]) / 100.0f,
            static_cast<float>(gyro_val_buf[0]) / 1000.0f,
            static_cast<float>(gyro_val_buf[1]) / 1000.0f,
            static_cast<float>(gyro_val_buf[2]) / 1000.0f);
        // led = !led;
        // ThisThread::sleep_for(BUSY_LOOP_DURATON);
        while (t.elapsed_time().count() < next_tick){
            // busy loop
        } 

    } */

    float accDiv = 100.0f;
    float gyrDiv = 1000.0f;
    int N = 1010;
    //int startSkip = 20;                 //amount of starting samples to skip because of irregularities at measurement start

    calibrate_acc_gyr(accDiv, gyrDiv, N);

    ThisThread::sleep_for(3s);

    /*
    // Calibration countdown 
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

    for (int i = 0; i < N; i++) {
        // int64_t next_tick = t.read_us() + time_between_samples_us;
        int64_t next_tick = t.elapsed_time().count() + time_between_samples_us;
        // printf("gyro enable val: %d\n", check_ena_g);
        acc_gyro.get_x_axes(acc_val_buf);
        acc_gyro.get_g_axes(gyro_val_buf);
        // return is 0 for success else its a fail
        // printf("ACC reading: x=%d, y=%d, z=%d\n", acc_val_buf[0], acc_val_buf[1], acc_val_buf[2]);
        // printf("GYRO reading: x=%d, y=%d, z=%d\n", gyro_val_buf[0], gyro_val_buf[1], gyro_val_buf[2]);

        if (i >= startSkip) {
            accTotX += acc_val_buf[0];
            accTotY += acc_val_buf[1];
            accTotZ += acc_val_buf[2];
            gyrTotX += gyro_val_buf[0];
            gyrTotY += gyro_val_buf[1];
            gyrTotZ += gyro_val_buf[2];
        }
        
        printf("%f\t%f\t%f\t%f\t%f\t%f\n",
            static_cast<float>(acc_val_buf[0]) / accDiv,            
            static_cast<float>(acc_val_buf[1]) / accDiv,
            static_cast<float>(acc_val_buf[2]) / accDiv,
            static_cast<float>(gyro_val_buf[0]) / gyrDiv,
            static_cast<float>(gyro_val_buf[1]) / gyrDiv,
            static_cast<float>(gyro_val_buf[2]) / gyrDiv);
        // led = !led;
        // ThisThread::sleep_for(BUSY_LOOP_DURATON);

        if (i % 20 == 0) {
            led1 = !led1;
        }

        while (t.elapsed_time().count() < next_tick){
            // busy loop
        } 

    }

    accAvgX = accTotX/(N-startSkip);
    accAvgY = accTotY/(N-startSkip);
    accAvgZ = accTotZ/(N-startSkip);
    gyrAvgX = gyrTotX/(N-startSkip);
    gyrAvgY = gyrTotY/(N-startSkip);
    gyrAvgZ = gyrTotZ/(N-startSkip);

    printf("Calibration Finished, starting Data Forwarder.\n\n\n");
    

    led1 = 1;

    printf("-----------------------------------------------------------------------------------------\n");
    printf("%f\t%f\t%f\t%f\t%f\t%f\n\n",
            accAvgX/accDiv,
            accAvgY/accDiv,
            accAvgZ/accDiv,
            gyrAvgX/gyrDiv,
            gyrAvgY/gyrDiv,
            gyrAvgZ/gyrDiv);
    printf("%f\t%f\t%f\t%f\t%f\t%f\n\n", accAvgX, accAvgY, accAvgZ, gyrAvgX, gyrAvgY, gyrAvgZ);

    */



    // int accTotX = 0; int accTotY = 0; int accTotZ = 0;
    // int gyrTotX = 0; int gyrTotY = 0; int gyrTotZ = 0;
    // int N2 = 100;
    while (1) {
        // int64_t next_tick = t.read_us() + time_between_samples_us;
        int64_t next_tick = t.elapsed_time().count() + time_between_samples_us;
        // printf("gyro enable val: %d\n", check_ena_g);
        acc_gyro.get_x_axes(acc_val_buf);
        acc_gyro.get_g_axes(gyro_val_buf);
        // return is 0 for success else its a fail
        // printf("ACC reading: x=%d, y=%d, z=%d\n", acc_val_buf[0], acc_val_buf[1], acc_val_buf[2]);
        // printf("GYRO reading: x=%d, y=%d, z=%d\n", gyro_val_buf[0], gyro_val_buf[1], gyro_val_buf[2]);

        // if (i >= startSkip) {
        //     accTotX += acc_val_buf[0] - accAvgX;
        //     accTotY += acc_val_buf[1] - accAvgY;
        //     accTotZ += acc_val_buf[2] - (accAvgZ - 1000);
        //     gyrTotX += gyro_val_buf[0] - gyrAvgX;
        //     gyrTotY += gyro_val_buf[1] - gyrAvgY;
        //     gyrTotZ += gyro_val_buf[2] - gyrAvgZ;
        // }
        

        // // numbers
        printf("%f\t%f\t%f\t%f\t%f\t%f\n",
            static_cast<float>(acc_val_buf[0]) / accDiv,            
            static_cast<float>(acc_val_buf[1]) / accDiv,
            static_cast<float>(acc_val_buf[2]) / accDiv,
            static_cast<float>(gyro_val_buf[0] - gyrOffX) / gyrDiv,
            static_cast<float>(gyro_val_buf[1] - gyrOffY) / gyrDiv,
            static_cast<float>(gyro_val_buf[2] - gyrOffZ) / gyrDiv);

        // graph version
        // valToGraph(static_cast<float>(acc_val_buf[0]) / accDiv, 20, 8);
        // printf("\t");
        // valToGraph(static_cast<float>(acc_val_buf[1]) / accDiv, 20, 8);
        // printf("\t");
        // valToGraph(static_cast<float>(acc_val_buf[2]) / accDiv, 20, 8);
        // printf("\t");
        // valToGraph(static_cast<float>(gyro_val_buf[0] - gyrAvgX) / gyrDiv, 200, 8);
        // printf("\t");
        // valToGraph(static_cast<float>(gyro_val_buf[1] - gyrAvgY) / gyrDiv, 200, 8);
        // printf("\t");
        // valToGraph(static_cast<float>(gyro_val_buf[2] - gyrAvgZ) / gyrDiv, 200, 8);
        // printf("\n");
        
        // led = !led;
        // ThisThread::sleep_for(BUSY_LOOP_DURATON);
        while (t.elapsed_time().count() < next_tick){
            // busy loop
        } 

    }

    // accAvgX = accTotX/(N2-startSkip);
    // accAvgY = accTotY/(N2-startSkip);
    // accAvgZ = accTotZ/(N2-startSkip);
    // gyrAvgX = gyrTotX/(N2-startSkip);
    // gyrAvgY = gyrTotY/(N2-startSkip);
    // gyrAvgZ = gyrTotZ/(N2-startSkip);

    // printf("-----------------------------------------------------------------------------------------\n");
    // printf("%f\t%f\t%f\t%f\t%f\t%f\n",
    //         accAvgX/accDiv,
    //         accAvgY/accDiv,
    //         accAvgZ/accDiv,
    //         gyrAvgX/gyrDiv,
    //         gyrAvgY/gyrDiv,
    //         gyrAvgZ/gyrDiv);


}
