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

#ifndef WIFI101_HELPER_HPP
#define WIFI101_HELPER_HPP

#undef swap  // @ToDo remove when library? bug fixed.
#include <cstdint>
#include <sstream>
#include <regex>
#include <string>
#include <map>
#include <Arduino.h>
#include <WiFi101.h>

#include "serial_debug_error.hpp"

#ifndef WIFI_CS
#define WIFI_CS 8
#endif

using namespace std;

/**
 * A collection of handy functions that WiFi101 does not provide.
 *
 * @see https://github.com/arduino-libraries/WiFi101
 * @see http://ww1.microchip.com/downloads/en/DeviceDoc/Atmel-42420-WINC1500-Software-Design-Guide_UserGuide.pdf
 */
struct wifi {
    static void cs_enable();
    static void cs_disable();

    static String ipv4_int_to_str(const uint32_t ip);
    static IPAddress ipv4_string_to_addr(const string& address);
    static String mac_uint8_to_str(uint8_t* mac);
    static String status_message(uint8_t s);
};
#endif //WIFI101_HELPER_HPP
