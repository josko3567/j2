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
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#ifdef __J2_DEV__
#include "../../../ext/viwerr/viwerr.h"
#endif

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

        struct j2string_st * memory = (struct j2string_st *)malloc(
                J2_STRING_GET_MEMORY_SIZE(parameters.allocate)
        );

        if(!memory) 
                return NULL;

        memory->padding   = parameters.padding;
        memory->allocated = parameters.allocate;
        memory->max       = parameters.max;
        memory->filled    = 0;
        memory->length    = 0;
        memory->ensure    = J2_STRING_ENSURE_HEXSPEAK;
        J2_STRING_REACH_FOR_STRING(memory)[memory->allocated] = '\0';

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

#ifdef __J2_DEV__
        if(string == NULL) 
                return;
        else if(J2_STRING_INFO(string) == NULL) 
                return;
#endif

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

#ifdef __J2_DEV__
        if(string == NULL 
        || newsize < 1){

                errno = EINVAL;
                return string;

        } else if( J2_STRING_INFO(string) == NULL){

                errno = EINVAL;
                return string;

        }
#endif

        struct j2string_st * info = J2_STRING_INFO(string);

        /// Calculate the updated size of the string.
        size_t calcsize = J2_STRING_GET_NEW_SIZE(
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
 *      of padding that can fit a amount of bytes described by size 
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

        // if(parameters.padding < 0)                     
        //         parameters.padding = 0;      

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
 *      @brief Initialize a @a j2string as a static or dynamic string.
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
 *                      If initially set to 0 or less, @c parameters.allocate will
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
        const char * cstr,
        j2string_param parameters)
{       

        // Update cstr to hold a string if given NULL.
        cstr = cstr ? cstr : "";

        // Calculate the length of the string in bytes.
        size_t allocated = j2string_strlen_sn(
                cstr, J2_STRING_MAX_SIZE-1
        );

        // Allocate memory for the string.
        j2string string = j2string_allocate(
                j2string_normalize_param(parameters, allocated+1)
        );

        if(!string) return NULL;

        // Fill the string up.
        struct j2string_st * info = J2_STRING_REACH_FOR_STRUCT(string);

        info->length = j2string_utf8_strcpy_sn(
                string, cstr, info->allocated, &info->filled
        );

        return string;
        // return NULL;

}

/**
 * @fn @c j2string_new_st(1)
 * 
 *      @brief 
 *      Initialize a j2string with all j2string_param values set to 0.
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