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
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <errno.h>
#ifdef __J2_DEV__
#include "../../../../viwerr/viwerr.h"
#endif

#define chunk_has_zero(v) \
        (((v) - 0x0101010101010101ULL) & ~(v) & 0x8080808080808080ULL)

#define chunk_has_utf(v)\
        (v & 0x8080808080808080ULL)

#define utf8_is_char(v)\
        ((v & 0xC0) != 0x80)

#define chunk_t int_fast64_t

// #define J2_STRCPY_SN_DEBUG

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
        // chunk_t * w = (chunk_t *)dst;
        chunk_t * r = (chunk_t *)str;
        

        register size_t i = 0, j = 0, chunksize = sizeof(chunk_t);
        while(1) {
                
                chunk = r[i];
                j+=1;
                #ifdef J2_STRCPY_SN_DEBUG
                printf("r+i = %llu\n", (size_t)r+i);
#endif


                if(j*chunksize > max-1) {

#ifdef J2_STRCPY_SN_DEBUG
                        printf("\tEntered finale:\n");
#endif

                        unsigned char * rs = (unsigned char *)(&r[i]);
                        // unsigned char * ws = (unsigned char *)(&w[i]);

#ifdef J2_STRCPY_SN_DEBUG
                        printf("\trs[0] = %d\n", rs[0]);
#endif

                        if(rs[0] == 0) return i*chunksize;
                        if(i*chunksize > max-1) return max;

#ifdef J2_STRCPY_SN_DEBUG
                        printf("\trs[1] = %d\n", rs[1]);
#endif

                        if(rs[1] == 0) return i*chunksize+1;
                        if(i*chunksize+1 > max-1) return max;


#ifdef J2_STRCPY_SN_DEBUG
                        printf("\trs[2] = %d\n", rs[2]);
#endif

                        if(rs[2] == 0) return i*chunksize+2;
                        if(i*chunksize+2 > max-1) return max;

#ifdef J2_STRCPY_SN_DEBUG
                        printf("\trs[3] = %d\n", rs[3]);
#endif
                
                        if(rs[3] == 0) return i*chunksize+3;
                        if(i*chunksize+3 > max-1) return max;

#ifdef J2_STRCPY_SN_DEBUG
                        printf("\trs[4] = %d\n", rs[4]);
#endif

                        if(rs[4] == 0) return i*chunksize+4;
                        if(i*chunksize+4 > max-1) return max;


#ifdef J2_STRCPY_SN_DEBUG
                        printf("\trs[5] = %d\n", rs[5]);
#endif

                        if(rs[5] == 0) return i*chunksize+5;
                        if(i*chunksize+5 > max-1) return max;

#ifdef J2_STRCPY_SN_DEBUG
                        printf("\trs[6]] = %d\n", rs[6]);
#endif

                        if(rs[6] == 0) return i*chunksize+6;
                        if(i*chunksize+6 > max-1) return max;

#ifdef J2_STRCPY_SN_DEBUG
                        printf("\trs[7] = %d\n", rs[7]);
#endif

                        if(rs[7] == 0) return i*chunksize+7;
                        return max;


                }
/**
 * https://graphics.stanford.edu/~seander/bithacks.html#ZeroInWord
 * Black magic woooo.
 * Also checks up if we are reaching the maximum available size
 **/
                if(chunk_has_zero(chunk)) {

#ifdef J2_STRCPY_SN_DEBUG
                        printf("\tEntered finale:\n");
#endif

                        unsigned char * rs = (unsigned char *)(&r[i]);

#ifdef J2_STRCPY_SN_DEBUG
                        printf("\trs[0] = %d\n", rs[0]);
#endif

                        if(rs[0] == 0) return i*chunksize;

#ifdef J2_STRCPY_SN_DEBUG
                        printf("\trs[1] = %d\n", rs[1]);
#endif

                        if(rs[1] == 0) return i*chunksize+1;

#ifdef J2_STRCPY_SN_DEBUG
                        printf("\trs[2] = %d\n", rs[2]);
#endif

                        if(rs[2] == 0) return i*chunksize+2;

#ifdef J2_STRCPY_SN_DEBUG
                        printf("\trs[3] = %d\n", rs[3]);
#endif
                
                        if(rs[3] == 0) return i*chunksize+3;

#ifdef J2_STRCPY_SN_DEBUG
                        printf("\trs[4] = %d\n", rs[4]);
#endif

                        if(rs[4] == 0) return i*chunksize+4;

#ifdef J2_STRCPY_SN_DEBUG
                        printf("\trs[5] = %d\n", rs[5]);
#endif

                        if(rs[5] == 0) return i*chunksize+5;

#ifdef J2_STRCPY_SN_DEBUG
                        printf("\trs[6]] = %d\n", rs[6]);
#endif

                        if(rs[6] == 0) return i*chunksize+6;

#ifdef J2_STRCPY_SN_DEBUG
                        printf("\trs[7] = %d\n", rs[7]);
#endif

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
        // chunk_t * w = (chunk_t *)dst;
        chunk_t * r = (chunk_t *)str;
        

        register size_t i = 0, j = 0, chunksize = sizeof(chunk_t);
        register size_t ch = 0; 
        while(1) {
                
                chunk = r[i];
                j+=1;
                #ifdef J2_STRCPY_SN_DEBUG
                printf("r+i = %llu\n", (size_t)r+i);
#endif


                if(j*chunksize > max-1) {

#ifdef J2_STRCPY_SN_DEBUG
                        printf("\tEntered finale:\n");
#endif

                        unsigned char * rs = (unsigned char *)(&r[i]);
                        // unsigned char * ws = (unsigned char *)(&w[i]);

#ifdef J2_STRCPY_SN_DEBUG
                        printf("\trs[0] = %d\n", rs[0]);
#endif

                        
                        if(rs[0] == 0) {
                                if(bytelen) (*bytelen) = i*chunksize;
                                return ch;
                        }
                        if(i*chunksize > max-1) {
                                if(bytelen) (*bytelen) = max;
                                return ch;
                        }
                        if(utf8_is_char(rs[0])) ch++;

#ifdef J2_STRCPY_SN_DEBUG
                        printf("\trs[1] = %d\n", rs[1]);
#endif

                        if(rs[1] == 0) {
                                if(bytelen) (*bytelen) = i*chunksize+1;
                                return ch;
                        }
                        if(i*chunksize+1 > max-1) {
                                if(bytelen) (*bytelen) = max;
                                return ch;
                        }
                        if(utf8_is_char(rs[1])) ch++;

#ifdef J2_STRCPY_SN_DEBUG
                        printf("\trs[2] = %d\n", rs[2]);
#endif

                        if(rs[2] == 0) {
                                if(bytelen) (*bytelen) = i*chunksize+2;
                                return ch;
                        }
                        if(i*chunksize+2 > max-1) {
                                if(bytelen) (*bytelen) = max;
                                return ch;
                        }
                        if(utf8_is_char(rs[2])) ch++;

#ifdef J2_STRCPY_SN_DEBUG
                        printf("\trs[3] = %d\n", rs[3]);
#endif
                
                        if(rs[3] == 0) {
                                if(bytelen) (*bytelen) = i*chunksize+3;
                                return ch;
                        }
                        if(i*chunksize+3 > max-1) {
                                if(bytelen) (*bytelen) = max;
                                return ch;
                        }
                        if(utf8_is_char(rs[3])) ch++;

#ifdef J2_STRCPY_SN_DEBUG
                        printf("\trs[4] = %d\n", rs[4]);
#endif

                        if(rs[4] == 0) {
                                if(bytelen) (*bytelen) = i*chunksize+4;
                                return ch;
                        }
                        if(i*chunksize+4 > max-1) {
                                if(bytelen) (*bytelen) = max;
                                return ch;
                        }
                        if(utf8_is_char(rs[4])) ch++;

#ifdef J2_STRCPY_SN_DEBUG
                        printf("\trs[5] = %d\n", rs[5]);
#endif

                        if(rs[5] == 0) {
                                if(bytelen) (*bytelen) = i*chunksize+5;
                                return ch;
                        }
                        if(i*chunksize+5 > max-1) {
                                if(bytelen) (*bytelen) = max;
                                return ch;
                        }
                        if(utf8_is_char(rs[5])) ch++;

#ifdef J2_STRCPY_SN_DEBUG
                        printf("\trs[6] = %d\n", rs[6]);
#endif

                        if(rs[6] == 0) {
                                if(bytelen) (*bytelen) = i*chunksize+6;
                                return ch;
                        }
                        if(i*chunksize+6 > max-1) {
                                if(bytelen) (*bytelen) = max;
                                return ch;
                        }
                        if(utf8_is_char(rs[6])) ch++;

#ifdef J2_STRCPY_SN_DEBUG
                        printf("\trs[7] = %d\n", rs[7]);
#endif

                        if(rs[7] == 0) {
                                if(bytelen) (*bytelen) = i*chunksize+7;
                                return ch;
                        }
                        if(i*chunksize+7 > max-1) {
                                if(bytelen) (*bytelen) = max;
                                return ch;
                        }
                        if(utf8_is_char(rs[7])) ch++;
                        
                        /// UNREACHABLE...
                        if(bytelen) (*bytelen) = max;
                        return ch;
                        
                        // if(utf8_is_char(rs[7])) ch++;

                        // fprintf(stderr, "1: J2 ABORTED THE PROGRAM: STRCPY WENT WRONG!\n");
                        // abort();

                } else if(chunk_has_zero(chunk)) {

/**
 * https://graphics.stanford.edu/~seander/bithacks.html#ZeroInWord
 * Black magic woooo.
 * Also checks up if we are reaching the maximum available size
 **/

#ifdef J2_STRCPY_SN_DEBUG
                        printf("\tEntered finale zero:\n");
#endif

                        unsigned char * rs = (unsigned char *)(&r[i]);

#ifdef J2_STRCPY_SN_DEBUG
                        printf("\trs[0] = %d\n", rs[0]);
#endif

                        if(rs[0] == 0) {
                                if(bytelen) (*bytelen) = i*chunksize;
                                return ch;

                        }
                        if(utf8_is_char(rs[0])) ch++;

#ifdef J2_STRCPY_SN_DEBUG
                        printf("\trs[1] = %d\n", rs[1]);
#endif

                        if(rs[1] == 0) {
                                if(bytelen) (*bytelen) = i*chunksize+1;
                                return ch;
                        }
                        if(utf8_is_char(rs[1])) ch++;

#ifdef J2_STRCPY_SN_DEBUG
                        printf("\trs[2] = %d\n", rs[2]);
#endif

                        if(rs[2] == 0) {
                                if(bytelen) (*bytelen) = i*chunksize+2;
                                return ch;
                        }
                        if(utf8_is_char(rs[2])) ch++;

#ifdef J2_STRCPY_SN_DEBUG
                        printf("\trs[3] = %d\n", rs[3]);
#endif
                
                        if(rs[3] == 0) {
                                if(bytelen) (*bytelen) = i*chunksize+3;
                                return ch;
                        }
                        if(utf8_is_char(rs[3])) ch++;

#ifdef J2_STRCPY_SN_DEBUG
                        printf("\trs[4] = %d\n", rs[4]);
#endif

                        if(rs[4] == 0) {
                                if(bytelen) (*bytelen) = i*chunksize+4;
                                return ch;
                        }
                        if(utf8_is_char(rs[4])) ch++;

#ifdef J2_STRCPY_SN_DEBUG
                        printf("\trs[5] = %d\n", rs[5]);
#endif

                        if(rs[5] == 0) {
                                if(bytelen) (*bytelen) = i*chunksize+4;
                                return ch;
                        }
                        if(utf8_is_char(rs[5])) ch++;

#ifdef J2_STRCPY_SN_DEBUG
                        printf("\trs[6] = %d\n", rs[6]);
#endif

                        if(rs[6] == 0) {
                                if(bytelen) (*bytelen) = i*chunksize+6;
                                return ch;
                        }
                        if(utf8_is_char(rs[6])) ch++;

#ifdef J2_STRCPY_SN_DEBUG
                        printf("\trs[7] = %d\n", rs[7]);
#endif

                        
                        if(bytelen) (*bytelen) = i*chunksize+7;
                        return ch;
                        

                } else if(chunk_has_utf(chunk)) {
                        
                        unsigned char * rs = (unsigned char *)(&r[i]);
                        if(utf8_is_char(rs[0])) ch++;
                        if(utf8_is_char(rs[1])) ch++;
                        if(utf8_is_char(rs[2])) ch++;
                        if(utf8_is_char(rs[3])) ch++;

                        if(utf8_is_char(rs[4])) ch++;
                        if(utf8_is_char(rs[5])) ch++;
                        if(utf8_is_char(rs[6])) ch++;
                        if(utf8_is_char(rs[7])) ch++;

                } else {

                        ch+=8;

                }

                i=j;

        }

        return 0;

}