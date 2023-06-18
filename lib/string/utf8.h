/*** LICENSE: LGPLV2. *************************************
 *                                                        *
 *                                                        *
 * This file is a part of the j2 project!                 *
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

/*** INTRODUCTION: ****************************************
 *                                                        *
 *                                                        *
 * UTF8 functionality for detecting and comparing.        *
 *                                                        *
 *                                                        *
 *********************************************************/

/*** INSCRIBING: FILE, AUTHOR & DATE INFORMATION. *********
 *                                                        *
 *                                                        *
 * FILE:                                                  *
 * Header for bubbling implementations to the surface     *
 * that the user is entrusted to use.                     *
 *                                                        *
 *                                                        *
 * AUTHOR:                                                *
 *      NAME:  Joško Križanović                           *
 *      EMAIL: jkriza02@fesb.com                          *
 *                                                        *
 *                                                        *
 * DATE:                                                  *
 * 16.6.2023.                                             *
 *                                                        *
 *                                                        *
 *********************************************************/

/*** INSCRIBING: COMPATIBILITY. ***************************
 *                                                        *
 *                                                        *
 * C:                                                     *
 * This library is written in compliance to the limited   *
 * functionality at the disposal of the C99 standard.     *
 *                                                        *
 *                                                        *
 * POSIX:                                                 *
 * This libraries creator has yet to inscribe the minimum *
 * POSIX compatibilty version that this library requires. *
 * In its absance the user of the library is free to use  *
 * any compatibilty version of the POSIX standard that is *
 * deemed to work through trial and error.                *
 *                                                        *
 *                                                        *
 *********************************************************/

/*** INSCRIBING: USED NAMESPACE ***************************
 *                                                        *
 *                                                        *
 * Used namespaces, ... indicates that all characters     *
 * that come after do not belong to that namespace:       *
 * -> UTF8_...                                            *
 * -> IS_UTF8_...                                         *
 *                                                        *
 *                                                        *
 *********************************************************/

/*** INSCRIBING: EXTERNAL FILES CALLED & PURPOSE **********
 *                                                        *
 *                                                        *
 * Name                     Description                   *
 * ----                     -----------                   *
 * <stdbool.h>              booleans ma dude              *
 *                                                        *
 *                                                        *
 * "ieee1541.h"             IEEE1541 constants like MiB   *
 *                                                        *
 *                                                        *
 *                                                        *
 *********************************************************/

/*** INSCRIBING: DEVELOPMENT HISTORY/VERSIONS PUBLISHED ***
 *                                                        *
 *                                                        *
 * Author  Release & Date  Description of modifications   *
 * ------  --------------  ----------------------------   *
 * josko3  V0.0            Inital creation.               *
 * 567     16.6.2023                                      *
 *********************************************************/

#include <stdarg.h>
#include <stdint.h>
// #include "ieee1541.h"

#ifndef J2_UTF8_H
#define J2_UTF8_H

#define UTF8_MAX_SIZE 4

#define UTF8_MAX_SIZE_BIT 32

#define UTF8_FLAG                     0x80
#define UTF8_FLAG_MIRROR              0x01

#define UTF8_TYPE_1_FLAG              0xC0
#define UTF8_TYPE_2_FLAG              0xE0
#define UTF8_TYPE_3_FLAG              0xF0

#define UTF8_TYPE_1_FLAG_MIRROR       0x03
#define UTF8_TYPE_2_FLAG_MIRROR       0x07
#define UTF8_TYPE_3_FLAG_MIRROR       0x0F

#define UTF8_TYPE_1_FLAG_RIGHT        0x06
#define UTF8_TYPE_2_FLAG_RIGHT        0x0E
#define UTF8_TYPE_3_FLAG_RIGHT        0x1E

#define UTF8_TYPE_1_READ_MASK         0x1F
#define UTF8_TYPE_2_READ_MASK         0x0F
#define UTF8_TYPE_3_READ_MASK         0x07

#define UTF8_PAYLOAD_BYTE_FLAG        0x80
#define UTF8_PAYLOAD_BYTE_FLAG_MIRROR 0x01

#define UTF8_PAYLOAD_BYTE_READ_MASK   0x3F

#define UTF8_NO  0
#define UTF8_T1  1
#define UTF8_T2  2
#define UTF8_T3  3
// #define UTF8_PL  4

/**
 * @brief 
 * Used mostly in functions to represent utf-8 characters that are not
 * complete.
 */
typedef struct st_utf8_t {

        uint8_t      type;
        uint8_t  payloads;
        uint32_t sequence;

} utf8_t;

/**
 * @brief 
 * Check if a byte has the 8th bit set.
 */
#define is_utf8(byte) ((byte & UTF8_FLAG)>>7)

/**
 * @brief 
 * Check if a byte does not have the 8th bit set.
 */
#define not_utf8(byte) ((!(byte & UTF8_FLAG))%127)

/**
 * @brief 
 * Check if a byte is a UTF-8 payload type.
 * Important function because counting all bytes that are not payloads
 * will return us the amount of characters a UTF-8 string has.
 */
#define is_utf8_payload(byte) \
        ((byte & UTF8_TYPE_1_FLAG) != UTF8_PAYLOAD_BYTE_FLAG)
        
/**
 * @brief 
 * Returns 1 if the byte is a UTF-8 head of any type otherwise 0.
 */
#define is_utf8_head(byte) ((v & UTF8_TYPE_1_FLAG) != UTF8_FLAG)

/**
 * @brief 
 * Check if the byte is the first byte of a UTF-8 sequence and return the type.
 */
#define is_utf8_head_w_type(byte)                                              \
        (byte>>6 == UTF8_TYPE_1_FLAG_MIRROR ?                                  \
                (byte>>5 == UTF8_TYPE_2_FLAG_MIRROR ?                          \
                        (byte>>4 == UTF8_TYPE_3_FLAG_MIRROR                    \
                                ? UTF8_T3 : UTF8_T2)                           \
                        : UTF8_T1)                                             \
                : UTF8_NO                                                      \
        )


/**
 * @brief 
 * Detect within a 16bit chunk if there are any bytes that could contain
 * a UTF-8 character.
 */
#define utf8_in_16(u) (u & (unsigned short)0x8080)
// #define zero_in_16()


/**
 * @brief 
 * Detect within a 32bit chunk if there are any bytes that could contain
 * a UTF-8 character.
 */
#define utf8_in_32(ld) (ld & 0x80808080UL)

/**
 * @brief 
 * Detect within a 64bit chunk if there are any bytes that could contain
 * a UTF-8 character.
 */
#define utf8_in_64(lld) (lld & 0x8080808080808080ULL)
#define zero_in_64(v) \
        (((v) - 0x0101010101010101ULL) & ~(v) & 0x8080808080808080ULL)

utf8_t utf8_init(
        uint8_t head );

utf8_t utf8_cat(
        utf8_t symbol, 
        uint8_t byte );

#endif