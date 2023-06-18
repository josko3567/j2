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
        for(size_t i = 0; acc[i] != '\0' && i < acceptinfo->filled; i++)
                table[acc[i]] = 1;
        size_t l;
        for(l = 0; s[l] != '\0' && l < strinfo->filled; l++)
                if(table[s[l]] != 1) break;

        return l;
}