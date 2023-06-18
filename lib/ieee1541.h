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
 * Standard C does not contain any defined sizes from     *
 * the IEEE 1541 (MiB, KiB, iB, ...) therefore this small *
 * library provides such macros to help you better        *
 * understand the code you make in the future.            *
 *                                                        *
 *                                                        *
 *********************************************************/

/*** INSCRIBING: FILE, AUTHOR & DATE INFORMATION. *********
 *                                                        *
 *                                                        *
 * FILE:                                                  *
 * Goddamit why are you still reading just use the damn   *
 * code. I know it's funny to place a 100 page            *
 * description for 10 lines of code but you don't need    *
 * to read it unless you are menataly inhibited, in which *
 * case this will still not help you understand it any    *
 * better since I can't write for shit.                   *
 *                                                        *
 *                                                        *
 * AUTHOR:                                                *
 *      NAME:  Joško Križanović <github.com>              *
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
 * functionality at the disposal of the C89 standard.     *
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
 * -> _IEEE1541...                                        *
 *                                                        *
 *                                                        *
 *********************************************************/

/*** INSCRIBING: EXTERNAL FILES CALLED & PURPOSE **********
 *                                                        *
 *                                                        *
 * Name                     Description                   *
 * ----                     -----------                   *
 * <stddef.h>               size_t                        *
 *                                                        *
 *                                                        *
 *********************************************************/

/*** INSCRIBING: DEVELOPMENT HISTORY/VERSIONS PUBLISHED ***
 *                                                        *
 *                                                        *
 * Author  Release & Date  Description of modifications   *
 * ------  --------------  ----------------------------   *
 * josko3  V1.0            Inital creation.               *
 * 567     6.12.2022                                      *
 * ------  --------------  ----------------------------   *
 *                                                        *
 *                                                        *
 *********************************************************/

#include <stddef.h>

#ifndef J2_IEEE1541_INCLUDE
#define J2_IEEE1541_INCLUDE

#ifdef __cplusplus
extern "C" {
#endif

/// Works with floats.
#define _IEEE1541_B(  amount) (size_t)(amount)
#define _IEEE1541_KiB(amount) (size_t)(amount*1024)
#define _IEEE1541_MiB(amount) (size_t)(amount*1048576)
#define _IEEE1541_GiB(amount) (size_t)(amount*1073741824)
#define _IEEE1541_TiB(amount) (size_t)(amount*1099511627776)
#define _IEEE1541_PiB(amount) (size_t)(amount*1125899906842624)
#define _IEEE1541_EiB(amount) (size_t)(amount*1152921504606846976)

///Quick conversion for integer values
#define _IEEE1541_INT_B(  amount) (size_t)amount
#define _IEEE1541_INT_KiB(amount) (size_t)(amount<<10)
#define _IEEE1541_INT_MiB(amount) (size_t)(amount<<20)
#define _IEEE1541_INT_GiB(amount) (size_t)(amount<<30)
#define _IEEE1541_INT_TiB(amount) (size_t)(amount<<40)
#define _IEEE1541_INT_PiB(amount) (size_t)(amount<<50)
#define _IEEE1541_INT_EiB(amount) (size_t)(amount<<60)

#ifdef __cplusplus
}
#endif

#endif /** @c J2_IEEE1541_INCLUDE */
