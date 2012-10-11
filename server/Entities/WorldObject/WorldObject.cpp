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
#include "WorldObject.hpp"
#include "shared/Opcodes.hpp"
#include "Map.hpp"

WorldObject::WorldObject(uint64 ObjID) :
ObjID                   (ObjID)
{
}

const uint32 WorldObject::GetMapID() const
{
    return pMap->GetID();
}

const uint64 WorldObject::GetObjectID() const
{
    return ObjID;
}

Map* WorldObject::GetMap() const
{
    return pMap;
}

uint16 WorldObject::GetX() const
{
    return Position.x;
}

uint16 WorldObject::GetY() const
{
    return Position.y;
}

Vector2i WorldObject::GetPosition() const
{
    return Position;
}

void WorldObject::UpdatePosition(Vector2i const& Position)
{
    this->Position = Position;
    WorldPacket Packet((uint16)MSG_MOVE_OBJECT);
    Packet << ObjID << GetX() << GetY();
    pMap->SendToPlayers(Packet);
}
