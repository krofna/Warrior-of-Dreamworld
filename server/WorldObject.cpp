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
#include "../shared/Opcodes.hpp"
#include "Map.hpp"

WorldObject::WorldObject(uint32 ObjID) :
ObjID(ObjID)
{
}

const uint16 WorldObject::GetMapID() const
{
    return pMap->MapID;
}

bool WorldObject::UpdateCoordinates(uint8 Direction)
{
    uint16 OldX = Position.x, OldY = Position.y;

    switch(Direction)
    {
    case MOVE_UP:
        if(pMap->TileGrid[Position.y-1][Position.x])
            return false;
        Position.y--;
        break;
    case MOVE_DOWN:
        if(pMap->TileGrid[Position.y+1][Position.x])
            return false;
        Position.y++;
        break;
    case MOVE_LEFT:
        if(pMap->TileGrid[Position.y][Position.x-1])
            return false;
        Position.x--;
        break;
    case MOVE_RIGHT:
        if(pMap->TileGrid[Position.y][Position.x+1])
            return false;
        Position.x++;
        break;        
    }

    pMap->TileGrid[OldY][OldX] = nullptr;
    //pMap->TileGrid[Position.y][Position.x] = this;

    return true;
}

void WorldObject::UpdatePosition(sf::Vector2i Position)
{
    this->Position = Position;
    sf::Packet Packet;
    Packet << (uint16)MSG_MOVE_OBJECT << ObjID << GetX() << GetY();
    pMap->SendToPlayers(Packet);
}