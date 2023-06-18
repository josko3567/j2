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

        size_t i;
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
