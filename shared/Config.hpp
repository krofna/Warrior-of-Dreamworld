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
    #endif

/** ************************ GNU GCC Compiler stuff ************************ **/
    #elif defined( __GNUC__ )
        #define WOD_DLL_DECL __attribute__((__visibility__("default"))) // FIXME: Check that !
    #else
        #define WOD_DLL_DECL export
    #endif

/** ************************ Generic stuff (Thanks to Boost.Config) ************************ **/
    #ifndef BOOST_NO_VARIADIC_TEMPLATES
        #define HAVE_VARIADIC_TEMPLATES
        #include "Utils.hpp"
    #endif

    #ifdef BOOST_NO_NULLPTR
        #define nullptr NULL
    #endif
    
    // Boost.TR1 autorecognize Standard TR1 and use it. (ref: http://www.boost.org/doc/libs/1_51_0/doc/html/boost_tr1/config.html)a
    #define BOOST_HAS_TR1

#endif // CONFIG_H
