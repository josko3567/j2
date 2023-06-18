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
 * bitflag.h is a simple library of macros to write, read *
 * and check binary flags inside a variable.              *
 *                                                        *
 *                                                        *
 *********************************************************/

/*** INSCRIBING: FILE, AUTHOR & DATE INFORMATION. *********
 *                                                        *
 *                                                        *
 * FILE:                                                  *
 * Implementation.                                        *
 *                                                        *
 *                                                        *
 * AUTHOR:                                                *
 *      NAME:  Joško Križanović                           *
 *      EMAIL: jkriza02@fesb.com                          *
 *                                                        *
 *                                                        *
 * DATE:                                                  *
 * 19.9.2022                                              *
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
 * -> _BITFLAG...                                         *
 *                                                        *
 *                                                        *
 *********************************************************/

/*** INSCRIBING: EXTERNAL FILES CALLED & PURPOSE **********
 *                                                        *
 *                                                        *
 * Name                     Description                   *
 * ----                     -----------                   *
 * None.                                                  *
 *                                                        *
 *                                                        *
 *********************************************************/

/*** INSCRIBING: DEVELOPMENT HISTORY/VERSIONS PUBLISHED ***
 *                                                        *
 *                                                        *
 * Author  Release & Date  Description of modifications   *
 * ------  --------------  ----------------------------   *
 * josko3  V0.0            Inital creation.               *
 * 567     19.9.2022                                      *
 * ------  --------------  ----------------------------   *
 *                                                        *
 *                                                        *
 *********************************************************/

#ifndef J2_BITFLAG_INCLUDE
#define J2_BITFLAG_INCLUDE

/**
 * @brief 
 * Set bits to 1 inside var from mask.
 * @example
 * int a = 0b01010000
 * j2_bitflag_add( a, 0b00001111 ) -> a = 0b01011111
 */
#define j2_bitflag_add( var, mask ) \
        (var |= mask)

/**
 * @brief 
 * Set bits to 0 inside of var if they are 1 inside mask.
 * @example
 * int a = 0b01010111
 * j2_bitflag_remove( a, 0b00001111 ) -> a = 0b01010000
 */
#define j2_bitflag_remove( var, mask ) \
        (var &= ~(mask ^ ((var ^ mask)&(~var & mask))))

/**
 * @brief 
 * Check if var has all bits set to 1 that correspond with the bits
 * inside mask.
 */
#define j2_bitflag_check( var, mask ) \
        ((var & mask) == mask ? 1 : 0)

#endif /** @c J2_BITFLAG_INCLUDE */
