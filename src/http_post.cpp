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


#include "http_post.hpp"

using ctrl = hardware_control;
extern WiFiClass WiFi;
extern Arducam_OV5642 camera;
using b64 = Base64;
using bat = battery;

/**
 * Begin an HTTP POST message.
 *
 * @param host the fully qualified hostname of the receiving server.
 * @param port the decimal port number for the connection.
 * @param url the url of the server's POST processing routine.
 */
Http_post::Http_post(const char* host, uint16_t port, const string& url) {
    if (WiFi.status() != WL_CONNECTED) DEBUG_PRINTLN(F("ERROR HTTP has no WiFi connection."));
    connected = http.connect(host, port);
    if (connected) {
        DEBUG_PRINT(F("HTTP connected to: ")); DEBUG_PRINTLN(host);
    } else {
        DEBUG_PRINTLN(F("No HTTP Connection."));
    }
    get_header(host, port, url);
}

/**
 * Begin an HTTP POST message.
 *
 * @param IPAddress the IP version 4 integer format address of the server.
 * @param port the decimal port number for the connection.
 * @param url the url of the server's POST processing routine.
 *
 * @see ./framework-arduino-samd-adafruit/cores/arduino/IPAddress.h
 */
Http_post::Http_post(const IPAddress& ip, uint16_t port, const string& url) {
    if (WiFi.status() != WL_CONNECTED) DEBUG_PRINTLN(F("ERROR HTTP has no WiFi connection."));
    connected = http.connect(ip, port);
    if (connected) {
        DEBUG_PRINT(F("HTTP connected to: ")); DEBUG_PRINTLN(ip);
    } else {
        DEBUG_PRINTLN(F("No HTTP Connection."));
    }
    get_header(ip, port, url);
}

Http_post::~Http_post() {
    stop();
}

/**
 * Format the raw HTTP headers for the POST.
 *
 * @param host the fully qualified hostname of the receiving server.
 * @param port the decimal port number for the connection.
 * @param url the url of the server's POST processing routine.
 */
void Http_post::get_header(const char* host, uint16_t port, const string& url) {
    buf.str(""); buf.clear();
    buf << "POST " << url << " HTTP/1.1" << ENDL
        << "Host: " << host << ":" << port <<ENDL
        << "User-Agent: 5MP_Motion_Camera/0.0.1" << ENDL
        << "Accept: text/plain" << ENDL;
}

/**
 * Format the raw HTTP headers for the POST.
 *
 * @param IPAddress the IP version 4 integer format address of the server.
 * @param port the decimal port number for the connection.
 * @param url the url of the server's POST processing routine.
 */
void Http_post::get_header(const IPAddress& ip, uint16_t port, const string& url) {
    buf.str(""); buf.clear();
    auto host = wifi::ipv4_int_to_str(uint32_t(ip));
    buf << "POST " << url << " HTTP/1.1" << ENDL
        << "Host: " << host.c_str() << ":" << port << ENDL
        << "User-Agent: 5MP_Motion_Camera/0.0.1" << ENDL
        << "Accept: text/plain" << ENDL;
}

/**
 * Poll for and process the HTTP response from the server.
 */
void Http_post::get_http_response() {
    int delay_count{0};
    size_t response_size{0};
    // Since we lack callbacks or async io, poll for the HTTP response while yielding.
    while (delay_count < HTTP_RESPONSE_TIMEOUT_MS and response_size == 0) {
        response_size = static_cast<size_t>(http.available());
        delay_count += 100;
        delay(100);
    }
    if (delay_count >= HTTP_RESPONSE_TIMEOUT_MS or response_size == 0) {
        ERROR_PRINTLN(F("No response from HTTP server."));
        http.stop();
        http_status = 0;
        return;
    }

    long http_response_code{0};
    DEBUG_PRINT(F("Response size: "));
    DEBUG_PRINT(response_size);
    DEBUG_PRINTLN(F(" bytes."));
    uint8_t response[response_size];
    if (http.read(response, response_size) != -1 and strlen((char *)response) > 0) {
        get_http_response_code((char *) response, &http_response_code);
        http_status = (int) http_response_code;
        DEBUG_PRINT(F("Response http_code: "));
        DEBUG_PRINTLN(http_status);
        if (http_status != 200) {
            ERROR_PRINT((char *) response);
            int bytes_read{0};
            if (http.available()) do {
                response_size = static_cast<size_t>(http.available());
                bytes_read = http.read(response, response_size);
                ERROR_PRINT((char *) response);
                delay(500);
            } while (bytes_read != -1);
        }
    } else {
        ERROR_PRINTLN(F("ERROR: Unable to read HTTP server response."));
    }
    http.stop();
}

/**
 * Parse the HTTP response body and return the HTTP response code.
 *
 * @param response_body the raw HTTP response body.
 * @param code the HTTP response code
 */
void Http_post::get_http_response_code(const char *response_body, long *code) {
    char response[15];
    memcpy(response, response_body, 15);
    string needle{"HTTP/1.1 "};
    string response_str{string(response)};
    size_t found = response_str.find(needle);
    if (found == string::npos) {
        *code = -1;
    } else {
        size_t code_begin = found + needle.length();
        char *end;
        *code = strtol(response_str.substr(code_begin, 3).c_str(), &end, 10);
    }
}

/**
 * Format a JSON string containing the battery voltage data.
 */
void Http_post::json_bat_v() {
    if (not connected) return;
    buf << "Content-Type: application/json" << ENDL;
    string json = R"END([{"name": "battery_percent", "value": ")END";
    json += to_string(bat::percent());
    json += R"END("}])END";
    size_t content_len = strlen(json.c_str());
    buf << "Content-Length: " << content_len << ENDL << ENDL << json.c_str() << ENDL;
    DEBUG_PRINTLN(F("RAW HTTP: ")); DEBUG_PRINTLN(buf.str().c_str());
    http.write(buf.str().c_str(), buf.str().length());
    http.flush();
    get_http_response();
}

/**
 * Format a JSON string to lead into the JPEG data.
 *
 * @param fifo_len the length of the camera's buffer.
 * @param frames the number of frames
 *
 * @see Arducam_OV5642::settings.frames
 */
void Http_post::json_jpeg(uint32_t fifo_len, uint8_t frames) {
    if (not connected) return;
    if (fifo_len == 0) return;
    http_status = 0;
    buf << "Content-Type: application/json" << ENDL;
    string json = R"END({ "body": [{"name": "battery_percent", "value": ")END";
    json += to_string(bat::percent());
    json += R"END("},{"name": "5MP_MC", "frames": )END";
    json += to_string(int(frames));
    json += R"END(, "content_type": "image/jpeg", "value": ")END";
    DEBUG_PRINT(F("FIFO length: ")); DEBUG_PRINTLN(fifo_len);
    DEBUG_PRINT(F("B64 length: ")); DEBUG_PRINTLN(b64::encode_len(fifo_len));

    uint32_t buffer_size = HTTP_MAX_BUFFER;
    size_t content_len = json.length() + b64::encode_len(fifo_len);
    content_len += 4; // for the "}]} at the end

    buf << "Content-Length: " << content_len << ENDL << ENDL;
    buf << json.c_str();
    DEBUG_PRINTLN(F("HTTP: ")); DEBUG_PRINTLN(buf.str().c_str());

    uint32_t bytes_http = json.length() + 4;

    uint32_t bytes_sent{0};
    uint32_t chunk_size{0};
    uint32_t b64_enc_size{0};

    auto b64_buffer_size = b64::encode_len(buffer_size);
    uint8_t buffer[buffer_size];
    char b64_buf[b64_buffer_size];

    memset(&buffer, 0x00, buffer_size);
    memset(&b64_buf, 0x00, b64_buffer_size);
    DEBUG_PRINT_V(F("Buffer size: ")); DEBUG_PRINTLN_V(buffer_size);
    DEBUG_PRINT_V(F("B64 buffer size: ")); DEBUG_PRINTLN_V(b64_buffer_size);
    DEBUG_PRINT_V(F("Free memory: ")); DEBUG_PRINTLN_V(ctrl::free_memory());
    while (bytes_sent < fifo_len) {
        chunk_size = std::min((fifo_len - bytes_sent), buffer_size);

        SPI.beginTransaction(spi_settings);
            camera.cs_enable();
            camera.cmd.set_fifo_burst();
            SPI.transfer(&buffer, chunk_size);
            camera.cs_disable();
        SPI.endTransaction();

        DEBUG_PRINTBUFFER_V(buffer, chunk_size);
        bytes_sent += chunk_size;
        b64_enc_size = b64::encode(b64_buf, buffer, chunk_size);

        DEBUG_PRINT_V(F("Base 64: ")); DEBUG_PRINT_V(chunk_size);
        DEBUG_PRINTLN_V(F(" bytes in."));
        DEBUG_PRINT_V(F("Expected ")); DEBUG_PRINT_V(b64::encode_len(chunk_size));
        DEBUG_PRINTLN_V(F(" bytes out."));
        DEBUG_PRINT_V(F("Encoded ")); DEBUG_PRINT_V(b64_enc_size); DEBUG_PRINTLN_V(F(" bytes."));

        bytes_http += b64_enc_size;
        if (buf.str().length() > 0) {
            wifi::cs_enable();
            http.write(buf.str().c_str(), buf.str().length());
            wifi::cs_disable();
            buf.str(""); buf.clear();
        }
        wifi::cs_enable();
        http.write(b64_buf, b64_enc_size);
        wifi::cs_disable();
        DEBUG_PRINTLN_V(b64_buf);
        memset(&buffer, 0x00, buffer_size);
        memset(&b64_buf, 0x00, b64_buffer_size);
    }
    buf << R"END("}]})END";

    wifi::cs_enable();
    http.write(buf.str().c_str());
    wifi::cs_disable();
    DEBUG_PRINTLN(buf.str().c_str());
    DEBUG_PRINT(F("Total bytes written to HTTP: ")); DEBUG_PRINTLN(bytes_http);
    http.flush();
    get_http_response();
}

/**
 * Close the HTTP connection and destruct the associated connection class.
 */
void Http_post::stop() {
    http.stop();
}