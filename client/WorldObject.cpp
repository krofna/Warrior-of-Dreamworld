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
#include "GameState.hpp"
#include "ObjectMgr.hpp"
#include "shared/WorldPacket.hpp"

WorldObject::WorldObject()
{

}

void WorldObject::Load(WorldPacket Argv)
{
    std::string Tileset;
    Argv >> Tileset >> ObjectName;
    uint16 x, y, tx, ty;
    Argv >> x >> y >> tx >> ty;

    ObjectSprite.setTexture(*sObjectMgr->GetTileset(Tileset, sf::Color::White));
    ObjectSprite.setTextureRect(sf::IntRect(tx * TILE_SIZE, ty * TILE_SIZE, TILE_SIZE, TILE_SIZE));
    ObjectSprite.setPosition((float)x * TILE_SIZE, (float)y * TILE_SIZE);
}

void WorldObject::UpdateCoordinates(uint16 x, uint16 y)
{/*
    switch(Direction)
    {
     case MOVE_UP:
        ObjectSprite.move(0, -1 * TILE_SIZE);
        break;
     case MOVE_DOWN:
        ObjectSprite.move(0, 1 * TILE_SIZE);
        break;
     case MOVE_LEFT:
        ObjectSprite.move(-1 * TILE_SIZE, 0);
        break;
     case MOVE_RIGHT:
        ObjectSprite.move(1 * TILE_SIZE, 0);
        break;
    }*/
    ObjectSprite.setPosition(x * TILE_SIZE, y * TILE_SIZE);
}

void WorldObject::Draw() 
{ 
    Window->draw(ObjectSprite); 
}

sf::Vector2f WorldObject::GetPosition() 
{ 
    return ObjectSprite.getPosition(); 
}

std::string WorldObject::GetObjectName() const
{
    return ObjectName;
}