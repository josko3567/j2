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

#ifdef __J2_DEV__
        if(!str1 || !str2) {
                errno = EINVAL;
                return 0;
        }
#endif

        struct j2string_st * info1 = J2_STRING_INFO(str1);
        struct j2string_st * info2 = J2_STRING_INFO(str2);

#ifdef __J2_DEV__
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