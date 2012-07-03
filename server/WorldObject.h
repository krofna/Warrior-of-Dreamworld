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
#ifndef WORLD_OBJECT_H
#define WORLD_OBJECT_H

#include "../client/Defines.h"
#include <SFML/Network.hpp>

struct Map;

class WorldObject
{
    const Uint32 ObjID;
public:
    WorldObject(std::string Tileset, Map* pMap, Uint32 ObjID, Uint16 x, Uint16 y, Uint16 tx, Uint16 ty);

    virtual void Update(Int32 diff) = 0;
    virtual sf::Packet PackData() = 0;

    void UpdateCoordinates(Uint8 Direction);

    Uint32 GetObjectID() const
    {
        return ObjID;
    }

    Uint16 GetMapID() const;

    Map* GetMap()
    {
        return pMap;
    }

protected:
    std::string Tileset;

    Map* pMap;
    Uint16 x;
    Uint16 y;
    Uint16 tx;
    Uint16 ty;
};

#endif