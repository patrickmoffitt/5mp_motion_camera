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

#include "feather_m0_sleep_wake_RCWL_0516.hpp"

feather_m0_sleep_wake_RCWL_0516::feather_m0_sleep_wake_RCWL_0516() {
    reset();
}

feather_m0_sleep_wake_RCWL_0516::~feather_m0_sleep_wake_RCWL_0516() {
}

/**
 * Sleep until the interrupt service routine is called.
 *
 * Interrupts are canceled preventing a race condition caused by
 * main::interrupt_service_routine() running while we're going to sleep.
 */
void feather_m0_sleep_wake_RCWL_0516::go_to_sleep() {
    DEBUG_PRINT(F("Sleeping. "));
    DEBUG_PRINTLN(F("ZZZzzz...\n"));
    noInterrupts();
    __DSB();
    __WFI(); // (Wake From Interrupt).
    interrupts();
}

/**
 * Reset the sleep/wake state variables.
 */
void feather_m0_sleep_wake_RCWL_0516::reset() {
    take_picture = false;
    awake = false;
}



