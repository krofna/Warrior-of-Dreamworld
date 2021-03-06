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
class Player;
class Pathfinder;

class WOD_DLL_DECL WorldObject
{
public:
    WorldObject(uint64 ObjID);
    virtual ~WorldObject() {}

    virtual void Update(int64 diff) = 0;
    virtual WorldPacket PackData() = 0;

    virtual void UpdatePosition(Vector2i const& Position);
    void SendPositionUpdateToMap();
    // Called when some a Player collides with *this
    virtual void OnInteract(Player* pWho) = 0;

    const uint64 GetObjectID() const;
    const uint32 GetMapID() const;
    Map* GetMap() const;
    uint16 GetX() const;
    uint16 GetY() const;
    Vector2i GetPosition() const;

protected:
    Map* pMap;
    Vector2i Position;

    const uint64 ObjID;
};

#endif
