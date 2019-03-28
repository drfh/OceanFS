/* Copyright (c) 2015 Francisco Cifuentes <franchoco@gmail.com>
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE. 
 */

#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <stdlib.h>
#include "base64.h"

static const char lookup_table[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
static const char pad = '=';

/* Out must have len / 3 + (len % 3 > 0? 1 : 0)*4 + 1*/
char * b64_encode (const uint8_t * buffer, size_t len )
{
    /* In Base 64 we represent 6 bits with each character */
    uint64_t temp;
    char * out = malloc(len / 3 + (len % 3 > 0) + 1);
    char * p = out;
    const uint8_t * cur = buffer;
    for(size_t i = 0; i < len/3; i++) {
        temp  = ( *cur++ ) << 16;
        temp += ( *cur++ ) << 8;
        temp += ( *cur++ );

        *p++ = lookup_table[( temp & 0x00FC0000 ) >> 18];
        *p++ = lookup_table[( temp & 0x0003F000 ) >> 12];
        *p++ = lookup_table[( temp & 0x00000FC0 ) >> 6 ];
        *p++ = lookup_table[( temp & 0x0000003F )      ];
    }
    switch(len % 3) {
        case 1:
            temp = ( *cur++ ) << 16;
            *p++ = lookup_table[( temp & 0x00FC0000 ) >> 18];
            *p++ = lookup_table[( temp & 0x0003F000 ) >> 12];
            *p++ = pad;
            *p++ = pad;
            break;
        case 2:
            temp = ( *cur++ ) << 16;
            temp += ( *cur++ ) << 8;
            *p++ = lookup_table[( temp & 0x00FC0000 ) >> 18];
            *p++ = lookup_table[( temp & 0x0003F000 ) >> 12];
            *p++ = lookup_table[( temp & 0x00000FC0 ) >> 6 ];
            *p++ = pad;
            break;
    }
    *p = '\0';

    return out;
}

uint8_t * b64_decode ( const char * input, size_t len )
{
    if ( len % 4 ) { 
        return NULL;
    }

    size_t padding = 0;
    if ( len ) {
        if ( input[len-1] == pad ) {
            padding++;
        }
        if ( input[len-2] == pad ) {
            padding++;
        }
    }
    uint8_t * out = malloc((len/4)*3 - padding);
    uint8_t * p = out;

    uint64_t temp = 0;
    const char * cur = input;
    while ( cur < input + len ) {
        for ( size_t i = 0; i < 4; i++ ) {
            temp <<= 6;
            if ( *cur >= 0x41 && *cur <= 0x5A ) { 
                temp |= *cur - 0x41;
            } else if ( *cur >= 0x61 && *cur <= 0x7A ) {
                temp |= *cur - 0x47;
            } else if ( *cur >= 0x30 && *cur <= 0x39 ) {
                temp |= *cur + 0x04;
            } else if ( *cur == 0x2B ) {
                temp |= 0x3E;
            } else if ( *cur == 0x2F ) {
                temp |= 0x3F;
            } else if ( *cur == pad ) {
                switch ( input + len - cur ) {
                    case 1:
                        *p++ = ( ( temp >> 16 ) & 0x000000FF );
                        *p++ = ( ( temp >> 8 ) & 0x000000FF );
                        return out;
                    case 2:
                        *p++ = ( ( temp >> 10 ) & 0x000000FF );
                        return out;
                    default:
                        goto on_error;
                }
            }  else {
                goto on_error;
            }
            cur++;
        }
        *p++ = ( ( temp >> 16 ) & 0x000000FF );
        *p++ = ( ( temp >> 8 ) & 0x000000FF );
        *p++ = ( ( temp ) & 0x000000FF );
    }
    return out;

on_error:
    free(out);
    return NULL;
}

