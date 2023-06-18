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
 * Every function the string.h library has.               *
 *                                                        *
 *                                                        *
 * AUTHOR:                                                *
 *      NAME:  Joško Križanović                           *
 *      EMAIL: jkriza02@fesb.com                          *
 *                                                        *
 *                                                        *
 * DATE:                                                  *
 * 16.3.2023.                                             *
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
#include "../../../ext/viwerr/viwerr.h"
#endif

// #define chunk_has_zero(v)
//         (((v) - 0x0101010101010101ULL) & ~(v) & 0x8080808080808080ULL)

// #define chunk_has_utf(v)
//         (v & 0x8080808080808080ULL)

// #define utf8_is_char(v)
//         ((v & 0xC0) != 0x80)

// #define utf8_is_first_and_type(v)
//         (v>>6 == 0x3 ? (v>>5 == 0x7 ? (v>>3 == 0x1E ? 3 : 2) : 1) : 0)

#define chunk_t uint_fast64_t

/**
 * @fn @c j2string_strcpy_sn(2)
 * 
 *      @brief 
 *      "Safe" strcpy implementation for this library.
 * 
 * 
 *      @param @b dst 
 *                A string to copy @c "src" into.    
 * 
 *      @param @b src 
 *                A string to copy into @c "dst".
 *
 *      @param @b max 
 *                The maximum amount of characters that can be copied from one 
 *                string to another.
 * 
 * 
 *      @throw @a EINVAL - @c "dst" or/and @c "src" is/are NULL pointers and/or
 *                         @c "max" is a number less than or equal to 0.
 * 
 * 
 *      @return The amount of characters written to @c "dst" not including the 
 *      null terminator.
 * 
 */
inline
size_t
j2string_strcpy_sn(
        char * __restrict dst,
        const char * __restrict src,
        size_t max )
{

#ifdef __J2_DEV__
        if(!dst){
                errno = EINVAL;
                viwerr(VIWERR_PUSH, &(viwerr_package){
                        .code    = EINVAL,
                        .name    = (char*)"j2err: Invalid parameter...",
                        .message = (char*)
                                   "j2string_strlen_sn() / j2.string.clen()"
                                   " parameter \"dst\" is a NULL ptr.",
                        .group   = (char*)"j2"
                });
                return 0;
        }
        if(!src){
                errno = EINVAL;
                viwerr(VIWERR_PUSH, &(viwerr_package){
                        .code    = EINVAL,
                        .name    = (char*)"j2err: Invalid parameter...",
                        .message = (char*)
                                   "j2string_strlen_sn() / j2.string.clen()"
                                   " parameter \"src\" is a NULL ptr.",
                        .group   = (char*)"j2"
                });
                return 0;
        } 
        if(max < 1) {
                errno = EINVAL;
                viwerr(VIWERR_PUSH, &(viwerr_package){
                        .code    = EINVAL,
                        .name    = (char*)"j2err: Invalid parameter...",
                        .message = (char*)
                                   "j2string_strlen_sn() / j2.string.clen()"
                                   " parameter \"max\" is less than 1.",
                        .group   = (char*)"j2"
                });
                return 0;
        }
#else
        if(max < 1) return 0;
#endif
        
        chunk_t 
                chunk = 0,
                *w = (chunk_t *)dst,
                *r = (chunk_t *)src;
        
        register size_t 
                i = 0, 
                j = 0, 
                chunksize = sizeof(chunk_t);

        while(1) {
                
                chunk = r[i];
                j+=1;

                if(j*chunksize > max-1) {

                        unsigned char * rs = (unsigned char *)(&r[i]);
                        unsigned char * ws = (unsigned char *)(&w[i]);

                        ws[0] = rs[0];
                        if(rs[0] == 0) return i*chunksize;
                        if(i*chunksize >= max-1) {
                                ws[0] = '\0';
                                return i*chunksize;
                        }

                        ws[1] = rs[1];
                        if(rs[1] == 0) return i*chunksize+1;
                        if(i*chunksize+1 >= max-1) {
                                ws[1] = '\0';
                                return i*chunksize+1;
                        }

                        ws[2] = rs[2];
                        if(rs[2] == 0) return i*chunksize+2;
                        if(i*chunksize+2 >= max-1) {
                                ws[2] = '\0';
                                return i*chunksize+2;
                        }
                
                        ws[3] = rs[3];
                        if(rs[3] == 0) return i*chunksize+3;
                        if(i*chunksize+3 >= max-1) {
                                ws[3] = '\0';
                                return i*chunksize+3;
                        }

                        ws[4] = rs[4];
                        if(rs[4] == 0) return i*chunksize+4;
                        if(i*chunksize+4 >= max-1) {
                                ws[4] = '\0';
                                return i*chunksize+4;
                        }

                        ws[5] = rs[5];
                        if(rs[5] == 0) return i*chunksize+5;
                        if(i*chunksize+5 >= max-1) {
                                ws[5] = '\0';
                                return i*chunksize+5;
                        }

                        ws[6] = rs[6];
                        if(rs[6] == 0) return i*chunksize+6;
                        if(i*chunksize+6 >= max-1) {
                                ws[6] = '\0';
                                return i*chunksize+6;
                        }

                        ws[7] = rs[7];
                        if(rs[7] == 0) return i*chunksize+7;
                        if(i*chunksize+7 >= max-1) {
                                ws[7] = '\0';
                                return i*chunksize+7;
                        }

                        ws[7] = '\0';
                        return i*chunksize+7;

                } else if(zero_in_64(chunk)) {
/**
 * https://graphics.stanford.edu/~seander/bithacks.html#ZeroInWord
 * Black magic woooo.
 * Also checks up if we are reaching the maximum available size
 **/

                        unsigned char * rs = (unsigned char *)(&r[i]);
                        unsigned char * ws = (unsigned char *)(&w[i]);

                        ws[0] = rs[0];
                        if(rs[0] == 0) return i*chunksize;

                        ws[1] = rs[1];
                        if(rs[1] == 0) return i*chunksize+1;

                        ws[2] = rs[2];
                        if(rs[2] == 0) return i*chunksize+2;

                        ws[3] = rs[3];
                        if(rs[3] == 0) return i*chunksize+3;

                        ws[4] = rs[4];
                        if(rs[4] == 0) return i*chunksize+4;

                        ws[5] = rs[5];
                        if(rs[5] == 0) return i*chunksize+5;

                        ws[6] = rs[6];
                        if(rs[6] == 0) return i*chunksize+6;

                        ws[7] = rs[7];
                        if(rs[7] == 0) return i*chunksize+7;
                        
                        ws[7] = '\0';

                }

                w[i] = chunk;
                i=j;

        }
        
        return 0;

}

/**
 * @fn @c j2string_utf8_strcpy_sn(2)
 * 
 *      @brief 
 *      "Safe" strcpy implementation for UTF-8 strings.
 * 
 * 
 *      @param @b dst 
 *                A string to copy @c "src" into.    
 * 
 *      @param @b src 
 *                A string to copy into @c "dst".
 *
 *      @param @b max 
 *                The maximum amount of bytes that can be copied from one 
 *                string to another.
 * 
 * 
 *      @throw @a EINVAL - @c "dst" or/and @c "src" is/are NULL pointers and/or
 *                         @c "max" is a number less than or equal to 0.
 * 
 * 
 *      @return The amount of UTF-8 characters written to @c "dst" not including 
 *      the null terminator. If "bytelen" is not a NULL pointer, we write the
 *      amount of bytes that were written to the strings.
 * 
 */
inline
size_t
j2string_utf8_strcpy_sn(
        char * __restrict dst,
        const char * __restrict src,
        size_t max,
        size_t * bytelen )
{

#ifdef __J2_DEV__
        if(!dst){
                errno = EINVAL;
                viwerr(VIWERR_PUSH, &(viwerr_package){
                        .code    = EINVAL,
                        .name    = (char*)"j2err: Invalid parameter...",
                        .message = (char*)
                                   "j2string_strlen_sn() / j2.string.clen()"
                                   " parameter \"dst\" is a NULL ptr.",
                        .group   = (char*)"j2"
                });
                return 0;
        }
        if(!src){
                errno = EINVAL;
                viwerr(VIWERR_PUSH, &(viwerr_package){
                        .code    = EINVAL,
                        .name    = (char*)"j2err: Invalid parameter...",
                        .message = (char*)
                                   "j2string_strlen_sn() / j2.string.clen()"
                                   " parameter \"src\" is a NULL ptr.",
                        .group   = (char*)"j2"
                });
                return 0;
        } 
        if(max < 1) {
                errno = EINVAL;
                viwerr(VIWERR_PUSH, &(viwerr_package){
                        .code    = EINVAL,
                        .name    = (char*)"j2err: Invalid parameter...",
                        .message = (char*)
                                   "j2string_strlen_sn() / j2.string.clen()"
                                   " parameter \"max\" is less than 1.",
                        .group   = (char*)"j2"
                });
                return 0;
        }
#else
        if(max < 1) return 0;
#endif

        chunk_t 
                chunk = 0,
                *w = (chunk_t *)dst,
                *r = (chunk_t *)src;
        
        register size_t 
                i = 0, 
                j = 0, 
                chunksize = sizeof(chunk_t),
                bytes = 0,
                ch = 0;

        while(1) {
                
                chunk = r[i];
                j+=1;

                if(j*chunksize > max-1) {

                        unsigned char * rs = (unsigned char *)(&r[i]);
                        unsigned char * ws = (unsigned char *)(&w[i]);

                        ws[0] = rs[0];
                        if(rs[0] == 0) {
                                bytes = i*chunksize;
                                break;
                        }
                        if(i*chunksize >= max-1) {
                                ws[0] = '\0';
                                bytes = i*chunksize;
                                break;
                        }
                        if(is_utf8_payload(rs[0])) ch++;


                        ws[1] = rs[1];
                        if(rs[1] == 0) {
                                bytes = i*chunksize+1;
                                break;
                        }
                        if(i*chunksize+1 >= max-1) {
                                ws[1] = '\0';
                                bytes = i*chunksize+1;
                                break;
                        }
                        if(is_utf8_payload(rs[1])) ch++;


                        ws[2] = rs[2];
                        if(rs[2] == 0) {
                                bytes = i*chunksize+2;
                                break;
                        }
                        if(i*chunksize+2 >= max-1) {
                                ws[2] = '\0';
                                bytes = i*chunksize+2;
                                break;
                        }
                        if(is_utf8_payload(rs[2])) ch++;
       

                        ws[3] = rs[3];
                        if(rs[3] == 0) {
                                bytes = i*chunksize+3;
                                break;
                        }
                        if(i*chunksize+3 >= max-1) {
                                ws[3] = '\0';
                                bytes = i*chunksize+3;
                                break;
                        }
                        if(is_utf8_payload(rs[3])) ch++;


                        ws[4] = rs[4];
                        if(rs[4] == 0) {
                                bytes = i*chunksize+4;
                                break;
                        }
                        if(i*chunksize+4 >= max-1) {
                                ws[4] = '\0';
                                bytes = i*chunksize+4;
                                break;
                        }
                        if(is_utf8_payload(rs[4])) ch++;


                        ws[5] = rs[5];
                        if(rs[5] == 0) {
                                bytes = i*chunksize+5;
                                break;
                        }
                        if(i*chunksize+5 >= max-1) {
                                ws[5] = '\0';
                                bytes = i*chunksize+5;
                                break;
                        }
                        if(is_utf8_payload(rs[5])) ch++;


                        ws[6] = rs[6];
                        if(rs[6] == 0) {
                                bytes = i*chunksize+6;
                                break;
                        }
                        if(i*chunksize+6 >= max-1) {
                                ws[6] = '\0';
                                bytes = i*chunksize+6;
                                break;
                        }
                        if(is_utf8_payload(rs[6])) ch++;


                        ws[7] = rs[7];
                        if(rs[7] == 0) {
                                bytes = i*chunksize+7;
                                break;
                        }
                        if(i*chunksize+7 >= max-1) {
                                ws[7] = '\0';
                                bytes = i*chunksize+7;
                                break;
                        }
                        ws[7] = '\0';
                        bytes = i*chunksize+7;
                        break;


                } else if(zero_in_64(chunk)) {

/**
 * https://graphics.stanford.edu/~seander/bithacks.html#ZeroInWord
 * Black magic woooo.
 * Also checks up if we are reaching the maximum available size
 **/

                        unsigned char * rs = (unsigned char *)(&r[i]);
                        unsigned char * ws = (unsigned char *)(&w[i]);


                        ws[0] = rs[0];
                        if(rs[0] == 0) {
                                bytes = i*chunksize;
                                goto ret_success;
                        }
                        if(is_utf8_payload(rs[0])) ch++;


                        ws[1] = rs[1];
                        if(rs[1] == 0)  {
                                bytes = i*chunksize+1;
                                goto ret_success;
                        }
                        if(is_utf8_payload(rs[1])) ch++;


                        ws[2] = rs[2];
                        if(rs[2] == 0)  {
                                bytes = i*chunksize+2;
                                goto ret_success;
                        }
                        if(is_utf8_payload(rs[2])) ch++;

                
                        ws[3] = rs[3];
                        if(rs[3] == 0)  {
                               bytes = i*chunksize+3;
                               goto ret_success;
                        }
                        if(is_utf8_payload(rs[3])) ch++;


                        ws[4] = rs[4];
                        if(rs[4] == 0)  {
                               bytes = i*chunksize+4;
                               goto ret_success;
                        }
                        if(is_utf8_payload(rs[4])) ch++;


                        ws[5] = rs[5];
                        if(rs[5] == 0)  {
                                bytes = i*chunksize+5;
                                goto ret_success;
                        }
                        if(is_utf8_payload(rs[5])) ch++;


                        ws[6] = rs[6];
                        if(rs[6] == 0)  {
                                bytes = i*chunksize+6;
                                goto ret_success;
                        }
                        if(is_utf8_payload(rs[6])) ch++;


                        ws[7] = '\0';
                        bytes = i*chunksize+7;
                        goto ret_success;


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

                w[i] = chunk;
                i=j;

        }
        
        int cnt = 0;
        unsigned char * rs = (unsigned char *)dst;
        
        /// Remove half baked UTF-8 characters from the end of the string.
        for(int64_t k = (int64_t)bytes; k >= 0; k--) {

                /// First '\0'.
                if(rs[k] == 0) continue;

                /// Detect if we start of in a UTF-8 byte.
                /// Counts the number of UTF-8 bytes and afterwards
                /// when we end up on a starting byte we will
                /// see if the byte is fully written or not.
                if(!is_utf8_payload(rs[k])) {
                        cnt++;
                        continue;
                }

                /// If the amount of bytes the UTF-8 uses isn't the
                /// the amount is_utf8_head_w_type returns,
                /// we remove those bytes and update the values
                /// acordingly.
                /// This also catches lone UTF-8 starting bytes.
                if(is_utf8_head_w_type(rs[k]) != cnt) {

                        rs[k] = '\0';
                        bytes -= cnt+1;
                        ch--; 

                }

                break;

        }

ret_success:
        if(bytelen) (*bytelen) = bytes;
        return ch;

}

/**
 * @fn @c j2string_utf8_nhbc_strcpy_sn(2)
 * 
 *      @brief 
 *      "Safe" strcpy implementation for UTF-8 strings. Does not check for
 *      half baked UTF-8 characters at the end.
 * 
 * 
 *      @param @b dst 
 *                A string to copy @c "src" into.    
 * 
 *      @param @b src 
 *                A string to copy into @c "dst".
 *
 *      @param @b max 
 *                The maximum amount of bytes that can be copied from one 
 *                string to another.
 * 
 * 
 *      @throw @a EINVAL - @c "dst" or/and @c "src" is/are NULL pointers and/or
 *                         @c "max" is a number less than or equal to 0.
 * 
 * 
 *      @return The amount of UTF-8 characters written to @c "dst" not including 
 *      the null terminator. If "bytelen" is not a NULL pointer, we write the
 *      amount of bytes that were written to the strings.
 * 
 */
inline
size_t
j2string_utf8_nhbc_strcpy_sn(
        char * __restrict dst,
        const char * __restrict src,
        size_t max,
        size_t * bytelen )
{

#ifdef __J2_DEV__
        if(!dst){
                errno = EINVAL;
                viwerr(VIWERR_PUSH, &(viwerr_package){
                        .code    = EINVAL,
                        .name    = (char*)"j2err: Invalid parameter...",
                        .message = (char*)
                                   "j2string_strlen_sn() / j2.string.clen()"
                                   " parameter \"dst\" is a NULL ptr.",
                        .group   = (char*)"j2"
                });
                return 0;
        }
        if(!src){
                errno = EINVAL;
                viwerr(VIWERR_PUSH, &(viwerr_package){
                        .code    = EINVAL,
                        .name    = (char*)"j2err: Invalid parameter...",
                        .message = (char*)
                                   "j2string_strlen_sn() / j2.string.clen()"
                                   " parameter \"src\" is a NULL ptr.",
                        .group   = (char*)"j2"
                });
                return 0;
        } 
        if(max < 1) {
                errno = EINVAL;
                viwerr(VIWERR_PUSH, &(viwerr_package){
                        .code    = EINVAL,
                        .name    = (char*)"j2err: Invalid parameter...",
                        .message = (char*)
                                   "j2string_strlen_sn() / j2.string.clen()"
                                   " parameter \"max\" is less than 1.",
                        .group   = (char*)"j2"
                });
                return 0;
        }
#else
        if(max < 1) return 0;
#endif

        chunk_t 
                chunk = 0,
                *w = (chunk_t *)dst,
                *r = (chunk_t *)src;
        
        register size_t 
                i = 0, 
                j = 0, 
                chunksize = sizeof(chunk_t),
                bytes = 0,
                ch = 0;

        while(1) {
                
                chunk = r[i];
                j+=1;

                if(j*chunksize > max-1) {

                        unsigned char * rs = (unsigned char *)(&r[i]);
                        unsigned char * ws = (unsigned char *)(&w[i]);

                        ws[0] = rs[0];
                        if(rs[0] == 0) {
                                bytes = i*chunksize;
                                break;
                        }
                        if(i*chunksize >= max-1) {
                                ws[0] = '\0';
                                bytes = i*chunksize;
                                break;
                        }
                        if(is_utf8_payload(rs[0])) ch++;


                        ws[1] = rs[1];
                        if(rs[1] == 0) {
                                bytes = i*chunksize+1;
                                break;
                        }
                        if(i*chunksize+1 >= max-1) {
                                ws[1] = '\0';
                                bytes = i*chunksize+1;
                                break;
                        }
                        if(is_utf8_payload(rs[1])) ch++;


                        ws[2] = rs[2];
                        if(rs[2] == 0) {
                                bytes = i*chunksize+2;
                                break;
                        }
                        if(i*chunksize+2 >= max-1) {
                                ws[2] = '\0';
                                bytes = i*chunksize+2;
                                break;
                        }
                        if(is_utf8_payload(rs[2])) ch++;
       

                        ws[3] = rs[3];
                        if(rs[3] == 0) {
                                bytes = i*chunksize+3;
                                break;
                        }
                        if(i*chunksize+3 >= max-1) {
                                ws[3] = '\0';
                                bytes = i*chunksize+3;
                                break;
                        }
                        if(is_utf8_payload(rs[3])) ch++;


                        ws[4] = rs[4];
                        if(rs[4] == 0) {
                                bytes = i*chunksize+4;
                                break;
                        }
                        if(i*chunksize+4 >= max-1) {
                                ws[4] = '\0';
                                bytes = i*chunksize+4;
                                break;
                        }
                        if(is_utf8_payload(rs[4])) ch++;


                        ws[5] = rs[5];
                        if(rs[5] == 0) {
                                bytes = i*chunksize+5;
                                break;
                        }
                        if(i*chunksize+5 >= max-1) {
                                ws[5] = '\0';
                                bytes = i*chunksize+5;
                                break;
                        }
                        if(is_utf8_payload(rs[5])) ch++;


                        ws[6] = rs[6];
                        if(rs[6] == 0) {
                                bytes = i*chunksize+6;
                                break;
                        }
                        if(i*chunksize+6 >= max-1) {
                                ws[6] = '\0';
                                bytes = i*chunksize+6;
                                break;
                        }
                        if(is_utf8_payload(rs[6])) ch++;


                        ws[7] = rs[7];
                        if(rs[7] == 0) {
                                bytes = i*chunksize+7;
                                break;
                        }
                        if(i*chunksize+7 >= max-1) {
                                ws[7] = '\0';
                                bytes = i*chunksize+7;
                                break;
                        }
                        ws[7] = '\0';
                        bytes = i*chunksize+7;
                        break;


                } else if(zero_in_64(chunk)) {

/**
 * https://graphics.stanford.edu/~seander/bithacks.html#ZeroInWord
 * Black magic woooo.
 * Also checks up if we are reaching the maximum available size
 **/

                        unsigned char * rs = (unsigned char *)(&r[i]);
                        unsigned char * ws = (unsigned char *)(&w[i]);


                        ws[0] = rs[0];
                        if(rs[0] == 0) {
                                bytes = i*chunksize;
                                goto ret_success;
                        }
                        if(is_utf8_payload(rs[0])) ch++;


                        ws[1] = rs[1];
                        if(rs[1] == 0)  {
                                bytes = i*chunksize+1;
                                goto ret_success;
                        }
                        if(is_utf8_payload(rs[1])) ch++;


                        ws[2] = rs[2];
                        if(rs[2] == 0)  {
                                bytes = i*chunksize+2;
                                goto ret_success;
                        }
                        if(is_utf8_payload(rs[2])) ch++;

                
                        ws[3] = rs[3];
                        if(rs[3] == 0)  {
                               bytes = i*chunksize+3;
                               goto ret_success;
                        }
                        if(is_utf8_payload(rs[3])) ch++;


                        ws[4] = rs[4];
                        if(rs[4] == 0)  {
                               bytes = i*chunksize+4;
                               goto ret_success;
                        }
                        if(is_utf8_payload(rs[4])) ch++;


                        ws[5] = rs[5];
                        if(rs[5] == 0)  {
                                bytes = i*chunksize+5;
                                goto ret_success;
                        }
                        if(is_utf8_payload(rs[5])) ch++;


                        ws[6] = rs[6];
                        if(rs[6] == 0)  {
                                bytes = i*chunksize+6;
                                goto ret_success;
                        }
                        if(is_utf8_payload(rs[6])) ch++;


                        ws[7] = '\0';
                        bytes = i*chunksize+7;
                        goto ret_success;


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

                w[i] = chunk;
                i=j;

        }

ret_success:
        if(bytelen) (*bytelen) = bytes;
        return ch;

}


