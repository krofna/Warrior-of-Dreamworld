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

#include <cstdint>

typedef uint8_t uint8;
typedef uint16_t uint16;
typedef uint32_t uint32;
typedef uint64_t uint64;

typedef int8_t int8;
typedef int16_t int16;
typedef int32_t int32;
typedef int64_t int64;

class Player;
class Unit;
class WorldObject;
class Creature;
class Map;
class Spell;
class SpellBox;

#include "Config.hpp"

#ifdef USE_BOOST
        #include <boost/shared_ptr.hpp>
        #include <boost/enable_shared_from_this.hpp>
        typedef boost::shared_ptr<Player> PlayerPtr;
        typedef boost::shared_ptr<Unit> UnitPtr;
        typedef boost::shared_ptr<Creature> CreaturePtr;
        typedef boost::shared_ptr<WorldObject> WorldObjectPtr;
        typedef boost::shared_ptr<Map> MapPtr;
        typedef boost::shared_ptr<Spell> SpellPtr;
        typedef boost::shared_ptr<SpellBox> SpellBoxPtr;
        using boost::enable_shared_from_this;
        using boost::static_pointer_cast;
#else
        #ifndef SMART_PTR_TR1_INCLUDE
            #include <memory>
        #else
            #include <tr1/memory>
        #endif 

        #ifdef SMART_PTR_TR1
            typedef std::tr1::shared_ptr<Player> PlayerPtr;
            typedef std::tr1::shared_ptr<Unit> UnitPtr;
            typedef std::tr1::shared_ptr<Creature> CreaturePtr;
            typedef std::tr1::shared_ptr<WorldObject> WorldObjectPtr;
            typedef std::tr1::shared_ptr<Map> MapPtr;
            typedef std::tr1::shared_ptr<Spell> SpellPtr;
            typedef std::tr1::shared_ptr<SpellBox> SpellBoxPtr;
            using std::tr1::enable_shared_from_this;
            using std::tr1::static_pointer_cast;
        #else
            typedef std::shared_ptr<Player> PlayerPtr;
            typedef std::shared_ptr<Unit> UnitPtr;
            typedef std::shared_ptr<Creature> CreaturePtr;
            typedef std::shared_ptr<WorldObject> WorldObjectPtr;
            typedef std::shared_ptr<Map> MapPtr;
            typedef std::shared_ptr<Spell> SpellPtr;
            typedef std::shared_ptr<SpellBox> SpellBoxPtr;
            using std::enable_shared_from_this;
            using std::static_pointer_cast;
       #endif

#endif

#define TILE_SIZE 32
#define MAP_COUNT 1

enum
{
    MOVE_STOP   = 0,
    MOVE_UP     = 1,
    MOVE_DOWN   = 2,
    MOVE_LEFT   = 4,
    MOVE_RIGHT  = 8
};

enum
{
    SEC_PLAYER    = 0,
    SEC_MOD       = 1,
    SEC_DEV       = 2,
    SEC_ADMIN     = 3,
    SEC_CONSOLE   = 4
};

enum
{
    SPELL_BOLT = 0
};

#endif
