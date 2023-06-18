/*** LICENSE: LGPLV2. *************************************
 *                                                        *
 *                                                        *
 * j2.h is a library for implementing abstract data       *
 * types.                                                 *
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
 * 3.12.2022.                                             *
 *                                                        *
 *                                                        *
 *********************************************************/
                  
/*** INTRODUCTION: ****************************************
 *                                                        *
 *                                                        *
 * j2.h is a library contaning a small set of abstract    *
 * data types not present in any of the standard          *
 * libraries.                                             *
 *                                                        *
 *                                                        *
 * The following list will inscribe all data types        *
 * implemented by the author that are protected from      *
 * having their syntax and algorthim be further modified  *
 * upon the public release of this library:               *
 *                                                        *
 *                                                        *
 *  - strings,                                            *
 *  - linked lists,                                       *
 *  - stacks,
 *  - queues,
 *  - doubly linked lists,                                *
 *  - hash tables &                                       *
 *  - plett lists.                                        *
 *                                                        *
 *                                                        *
 * A exception to the "protection" rule is                *
 * in the times of need to fix major bugs that can't      *
 * avoid some improved syntax/algorithm modification.     *
 *                                                        *
 *                                                        *
 * Algorthims can also be further modified for improved   *
 * run times, but these faster run time changes cannot    *
 * modifiy the input/output of the algorithim. For such   *
 * modifications the only option is to add the renewed    *
 * alogrithim to a new function instead.                  *
 *                                                        *
 *                                                        *
 * Other data types may be contained within this library  *
 * but their protection from future syntax and algorithim *
 * changes are not guaranteed due to them not being       *
 * listed inside the list of protected data types.        *
 *                                                        *
 *                                                        *
 * This library is meant to be reused for newer releases  *
 * of the jdt# abstract data type libraries so that the   *
 * author may change important details around the syntax  *
 * and algorithims that the current protected data types  *
 * use. In addition to the changes on every new release   *
 * there will be (with absolutley no guarantee) new       *
 * abstract data types added to the protected list and    *
 * general improvments to the library.                    *
 *                                                        *
 *                                                        *
 * Upon every new release of a jdt# library the version   *
 * whom the new release is a successor will be deemed     *
 * depricated. Author support of the release will not     *
 * cease but will certaintly not be any faster than it    *
 * was. At some point in time when enough versions get    *
 * released support from the author will cease to exist   *
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
 * -> j2...                                               *
 * -> J2...                                               *
 *                                                        *
 *                                                        *
 *********************************************************/

/*** INSCRIBING: EXTERNAL FILES CALLED & PURPOSE **********
 *                                                        *
 *                                                        *
 * Name                     Description                   *
 * ----                     -----------                   *
 * lib/string/string.h      j2_string type and func.      *
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
 *                                                        *
 *                                                        *
 *********************************************************/

#include "lib/string/string.h"
#include <stdlib.h>
#include <assert.h>
#include <assert.h>

#ifdef __J2_DEV__
#include "ext/viwerr/viwerr.h"
#endif


#ifndef J2_INCLUDE
#define J2_INCLUDE     

#define J2_VIWERR_GROUP "j2.h"

struct j2_interface{

        struct {
                j2string (*init)(
                        const char * cstr, 
                        j2string_param parameters
                );
                struct {
                      j2string (*st)( 
                                const char * cstr);  
                      j2string (*dy)( 
                                const char * cstr);  
                } new;
                // long (*rchrs)(
                //         const_j2string string,
                //         unsigned char c,
                //         long position
                // );
                // long (*chrs)(
                //         const_j2string string,
                //         unsigned char c,
                //         long position
                // );
                // long (*cnt)(
                //         const_j2string string,
                //         unsigned char c
                // );
                long (*ccpy)(
                        restrict_j2string* destination,
                        const char * __restrict source
                );
                long (*cpy)(
                        restrict_j2string* destination,
                        const_restrict_j2string source
                );
                long (*ccat)(
                        restrict_j2string* destination,
                        const char * __restrict source
                );
                long (*cat)(
                        restrict_j2string* destination,
                        const_restrict_j2string source
                );
                long (*cmp)(
                        const_restrict_j2string str1, 
                        const_restrict_j2string str2
                );
                struct {
                        
                        long (*ccpy)(
                                restrict_j2string* destination,
                                const char * __restrict source
                        );

                        long (*cpy)(
                                restrict_j2string* destination,
                                const_restrict_j2string source
                        );
                        long (*ccat)(
                                restrict_j2string* destination,
                                const char * __restrict source
                        );
                        long (*cat)(
                                restrict_j2string* destination,
                                const_restrict_j2string source
                        );

                } noutf8;

                // long (*cspn)(
                //         const_restrict_j2string string,
                //         const_restrict_j2string reject
                // );
                // long (*spn)(
                //         const_restrict_j2string string,
                //         const_restrict_j2string accept
                // );
                // long (*len)(
                //         const char * string
                // );
                // size_t (*clen)(
                //         const char * string,
                //         size_t max
                // );
        } string;
};

#ifdef __GNUC__
__attribute__((constructor))
#endif
struct j2_interface *
_j2()
{
        static struct j2_interface * interface = NULL;
        if(interface == NULL) {
                interface = malloc(sizeof(struct j2_interface));
                
                /// @a String_setup:
                interface->string.init   = &j2string_init;
                interface->string.new.st = &j2string_new_st;
                interface->string.new.dy = &j2string_new_dy;

                interface->string.ccpy   = &j2string_ccpy;
                interface->string.cpy    = &j2string_cpy;

                interface->string.cat    = &j2string_cat;
                interface->string.ccat   = &j2string_ccat;

                // interface->string.chrs   = &j2string_chrs;
                // interface->string.rchrs  = &j2string_rchrs;

                // interface->string.cnt    = &j2string_cnt;
                interface->string.cmp    = &j2string_cmp;

                // interface->string.cspn   = &j2string_cspn;
                // interface->string.spn    = &j2string_spn;
                
                // interface->string.len    = &j2string_len;
                // interface->string.clen   = &j2string_strlen_sn;


                /// @a Noutf8_setup:
                interface->string.noutf8.ccpy   = &j2string_noutf8_ccpy;
                interface->string.noutf8.cpy    = &j2string_noutf8_cpy;
                interface->string.noutf8.cat    = &j2string_noutf8_cat;
                interface->string.noutf8.ccat   = &j2string_noutf8_ccat;

        }
        assert(interface != NULL);
        return interface;
}

#ifdef __GNUC__
__attribute__((destructor))
#endif
void
_j2_free(){
        free(_j2());
}

#define j2 (*_j2())


#endif /** @c J2_INCLUDE */
