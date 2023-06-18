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

/*******************************************************************************
 * @fn @c j2string_ccat(2)
 * 
 *      @brief Concatenate @c "src" onto the end of @c "(*dst)"
 *      whilst being careful we do not leave half baked UTF8 characters
 *      on the end of the string.
 * 
 *     
 *      @param @b dst
 *             j2string type string to concatenate to.
 * 
 *      @param @b src
 *             C type string that will be concatenated.
 * 
 *      
 *      @throw @a EINVAL - If @c dst and/or @c src and/or @c (*dst) are NULL 
 *                         pointers,
 *                         If @c (*dst) is not a valid j2string type.
 * 
 *      @returns The number of UTF8 characters concatenated to @c "(*dst)".
 * 
 */
long
j2string_ccat(
        restrict_j2string * dst,
        const char * __restrict src)
{

#ifdef __J2_DEV__
        if(!dst || !src) {
                errno = EINVAL;
                viwerr(VIWERR_PUSH, &(viwerr_package){
                        .code    = EINVAL,
                        .group   = (char*)"j2",
                        .name    = (char*)"j2err: Invalid parameter...",
                        .message = (char*)"j2string_ccat() / j2.string.ccat()"
                                   " parameter dst/src is a NULL ptr."
                });
                return 0;
        }
        if(!(*dst)) {
                errno = EINVAL;
                viwerr(VIWERR_PUSH, &(viwerr_package){
                        .code    = EINVAL,
                        .group   = (char*)"j2",
                        .name    = (char*)"j2err: Invalid parameter...",
                        .message = (char*)"j2string_ccat() / j2.string.ccat()"
                                   " parameter dst is a pointer to a NULL ptr."
                });
                return 0;
        }
#endif

        struct j2string_st * infodst = J2_STRING_INFO((*dst));

#ifdef __J2_DEV__
        if(!infodst){
                errno = EINVAL;
                viwerr(VIWERR_PUSH, &(viwerr_package){
                        .code    = EINVAL,
                        .group   = (char*)"j2",
                        .name    = (char*)"j2err: Invalid parameter...",
                        .message = (char*)"j2string_ccat() / j2.string.ccat()"
                                   " j2string type parameters \"dst\" & \"src\""
                                   " both contain invalid j2string info."
                });
                return 0;
        }
#endif

        size_t filled = j2string_strlen_sn(src, J2_STRING_MAX_SIZE);

        (*dst) = j2string_update((*dst), 
                filled + infodst->filled + 1);
        if(!(*dst)) return 0;

        infodst = J2_STRING_INFO((*dst));

        size_t written = 0;
        size_t length = j2string_utf8_strcpy_sn(
                (*dst)+infodst->filled, src, 
                infodst->allocated - infodst->filled,
                &written
        );

        infodst->filled += written;
        infodst->length += length;

        return (long)length;

}


/*******************************************************************************
 * @fn @c j2string_noutf8_ccat(2)
 * 
 *      @brief Concatenate @c "src" onto the end of @c "(*dst)".
 * 
 *     
 *      @param @b dst
 *             j2string type string to concatenate to.
 * 
 *      @param @b src
 *             C type string that will be concatenated.
 * 
 *      
 *      @throw @a EINVAL - If @c dst and/or @c src and/or @c (*dst) are NULL 
 *                         pointers,
 *                         If @c (*dst) is not a valid j2string type.
 * 
 *      @returns The number of bytes concatenated to @c "(*dst)".
 * 
 */
long
j2string_noutf8_ccat(
        restrict_j2string * dst,
        const char * __restrict src)
{

#ifdef __J2_DEV__
        if(!dst || !src) {
                errno = EINVAL;
                viwerr(VIWERR_PUSH, &(viwerr_package){
                        .code    = EINVAL,
                        .group   = (char*)"j2",
                        .name    = (char*)"j2err: Invalid parameter...",
                        .message = (char*)"j2string_noutf8_ccat() /"
                                   " j2.string.noutf8.ccat()"
                                   " parameter dst/src is a NULL ptr."
                });
                return 0;
        }
        if(!(*dst)) {
                errno = EINVAL;
                viwerr(VIWERR_PUSH, &(viwerr_package){
                        .code    = EINVAL,
                        .group   = (char*)"j2",
                        .name    = (char*)"j2err: Invalid parameter...",
                        .message = (char*)"j2string_noutf8_ccat() /"
                                   " j2.string.noutf8.ccat()"
                                   " parameter dst is a pointer to a NULL ptr."
                });
                return 0;
        }
#endif

        struct j2string_st * infodst = J2_STRING_INFO((*dst));

#ifdef __J2_DEV__
        if(!infodst){
                errno = EINVAL;
                viwerr(VIWERR_PUSH, &(viwerr_package){
                        .code    = EINVAL,
                        .group   = (char*)"j2",
                        .name    = (char*)"j2err: Invalid parameter...",
                        .message = (char*)"j2string_noutf8_ccat() /"
                                   " j2.string.noutf8.ccat()"
                                   " j2string type parameters \"dst\" & \"src\""
                                   " both contain invalid j2string info."
                });
                return 0;
        }
#endif

        size_t filled = j2string_strlen_sn(src, J2_STRING_MAX_SIZE);

        (*dst) = j2string_update((*dst), 
                filled + infodst->filled + 1);
        if(!(*dst)) return 0;

        infodst = J2_STRING_INFO((*dst));

        size_t written = 0;
        size_t length = j2string_utf8_nhbc_strcpy_sn(
                (*dst)+infodst->filled, src, 
                infodst->allocated - infodst->filled,
                &written
        );

        infodst->filled += written;
        infodst->length += length;

        return (long)written;
}