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

#include <USB/USBAPI.h>
#include <stdint.h>

/// Uncomment/comment to turn on/off debug output messages.
// #define SERIAL_DEBUG
/// Uncomment/comment to turn on/off verbose debug output messages.
// #define SERIAL_DEBUG_V
/// Uncomment/comment to turn on/off error output messages.
// #define SERIAL_ERROR

/// Set where debug messages will be printed.
// #define DEBUG_PRINTER Serial
// If using something like Zero or Due, change the above to SerialUSB

/// Define actual debug output functions when necessary.
#ifdef SERIAL_DEBUG
    #define DEBUG_PRINT(...) { DEBUG_PRINTER.print(__VA_ARGS__); }
    #define DEBUG_PRINTLN(...) { DEBUG_PRINTER.println(__VA_ARGS__); }
    #define DEBUG_PRINTBUFFER(buffer, len) { printBuffer(buffer, len); }
#else
    #define DEBUG_PRINT(...) {}
    #define DEBUG_PRINTLN(...) {}
    #define DEBUG_PRINTBUFFER(buffer, len) {}
#endif

/// Define actual debug output functions when necessary.
#ifdef SERIAL_DEBUG_V
#define DEBUG_PRINT_V(...) { DEBUG_PRINTER.print(__VA_ARGS__); }
#define DEBUG_PRINTLN_V(...) { DEBUG_PRINTER.println(__VA_ARGS__); }
#define DEBUG_PRINTBUFFER_V(buffer, len) { printBuffer(buffer, len); }
#else
#define DEBUG_PRINT_V(...) {}
    #define DEBUG_PRINTLN_V(...) {}
    #define DEBUG_PRINTBUFFER_V(buffer, len) {}
#endif

#ifdef SERIAL_ERROR
    #define ERROR_PRINT(...) { DEBUG_PRINTER.print(__VA_ARGS__); }
    #define ERROR_PRINTLN(...) { DEBUG_PRINTER.println(__VA_ARGS__); }
    #define ERROR_PRINTBUFFER(buffer, len) { printBuffer(buffer, len); }
    #define ERROR_PRINTBYTE(buffer) { printByte(buffer); }
#else
    #define ERROR_PRINT(...) {}
    #define ERROR_PRINTLN(...) {}
    #define ERROR_PRINTBUFFER(buffer, len) {}
    #define ERROR_PRINTBYTE(buffer, len) {}
#endif

void printBuffer(const uint8_t *buffer, uint16_t len);
void dumpBuffer(const uint8_t *buffer, uint16_t len);
void printByte(const uint8_t *buffer);