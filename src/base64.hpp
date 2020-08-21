/*
 * File: base64.h
 * --------------
 * This file declares a set of functions for encoding and decoding binary data
 * in the base64 format.  See:
 * http://en.wikipedia.org/wiki/Base64
 *
 * @author Marty Stepp, based upon open-source Apache Base64 en/decoder
 * @version 2014/08/03
 * @since 2014/08/03
 */

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

#ifndef BASE64_HPP
#define BASE64_HPP

#include <cstring>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

using namespace std;

class Base64 {
public:

    static int encode_len(int len);

    static int encode(char *coded_dst, const unsigned char *plain_src, int len_plain_src);

    static unsigned int decode_len(const char *buf_coded);

    static unsigned int decode(char *buf_plain, const char *buf_coded);

    static string encode(const std::string &s);

    static string decode(const std::string &s);
};

#endif //BASE64_HPP
