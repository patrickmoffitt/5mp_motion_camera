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

#ifndef ARDUCAM_OV5642_HPP
#define ARDUCAM_OV5642_HPP

#undef min  // @ToDo remove when library? bug fixed.
#undef max  // @ToDo remove when library? bug fixed.
#include <map>
#include <Wire.h>
#include <SPI.h>
#include "ArduCAM.h"
#include "serial_debug_error.hpp"
#include "wifi101_helper.hpp"
#include "memorysaver.h"
#include "ov5642_regs.h"
#include "motion_camera_settings_generated.h"
#include <cmath>

#ifndef OV5642_JPEG_SIZE
#define OV5642_JPEG_SIZE 0
#endif

#define SPI_TIMEOUT_MILLIS 250
#define SPI_CS 6 ///< Edit this value for your hardware.
#define SPI_DELAY 25

#define ARDUCHIP_VERSION_YEAR 0x46
#define ARDUCHIP_VERSION_MONTH 0x47
#define ARDUCHIP_VERSION_DAY 0x48

extern SPISettings spi_settings; // Defined in main.cpp
using namespace OV5642_Settings;

/**
 * A class to extend the generic ArduCAM class specifically for the OV5642 model.
 *
 * Reminder: Edit memorysaver.h and adjust the hardware platform.
 */
class Arducam_OV5642 {
protected:

public:
    Arducam_OV5642();
    ArduCAM cmd;

    struct settings_type {
        uint8_t jpeg_size = 0;         // OV5642_320x240
        uint8_t quality = 1;           // default_quality
        uint8_t frames = 0;            // one frame
        uint8_t light_mode = 0;        // Advanced_AWB
        uint8_t color_saturation = 4;  // Saturation0
        uint8_t brightness = 4;        // Brightness0
        uint8_t contrast = 4;          // Contrast0
        uint8_t hue = 6;               // degree_0
        uint8_t effect = 7;            // Normal
        uint8_t exposure = 5;          // Exposure_default
        uint8_t sharpness = 0;         // Auto_Sharpness_default
        uint8_t mirror = 7;            // Normal
        uint8_t test_pattern = 7;      // Normal
    };
    /**
     * A  struct expressing the OV5642's default camera settings. This is the very thing
     * serialized and deserialized by the Google FlatBuffer.
     *
     * @see https://google.github.io/flatbuffers/index.html
     * @ref motion_camera_settings_generated.h
     *
     * @param  jpeg_size default is 0; OV5642_320x240
     * @param  quality default is 1; default_quality
     * @param  frames default is 0; one frame
     * @param  light_mode default is 0; Advanced_AWB
     * @param  color_saturation default is 4; Saturation0
     * @param  brightness default is 4; Brightness0
     * @param  contrast default is 4; Contrast0
     * @param  hue default is 6; degree_0
     * @param  effect default is 7; Normal
     * @param  exposure default is 5; Exposure_default
     * @param  sharpness default is 0; Auto_Sharpness_default
     * @param  mirror default is 7; Normal
     * @param  test_pattern default is 7; Normal
     */
    volatile settings_type settings;

    uint32_t capture();
    void cs_disable();
    void cs_enable();
    bool init_jpeg();
    bool is_model_ov5642();
    bool is_spi_ok();
    const char* jpeg_quality(uint8_t key);
    const char* image_size(uint8_t key);
    void load_default_settings();
    void load_settings(const Settings *s);
    void print_settings();
    const char* read_chip_date();
    const char* read_chip_version();
    void rewind_fifo_read_ptr();
    void rewind_fifo_write_ptr();
    void reset_cpld();
    void sensor_pwr_off();
    void sensor_pwr_on();
    void set_camera();
    bool test_jpeg(uint8_t size);
    void update_settings(OV5642_Settings::Settings *settings);
};


#endif //ARDUCAM_OV5642_HPP
