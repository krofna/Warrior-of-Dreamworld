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

#include "../shared/Defines.hpp"
#include "../shared/Config.hpp"
#include <SFML/Network.hpp>

struct Map;
class Pathfinder;

class WOD_DLL_DECL WorldObject
{
    friend class Pathfinder;
public:
    WorldObject(Uint32 ObjID);
    virtual ~WorldObject() {}

    virtual void Update(Int32 diff) = 0;
    virtual sf::Packet PackData() = 0;

    bool UpdateCoordinates(Uint8 Direction);
    void UpdatePosition(sf::Vector2i Position);

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
        return Position.x;
    }

    Uint16 GetY() const
    {
        return Position.y;
    }

    sf::Vector2i& GetPosition()
    {
        return Position;
    }

protected:
    Map* pMap;
    sf::Vector2i Position;

    const Uint32 ObjID;
};

#endif
