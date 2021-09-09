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

// Set the sampling frequency in Hz
static int16_t sampling_freq = 101;
static int64_t time_between_samples_us = (1000000 / (sampling_freq - 1));

// Blinking rate in milliseconds
#define BLINKING_RATE    500ms
#define BUSY_LOOP_DURATON 9ms

// Check if this checks out for B-L4S5I board
static DevI2C devI2C(PB_11, PB_10);

static LSM6DSLSensor acc_gyro(&devI2C, LSM6DSL_ACC_GYRO_I2C_ADDRESS_LOW, PD_11);

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
    }
}
