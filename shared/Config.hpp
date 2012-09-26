/*
Warrior of Dreamworld, 2D Massivly Mutiplayer Online Role-playing Game
Copyright (C) 2012  Mislav Blazevic

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*/
#ifndef CONFIG_H
#define CONFIG_H

/** ************************ Windows stuff ************************ **/
    #if defined(WIN32) || defined(_WIN32)
    #define WOD_DLL_DECL __declspec(dllexport)

/** ************************ Microsoft Visual Studio C++ stuff ************************ **/
    #if defined(_MSC_VER)
        #define MSVC
        #define secure_vsnprintf vsnprintf_s
        #pragma warning(disable : 4251)

        #if(_MSC_VER == 1500)
            #define SMART_PTR_TR1
        #else
            #define USE_BOOST
        #endif
    #endif

/** ************************ GNU GCC Compiler stuff ************************ **/
    #elif defined( __GNUC__ )
        #define GPROF
        #define WOD_DLL_DECL __attribute__((__visibility__("default"))) // FIXME: Check that !

        #if defined(__VARIADIC_TEMPLATES) || (__GNUC__ > 4) || ((__GNUC__ == 4) && (__GNUC_MINOR__ >= 4) && defined(__GXX_EXPERIMENTAL_CXX0X__))
            #define HAVE_VARIADIC_TEMPLATES
            #include "Utils.hpp"
        #else
            #define secure_vsnprintf vsprintf // FIXME: Implement a secure vsprintf for replace all others "secure_vsprintf"
        #endif

        #if ((__GNUC_ == 4) &&(__GNUC_MINOR__ >= 1 && __GNU_MINOR < 3))
            #define SMART_PTR_TR1_INCLUDE
        #elif (__GNUC__ > 4 || (__GNUC__ == 4) && (__GNUC_MINOR >= 3))
            #define SMART_PTR
        #else
            #define USE_BOOST
        #endif
    #else
        #define WOD_DLL_DECL export
    #endif

#endif // CONFIG_H
