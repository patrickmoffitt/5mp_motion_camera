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

#include "serial_debug_error.hpp"

/**
 * Print "pretty" binary data in hexadecimal format to the Serial device.
 *
 * @param buffer the data to print
 * @param len the length of the data buffer
 */
void printBuffer(const uint8_t *buffer, uint16_t len) {
#ifdef DEBUG_PRINTER
    for (uint16_t i=0; i<len; i++) {
        if (isprint(buffer[i])) {
            DEBUG_PRINTER.write(buffer[i]);
        } else {
            DEBUG_PRINTER.print(" ");
        }
        DEBUG_PRINTER.print(F(" [0x"));
        if (buffer[i] < 0x10) {
            DEBUG_PRINTER.print("0");
        }
        DEBUG_PRINTER.print(buffer[i], HEX);
        DEBUG_PRINTER.print("], ");
        if (i % 8 == 7) {
            DEBUG_PRINTER.print("\n");
        }
    }
    DEBUG_PRINTER.println();
#endif
}

/**
 * Dump binary data in hexadecimal format to the Serial device.
 *
 * This format is handy when you intend to copy and paste the data into
 * a variable definition.
 *
 * @param buffer the data to print
 * @param len the length of the data buffer
 */
void dumpBuffer(const uint8_t *buffer, uint16_t len) {
#ifdef DEBUG_PRINTER
    for (uint16_t i=0; i<len; i++) {
        if (buffer[i] < 0x10) {
            DEBUG_PRINTER.print("0");
        }
        DEBUG_PRINTER.print(buffer[i], HEX);
        DEBUG_PRINTER.print(" ");
    }
    DEBUG_PRINTER.println();
#endif
}

/**
 * Print a a byte of data in hexadecimal format.
 * @param buffer the data to print
 */
void printByte(const uint8_t *buffer) {
#ifdef DEBUG_PRINTER
    DEBUG_PRINTER.print(F(" 0x"));
    if (*buffer < 0x10)
        DEBUG_PRINTER.print("0");
    DEBUG_PRINTER.print(*buffer, HEX);
    DEBUG_PRINTER.print(", ");
#endif
}