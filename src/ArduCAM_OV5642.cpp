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

#include "ArduCAM_OV5642.hpp"

Arducam_OV5642::Arducam_OV5642() {
    cmd = ArduCAM(OV5642, SPI_CS);
}

/**
 * Capture frame(s) of image(s) at size.
 *
 * @return the FIFO length in bytes.
 *
 * @see ArduCAM::jpeg_size for a list of the sizes. Invalid values default
 * to size OV5642_320x240
 */
uint32_t Arducam_OV5642::capture() {
    uint32_t fifo_len{0};

#ifdef SERIAL_DEBUG
    if (settings.frames != 0xFF) {
        DEBUG_PRINT(F("Capturing "));
        DEBUG_PRINT(settings.frames);
        DEBUG_PRINT(F(" frame(s) of "));
        DEBUG_PRINTLN(image_size(settings.jpeg_size));
    } else {
        DEBUG_PRINT(F("Capturing a full-buffer movie of "));
        DEBUG_PRINTLN(image_size(settings.jpeg_size));
    }
#endif

    SPI.beginTransaction(spi_settings);
        cs_enable();
        cmd.clear_fifo_flag();
        cmd.start_capture();
        while (!(cmd.read_reg(ARDUCHIP_TRIG) & CAP_DONE_MASK)) { delayMicroseconds(SPI_DELAY); }
        cs_disable();
        if (cmd.get_bit(ARDUCHIP_TRIG, CAP_DONE_MASK)) {
            fifo_len = cmd.read_fifo_length();
        }
    SPI.endTransaction();
#ifdef SERIAL_DEBUG
    if (settings.frames > 0) {
        DEBUG_PRINT(F("Movie frames size: "));
        if (settings.frames == 255) {
            DEBUG_PRINT(F("unknown."));
        } else {
            DEBUG_PRINT(lround(fifo_len / settings.frames));
        }
        DEBUG_PRINT(F(" (")); DEBUG_PRINT(settings.frames); DEBUG_PRINTLN(F(") frames."));
    }
#endif
    return fifo_len;
}

/**
 * Set the camera's SPI CS to HIGH.
 */
void Arducam_OV5642::cs_disable() {
    DEBUG_PRINTLN_V(F("Camera SPI CS disabled."))
    digitalWrite(SPI_CS, HIGH);
    cmd.CS_HIGH();
}

/**
 * Set the camera's SPI CS to LOW.
 */
void Arducam_OV5642::cs_enable() {
    DEBUG_PRINTLN_V(F("Camera SPI CS enabled."));
    digitalWrite(SPI_CS, LOW);
    cmd.CS_LOW();
}

/**
 * Initialize the JPEG features of the camera.
 *
 * @param size the size of image.
 * @param frames the number of frames to capture numbered from zero.
 *
 * @see ArduCAM::image_size for a list of the sizes. Invalid values default
 * to size OV5642_320x240
 *
 * @ref If you want to manually set the settings, call one of the mutate_[setting]()
 * functions, for example mutate_jpeg_size(uint8_t _jpeg_size) found in
 * motion_camera_settings_generated.h
 */
bool Arducam_OV5642::init_jpeg() {
    // Change to JPEG capture mode and initialize the OV5642 module.
    DEBUG_PRINTLN(F("+ + + + + + + + + + + + + + +"));
    DEBUG_PRINTLN(F("Initializing ArduCAM OV5642."));
    auto spi = is_spi_ok();
    auto model = is_model_ov5642();
    cmd.set_format(JPEG);
    cmd.InitCAM();
    SPI.beginTransaction(spi_settings);
        cs_enable();
        cmd.write_reg(ARDUCHIP_TIM, VSYNC_LEVEL_MASK);   // VSYNC is active HIGH
        cs_disable();
    SPI.endTransaction();
    set_camera();
    DEBUG_PRINTLN(F("Saved camera settings:"));
    print_settings();
    if (spi and model) {
        DEBUG_PRINTLN(F(". . . . . INIT JPEG PASSED . . . . "));
    } else {
        DEBUG_PRINTLN(F(". . . . . INIT JPEG FAILED . . . . . "));
    }
    DEBUG_PRINTLN(F("+ + + + + + + + + + + + + + +"));
    return (spi and model);
}

/**
 * Reads the relevant camera control registers to determine the chip's id.
 * This is useful as a proof-of-life test.
 */
bool Arducam_OV5642::is_model_ov5642() {
    bool okay{false};
    uint8_t vid{0};
    uint8_t pid{0};
    uint16_t timer{0};
    while(timer <= SPI_TIMEOUT_MILLIS and (vid != 0x56 or pid != 0x42)) {
        // Check if the camera module type is OV5642
        cmd.wrSensorReg16_8(0xff, 0x01);
        cmd.rdSensorReg16_8(OV5642_CHIPID_HIGH, &vid);
        cmd.rdSensorReg16_8(OV5642_CHIPID_LOW, &pid);
        DEBUG_PRINT_V(F("vid: "));DEBUG_PRINTBUFFER_V(&vid, sizeof(vid));
        DEBUG_PRINT_V(F("pid: "));DEBUG_PRINTBUFFER_V(&pid, sizeof(pid));
        delay(SPI_DELAY);
        timer += SPI_DELAY;
    }
    if (vid == 0x56 and pid == 0x42) {
        DEBUG_PRINTLN(F("ArduCAM model OV5642 detected."));
        okay = true;
    }
    if (timer == SPI_TIMEOUT_MILLIS and not okay) {
        DEBUG_PRINTLN(F("Can't find OV5642 module!"));
        DEBUG_PRINTLN(F("OV5642 detection timeout."));
    }
    return okay;
}

/**
 * Simple proof-of-life test that writes a value to the Test Register and reads it back.
 * If we get back the same value we wrote that's a good sign. If it fails it may mean
 * the SPI stack isn't initialized or the camera is off.
 *
 * @see The ArduChip Register Table for more details
 * https://www.arducam.com/docs/spi-cameras-for-arduino/hardware/arducam-chip/
 */
bool Arducam_OV5642::is_spi_ok() {
    bool okay{false};
    uint8_t temp{0};
    uint16_t timer{0};
    const uint8_t test_data{0x55};
    while(timer <= SPI_TIMEOUT_MILLIS and temp != test_data) {
        // Check if the SPI bus connection to the ArduCAM is OK.
        SPI.beginTransaction(spi_settings);
            cs_enable();
            cmd.write_reg(ARDUCHIP_TEST1, test_data);
            temp = cmd.read_reg(ARDUCHIP_TEST1);
            cs_disable();
        SPI.endTransaction();
        if (temp != test_data) {
            DEBUG_PRINTLN(F("SPI interface Error!"));
            delay(SPI_DELAY);
            timer += SPI_DELAY;
        }
    }
    if (temp == test_data){
        DEBUG_PRINTLN(F("SPI interface okay."));
        okay = true;
    }
    if ((timer == SPI_TIMEOUT_MILLIS) and not okay) {
        DEBUG_PRINTLN(F("SPI timeout connecting to ArduCAM."));
    }
    return okay;
}

/**
 * A convenience for converting from integer JPEG compression value back
 * to text strings. Used in debug output.
 *
 * @param key the JPEG compression value to decode
 * @return the text label corresponding to the JPEG compression value.
 */
const char* Arducam_OV5642::jpeg_quality(const uint8_t key) {
    std::map<uint8_t, const char*> jpeg_quality_map = {
            {low_quality, "low_quality"},
            {default_quality, "default_quality"},
            {high_quality, "high_quality"}
    };
    return jpeg_quality_map[key];
}

/**
 * A convenience for converting from integer image sizes back to text strings.
 * Used in debug output.
 *
 * @param key the image size to decode
 * @return the text label corresponding to the size.
 */
const char* Arducam_OV5642::image_size(uint8_t key) {
    std::map<uint8_t, const char*> jpeg_size_map = {
            {OV5642_320x240,   "OV5642_320x240"},
            {OV5642_640x480,   "OV5642_640x480"},
            {OV5642_1024x768,  "OV5642_1024x768"},
            {OV5642_1280x960,  "OV5642_1280x960"},
            {OV5642_1600x1200, "OV5642_1600x1200"},
            {OV5642_2048x1536, "OV5642_2048x1536"},
            {OV5642_2592x1944, "OV5642_2592x1944"}
    };
    return jpeg_size_map[key];
}

/**
 * Load the default values for Arducam_OV5642::settings into the member struct Arducam_OV5642::settings.
 */
void Arducam_OV5642::load_default_settings() {
    settings_type s{};
    settings.jpeg_size = s.jpeg_size;
    settings.quality = s.quality;
    settings.frames = s.frames;
    settings.light_mode = s.light_mode;
    settings.color_saturation = s.color_saturation;
    settings.brightness = s.brightness;
    settings.contrast = s.contrast;
    settings.hue = s.hue;
    settings.effect = s.effect;
    settings.exposure = s.exposure;
    settings.sharpness = s.sharpness;
    settings.mirror = s.mirror;
    settings.test_pattern = s.test_pattern;
}

/**
 * Load an OV5642_Settings::Settings object into the member struct Arducam_OV5642::settings.
 * 
 * @param s Arducam_OV5642::settings
 */
void Arducam_OV5642::load_settings(const Settings *s) {
    settings.jpeg_size = s->jpeg_size();
    settings.quality = s->quality();
    settings.frames = s->frames();
    settings.light_mode = s->light_mode();
    settings.color_saturation = s->color_saturation();
    settings.brightness = s->brightness();
    settings.contrast = s->contrast();
    settings.hue = s->hue();
    settings.effect = s->effect();
    settings.exposure = s->exposure();
    settings.sharpness = s->sharpness();
    settings.mirror = s->mirror();
    settings.test_pattern = s->test_pattern();
}
/**
 * Prints the current camera settings when SERIAL_DEBUG is defined.
 */
void Arducam_OV5642::print_settings() {
    DEBUG_PRINT(F("jpeg_size: "));        DEBUG_PRINTLN(settings.jpeg_size);
    DEBUG_PRINT(F("quality: "));          DEBUG_PRINTLN(settings.quality);
    DEBUG_PRINT(F("frames: "));           DEBUG_PRINTLN(settings.frames);
    DEBUG_PRINT(F("light_mode: "));       DEBUG_PRINTLN(settings.light_mode);
    DEBUG_PRINT(F("color_saturation: ")); DEBUG_PRINTLN(settings.color_saturation);
    DEBUG_PRINT(F("brightness: "));       DEBUG_PRINTLN(settings.brightness);
    DEBUG_PRINT(F("contrast: "));         DEBUG_PRINTLN(settings.contrast);
    DEBUG_PRINT(F("hue: "));              DEBUG_PRINTLN(settings.hue);
    DEBUG_PRINT(F("effect: "));           DEBUG_PRINTLN(settings.effect);
    DEBUG_PRINT(F("exposure: "));         DEBUG_PRINTLN(settings.exposure);
    DEBUG_PRINT(F("sharpness: "));        DEBUG_PRINTLN(settings.sharpness);
    DEBUG_PRINT(F("mirror: "));           DEBUG_PRINTLN(settings.mirror);
    DEBUG_PRINT(F("test_pattern: "));     DEBUG_PRINTLN(settings.test_pattern);
    DEBUG_PRINTLN();
}
/**
 * Return the ArduChip version year, month, and day.
 *
 * @return the version in YYYY-MM-DD format.
 */
const char *Arducam_OV5642::read_chip_date() {
    uint32_t year, month, day;
    static char buf[13]{"0000-00-00"};
    SPI.beginTransaction(spi_settings);
        cs_enable();
        year = cmd.read_reg(ARDUCHIP_VERSION_YEAR) + 2000;
        month = cmd.read_reg(ARDUCHIP_VERSION_MONTH);
        day = cmd.read_reg(ARDUCHIP_VERSION_DAY);
        cs_disable();
    SPI.endTransaction();
    DEBUG_PRINT(F("Year: ")); DEBUG_PRINTLN(year);
    DEBUG_PRINT(F("Month: ")); DEBUG_PRINTLN(month);
    DEBUG_PRINT(F("Day: ")); DEBUG_PRINTLN(day);
    sprintf(reinterpret_cast<char *>(buf), "%lu-%02lu-%02lu", year, month, day);
    return reinterpret_cast<const char *>(buf);
}

/**
 * Report the ArduChip version number.
 *
 * @return The version number in major.minor format.
 */
const char *Arducam_OV5642::read_chip_version() {
    SPI.beginTransaction(spi_settings);
        cs_enable();
        uint8_t version = cmd.read_reg(0x40);
        cs_disable();
    SPI.endTransaction();
    auto major = version >> 4U;
    auto minor = version ^ (major << 4U);
    static char buf[6]{"0.0"};
    sprintf(reinterpret_cast<char *>(buf), "%i.%i", major, minor);
    DEBUG_PRINT(F("Camera chip version: ")); DEBUG_PRINTLN(buf);
    return reinterpret_cast<const char *>(buf);
}

/**
 * Reset the camera's Complex Programmable Logic Device (CPLD).
 * If the camera has a Power On Reset (POR) capability it isn't reliable.
 * Resetting the CPLD seems to reset the control registers.
 *
 * @see https://www.arducam.com/docs/spi-cameras-for-arduino/hardware/arducam-chip/
 */
void Arducam_OV5642::reset_cpld() {
    DEBUG_PRINTLN(F("Resetting camera's Complex Programmable Logic Device (CPLD)."));
    SPI.beginTransaction(spi_settings);
        cs_enable();
        cmd.write_reg(0x07, 0x80); // 0x01 << 7
        cs_disable();
    SPI.endTransaction();
    delay(SPI_DELAY);
    SPI.beginTransaction(spi_settings);
        cs_enable();
        cmd.write_reg(0x07, 0x00);
        cs_disable();
    SPI.endTransaction();
    delay(300); // Reboot delay.
};

/**
 * Resets the camera's FIFO read-pointer to it's beginning so the FIFO
 * can be reread. Useful when you want to peak at the header to see if it
 * contains valid data.
 */
void Arducam_OV5642::rewind_fifo_read_ptr() {
    SPI.beginTransaction(spi_settings);
        cs_enable();
        cmd.write_reg(ARDUCHIP_FIFO, FIFO_RDPTR_RST_MASK);
        cs_disable();
    SPI.endTransaction();
}

/**
 * Resets the camera's FIFO write-pointer to it's beginning so the FIFO
 * can be overwritten. Useful when you want to create a multiple exposure.
 * This technique can sometimes capture fast moving subjects better than a
 * series of frames.
 */
void Arducam_OV5642::rewind_fifo_write_ptr() {
    SPI.beginTransaction(spi_settings);
        cs_enable();
        cmd.write_reg(ARDUCHIP_FIFO, FIFO_WRPTR_RST_MASK);
        cs_disable();
    SPI.endTransaction();
}

/**
 * Turn the camera's image sensor off.
 *
 * Provides a power management option to reduce power consumption.
 */
void Arducam_OV5642::sensor_pwr_off() {
    DEBUG_PRINTLN(F("Camera sensor power off."));
    SPI.beginTransaction(spi_settings);
        cs_enable();
        cmd.write_reg(ARDUCHIP_GPIO, GPIO_PWDN_MASK);
        cs_disable();
    SPI.endTransaction();
    delay(SPI_DELAY);
}

/**
 * Turn the camera's image sensor on.
 *
 */
void Arducam_OV5642::sensor_pwr_on() {
    DEBUG_PRINTLN(F("Camera sensor reset."));
    SPI.beginTransaction(spi_settings);
        cs_enable();
        cmd.write_reg(ARDUCHIP_GPIO, 0x00);
        cs_disable();
    SPI.endTransaction();
    delay(SPI_DELAY);
    DEBUG_PRINTLN(F("Camera sensor power on."));
    SPI.beginTransaction(spi_settings);
        cs_enable();
        cmd.write_reg(ARDUCHIP_GPIO, GPIO_PWREN_MASK);
        cs_disable();
    SPI.endTransaction();
    delay(SPI_DELAY);
}

/**
 * Apply the Settings structure setting to the OV5642.
 */
void Arducam_OV5642::set_camera() {
    SPI.beginTransaction(spi_settings);
    cs_enable();
    cmd.clear_fifo_flag();
    cmd.bus_write(ARDUCHIP_FRAMES | 0x80, settings.frames);
    cs_disable();
    SPI.endTransaction();

    cmd.OV5642_set_JPEG_size(settings.jpeg_size);
    cmd.OV5642_set_Compress_quality(settings.quality);
    /// It appears, through testing, that the sensor needs extra time to initialize larger sizes.
    /// Since the sensor has no sensor-ready event we can't poll to determine when the wait is over.
    /// In lieu of an event, we wait for the sensor to catch up.
    if (settings.jpeg_size <= OV5642_1600x1200) delay (100);
    if (settings.jpeg_size > OV5642_1600x1200
        and settings.quality == low_quality) delay(125);
    if (settings.jpeg_size > OV5642_1600x1200
        and settings.quality == default_quality) delay(175);
    if (settings.jpeg_size > OV5642_1600x1200
        and settings.quality == high_quality) delay(225);

    cmd.OV5642_set_Light_Mode(settings.light_mode);
    cmd.OV5642_set_Color_Saturation(settings.color_saturation);
    cmd.OV5642_set_Brightness(settings.brightness);
    cmd.OV5642_set_Contrast(settings.contrast);
    cmd.OV5642_set_hue(settings.hue);
    cmd.OV5642_set_Special_effects(settings.effect);
    cmd.OV5642_set_Exposure_level(settings.exposure);
    cmd.OV5642_set_Sharpness(settings.sharpness);
    cmd.OV5642_set_Mirror_Flip(settings.mirror); delay(100);
    cmd.OV5642_Test_Pattern(settings.test_pattern);
}

/**
 * Perform an operational test of the JPEG function by taking a picture and
 * reading the head of the image buffer to see if it contains valid JPEG metadata.
 *
 * @see https://dev.exiv2.org/projects/exiv2/wiki/The_Metadata_in_JPEG_files
 *
 */
bool Arducam_OV5642::test_jpeg(uint8_t size) {
    bool ok{false};
    uint32_t length{640};
    auto capture_length = capture();
    uint8_t buffer[length];
    memset(&buffer, 0x00, length);
    DEBUG_PRINT(F("FiFo buffer length: [ "));
    DEBUG_PRINT(capture_length); DEBUG_PRINTLN(F(" ]"));
    if (capture_length >= MAX_FIFO_SIZE) {
        DEBUG_PRINTLN(F("FiFo buffer length is over size. Exit."));
        return ok;
    }
    if (capture_length == 0 ) {
        DEBUG_PRINTLN(F("FiFo buffer length is 0. Exit."));
        return ok;
    }

    SPI.beginTransaction(spi_settings);
        cs_enable();
        cmd.set_fifo_burst();
        SPI.transfer(&buffer, length);
        cs_disable();
    SPI.endTransaction();

    bool head_idx_beg{false};
    bool head_idx_end{false};
    int dqt{0};       // Define Quantization Table(s). [0xFF, 0xDB]
    int dht{0};       // Define Huffman Table(s) [0xFF, 0xC4]
    bool sof0{false}; // Start Of Frame SOF0 (Baseline DCT) [0xFF, 0xC0]
    for (uint32_t i=0; i<length; i++) {
        if (buffer[i] == 0xFF && buffer[i+1] == 0xD8) { ///< JPEG Start of Image (SOI)
            DEBUG_PRINT(F("JPEG header begins at offset DEC: ")); DEBUG_PRINTLN(i);
            head_idx_beg = true;
        }
        if (buffer[i] == 0xFF && buffer[i+1] == 0xDA) { ///< JPEG Start of Scan (SOS)
            DEBUG_PRINT(F("JPEG header ends at offset DEC: ")); DEBUG_PRINTLN(i);
            head_idx_end = true;
        }
        if (buffer[i] == 0xFF && buffer[i+1] == 0xDB) {
            dqt++;
            DEBUG_PRINT(F("Define Quantization Table at offset DEC: ")); DEBUG_PRINTLN(i);
        }
        if (buffer[i] == 0xFF && buffer[i+1] == 0xC4) {
            dht++;
            DEBUG_PRINT(F("Define Huffman Table at offset DEC: ")); DEBUG_PRINTLN(i);
        }
        if (buffer[i] == 0xFF && buffer[i+1] == 0xC0) {
            sof0 = true;
            DEBUG_PRINT(F("Start Of Frame SOF0 (Baseline DCT) at offset DEC: ")); DEBUG_PRINTLN(i);
        }
    }
    if (not head_idx_beg) {
        DEBUG_PRINTLN(F("JPEG header error: begin [0xFF, 0xD8] not found."));
    }
    if (not head_idx_end) {
        DEBUG_PRINTLN(F("JPEG header error: end [0xFF, 0xDA] not found."));
    }
    if (dqt == 0) {
        DEBUG_PRINTLN(F("JPEG header error: no Define Quantization Table(s) (DQT) found."))
    } else {
        DEBUG_PRINT(F("JPEG header Define Quantization Table(s) counted: ")); DEBUG_PRINTLN(dqt);
    }
    if (dht == 0) {
        DEBUG_PRINTLN(F("JPEG header error: no Define Huffman Table(s) (DHT) found."))
    } else {
        DEBUG_PRINT(F("JPEG header Define Huffman Table(s) counted: ")); DEBUG_PRINTLN(dht);
    }
    if (not sof0) {
        DEBUG_PRINTLN(F("JPEG header error: no SOF0 Start Of Frame (Baseline DCT) found."))
    } else {
        DEBUG_PRINTLN(F("JPEG header found SOF0 Start Of Frame (Baseline DCT)."));
    }
    bool jfif{false};
    for (auto i=0; i<32; i++) {
        if (buffer[i] == 0x4A && buffer[i+1] == 0x46 && buffer[i+2] == 0x49 && buffer[i+3] == 0x46 ) {
            jfif = true;
            DEBUG_PRINTLN(F("Camera JPEG JFIF mark found."));
        }
    }
    if (not jfif) {
        DEBUG_PRINTLN(F("JPEG header error: missing JFIF signature."));
    }

    if (head_idx_beg
        and head_idx_end
        and jfif
        and dqt > 0
        and dht > 0
        and sof0) ok = true;

    if (not ok) {
        ERROR_PRINTBUFFER((uint8_t *) buffer, length);
        DEBUG_PRINTLN(F("Camera JPEG function [ FAILED ]."));
    } else {
        DEBUG_PRINTLN(F("Camera JPEG function [ PASSED ]."));
    }

    DEBUG_PRINTLN(F("END: Test Camera JPEG Function."));
    DEBUG_PRINTLN(F("* * * * * * * * * * * * * * * * *"));
    DEBUG_PRINTLN(F(""));
    return ok;
}

void Arducam_OV5642::update_settings(OV5642_Settings::Settings *settings) {

}


