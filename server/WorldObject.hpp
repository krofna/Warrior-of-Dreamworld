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

#include "../client/Defines.hpp"
#include <SFML/Network.hpp>

struct Map;

class WorldObject
{
public:
    WorldObject(Uint32 ObjID);
    virtual ~WorldObject() {}

    virtual void Update(Int32 diff) = 0;
    virtual sf::Packet PackData() = 0;
    virtual void SaveToDB() = 0;

    bool UpdateCoordinates(Uint8 Direction);

    const Uint32 GetObjectID() const
    {
        return ObjID;
    }

    const Uint16 GetMapID() const;

    Map* GetMap() const
    {
        return pMap;
    }

    Uint16 GetX() const
    {
        return x;
    }

    Uint16 GetY() const
    {
        return y;
    }

protected:
    std::string Tileset;

    Map* pMap;
    Uint16 x;
    Uint16 y;
    Uint16 tx;
    Uint16 ty;

    const Uint32 ObjID;
};

#endif