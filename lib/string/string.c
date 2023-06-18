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
 * 6.12.2022.                                             *
 *                                                        *
 *                                                        *
 *********************************************************/

#include <stdarg.h>
#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <malloc.h>
#include <errno.h>
#include <math.h>

#include "string.h"
#include "../ieee1541.h"
#include "../bitflag.h"

#ifndef J2_STRING_C_INCLUDE
#define J2_STRING_C_INCLUDE

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
long
j2string_strlen_sn(
        const char * str,
        long max )
{

#ifndef J2_REMOVE_CHECKS
        if(!str && max <= 0) {
                errno = EINVAL;
                return 0;
        }
#endif

        long len = 0;
        for(;str[len] != '\0' && len < max; len++);
        return len;

}

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
long
j2string_strcpy_sn(
        char * __restrict dst,
        const char * __restrict src,
        long max )
{

#ifndef J2_REMOVE_CHECKS
        if(!dst || !src || max <= 0) {
                errno = EINVAL;
                return 0;
        }
#endif

        long len = 0;
        for( ; src[len] != '\0' && len < max-1; len++) 
                dst[len] = src[len];
        dst[len] = '\0';
        return len;

}

/**
 * @fn @c j2string_allocate(1)
 * 
 *      @brief  
 *      Allocate enough memory for a j2string.
 * 
 * 
 *      @param @b parameters
 *                Set of parameters with which we create the j2string,
 *                not required but it is good to pass the parameters through
 *                normalization via j2string_normalize_param(2).
 * 
 * 
 *      @throw @a ENOMEM - Exhausted all memory spaces.
 * 
 * 
 *      @return A j2string pointer of size @c parameters.allocate and with
 *      the j2string info filled with the given @c parameters. If calloc()
 *      fails to allocate memory for the string, the func. returns NULL.
 * 
 *
 */
j2string
j2string_allocate(
        j2string_param parameters )
{

        struct j2string_st * memory = (struct j2string_st *)calloc(
                J2_STRING_GET_MEMORY_SIZE(parameters.allocate), 1
        );

        if(!memory) 
                return NULL;

        memory->padding   = parameters.padding;
        memory->allocated = parameters.allocate;
        memory->max       = parameters.max;
        memory->filled    = 0;
        memory->ensure    = J2_STRING_ENSURE_HEXSPEAK;

        return J2_STRING_REACH_FOR_STRING(memory);
 
}

/**
 * @fn @c j2string_free(1)
 * 
 *      @brief 
 *      Release the memory of a j2string back into the wilderness.
 * 
 * 
 *      @param @b string
 *                A j2string type string that will be released.
 * 
 * 
 *      @throw None.
 *
 * 
 *      @return Nothing.
 * 
 */
inline
void
j2string_free(
        j2string string )
{

        if(string == NULL) 
                return;
        else if(J2_STRING_INFO(string) == NULL) 
                return;

        free(J2_STRING_REACH_FOR_STRUCT(string));

}

/**
 * @fn @c j2string_update(2)
 * 
 *      @brief 
 *      Update the size of the j2string @c "string" to the @c "newsize" if 
 *      possible. 
 *      
 * 
 *      @param @b string
 *                The j2string type string to whom shall be asigned a new size.
 *      
 *      @param @b newsize
 *                The new size of the @c "string".
 * 
 * 
 *      @throw @a EINVAL - If @c "string" points NULL, @c "string" isn't of type
 *                         j2string or newsize <= 0.
 * 
 *      @throw @a ENOMEM - If @c realloc() fails with ENOMEM.
 * 
 * 
 *      @return A initalized j2string with the contents of @c "string" but with 
 *      a different allocated size. If the new string is smaller than the 
 *      original @c "string" we fit as much as we can from @c "string" into the
 *      new j2string. If the size was not altered or an error occures, we return 
 *      the unaltered original @c "string". 
 * 
 */
j2string
j2string_update(
        j2string string, 
        long newsize )
{

#ifndef J2_REMOVE_CHECKS
        if(string == NULL 
        || newsize <= 0){

                errno = EINVAL;
                return string;

        } else if( J2_STRING_INFO(string) == NULL){

                errno = EINVAL;
                return string;

        }
#endif

        struct j2string_st * info = J2_STRING_INFO(string);

        /// Calculate the updated size of the string.
        long calcsize = J2_STRING_GET_NEW_SIZE(
                info->allocated,
                newsize,
                info->padding,
                info->max
        );

        if(calcsize != info->allocated) {

                /// Reallocate to fit newly calculated size.
                errno = 0; 
                info  = realloc(
                        info, J2_STRING_GET_MEMORY_SIZE(calcsize));
                if(errno == ENOMEM || info == NULL) return string;

                /// Failsafe NULL value.
                info->allocated = calcsize;
                J2_STRING_REACH_FOR_STRING(info)[
                        calcsize // J2_STRING_GET_MEMORY_SIZE has one byte
                                 // outside of the string for storing a failsafe
                                 // null.
                ] = '\0';

                /// Update info->filled if allocated became smaller than filled.
                if(info->allocated <= info->filled) {
                        info->filled = info->allocated-1;
                        J2_STRING_REACH_FOR_STRING(info)[
                                info->allocated
                        ] = '\0';
                }

        }

        return J2_STRING_REACH_FOR_STRING(info);

}

/**
 * @fn @c j2string_normalize_param(2)
 * 
 *      @brief 
 *      Sanity check for all the inputed variables, and respecting their setup 
 *      interactions.
 *      
 *      @tparam SANITY:
 *      The following sanity checks are given.
 *          If parameters.max <= 0, we give it the default size of
 *      J2_STRING_MAX_SIZE (1MiB as of 7. Mar. 2023.)
 *          If parameters.padding < 0, we clamp it to 0 which indicates that the 
 *      given string is static meaning that it will not update its size to fit a 
 *      new input nor will it lessen it's size for trimming of the current 
 *      values.
 *          If the value "length" is less than 1, we clamp it back to 1.
 * 
 *      @tparam INTERACTIONS:
 *      The following interactions occur between the values of a j2string_param:
 *          If parameters.allocate is <= 0 and parameters.padding is also <= 0
 *      then the value of parameters.allocate is set to the value of "length"
 *      ie. the size of the string.
 *          If parameters.allocate is <= 0 and parameters.padding is bigger than 
 *      0 then the value of parameters.allocate is set to the nearest multiple 
 *      of padding that can fit a amount of characters described by size 
 *      "length".
 *          If parameters.allocate and parameters.padding are both bigger than 
 *      0, then we set the parameters.allocate to the nearest multiple of the 
 *      given value of parameters.allocate.
 * 
 * 
 *      @param @b parameters 
 *             j2string_param type, should contain values bigger than 0.
 *       
 *      @param @b length
 *             length of the string (usually of C type) we are trying to fit.
 * 
 * 
 *      @throw None.
 * 
 * 
 *      @return Normalized j2string_param type.
 * 
 */
inline
j2string_param 
j2string_normalize_param(
        j2string_param parameters,
        long length )
{

        /// SANITY:

        if(parameters.max <= 0)                        
                parameters.max = J2_STRING_MAX_SIZE;   

        if(parameters.padding < 0)                     
                parameters.padding = 0;      

        if(length <= 0)
                length = 1;

        /// INTERACTION:

        if(parameters.allocate <= 0){

                if(parameters.padding <= 0) {

                        parameters.allocate = length;

                } else {
                        
                        parameters.allocate = J2_STRING_GET_NEW_SIZE(
                                length, length, 
                                parameters.padding,
                                parameters.max
                        );    
                }    

        } else if(parameters.allocate > 0
               && parameters.padding  > 0 ) {

                parameters.allocate = J2_STRING_GET_NEW_SIZE(
                        parameters.allocate, parameters.allocate, 
                        parameters.padding,
                        parameters.max
                );   

        }

        /// SANITY:
                                              
        if(parameters.allocate > parameters.max)       
                parameters.allocate = parameters.max;                                           

        return parameters;

} 


/**
 * @fn @c j2string_init(2) 
 * 
 *      @brief Initilaze a @a j2string as a static or dynamic string.
 * 
 * 
 *      @param @b c_string
 *                C's char* type string that will fill the new j2string.
 *      @param @b parameters
 *                A j2string_param structure from which the following 
 *                parameters are used:
 * 
 *             @param @b parameters.allocate
 *                    The amount of memory to allocate at the start.
 *                      If @c parameters.padding is not less than or equal to 0 
 *                    it is set to a multiple of padding that can fit the passed
 *                    @c parameters.allocate.
 *                      If initaly set to 0 or less, @c parameters.allocate will
 *                    calculate the size of @c "c_string" and use its value
 *                    instead.
 * 
 *             @param @b paramters.max
 *                    The maximum amount of memory a dynamic string is allowed
 *                    to grow to, the minimum will always be 1 for a null value.
 *                      If initaly set to 0 or less, @c parameters.max will take
 *                    upon itself the value of @c J2_STRING_MAX_SIZE ~ 1MiB.
 *                      
 *             @param @b parameters.padding
 *                    The step a dynamic string takes to fit new input inside of
 *                    itself.
 *                      A j2string can be static or dynamic depending on the
 *                    @c parameters.padding.
 *                      If @c parameters.padding is 0 or less then the value
 *                    of @c padding is set to 0 and the string is initalized as
 *                    a static type j2string.
 *                      If @c parameters.padding is 1 or more then the j2string
 *                    is initalized as a dynamic type j2string that grows in
 *                    steps of size @c parameters.padding.
 * 
 * 
 *      @throw @a ENOMEM - From j2string_allocate, exhausted all memory spaces.
 *      @throw @a ENIVAL - Throwable by j2tstring_strlen_sn() and
 *                         j2string_strcpy_sn, very unlikely.
 * 
 * 
 *      @returns a initialized @c j2string.
 *   
 */
j2string 
j2string_init(
        const char * c_string,
        j2string_param parameters)
{

        c_string = c_string ? c_string : "";

        long allocated = j2string_strlen_sn(c_string, J2_STRING_MAX_SIZE-1)+1;
        j2string string = j2string_allocate(
                j2string_normalize_param(parameters, allocated)); 
        if(!string) return NULL;
        struct j2string_st * info = J2_STRING_REACH_FOR_STRUCT(string);
        info->filled = j2string_strcpy_sn(string, c_string, info->allocated);
        
        return string;

}

/**
 * @fn @c j2string_new_st(1)
 * 
 *      @brief 
 *      Initalize a j2string with all j2string_param values set to 0.
 * 
 * 
 *      @param @b c_string
 *                C type string to get the inital text for our string.
 * 
 * 
 *      @throw @a ENOMEM - j2string_init() throwable.
 *      @throw @a ENIVAL - j2string_init() throwable.
 * 
 * 
 *      @return A initalized @c static @a j2string.
 * 
 */
j2string
j2string_new_st(
        const char * c_string )
{
        return j2string_init(c_string, (j2string_param){
                .allocate = 0,
                .max = 0,
                .padding = 0
        });
}

/**
 * @fn @c j2string_new_dy(1)
 * 
 *      @brief 
 *      Initalize a j2string with padding set to 1.
 * 
 * 
 *      @param @b c_string
 *                C type string to get the inital text for our string.
 * 
 * 
 *      @throw @a ENOMEM - j2string_init() throwable.
 *      @throw @a ENIVAL - j2string_init() throwable.
 * 
 * 
 *      @return A initalized @c dynamic @a j2string.
 * 
 */
j2string
j2string_new_dy(
        const char * c_string )
{
        return j2string_init(c_string, (j2string_param){
                .allocate = 0,
                .max = 0,
                .padding = 1
        });
}

/**
 * @fn @c j2string_recalc_filled(1)
 * 
 *      @brief 
 *      If a string fails the J2_STRING_ASSERT_FILLED check we send it off to 
 *      this function to recalculate its filled size.
 * 
 * 
 *      @param @b string
 *             j2string type string with wrong info.
 * 
 * 
 *      @throw EINVAL - Invalid parameters.
 *      @throw EILSEQ - info->allocated was less than 1.
 * 
 * 
 *      @return 0 upon successfully reinstating the filled amount, non 0 upon
 *      failure, call upon errno for a further description.
 * 
 */
int
j2string_recalc_filled(
        j2string string)
{
        
#ifndef J2_REMOVE_CHECKS
        if(!string) {
                errno = EINVAL;
                return 1;
        }
#endif

        struct j2string_st * info = J2_STRING_INFO(string);

#ifndef J2_REMOVE_CHECKS
        if(!info) {
                errno = EINVAL;
                return 1;
        }

        if(info->allocated <= 0) {
                errno = EILSEQ;
                return 1;
        }
#endif

        info->filled = j2string_strlen_sn(
                string, info->allocated
        );

        if(info->filled == info->allocated){
                info->filled--;
                string[info->filled] = '\0';
        }

        return 0;

}

/**
 * @fn @c j2string_cpy_c(2)
 * 
 *      @brief 
 *      Copy a C type string into a j2string.
 * 
 * 
 *      @param @b dst 
 *             j2string type string to be written into.
 * 
 *      @param @b src 
 *             C type string to read from.
 * 
 * 
 *      @throw @a EINVAL - If @c dst and/or @c src and/or @c (*dst) are NULL 
 *                         pointers,
 *                         if (*dst) is not a valid j2string type.
 *
 * 
 *      @return Amount of bytes written into @c (*dst).
 * 
 */
long
j2string_cpy_c(
        restrict_j2string *   dst, //[in, out]
        const char * __restrict src  //[in] 
        )
{

#ifndef J2_REMOVE_CHECKS
        if(!src || !dst){
                errno = EINVAL;
                return 0;
        } else if(!(*dst)) {
                errno = EINVAL;
                return 0;
        }
#endif

        struct j2string_st * dstinfo = J2_STRING_INFO((*dst));

#ifndef J2_REMOVE_CHECKS
        if(!dstinfo){
                errno = EINVAL;
                return 0;
        }
#endif

        long size = j2string_strlen_sn(src, dstinfo->max-1)+1;

        errno = 0;
        (*dst) = j2string_update((*dst), size);
        if(errno != 0) 
                return 0;
        else 
                dstinfo = J2_STRING_INFO((*dst));

        dstinfo->filled = j2string_strcpy_sn(
                (*dst), src, 
                dstinfo->allocated
        );

        return dstinfo->filled;

}

/**
 * @fn @c j2string_cpy(2)
 * 
 *      @brief 
 *      Copy the contents of a j2string to another j2string.
 *      
 * 
 *      @param @b dst
 *             Pointer to a j2string, j2string_cpy is allowed to change the size
 *             of dst to better accomidate src.
 *      
 *      @param @b src
 *             Source string of type j2string from which we read text from.
 * 
 * 
 *      @throw @a EINVAL - Invalid parameters ie. @c dst and/or @c (*dst) and/or 
 *                         @c src are not NULL pointers, 
 *                         @c (*dst) and/or src are not valid j2string types,
 *                         if @c j2string_recalc_filled() had bad parameters.
 *      @throw @a ENOMEM - Exhaused all memory spaces for resizing.
 *      @throw @a EILSEQ - @c j2string_recalc_filled() recived a j2string with
 *                         the allocated size being less than 1.
 *             
 * 
 *      @return The amount of bytes written to @c (*dst).
 * 
 */
long
j2string_cpy(
        restrict_j2string * dst,
        const_restrict_j2string src)
{

#ifndef J2_REMOVE_CHECKS
        if(!dst || !src) {
                errno = EINVAL;
                return 0;
        }
        if(!(*dst)) {
                errno = EINVAL;
                return 0;
        }
#endif

        struct j2string_st * infodst = J2_STRING_INFO((*dst));
        struct j2string_st * infosrc = J2_STRING_INFO(src);

#ifndef J2_REMOVE_CHECKS
        if(!infosrc || !infodst){
                errno = EINVAL;
                return 0;
        }
#endif

        (*dst) = j2string_update((*dst), infosrc->filled+1);
        if(!(*dst)) 
                return 0;

        infodst = J2_STRING_INFO((*dst));
        infodst->filled = j2string_strcpy_sn(
                (*dst), src, 
                infodst->allocated < infosrc->allocated ?
                        infodst->allocated :
                        infosrc->allocated
        );

        return infodst->filled;

}

/**
 * @fn @c j2string_cmp(2)
 * 
 *      @brief Compare characters of 2 strings until we reach 2 that are not
 *      the same or we reach the end.
 * 
 * 
 *      @param @b str1 
 *             j2string type string.
 *      @param @b str2 
 *             Another j2string type string.
 *      
 * 
 *      @throw @a EINVAL - If str1 and/or str2 are/is NULL pointers,
 *                         If str1 and/or str2 are/is not a valid j2string. 
 * 
 * 
 *      @return When we reach 2 characters that are not equal if str1[i] < 
 *      str2[i] we return -1, if str1[i] > str2[i] then we return 1, if all 
 *      characters were equal, we return 0.
 * 
 */
long
j2string_cmp(
        const_restrict_j2string str1,
        const_restrict_j2string str2 )
{

#ifndef J2_REMOVE_CHECKS
        if(!str1 || !str2) {
                errno = EINVAL;
                return 0;
        }
#endif

        struct j2string_st * info1 = J2_STRING_INFO(str1);
        struct j2string_st * info2 = J2_STRING_INFO(str2);

#ifndef J2_REMOVE_CHECKS
        if(!info1 || !info2){
                errno = EINVAL;
                return 0;
        }
#endif

        // if(!J2_STRING_ASSERT_FILLED_NOCH(str1, info1->filled))
        //         if(j2string_recalc_filled(str1) != 0)
        //                 return 0;
        
        // if(!J2_STRING_ASSERT_FILLED_NOCH(str2, info2->filled)) 
        //         if(j2string_recalc_filled(str2) != 0)
        //                 return 0;

        /// Extract info and create unsigned char strings due to
        /// the fact that char is signed, so comparisons would be broken
        /// for characters with a value bigger than 127.

        
        const unsigned char *s1 = (const unsigned char *) str1;
        const unsigned char *s2 = (const unsigned char *) str2;
        
        unsigned char c1, c2;
        
        /// Maximum amount of characters allowed to be scanned.
        long max = info1->filled < info2->filled ? 
                info1->filled : info2->filled;
        int i;
        for(i = 0; s1[i] != '\0' && s2[i] != '\0' && i < max; i++) {
                c1 = s1[i];
                c2 = s2[i];
                if(c1 < c2) {
                        return -1;
                } else if (c1 > c2) {
                        return 1;
                }
        }

        if(s1[i] == '\0' && s2[i] != '\0') {
                return -1;
        }

        if(s1[i] != '\0' && s2[i] == '\0') {
                return 1;
        }

        return 0;

}

/**
 * @fn @c j2string_cspn(2)
 * 
 *      @brief Compare characters of 2 strings until we reach 2 that are not
 *      the same or we reach the end.
 * 
 * 
 *      @param @b str1 
 *             j2string type string.
 *      @param @b str2 
 *             Another j2string type string.
 *      
 * 
 *      @throw @a EINVAL - If str1 and/or str2 are/is NULL pointers,
 *                         If str1 and/or str2 are/is not a valid j2string. 
 * 
 * 
 *      @return When we reach 2 characters that are not equal if str1[i] < 
 *      str2[i] we return -1, if str1[i] > str2[i] then we return 1, if all 
 *      characters were equal, we return 0.
 * 
 */
long
j2string_cspn(
        const_restrict_j2string str,
        const_restrict_j2string reject )
{

#ifndef J2_REMOVE_CHECKS
        if(!str || !reject) {
                errno = EINVAL;
                return 0;
        }
#endif

        struct j2string_st * strinfo = J2_STRING_INFO(str);
        struct j2string_st * rejectinfo = J2_STRING_INFO(reject);

#ifndef J2_REMOVE_CHECKS
        if(!strinfo || !rejectinfo){
                errno = EINVAL;
                return 0;
        }
#endif

        const unsigned char *s   = (const unsigned char *) str;
        const unsigned char *rej = (const unsigned char *) reject;

        /// Maximum amount of characters allowed to be scanned.

        char table[256] = {0};
        for(int i = 0; rej[i] != '\0' && i < rejectinfo->filled; i++)
                table[rej[i]] = 1;
        int l;
        for(l = 0; s[l] != '\0' && l < strinfo->filled; l++)
                if(table[s[l]] == 1) break;
        return l;

}

/**
 * @fn @c j2string_len(1)
 * 
 *      @brief Return the size of the text held by the passed j2string.
 * 
 * 
 *      @param @b string
 *             j2string type string.
 * 
 * 
 *      @throw EINVAL - If @c "string" is a invalid pointer and/or is not a
 *                      j2string type string.
 * 
 *      @return J2_STRING_INFO(string)->filled...
 */
inline
long
j2string_len(
        const_j2string string )
{       
#ifndef J2_REMOVE_CHECKS
        if(!string){
                errno = EINVAL;
                return 0;
        } else if(J2_STRING_INFO(string) == NULL) {
                errno = EINVAL;
                return 0;
        }
#endif

        return J2_STRING_INFO(string)->filled;

}

/**
 * @fn @c j2string_cat(2)
 * 
 *      @brief Concatenate @c "src" onto the end of @c "(*dst)".
 * 
 *     
 *      @param @b dst
 *             j2string type string to concatinate to.
 * 
 *      @param @b src
 *             j2string type string that will be concatinated.
 * 
 *      
 *      @throw @a EINVAL - Invalid arguments.
 * 
 *      @returns The number of bytes concatinated to @c "(*dst)".
 * 
 */
long
j2string_cat(
        restrict_j2string * dst,
        const_restrict_j2string src)
{

        #ifndef J2_REMOVE_CHECKS
        if(!dst || !src) {
                errno = EINVAL;
                return 0;
        }
        if(!(*dst)) {
                errno = EINVAL;
                return 0;
        }
#endif

        struct j2string_st * infodst = J2_STRING_INFO((*dst));
        struct j2string_st * infosrc = J2_STRING_INFO(src);

#ifndef J2_REMOVE_CHECKS
        if(!infosrc || !infodst){
                errno = EINVAL;
                return 0;
        }
#endif

        (*dst) = j2string_update((*dst), 
                infosrc->filled + infodst->filled + 1);
        if(!(*dst)) return 0;

        infodst = J2_STRING_INFO((*dst));
        long written = j2string_strcpy_sn(
                (*dst)+infodst->filled, src, 
                infodst->allocated - infodst->filled
        );
        infodst->filled += written;
        return written;

}

/**
 * @brief strchr but it returns a long instead
 * 
 */
long
j2string_chrs(
        const_j2string string,
        unsigned char c,
        long position)
{

#ifndef J2_REMOVE_CHECKS
        if(!string || position < 0) {
                errno = EINVAL;
                return -1;
        } 
#endif

        struct j2string_st * info = J2_STRING_INFO(string);

#ifndef J2_REMOVE_CHECKS
        if(!info){
                errno = EINVAL;
                return -1;
        }
#endif
        /// We cannot find null so return -1.
        if(c == '\0') return -1;

        long i;
        for(i = position; i < info->filled; i++) {

                if(string[i] != '\0') {

                        if(string[i] == c) break; 

                } else {

                        break;

                }

        }
        
        if(string[i] != c) return -1;
        return i;
        
}

/**
 * @brief strchr but it returns a long instead
 * 
 */
long
j2string_rchrs(
        const_j2string string,
        unsigned char c,
        long position)
{

#ifndef J2_REMOVE_CHECKS
        if(!string || position < 0) {
                errno = EINVAL;
                return -1;
        } 
#endif

        struct j2string_st * info = J2_STRING_INFO(string);

#ifndef J2_REMOVE_CHECKS
        if(!info){
                errno = EINVAL;
                return -1;
        }
#endif
        /// We cannot find null so return -1.
        if(c == '\0') return -1;

        long i;
        for(i = position; i >= 0; i--) {

                if(string[i] == c) break;
                // printf("--- %d\n", i);
        }
        
        if(string[i] != c) return -1;
        return i;
        
}

/**
 * @brief 
 * count character
 * @param string 
 * @param c 
 * @return long 
 */
long
j2string_cnt(
       const_j2string string,
       unsigned char c )
{

#ifndef J2_REMOVE_CHECKS
        if(!string) {
                errno = EINVAL;
                return 0;
        } 
#endif
s
        struct j2string_st * info = J2_STRING_INFO(string);

#ifndef J2_REMOVE_CHECKS
        if(!info){
                errno = EINVAL;
                return 0;
        }
#endif

        if(c == '\0') return 0;

        long i, counter;
        for(i = counter = 0; string[i] != '\0' && i < info->filled; i++) {
                if(string[i] == c) counter++;
        }
        return counter;

}


long
j2string_spn(
        const_restrict_j2string str,
        const_restrict_j2string accept )
{

#ifndef J2_REMOVE_CHECKS
        if(!str || !accept) {
                errno = EINVAL;
                return 0;
        }
#endif

        struct j2string_st * strinfo = J2_STRING_INFO(str);
        struct j2string_st * acceptinfo = J2_STRING_INFO(accept);

#ifndef J2_REMOVE_CHECKS
        if(!strinfo || !acceptinfo){
                errno = EINVAL;
                return 0;
        }
#endif

        const unsigned char *s   = (const unsigned char *) str;
        const unsigned char *acc = (const unsigned char *) accept;

        /// Maximum amount of characters allowed to be scanned.

        char table[256] = {0};
        for(int i = 0; acc[i] != '\0' && i < acceptinfo->filled; i++)
                table[acc[i]] = 1;
        int l;
        for(l = 0; s[l] != '\0' && l < strinfo->filled; l++)
                if(table[s[l]] != 1) break;

        return l;
}

#endif /** @c J2_STRING_C_INCLUDE */
