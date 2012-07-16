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
#include "WorldObject.h"
#include "Opcodes.h"
#include "Map.h"

WorldObject::WorldObject(Uint32 ObjID) :
ObjID(ObjID)
{
}

const Uint16 WorldObject::GetMapID() const
{
    return pMap->MapID;
}

bool WorldObject::UpdateCoordinates(Uint8 Direction)
{
    Uint16 OldX = x, OldY = y;

    switch(Direction)
    {
    case MOVE_UP:
        if(pMap->TileGrid[y-1][x].pWorldObject)
            return false;
        y--;
        break;
    case MOVE_DOWN:
        if(pMap->TileGrid[y+1][x].pWorldObject)
            return false;
        y++;
        break;
    case MOVE_LEFT:
        if(pMap->TileGrid[y][x-1].pWorldObject)
            return false;
        x--;
        break;
    case MOVE_RIGHT:
        if(pMap->TileGrid[y][x+1].pWorldObject)
            return false;
        x++;
        break;        
    }

    pMap->TileGrid[OldY][OldX].pWorldObject = nullptr;
    pMap->TileGrid[y][x].pWorldObject = this;

    return true;
}