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

#ifdef __J2_DEV__
        if(!str || !reject) {
                errno = EINVAL;
                return 0;
        }
#endif

        struct j2string_st * strinfo = J2_STRING_INFO(str);
        struct j2string_st * rejectinfo = J2_STRING_INFO(reject);

#ifdef __J2_DEV__
        if(!strinfo || !rejectinfo){
                errno = EINVAL;
                return 0;
        }
#endif

        const unsigned char *s   = (const unsigned char *) str;
        const unsigned char *rej = (const unsigned char *) reject;

        /// Maximum amount of characters allowed to be scanned.

        char table[256] = {0};
        for(size_t i = 0; rej[i] != '\0' && i < rejectinfo->filled; i++)
                table[rej[i]] = 1;
        size_t l;
        for(l = 0; s[l] != '\0' && l < strinfo->filled; l++)
                if(table[s[l]] == 1) break;
        return l;

}