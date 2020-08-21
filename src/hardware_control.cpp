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

#include "hardware_control.hpp"

///< https://github.com/mpflaga/Arduino-MemoryFree
#ifdef __arm__
    // should use uinstd.h to define sbrk but Due causes a conflict
    extern "C" char* sbrk(int incr);
#else   // __ARM__
    extern char *__brkval;
#endif  // __arm__ * @param buffer

/**
 * Return the size of free memory in bytes.
 *
 * @see https://learn.adafruit.com/memories-of-an-arduino/measuring-free-memory
 * @see https://github.com/mpflaga/Arduino-MemoryFree
 */
int hardware_control::free_memory() {
    char top;
    #ifdef __arm__
        return &top - reinterpret_cast<char*>(sbrk(0));
    #elif defined(CORE_TEENSY) || (ARDUINO > 103 && ARDUINO != 151)
        return &top - __brkval;
    #else   // __arm__
        return __brkval ? &top - __brkval : &top - __malloc_heap_start;
    #endif  // __arm__
}

/**
 * Control the onboard LED.
 */
void hardware_control::setup_onboard_led() {
    pinMode(LED_BUILTIN, OUTPUT);
    hardware_control::onboard_led_off();
}

/**
 * Control the battery voltage pin.
 */
void hardware_control::setup_battery_v() {
    pinMode(BATTERY_PIN, INPUT);
}

/**
 * Control the power on/off pin for the camera.
 */
void hardware_control::setup_camera() {
    pinMode(CAMERA_EN, OUTPUT);
    hardware_control::camera_on();
}

/**
 * Binds the Interrupt Service Routing (ISR) function to the interrupt pin.
 *
 * @param isr pointer to the interrupt_service_routine function.
 */
void hardware_control::setup_motion_sensor(void (*isr)()) {
    pinMode(MOTION_PIN, INPUT_PULLDOWN);
    attachInterrupt(digitalPinToInterrupt(MOTION_PIN), isr, RISING);
    pinMode(PIN_A0, OUTPUT);
}

/**
 * Turn the motion sensor off.
 *
 * The RCWL-0516 motion sensor is powered by analog pin 0.
 */
void hardware_control::motion_sensor_off() {
    if (digitalRead(PIN_A0) == HIGH) {
        noInterrupts();
        while (digitalRead(MOTION_PIN) != LOW) {
            digitalWrite(PIN_A0, LOW);
            delayMicroseconds(500);
        }
        interrupts();
        DEBUG_PRINTLN(F("Waking up. Motion sensor off.\n"));
    }
}

/**
 * Turn the motion sensor on.
 *
 * The RCWL-0516 motion sensor is powered by analog pin 0.
 *
 * The motion sensor is powered up each time the feather goes to sleep.
 * It's output goes high as it starts up potentially giving a false alarm.
 * The interrupt guards and delayMicroseconds prevent this.
 */
void hardware_control::motion_sensor_on() {
    if (digitalRead(PIN_A0) == LOW) {
        noInterrupts();
        while (digitalRead(MOTION_PIN) != HIGH) {
            digitalWrite(PIN_A0, HIGH);
            delayMicroseconds(500);
        }
        interrupts();
        DEBUG_PRINTLN(F("Motion sensor on.\n"));
    }
}

/**
 * Turn the onboard LED on.
 */
void hardware_control::onboard_led_on() {
    digitalWrite(LED_BUILTIN, HIGH);
}

/**
 * Turn the onboard LED off.
 */
void hardware_control::onboard_led_off() {
    digitalWrite(LED_BUILTIN, LOW);
}

/**
 * Turn the camera on.
 */
void hardware_control::camera_on() {
    if (digitalRead(CAMERA_EN) == LOW) {
        noInterrupts();
        while (digitalRead(CAMERA_EN) != HIGH) {
            digitalWrite(CAMERA_EN, HIGH);
            delayMicroseconds(500);
        }
        interrupts();
        DEBUG_PRINTLN_V(F("Camera power on."));
    }
}

/**
 * Turn the camera off.
 */
void hardware_control::camera_off() {
    if (digitalRead(CAMERA_EN) == HIGH) {
        noInterrupts();
        while (digitalRead(CAMERA_EN) != LOW) {
            digitalWrite(CAMERA_EN, LOW);
            delayMicroseconds(500);
        }
        interrupts();
        DEBUG_PRINTLN_V(F("Camera power off."));
    }
}