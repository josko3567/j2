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
 * string.h is a part of the j2 types. The idea of        *
 * the structure of our string type comes from the        *
 * library 'sds' by antirez                               *
 * (https://github.com/antirez/sds/). With the structure  *
 * he created we can incorporate our string types into    *
 * normal C string functions without any hiccups like     *
 * string->buffer or get_buffer(string). printf(string)   *
 * will work perfectly fine due to magic you can read     *
 * at his github repository.                              *
 *                                                        *
 *                                                        *
 * But to summerize, unlike most other string libraries   *
 * that store data in 2 blocks inside memory:             *
 *      1. a struct that stores our string info,          *
 *      2. a char* that stores the string contents.       *
 * 'sds' instead stores everything in one block like      *
 * this \                                                 *
 *      |                                                 *
 *      ˇ                                                 *
 * +--------+----------------------------------+          *
 * | struct | char*                            |          *
 * +--------+----------------------------------+          *
 *          ^                                             *
 *      sds' Pointer                                      *
 *                                                        *
 *                                                        *
 * Even though the struct is the first part of the        +
 * block the pointer instead points to the char*          *
 * part that way we can pass it as a string to std        *
 * functions. To reach back for the structure             *
 * just read negativly into the current pointer:          *
 *                                                        *
 *                                                        *
 * struct sds_st = string[-(sizeof struct sds_st)]        *
 *                                                        *
 *                                                        *
 * So instead of making it hard to reach the data         *
 * stored inside the string we make it hard to reach      *
 * info about the string.                                 *
 *                                                        *
 *                                                        *
 * string.h doesn't use the 'sds' implementation but      *
 * uses the theory that created it.                       *
 *                                                        *
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
 * 6.12.2022.                                             *
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
 * -> j2string...                                         *
 * -> j2_string...                                        *
 * -> J2_STRING...                                        *
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
 * 567     6.12.2022                                      *
 * ------  --------------  ----------------------------   *
 * josko3  V0.5            Added core functionality,      *
 * 567     14.3.2023.      copy & comparison functions    *
 *********************************************************/

#include <math.h>
#include <stdbool.h>
#include <stdint.h>

#ifdef __J2_DEV__
#include "ext/viwerr/viwerr.h"
#endif

#include "../ieee1541.h"

#ifndef J2_STRING_INCLUDE
#define J2_STRING_INCLUDE

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief
 * THE j2string type, if a functions parameter type is a j2string sadly
 * it cannot accept regular char * strings.
 */
typedef char * j2string;

/**
 * @brief 
 * Extra types because even after 50+ years of existince you still
 * can't typedef your type and add a const before it like
 * const j2string or const j2string __restrict :(
 */
typedef const char * const_j2string;
typedef char * __restrict restrict_j2string;
typedef const char * __restrict const_restrict_j2string;


/**
 * @struct @c j2string_st
 * 
 *      @brief Contains all the parameters that are 
 *      inscribed to the j2string.
 * 
 * 
 *      @tparam @b allocated
 *      @tparam @b filled
 *      @tparam @b max
 *      @tparam @b padding
 *      @tparam @b ensure
 *   
 */
struct j2string_st{

        /**
         * @tparam @b allocated
         * Indicates the amount of space that has been allocated for the string 
         * (not including this structure or failsafe null value).
         */
        size_t allocated;
        
        /**
         * @tparam @b padding
         * The size a dynamic string "jumps" to cover a certaint amount of 
         * bytes, and vice versa for deallocating memory.
         */
        size_t padding;

        /**
         * @tparam @b max
         * The maximum size a dynamic string is allowed to reach.
         */
        size_t max;

        /**
         * @tparam @b filled
         * Indicates the amount of space that the data stored inside the string 
         * occupies.
         */
        size_t filled;

        /**
         * @tparam @b length 
         * The length of the string, due to UTF-8 it can be different than
         * filled.
         */
        size_t length;

        /**
         * @tparam @b ensure
         * Ensures (somewhat) the function that it is reading into
         * a valid j2string_st by being set to 0xDEAFC0DE.
         */
        unsigned long ensure; /** Constant 0xDEAFC0DE always */

};

/**
 * @brief 
 * Parameters we pass to j2.string.new:
 */
typedef struct j2string_param {

        size_t allocate; 
        size_t  padding;
        size_t      max; 

} j2string_param;

/**
 * @brief 
 * Returns the closest multiple of the parameter "padding" that is bigger than 
 * the parameter "size".
 */
#define J2_STRING_GET_NEAREST_PADDING(size, padding)                           \
        ( ((long)ceil((double)size / (double)padding))*padding )

/**
 * @brief 
 * Used for calculating the updated size of a j2string.
 * 
 * Returns oldsize if padding == 0, otherwise we return max if the nearest
 * padding multiple is bigger than it or we return the padding multiple if
 * that is not the case.
 */
#define J2_STRING_GET_NEW_SIZE(oldsize, newsize, padding, max)                 \
        (padding == 0 ?                                                        \
                (oldsize <= 0 ? 1 : oldsize)                                   \
                :                                                              \
                (J2_STRING_GET_NEAREST_PADDING(newsize, padding) > max ?       \
                        max :                                                  \
                        J2_STRING_GET_NEAREST_PADDING(newsize, padding)))      

/**
 * @defgroup Dynamic size defaults
 * @brief 
 * Default minimum and maximum specifiers,
 * minimum is unused.
 * @{
 */
#define J2_STRING_MIN_SIZE _IEEE1541_B(1)
#define J2_STRING_MAX_SIZE _IEEE1541_INT_MiB(1)
/**
 * @}
 */

/**
 * @brief 
 * 
 */
// #define J2_STRING_UTF8_


/**
 * @brief 
 * Macro to calculate the size of a j2string
 * by the given string length.
 */
#define J2_STRING_GET_MEMORY_SIZE(length)                                      \
(sizeof(struct j2string_st)+((size_t)length+(size_t)1)*sizeof(char))

/**
 * @brief 
 * Default value of j2string_st->ensure. 
 */
#define J2_STRING_ENSURE_HEXSPEAK   0xDEAFC0DE

/**
 * @brief 
 * Jump to the string part from the structure part
 * of a j2string type.
 */
#define J2_STRING_REACH_FOR_STRING(p)                                          \
((j2string)( ((uint8_t*)p)+sizeof(struct j2string_st) ))

/**
 * @brief 
 * Check ensure if it contains 0xDEAFC0DE, if true
 * give a pointer to the structure otherwise NULL.
 */
#define J2_STRING_REACH_FOR_STRUCT(p)                                          \
((struct j2string_st *)(*((long*)( (uint8_t*)p - sizeof(long)))                \
== J2_STRING_ENSURE_HEXSPEAK ?                                                 \
        (struct j2string_st *)( ((uint8_t*)p) -                                \
        sizeof(struct j2string_st) )                                           \
        :                                                                      \
        NULL))

#define J2_STRING_INFO(string) J2_STRING_REACH_FOR_STRUCT(string)

/**
 * @brief 
 * Assert the fact that the string ends on a null character.
 */
#define J2_STRING_ASSERT_FILLED(string)\
        (J2_STRING_INFO(string) == NULL ? 0 : \
        (string[J2_STRING_INFO(string)->filled] == '\0' ? 1 : 0))

/**
 * @brief 
 * Assert the fact that the string ends on a null character.
 * Pass the position where you expect a null value.
 */
#define J2_STRING_ASSERT_FILLED_NOCH(string, pos)\
        (string[pos] == '\0' ? 1 : 0)

size_t
j2string_strlen_sn(
        const char * str,
        size_t max 
);

size_t
j2string_strcpy_sn(
        char * __restrict dst,
        const char * __restrict src,
        size_t max 
);

size_t
j2string_utf8_strcpy_sn(
        char * __restrict       dst,
        const char * __restrict src,
        size_t                  max,
        size_t *            bytelen 
);

size_t
j2string_utf8_nhbc_strcpy_sn(
        char * __restrict       dst,
        const char * __restrict src,
        size_t                  max,
        size_t *            bytelen 
);

size_t
j2string_utf8_strlen_sn(
        const char * str,
        size_t       max,
        size_t * bytelen 
);

j2string
j2string_allocate(
        j2string_param parameters 
);

j2string_param 
j2string_normalize_param(
        j2string_param parameters,
        long length 
);

j2string 
j2string_init(
        const char *  c_string,
        j2string_param parameters
);

j2string 
j2string_new_st(
        const char *  c_string
);

j2string 
j2string_new_dy(
        const char *  c_string
);

j2string
j2string_update(
        j2string string, 
        long newsize 
);

void
j2string_free(
        j2string string 
);

long
j2string_ccpy(
        restrict_j2string *   dst, 
        const char * __restrict src 
);

long
j2string_noutf8_ccpy(
        restrict_j2string *   dst, 
        const char * __restrict src 
);

long
j2string_cpy(
        restrict_j2string * dst, 
        const_restrict_j2string src 
);

long
j2string_noutf8_cpy(
        restrict_j2string * dst, 
        const_restrict_j2string src 
);

long
j2string_cat(
        restrict_j2string * dst, 
        const_restrict_j2string src 
);

long
j2string_noutf8_cat(
        restrict_j2string * dst, 
        const_restrict_j2string src 
);


long
j2string_ccat(
        restrict_j2string * dst, 
        const char * __restrict src 
);

long
j2string_noutf8_ccat(
        restrict_j2string * dst, 
        const char * __restrict src 
);

long
j2string_cmp(
        const_restrict_j2string str1, 
        const_restrict_j2string str2 
);

long
j2string_cspn(
        const_restrict_j2string str,
        const_restrict_j2string reject 
);

long
j2string_spn(
        const_restrict_j2string str,
        const_restrict_j2string accept 
);

long
j2string_len(
        const_j2string string
);

long
j2string_chrs(
        const_j2string string,
        unsigned char c,
        long position
);

long
j2string_rchrs(
        const_j2string string,
        unsigned char c,
        long position
);

long
j2string_cnt(
       const_j2string string,
       unsigned char c 
);

#ifdef __cplusplus
}
#endif

#endif /** @c J2_STRING_INCLUDE */
