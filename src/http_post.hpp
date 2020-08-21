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


#ifndef INC_5MP_MOTION_CAMERA_HTTP_POST_HPP
#define INC_5MP_MOTION_CAMERA_HTTP_POST_HPP

#include <sstream>
#include "base64.hpp"
#include "read_battery.hpp"
#include <WiFi101.h>
#include <WiFiClient.h>
#include "ArduCAM_OV5642.hpp"
#include "hardware_control.hpp"
#include "wifi101_helper.hpp"

#define ENDL "\r\n"
#define HTTP_RESPONSE_TIMEOUT_MS 8000

/**
 * The WIN1500 has an MTU of 1400. Using Base64 encoding expands data to 1 1/3 the original size.
 * Therefor when sending Base64 encoded data the maximum amount of data that can be read from SPI,
 * encoded in Base64, and passed to the WINC1500 is 1050.
 */
constexpr uint16_t HTTP_MAX_BUFFER = (SOCKET_BUFFER_MAX_LENGTH * 0.75);

using namespace std;
extern SPISettings spi_settings; // Defined in main.cpp

class Http_post {
private:
    stringstream buf;

    void get_header(const char* host, uint16_t port, const string& url);
    void get_header(const IPAddress& ip, uint16_t port, const string& url);
    void get_http_response();
    void get_http_response_code(const char *response_body, long *code);

public:
    Http_post(const char* host, uint16_t port, const string& url);
    Http_post(const IPAddress& ip, uint16_t port, const string& url);
    ~Http_post();

    bool connected{false};
    int http_status{0};
    void json_bat_v();
    void json_jpeg(uint32_t fifo_len, uint8_t frames);
    void stop();

    WiFiClient http;
};


#endif //INC_5MP_MOTION_CAMERA_HTTP_POST_HPP
