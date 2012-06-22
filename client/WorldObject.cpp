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
#include "ResourceManager.h"

WorldObject::WorldObject(std::string Tileset, Uint16 x, Uint16 y, Uint16 tx, Uint16 ty)
{
    ObjectSprite.setTexture(*ResourceManager::GetTileset(Tileset));
    ObjectSprite.setTextureRect(sf::IntRect(tx * TILE_SIZE, ty * TILE_SIZE, TILE_SIZE, TILE_SIZE));
    ObjectSprite.setPosition((float)x * TILE_SIZE, (float)y * TILE_SIZE);
}