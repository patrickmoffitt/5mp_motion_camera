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

#include <numeric>
#include "read_battery.hpp"

using namespace std;

/**
 * Read the battery voltage.
 *
 * @return the voltage as a float.
 */
float battery::vdc() {
    const size_t readings_len{30};
    array<int, readings_len> readings;
    fill_n(begin(readings), readings_len, 0);
    analogReadResolution(10);
    analogReference(AR_DEFAULT);
    for (int i=0; i < (int) readings_len; i++) {
        readings[i] = analogRead(ADC_PIN);
        delay(33);
    }
    int sum = accumulate(begin(readings), end(readings), 0, plus<int>());
    int level = (sum / readings_len);
    DEBUG_PRINT(F("Raw ADC value: "));
    DEBUG_PRINTLN(level);
    // Multiply by 2 to reverse the voltage division.
    // Multiply by the voltage reference and divide by 1024 to get volts.
    return ((float) level * ADC_10_BIT_CORRECTION) * 2.0 * 3.3 / 1024.0;
}

/**
 * Format the battery voltage as a percent.
 *
 * @return an integer between 0 and 100.
 */
int battery::percent() {
    float vdc = battery::vdc();
    DEBUG_PRINT(F("Battery: ")); DEBUG_PRINT(vdc); DEBUG_PRINTLN(F(" VDC"));
    int level;
    // Convert battery level to percent.
    level = battery::map(vdc, MIN_BATTERY_VDC, MAX_BATTERY_VDC, 0.0, 100.0);
    DEBUG_PRINT(F("Battery level: "));
    DEBUG_PRINT(level);
    DEBUG_PRINTLN(F("%"));
    return level;
}

/**
 * Maps the battery voltage onto a percentage.
 *
 * @param x the battery voltage as a float
 * @param in_min the battery minimum cut-off voltage.
 * @param in_max the battery maximum cut-off voltage.
 * @param out_min the start of the percentage map
 * @param out_max the end of the percentage map
 * @return an integer between 0 and 100.
 */
int battery::map(float x, float in_min, float in_max, float out_min, float out_max) {
    float divisor = (in_max - in_min);
    if(divisor == 0){
        return -1; //AVR returns -1, SAM returns 0
    }
    return (x - in_min) * (out_max - out_min) / divisor + out_min;
}