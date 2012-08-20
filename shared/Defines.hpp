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


#include <boost/asio.hpp>
#include <boost/shared_ptr.hpp>

#ifndef NOT_AVAILABLE_SHARED_PTR
    typedef boost::shared_ptr<Player> PlayerPtr;
    typedef boost::shared_ptr<Unit> UnitPtr;
    typedef boost::shared_ptr<Creature> CreaturePtr;
    typedef boost::shared_ptr<WorldObject> WorldObjectPtr;
    typedef boost::shared_ptr<Map> MapPtr;
    typedef boost::shared_ptr<Spell> SpellPtr;
#else
    typedef std::shared_ptr<Player> PlayerPtr;
    typedef std::shared_ptr<Unit> UnitPtr;
    typedef std::shared_ptr<Creature> CreaturePtr;
    typedef std::shared_ptr<WorldObject> WorldObjectPtr;
    typedef std::shared_ptr<Map> MapPtr;
    typedef std::shared_ptr<Spell> SpellPtr;
#endif

typedef boost::asio::ip::tcp::socket Socket;
typedef boost::shared_ptr<Socket> SocketPtr;

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
    SPELL_BOLT = 0
};

#endif
