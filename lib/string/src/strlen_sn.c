/*** LICENSE: LGPLV2. *************************************
 *                                                        *
 *                                                        *
 * This file is a part of the j2 project.                 *
 * Copyright (C) 2022 Joško Križanović                    *
 * <jkriza02@fesb.com>                                    *
 *                                                        *
 * This library is free software; you can redistribute    *
 * it and/or modify it under the terms of the GNU Lesser  *
 * General Public License as published by the Free        *
 * Software Foundation; either version 2.1 of the         *
 * License, or (at your option) any later version.        *
 *                                                        *
 * This library is distributed in the hope that it will   *
 * be useful,                                             *
 * but WITHOUT ANY WARRANTY; without even the implied     *
 * warranty of MERCHANTABILITY or FITNESS FOR A           *
 * PARTICULAR PURPOSE. See the GNU Lesser General Public  *
 * License for more details.                              *
 *                                                        *
 * You should have received a copy of the GNU Lesser      *
 * General Public License along with this library; if     *
 * not, write to the Free Software Foundation, Inc.,      *
 * 51 Franklin Street, Fifth Floor, Boston,               *
 * MA  02110-1301  USA                                    *
 *                                                        *
 *                                                        *
 *********************************************************/

/*** INSCRIBING: FILE, AUTHOR & DATE INFORMATION. *********
 *                                                        *
 *                                                        *
 * FILE:                                                  *
 * Somewhat more safer strlen for j2string.               *
 *                                                        *
 *                                                        *
 * AUTHOR:                                                *
 *      NAME:  Joško Križanović                           *
 *      EMAIL: jkriza02@fesb.com                          *
 *                                                        *
 *                                                        *
 * DATE:                                                  *
 * 6.12.2022.                                             *
 *                                                        *
 *                                                        *
 *********************************************************/

#include "../string.h"
#include "../utf8.h"
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <errno.h>
#ifdef __J2_DEV__
#include "../../../../viwerr/viwerr.h"
#endif

// #define zero_in_64(v)
//         (((v) - 0x0101010101010101ULL) & ~(v) & 0x8080808080808080ULL)

// #define utf8_in_64(v)
//         (v & 0x8080808080808080ULL)

// #define is_utf8_payload(v)
//         ((v & 0xC0) != 0x80)

#define chunk_t uint_fast64_t

/**
 * @fn @c j2string_strlen_sn(2)
 * 
 *      @brief 
 *      "Safe" strlen implementation for this library.
 * 
 * 
 *      @param @b str 
 *                String from which we will count the number of characters.     
 * 
 *      @param @b max 
 *                Maximum amount of characters we are allowed to count to.
 * 
 *      
 *      @throw @a EINVAL - If @c "str" is a NULL pointer and/or @c "max" is a 
 *                         value smaller than or equal to 0.
 *      
 * 
 *      @return The amount of characters  @c "str" holds not including the null
 *      terminator. If the amount of characters read reaches @c "max" and no
 *      null terminator was encountered, we return @c "max".
 * 
 */
inline
size_t
j2string_strlen_sn(
        const char * str,
        size_t max )
{

#ifdef __J2_DEV__
        if(!str) {
                errno = EINVAL;
                viwerr(VIWERR_PUSH, &(viwerr_package){
                        .code = EINVAL,
                        .name = (char*)"j2err: Invalid parameter...",
                        .message = (char*)
                                   "j2string_strlen_sn() / j2.string.clen()"
                                   " parameter \"str\" is a NULL ptr.",
                        .group = (char*)"j2"
                });
                return 0;
        }
        if(max < 1) {
                errno = EINVAL;
                viwerr(VIWERR_PUSH, &(viwerr_package){
                        .code = EINVAL,
                        .name = (char*)"j2err: Invalid parameter...",
                        .message = (char*)
                                   "j2string_strlen_sn() / j2.string.clen()"
                                   " parameter \"max\" is less than 1.",
                        .group = (char*)"j2"
                });
                return 0;
        }
#else
        if(max < 1) return 0;
#endif

        chunk_t chunk = 0;
        chunk_t * r = (chunk_t *)str;
        

        register size_t i = 0, j = 0, chunksize = sizeof(chunk_t);
        while(1) {
                
                chunk = r[i];
                j+=1;

                if(j*chunksize > max-1) {

                        unsigned char * rs = (unsigned char *)(&r[i]);
                        
                        if(rs[0] == 0) return i*chunksize;
                        if(i*chunksize > max-1) return max;

                        if(rs[1] == 0) return i*chunksize+1;
                        if(i*chunksize+1 > max-1) return max;

                        if(rs[2] == 0) return i*chunksize+2;
                        if(i*chunksize+2 > max-1) return max;
                
                        if(rs[3] == 0) return i*chunksize+3;
                        if(i*chunksize+3 > max-1) return max;

                        if(rs[4] == 0) return i*chunksize+4;
                        if(i*chunksize+4 > max-1) return max;

                        if(rs[5] == 0) return i*chunksize+5;
                        if(i*chunksize+5 > max-1) return max;

                        if(rs[6] == 0) return i*chunksize+6;
                        if(i*chunksize+6 > max-1) return max;

                        if(rs[7] == 0) return i*chunksize+7;
                        return max;


                }
/**
 * https://graphics.stanford.edu/~seander/bithacks.html#ZeroInWord
 * Black magic woooo.
 * Also checks up if we are reaching the maximum available size
 **/
                if(zero_in_64(chunk)) {


                        unsigned char * rs = (unsigned char *)(&r[i]);

                        if(rs[0] == 0) return i*chunksize;
                        if(rs[1] == 0) return i*chunksize+1;
                        if(rs[2] == 0) return i*chunksize+2;
                        if(rs[3] == 0) return i*chunksize+3;
                        if(rs[4] == 0) return i*chunksize+4;
                        if(rs[5] == 0) return i*chunksize+5;
                        if(rs[6] == 0) return i*chunksize+6;

                        return i*chunksize+7;


                }
                i=j;

        }
        
        return 0;

}


inline
size_t
j2string_utf8_strlen_sn(
        const char * str,
        size_t       max,
        size_t * bytelen )
{

#ifdef __J2_DEV__
        if(!str) {
                errno = EINVAL;
                viwerr(VIWERR_PUSH, &(viwerr_package){
                        .code = EINVAL,
                        .name = (char*)"j2err: Invalid parameter...",
                        .message = (char*)
                                   "j2string_strlen_sn() / j2.string.clen()"
                                   " parameter \"str\" is a NULL ptr.",
                        .group = (char*)"j2"
                });
                return 0;
        }
        if(max < 1) {
                errno = EINVAL;
                viwerr(VIWERR_PUSH, &(viwerr_package){
                        .code = EINVAL,
                        .name = (char*)"j2err: Invalid parameter...",
                        .message = (char*)
                                   "j2string_strlen_sn() / j2.string.clen()"
                                   " parameter \"max\" is less than 1.",
                        .group = (char*)"j2"
                });
                return 0;
        }
#else
        if(max < 1) return 0;
#endif

        chunk_t chunk = 0;
        chunk_t * r = (chunk_t *)str;
        

        register size_t i = 0, j = 0, chunksize = sizeof(chunk_t);
        register size_t ch = 0; 
        while(1) {
                
                chunk = r[i];
                j+=1;

                if(j*chunksize > max-1) {


                        unsigned char * rs = (unsigned char *)(&r[i]);

                        
                        if(rs[0] == 0) {
                                if(bytelen) (*bytelen) = i*chunksize;
                                return ch;
                        }
                        if(i*chunksize > max-1) {
                                if(bytelen) (*bytelen) = max;
                                return ch;
                        }
                        if(is_utf8_payload(rs[0])) ch++;


                        if(rs[1] == 0) {
                                if(bytelen) (*bytelen) = i*chunksize+1;
                                return ch;
                        }
                        if(i*chunksize+1 > max-1) {
                                if(bytelen) (*bytelen) = max;
                                return ch;
                        }
                        if(is_utf8_payload(rs[1])) ch++;


                        if(rs[2] == 0) {
                                if(bytelen) (*bytelen) = i*chunksize+2;
                                return ch;
                        }
                        if(i*chunksize+2 > max-1) {
                                if(bytelen) (*bytelen) = max;
                                return ch;
                        }
                        if(is_utf8_payload(rs[2])) ch++;

                
                        if(rs[3] == 0) {
                                if(bytelen) (*bytelen) = i*chunksize+3;
                                return ch;
                        }
                        if(i*chunksize+3 > max-1) {
                                if(bytelen) (*bytelen) = max;
                                return ch;
                        }
                        if(is_utf8_payload(rs[3])) ch++;


                        if(rs[4] == 0) {
                                if(bytelen) (*bytelen) = i*chunksize+4;
                                return ch;
                        }
                        if(i*chunksize+4 > max-1) {
                                if(bytelen) (*bytelen) = max;
                                return ch;
                        }
                        if(is_utf8_payload(rs[4])) ch++;


                        if(rs[5] == 0) {
                                if(bytelen) (*bytelen) = i*chunksize+5;
                                return ch;
                        }
                        if(i*chunksize+5 > max-1) {
                                if(bytelen) (*bytelen) = max;
                                return ch;
                        }
                        if(is_utf8_payload(rs[5])) ch++;


                        if(rs[6] == 0) {
                                if(bytelen) (*bytelen) = i*chunksize+6;
                                return ch;
                        }
                        if(i*chunksize+6 > max-1) {
                                if(bytelen) (*bytelen) = max;
                                return ch;
                        }
                        if(is_utf8_payload(rs[6])) ch++;


                        if(rs[7] == 0) {
                                if(bytelen) (*bytelen) = i*chunksize+7;
                                return ch;
                        }
                        if(i*chunksize+7 > max-1) {
                                if(bytelen) (*bytelen) = max;
                                return ch;
                        }
                        if(is_utf8_payload(rs[7])) ch++;
                        
                        if(bytelen) (*bytelen) = max;
                        return ch;
                        

                } else if(zero_in_64(chunk)) {

/**
 * https://graphics.stanford.edu/~seander/bithacks.html#ZeroInWord
 * Black magic woooo.
 * Also checks up if we are reaching the maximum available size
 **/

                        unsigned char * rs = (unsigned char *)(&r[i]);

                        if(rs[0] == 0) {
                                if(bytelen) (*bytelen) = i*chunksize;
                                return ch;

                        }
                        if(is_utf8_payload(rs[0])) ch++;


                        if(rs[1] == 0) {
                                if(bytelen) (*bytelen) = i*chunksize+1;
                                return ch;
                        }
                        if(is_utf8_payload(rs[1])) ch++;


                        if(rs[2] == 0) {
                                if(bytelen) (*bytelen) = i*chunksize+2;
                                return ch;
                        }
                        if(is_utf8_payload(rs[2])) ch++;

                
                        if(rs[3] == 0) {
                                if(bytelen) (*bytelen) = i*chunksize+3;
                                return ch;
                        }
                        if(is_utf8_payload(rs[3])) ch++;


                        if(rs[4] == 0) {
                                if(bytelen) (*bytelen) = i*chunksize+4;
                                return ch;
                        }
                        if(is_utf8_payload(rs[4])) ch++;


                        if(rs[5] == 0) {
                                if(bytelen) (*bytelen) = i*chunksize+4;
                                return ch;
                        }
                        if(is_utf8_payload(rs[5])) ch++;


                        if(rs[6] == 0) {
                                if(bytelen) (*bytelen) = i*chunksize+6;
                                return ch;
                        }
                        if(is_utf8_payload(rs[6])) ch++;

                        
                        if(bytelen) (*bytelen) = i*chunksize+7;
                        return ch;
                        

                } else if(utf8_in_64(chunk)) {
                        
                        unsigned char * rs = (unsigned char *)(&r[i]);
                        if(is_utf8_payload(rs[0])) ch++;
                        if(is_utf8_payload(rs[1])) ch++;
                        if(is_utf8_payload(rs[2])) ch++;
                        if(is_utf8_payload(rs[3])) ch++;

                        if(is_utf8_payload(rs[4])) ch++;
                        if(is_utf8_payload(rs[5])) ch++;
                        if(is_utf8_payload(rs[6])) ch++;
                        if(is_utf8_payload(rs[7])) ch++;

                } else {

                        ch+=8;

                }

                i=j;

        }

        return 0;

}