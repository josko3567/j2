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
#include "../../../../viwerr/viwerr.h"
#endif

/*******************************************************************************
 * @fn @c j2string_ccpy(2)
 * 
 *      @brief Copy @c "src" into @c "(*dst)" whilst being careful we do not 
 *      leave half baked UTF8 characters on the end of the string.
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
 *                         If @c (*dst) is not a valid j2string type.
 * 
 *      @return Amount of UTF8 characters written into @c (*dst).
 * 
 */
long
j2string_ccpy(
        restrict_j2string *   dst, //[in, out]
        const char * __restrict src  //[in] 
        )
{

#ifdef __J2_DEV__
        if(!src || !dst){
                errno = EINVAL;
                viwerr(VIWERR_PUSH, &(viwerr_package){
                        .code    = EINVAL,
                        .group   = (char*)"j2",
                        .name    = (char*)"j2err: Invalid parameter...",
                        .message = (char*)"j2string_ccpy() / j2.string.ccpy()"
                                   " parameter dst/src is a NULL ptr."
                });
                return 0;

        } else if(!(*dst)) {

                errno = EINVAL;
                viwerr(VIWERR_PUSH, &(viwerr_package){
                        .code    = EINVAL,
                        .group   = (char*)"j2",
                        .name    = (char*)"j2err: Invalid parameter...",
                        .message = (char*)"j2string_ccpy() / j2.string.ccpy()"
                                   " parameter dst is a pointer to a NULL ptr."
                });
                return 0;

        }
#endif

        struct j2string_st * dstinfo = J2_STRING_INFO((*dst));

#ifdef __J2_DEV__
        if(!dstinfo){
                errno = EINVAL;
                viwerr(VIWERR_PUSH, &(viwerr_package){
                        .code    = EINVAL,
                        .group   = (char*)"j2",
                        .name    = (char*)"j2err: Invalid parameter...",
                        .message = (char*)"j2string_cat() / j2.string.cat()"
                                   " j2string type parameter \"dst\""
                                   " contains invalid j2string info."
                });
                return 0;
        }
#endif

        size_t size = j2string_strlen_sn(src, dstinfo->max-1)+1;

        (*dst) = j2string_update((*dst), (long)size);
        if((*dst) == NULL) return 0;

        dstinfo = J2_STRING_INFO((*dst));

        dstinfo->length = j2string_utf8_strcpy_sn(
                (*dst), src, 
                dstinfo->allocated,
                &dstinfo->filled
        );

        return dstinfo->length;

}

/*******************************************************************************
 * @fn @c j2string_noutf8_ccpy(2)
 * 
 *      @brief Copy @c "src" into @c "(*dst)".
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
 *                         If @c (*dst) is not a valid j2string type.
 * 
 *      @return Amount of bytes written into @c (*dst).
 * 
 */
long
j2string_noutf8_ccpy(
        restrict_j2string *   dst, //[in, out]
        const char * __restrict src  //[in] 
        )
{

#ifdef __J2_DEV__
        if(!src || !dst){
                errno = EINVAL;
                viwerr(VIWERR_PUSH, &(viwerr_package){
                        .code    = EINVAL,
                        .group   = (char*)"j2",
                        .name    = (char*)"j2err: Invalid parameter...",
                        .message = (char*)"j2string_ccpy() / j2.string.ccpy()"
                                   " parameter dst/src is a NULL ptr."
                });
                return 0;

        } else if(!(*dst)) {

                errno = EINVAL;
                viwerr(VIWERR_PUSH, &(viwerr_package){
                        .code    = EINVAL,
                        .group   = (char*)"j2",
                        .name    = (char*)"j2err: Invalid parameter...",
                        .message = (char*)"j2string_ccpy() / j2.string.ccpy()"
                                   " parameter dst is a pointer to a NULL ptr."
                });
                return 0;

        }
#endif

        struct j2string_st * dstinfo = J2_STRING_INFO((*dst));

#ifdef __J2_DEV__
        if(!dstinfo){
                errno = EINVAL;
                viwerr(VIWERR_PUSH, &(viwerr_package){
                        .code    = EINVAL,
                        .group   = (char*)"j2",
                        .name    = (char*)"j2err: Invalid parameter...",
                        .message = (char*)"j2string_cat() / j2.string.cat()"
                                   " j2string type parameter \"dst\""
                                   " contains invalid j2string info."
                });
                return 0;
        }
#endif

        size_t size = j2string_strlen_sn(src, dstinfo->max-1)+1;

        (*dst) = j2string_update((*dst), (long)size);
        if((*dst) == NULL) return 0;

        dstinfo = J2_STRING_INFO((*dst));

        dstinfo->length = j2string_utf8_nhbc_strcpy_sn(
                (*dst), src, 
                dstinfo->allocated,
                &dstinfo->filled
        );

        return dstinfo->filled;

}