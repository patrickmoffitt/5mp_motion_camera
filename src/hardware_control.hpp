/*
    Copyright (c) 2020 Patrick Moffitt

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in all
    copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
    SOFTWARE.
 */

#ifndef MOTION_CAMERA_HARDWARE_HPP
#define MOTION_CAMERA_HARDWARE_HPP
#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>
#include "serial_debug_error.hpp"

#define BATTERY_PIN PIN_A7
#define CAMERA_EN 10
#define MOTION_PIN 5

/*
 * A collection of hardware control functions for the project.
 */
struct hardware_control{
    static int free_memory();
    static void setup_onboard_led();
    static void setup_battery_v();
    static void setup_camera();
    static void setup_motion_sensor(void (*isr)());
    static void motion_sensor_off();
    static void motion_sensor_on();
    static void onboard_led_on();
    static void onboard_led_off();
    static void camera_on();
    static void camera_off();
};


#endif //MOTION_CAMERA_HARDWARE_HPP
