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
#ifndef DEFINES_H
#define DEFINES_H

struct MapDeleter
{
    template<typename Type>
    void operator() (Type toDelete)
    {
        delete toDelete.second;
    }
};

struct Deleter
{
    template<typename Type>
    void operator() (Type toDelete)
    {
        delete toDelete;
    }
};

#include <cstdint>

typedef uint8_t uint8;
typedef uint16_t uint16;
typedef uint32_t uint32;
typedef uint64_t uint64;

typedef int8_t int8;
typedef int16_t int16;
typedef int32_t int32;
typedef int64_t int64;

class WorldSession;

#include "Config.hpp"

#ifdef USE_BOOST
        #include <boost/shared_ptr.hpp>
        #include <boost/array.hpp>
        #include <boost/enable_shared_from_this.hpp>
        #include <boost/make_shared.hpp>
        
        typedef boost::shared_ptr<WorldSession> WorldSessionPtr;
        using boost::enable_shared_from_this;
        using boost::make_shared;
        using boost::array;

        #define static_template_array boost::array
#else
        #ifndef SMART_PTR_TR1_INCLUDE
            #include <memory>
            #include <array>
        #else
            #include <tr1/memory>
        #endif 

        #ifdef SMART_PTR_TR1
            typedef std::tr1::shared_ptr<WorldSession> WorldSessionPtr;
            using std::tr1::enable_shared_from_this;
            using std::tr1::make_shared;
            using std::tr1::array;

            #define static_template_array std::tr1::array;
        #else
            typedef std::shared_ptr<WorldSession> WorldSessionPtr;
            using std::enable_shared_from_this;
            using std::make_shared;
            using std::array;

            #define static_template_array std::array
       #endif

#endif

#define TILE_SIZE 32

enum
{
    MOVE_STOP   = 0,
    MOVE_UP     = 1,
    MOVE_DOWN   = 2,
    MOVE_LEFT   = 4,
    MOVE_RIGHT  = 8
};

#endif
