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

  #if defined(WIN32) || defined(_WIN32)
    #define WOD_DLL_DECL __declspec(dllexport)

    #ifdef _MSC_VER
      #pragma warning(disable : 4251)
    #endif

  #elif defined( __GNUC__ )
    #define WOD_DLL_DECL

  #else
    #define WOD_DLL_DECL export
  #endif
  
#if defined(__VARIADIC_TEMPLATES) || (__GNUC__ > 4) || ((__GNUC__ == 4) && (__GNUC_MINOR__ >= 4) && defined(__GXX_EXPERIMENTAL_CXX0X__))
  #define HAVE_VARIADIC_TEMPLATES
#endif

#endif