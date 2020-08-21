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

#include "wifi101_helper.hpp"

/**
 * Chip Select Enable function for the WIN1500. Sets the WIFI_CS pin LOW.
 *
 * @ref wifi::WIFI_CS
 */
void wifi::cs_enable() {
    digitalWrite(WIFI_CS, LOW);
    DEBUG_PRINTLN_V(F("WiFi CS Enabled."));
}

/**
 * Chip Select Disable function for the WIN1500. Sets the WIFI_CS pin HIGH.
 *
 * @ref wifi::WIFI_CS
 */
void wifi::cs_disable() {
    digitalWrite(WIFI_CS, HIGH);
    DEBUG_PRINTLN_V(F("WiFi CS Disabled."));
}

/**
 * Converts an IP v4 address in integer representation to std::string dotted-quad notation.
 *
 * @param ip an IP v4 address in integer representation
 * @return an IP v4 address in dotted-quad notation.
 *
 * @ref WiFi101::localIP()
 */
String wifi::ipv4_int_to_str(const uint32_t ip) {
    std::stringstream str_buf;
    unsigned char bytes[4];
    bytes[0] = ip & 0xFF;
    str_buf << to_string((int) bytes[0]) << ".";
    bytes[1] = (ip >> 8) & 0xFF;
    str_buf << to_string((int) bytes[1]) << ".";
    bytes[2] = (ip >> 16) & 0xFF;
    str_buf << to_string((int) bytes[2]) << ".";
    bytes[3] = (ip >> 24) & 0xFF;
    str_buf << to_string((int) bytes[3]) << std::ends;
    return String(str_buf.str().c_str());
}

/**
 * Converts an IP v4 address in std::string dotted-quad notation to integer representation.
 *
 * @param address an IP v4 address in dotted-quad notation.
 * @return an IP v4 address in integer representation.
 *
 * @ref WiFiClient::connect()
 */
IPAddress wifi::ipv4_string_to_addr(const string& address) {
    const char ipv4[] = R"END(([0-9]{1,3})\.([0-9]{1,3})\.([0-9]{1,3})\.([0-9]{1,3}))END";
    regex ipv4_regex{ipv4, regex_constants::optimize};
    smatch match;
    regex_search(address, match, ipv4_regex);
    if (not match.empty()) {
        short byte1 = atoi( match[1].str().data());
        short byte2 = atoi( match[2].str().data());
        short byte3 = atoi( match[3].str().data());
        short byte4 = atoi( match[4].str().data());
        IPAddress ip(byte1, byte2, byte3, byte4);
        return ip;
    } else {
        IPAddress ip(0, 0, 0, 0);
        return ip;
    }
}

/**
 * Converts an IEEE 802.x Medium Access Controller (MAC) address from unsigned char*
 * to canonical two-digit hexadecimal, colon separated, String format.
 *
 * @param mac MAC address
 * @return the same address formatted: 00:00:00:00:00:00
 *
 * @ref WiFi101::macAddress()
 */
String wifi::mac_uint8_to_str(uint8_t* mac) {
    String mac_str;
    char byte[3] = {0, 0, 0};
    for (int i=5; i>=0; i--) {
        sprintf(byte, "%02x", mac[i]);
        mac_str += byte;
        if (i > 0) mac_str += ":";
    }
    return mac_str;
}

/**
 * Convert from WiFi101::wl_status_t integers to String representation of
 * the corresponding named constant in the enum. Useful for debugging status codes.
 *
 * @param s WiFi101::wl_status_t integer
 * @return corresponding named constant
 */
String wifi::status_message(uint8_t s) {
    std::map<uint8_t, String>message{
        {WL_NO_SHIELD,           "WL_NO_SHIELD"},
        {WL_IDLE_STATUS,         "WL_IDLE_STATUS"},
        {WL_NO_SSID_AVAIL,       "WL_NO_SSID_AVAIL"},
        {WL_SCAN_COMPLETED,      "WL_SCAN_COMPLETED"},
        {WL_CONNECTED,           "WL_CONNECTED"},
        {WL_CONNECT_FAILED,      "WL_CONNECT_FAILED"},
        {WL_CONNECTION_LOST,     "WL_CONNECTION_LOST"},
        {WL_DISCONNECTED,        "WL_DISCONNECTED"},
        {WL_AP_LISTENING,        "WL_AP_LISTENING"},
        {WL_AP_CONNECTED,        "WL_AP_CONNECTED"},
        {WL_AP_FAILED,           "WL_AP_FAILED"},
        {WL_PROVISIONING,        "WL_PROVISIONING"},
        {WL_PROVISIONING_FAILED, "WL_PROVISIONING_FAILED"}
    };
    return message.at(s);
}

