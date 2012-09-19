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

#include "shared/WorldPacket.hpp"
#include "shared/Vector2.hpp"

class Map;
class Pathfinder;

class WOD_DLL_DECL WorldObject
{
    friend class Pathfinder;
public:
    WorldObject(uint64 ObjID);
    virtual ~WorldObject() {}

    virtual void Update(int64 diff) = 0;
    virtual WorldPacket PackData() = 0;

    bool UpdateCoordinates(uint8 Direction);
    void UpdatePosition(Vector2i Position);

    const uint64 GetObjectID() const
    {
        return ObjID;
    }

    const uint16 GetMapID() const;

    Map* GetMap() const
    {
        return pMap;
    }

    uint16 GetX() const
    {
        return Position.x;
    }

    uint16 GetY() const
    {
        return Position.y;
    }

    Vector2i GetPosition() const
    {
        return Position;
    }

protected:
    Map* pMap;
    Vector2i Position;

    const uint64 ObjID;
};

#endif
