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

#include <SFML/Graphics.hpp>

#define RETURN_IF_PACKET_TOO_BIG if(!Packet.endOfPacket()) { printf("Packet is too big!\n"); return; }

using sf::Uint8;
using sf::Uint16;
using sf::Uint32;
using sf::Uint64;
using sf::Int8;
using sf::Int16;
using sf::Int32;
using sf::Int64;

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
