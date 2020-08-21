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

#ifndef READ_BATTERY_HPP
#define READ_BATTERY_HPP
#define ADC_PIN PIN_A7


#include <Arduino.h>
#undef min  // @ToDo remove when library? bug fixed.
#undef max  // @ToDo remove when library? bug fixed.
#include <array>
#include <algorithm>
#include "serial_debug_error.hpp"


// Analog read level is 10 bit 0-1023 (0V-1V).
// The built-in double-100K resistor divider takes the max
// LiPo value of 4.2V and divides it by 2 to get 2.1.
// this means our min analog read value should be 1.5 (3V / 2).

#define MIN_BATTERY_VDC 3.00f  // Discharge Cut-off Voltage

#define MAX_BATTERY_VDC 4.20f  // Charging Cut-off Voltage

#define ADC_10_BIT_CORRECTION 0.9990234375f  // 1 - (1/1024)

struct battery {
    static float vdc();
    static int percent();
    static int map(float x, float in_min, float in_max, float out_min, float out_max);
};
#endif //READ_BATTERY_HPP
