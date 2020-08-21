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

#ifndef FEATHER_M0_SLEEP_WAKE_RCWL_0516_HPP
#define FEATHER_M0_SLEEP_WAKE_RCWL_0516_HPP
#include <Arduino.h>
#undef min  // @ToDo remove when library? bug fixed.
#undef max  // @ToDo remove when library? bug fixed.
#include <functional>
#include "serial_debug_error.hpp"

using time_type = decltype(millis());

#ifndef MOTION_PIN
#define MOTION_PIN 5
#endif

/**
 * A class that controls the sleep/wake functions of the Adafruit Feather M0 using
 * a microwave motion sensor.
 *
 * @var take_picture When true the loop() should take a picture.
 * @var awake When true, the loop() should not be interrupted by the
 * main::interrupt_service_routine().
 */
struct feather_m0_sleep_wake_RCWL_0516 {
    volatile bool take_picture; ///< When true the loop() should take a picture.
    volatile bool awake; ///<When true the loop() should not be interrupted by the interrupt_service_routine().

    feather_m0_sleep_wake_RCWL_0516();
    ~feather_m0_sleep_wake_RCWL_0516();
    void go_to_sleep();
    void reset();
};


#endif //FEATHER_M0_SLEEP_WAKE_RCWL_0516_HPP
