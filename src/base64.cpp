/*
 * File: base64.cpp
 * ----------------
 * This file defines a set of functions for encoding and decoding binary data
 * in the base64 format, as declared in base64.h.  See:
 * http://en.wikipedia.org/wiki/Base64
 *
 * @author Marty Stepp, based upon open-source Apache Base64 en/decoder
 * @version 2017/10/18
 * - fixed compiler warnings
 * @version 2014/10/08
 * - removed 'using namespace' statement
 * 2014/08/14
 * - Fixed bug with variables declared with deprecated 'register' keyword.
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
#include "base64.hpp"

/* ASCII Table */
static const unsigned char ptr2six[256]{
        64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
        64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
        64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 62, 64, 64, 64, 63,
        52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 64, 64, 64, 64, 64, 64,
        64, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14,
        15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 64, 64, 64, 64, 64,
        64, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40,
        41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 64, 64, 64, 64, 64,
        64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
        64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
        64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
        64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
        64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
        64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
        64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
        64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64
};

/**
 * Calculates and returns the decoded (plain) length for a given
 * base64 encoded C string.
 *
 * @param buf_coded base64 encoded C string
 * @return the decoded (plain) length in bytes.
 */
unsigned int Base64::decode_len(const char *buf_coded) {
    unsigned int n_bytes_decoded;
    const unsigned char *buf_in;
    unsigned int n_ptr_bytes;

    buf_in = (const unsigned char *) buf_coded;
    while (ptr2six[*(buf_in++)] <= 63);

    n_ptr_bytes = buf_in - (const unsigned char *) buf_coded - 1;
    n_bytes_decoded = ((n_ptr_bytes + 3) / 4) * 3;

    return n_bytes_decoded;
}

/**
 * Decodes the base64 encoded C string buf_coded and places it in buf_plain.
 *
 * @param buf_plain base64 decoded (plain) C string (output).
 * @param buf_coded base64 encoded C string (input).
 * @return the number of bytes decoded.
 */
unsigned int Base64::decode(char *buf_plain, const char *buf_coded) {
    unsigned int n_bytes_decoded;
    const unsigned char *buf_in;
    unsigned char *buf_out;
    unsigned int n_ptr_bytes;

    buf_in = (const unsigned char *) buf_coded;
    while (ptr2six[*(buf_in++)] <= 63);
    n_ptr_bytes = (buf_in - (const unsigned char *) buf_coded) - 1;
    n_bytes_decoded = ((n_ptr_bytes + 3) / 4) * 3;

    buf_out = (unsigned char *) buf_plain;
    buf_in = (const unsigned char *) buf_coded;

    while (n_ptr_bytes > 4) {
        *(buf_out++) =
                (unsigned char) (ptr2six[*buf_in] << 2U | ptr2six[buf_in[1]] >> 4U);
        *(buf_out++) =
                (unsigned char) (ptr2six[buf_in[1]] << 4U | ptr2six[buf_in[2]] >> 2U);
        *(buf_out++) =
                (unsigned char) (ptr2six[buf_in[2]] << 6U | ptr2six[buf_in[3]]);
        buf_in += 4;
        n_ptr_bytes -= 4;
    }

    /* Note: (n_ptr_bytes == 1) would be an error, so just ignore that case. */
    if (n_ptr_bytes > 1) {
        *(buf_out++) =
                (unsigned char) (ptr2six[*buf_in] << 2U | ptr2six[buf_in[1]] >> 4U);
    }
    if (n_ptr_bytes > 2) {
        *(buf_out++) =
                (unsigned char) (ptr2six[buf_in[1]] << 4U | ptr2six[buf_in[2]] >> 2U);
    }
    if (n_ptr_bytes > 3) {
        *(buf_out++) =
                (unsigned char) (ptr2six[buf_in[2]] << 6U | ptr2six[buf_in[3]]);
    }

    n_bytes_decoded -= (4U - n_ptr_bytes) & 3U;
    return n_bytes_decoded;
}

static const unsigned char basis_64[] =
        "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

/**
 * Calculates and returns the encoded (base64) length for a given
 * plain C string.
 *
 * @param len the length of the plain string in bytes.
 * @return the encoded length for the given input.
 */
int Base64::encode_len(int len) {
    return ((len + 2) / 3 * 4);
}

/**
 * Base64 encodes the C string plain_src and places it in coded_dst.
 *
 * @param coded_dst base64 encoded C string (input).
 * @param plain_src base64 decoded (plain) C string (output).
 * @param len_plain_src the length of plain_src in bytes.
 * @return the number of bytes encoded.
 */
int Base64::encode(char *coded_dst, const unsigned char *plain_src, int len_plain_src) {
    int i;
    char *p;

    p = coded_dst;
    for (i = 0; i < len_plain_src - 2; i += 3) {
        *p++ = basis_64[(plain_src[i] >> 2U) & 0x3F];
        *p++ = basis_64[((plain_src[i] & 0x3) << 4U) | ((plain_src[i + 1] & 0xF0) >> 4U)];
        *p++ = basis_64[((plain_src[i + 1] & 0xF) << 2U) | ((plain_src[i + 2] & 0xC0) >> 6U)];
        *p++ = basis_64[plain_src[i + 2] & 0x3F];
    }
    if (i < len_plain_src) {
        *p++ = basis_64[(plain_src[i] >> 2) & 0x3F];
        if (i == (len_plain_src - 1)) {
            *p++ = basis_64[((plain_src[i] & 0x3) << 4U)];
            *p++ = '=';
        } else {
            *p++ = basis_64[((plain_src[i] & 0x3) << 4U) | ((plain_src[i + 1] & 0xF0) >> 4U)];
            *p++ = basis_64[((plain_src[i + 1] & 0xF) << 2U)];
        }
        *p++ = '=';
    }

    return p - coded_dst;
}

/**
 * Returns a Base64-encoded equivalent of the given string.
 *
 * @param string the plain string to encode.
 * @return the base64 encoded version of the input string.
 */
string Base64::encode(const string &s) {
    // make C char* buffer to store the encoded output (for compatibility)
    int len = encode_len(s.length());
    char *buf = (char *) malloc(len);
    memset(buf, 0, len);
    encode(buf, reinterpret_cast<const uint8_t *>(s.data()), s.length());

    // convert back into a C++ string, and return it
    // (unlike below in decode(), I can just directly construct the C++
    // string from the C one, because the Base64-encoded C string will
    // not contain any intermediate null bytes by definition)
    string result(buf);
    free(buf);
    return result;
}

/**
 * Decodes the given Base64-encoded string and returns the decoded
 * original contents.
 *
 * @param string the base64 encoded string to decode.
 * @return the plain version of the input string.
 */
string Base64::decode(const string &s) {
    // convert into C string and decode into that char* buffer
    const char *cstr = s.c_str();
    auto len = decode_len(cstr);
    char *buf = (char *) malloc(len);
    memset(buf, 0, len);
    decode(buf, cstr);

    // read bytes from that buffer into a C++ string
    // (cannot just construct/assign C++ string from C char* buffer,
    // because that will terminate the string at the first null \0 byte)
    ostringstream out;
    for (unsigned int i = 0; i < len; i++) {
        out << buf[i];
    }
    string result = out.str();

    free(buf);
    return result;
}

