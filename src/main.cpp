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

#include <Arduino.h>
#include "hardware_control.hpp"
#include "feather_m0_sleep_wake_RCWL_0516.hpp"
#include "ArduCAM_OV5642.hpp"
#include <WiFi101.h>
#include <WebSocketClient.h>
#include "wifi101_helper.hpp"
#undef swap  // @ToDo remove when library? bug fixed.
#include "http_post.hpp"

/**
 * If time is of the essence, enable this to take pictures before loading settings.
 */
#define SHOOT_FIRST

using namespace std;
using ctrl = hardware_control;

/// Global objects.
feather_m0_sleep_wake_RCWL_0516 feather;
Arducam_OV5642 camera;
extern WiFiClient http;
WebSocketClient ws_c;

/**
 * ArduCAM Raspberry Pi 8MHz bus speed does not work on a Feather M0.
 * The best speed is only 75% of that (6MHz).
 */
SPISettings spi_settings{6000000, MSBFIRST, SPI_MODE0};

/**
 * Advance function declarations. Definitions below the loop().
 */
void shutdown_sleep();

/**
 * An interrupt handler bound to the motion sensor pin.
 *
 * The motion sensor is powered up each time the feather goes to sleep.
 */
void interrupt_service_routine() {
    if (not feather.awake
        and not feather.take_picture) {
        feather.take_picture = true;
        ctrl::motion_sensor_off();
        ctrl::camera_on();
    }
}

void setup() {
    ctrl::setup_motion_sensor(interrupt_service_routine);

    /// Set external 32k oscillator to run when idle or sleep mode is chosen
    SYSCTRL->XOSC32K.reg |= SYSCTRL_XOSC32K_RUNSTDBY | SYSCTRL_XOSC32K_ONDEMAND;

    /// Generic clock multiplexer id for the external interrupt controller
    REG_GCLK_CLKCTRL |= GCLK_CLKCTRL_ID(GCM_EIC)
                     | GCLK_CLKCTRL_GEN_GCLK1     // generic clock 1 which is xosc32k
                     | GCLK_CLKCTRL_CLKEN;        // enable it
    while (GCLK->STATUS.bit.SYNCBUSY);            // write protected, wait for sync

    /// Set External Interrupt Controller (EIC) to use channel 4 (pin 6)
    EIC->WAKEUP.reg |= EIC_WAKEUP_WAKEUPEN4;

    //PM->SLEEP.reg |= PM_SLEEP_IDLE_CPU;   // Enable Idle0 mode - sleep CPU clock only
    //PM->SLEEP.reg |= PM_SLEEP_IDLE_AHB;   // Idle1 - sleep CPU and AHB clocks
    PM->SLEEP.reg |= PM_SLEEP_IDLE_APB;     // Idle2 - sleep CPU, AHB, and APB clocks

    /// It is either Idle mode or Standby mode, not both.
    SCB->SCR |= SCB_SCR_SLEEPDEEP_Msk;      // Enable Standby or "deep sleep" mode
    __DSB();                                // Ensure effect of last store takes effect

#ifdef SERIAL_DEBUG  // Defined in serial_debug_error.hpp
    Serial.begin(115200); // @ToDo remove for production
#endif
    ctrl::setup_onboard_led();
    ctrl::setup_battery_v();

    SPI.begin();
    Wire.begin();

    ctrl::setup_camera();
    camera.reset_cpld();
    camera.cs_disable();

    WiFi.setPins(WIFI_CS, 7, 4, 2); // Note that this call does NOT set the WiFi CS pinMode().
    pinMode(WIFI_CS, OUTPUT);
    wifi::cs_disable();
    WiFi.maxLowPowerMode();
#ifdef SERIAL_DEBUG
    delay(4000);
#endif
    DEBUG_PRINTLN(F("Default camera settings:"));
    camera.print_settings(); // Enable SERIAL_DEBUG to see output.

    shutdown_sleep();
}

void loop() {
    DEBUG_PRINTLN(F("Loop running."));
    if (feather.take_picture == true) {
        IPAddress ip;
        feather.awake = true;
        ctrl::onboard_led_on();
        DEBUG_PRINTLN(F("Motion detected."));
        bool camera_init{false};
#ifdef SHOOT_FIRST
        /// Initialize the camera and take picture(s).
        camera_init = camera.init_jpeg();
        auto fifo_len = camera.capture();
#else
        camera.cmd.InitCAM();
#endif
        /// Boot up WiFi.
        wifi::cs_enable();
        if (WiFi.status() != WL_CONNECTED) {
            uint8_t mac_address[6];
            WiFi.macAddress(mac_address);
            DEBUG_PRINT(F("MAC Address: "));
            DEBUG_PRINTLN(wifi::mac_uint8_to_str(mac_address));
            auto status = WiFi.begin(WIFI_SSID, WIFI_PASS);
            DEBUG_PRINT(F("WiFi status: "));
            DEBUG_PRINTLN(wifi::status_message(status));
        }
        if (WiFi.status() == WL_CONNECTED) {
            DEBUG_PRINT(F("IP: "));
            DEBUG_PRINTLN(wifi::ipv4_int_to_str(WiFi.localIP()));
            String new_buffer{};
            DEBUG_PRINTLN(F("Retrieve camera settings."));
            ip = wifi::ipv4_string_to_addr(HTTP_HOST);
            Http_post client(ip, SOCKET_PORT, "/");
            if (not client.connected) {
                client.stop();
                ERROR_PRINTLN(F("WebSocket connect failed."));
            } else {
                ws_c.host = (char *) HTTP_HOST;
                ws_c.path = (char *) "/ws";
                auto handshake = ws_c.handshake(client.http);
                if (handshake) {
                    DEBUG_PRINTLN_V(F("WebSocket handshake okay."));
                    ws_c.sendData("getNew", WS_OPCODE_BINARY);
                    new_buffer = ws_c.getData();
                } else {
                    ERROR_PRINTLN(F("WebSocket handshake failed."));
                }
                client.stop();
                DEBUG_PRINT_V(F("Received buffer length: "));
                DEBUG_PRINTLN_V(new_buffer.length());
                if (new_buffer.length() > 0) {
                    DEBUG_PRINTBUFFER_V(reinterpret_cast<const uint8_t *>(new_buffer.c_str()),
                                        new_buffer.length());
                    auto verify = flatbuffers::Verifier(reinterpret_cast<const uint8_t *>(new_buffer.c_str()),
                                                        new_buffer.length());
                    if (VerifySettingsBuffer(verify)) {
                        const Settings *s = GetSettings(new_buffer.c_str());
                        camera.load_settings(s);
                    }
                    DEBUG_PRINTLN("New camera settings: ");
                } else {
                    DEBUG_PRINTLN("Default camera settings: ");
                    camera.load_default_settings();
                }
                camera.print_settings();
            }

            wifi::cs_disable();
#ifndef SHOOT_FIRST
            /// Initialize the camera.
            camera_init = camera.init_jpeg();
            /// Take picture(s).
            auto fifo_len = camera.capture();
#endif
            ip = wifi::ipv4_string_to_addr(HTTP_HOST);
            Http_post send(ip, HTTP_HOST_PORT, HTTP_HOST_URL);
            if (send.connected
                and camera_init
                and fifo_len > 0) {
                send.json_jpeg(fifo_len, camera.settings.frames);
            } else {
                DEBUG_PRINTLN(F("ERROR Http_post not connected."));
            }
            send.stop();
        }
        wifi::cs_disable();
    }
    shutdown_sleep();
}

void shutdown_sleep() {
    DEBUG_PRINTLN(F("Waiting for motion...."));
    ctrl::onboard_led_off();
    ctrl::camera_off();
    WiFi.disconnect();
    ctrl::motion_sensor_on();
    feather.reset();
    feather.go_to_sleep();
}
