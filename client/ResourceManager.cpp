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
#include "../client/ResourceManager.hpp"
#include <iostream>

std::map<std::string, sf::Texture*> ResourceManager::Tilesets;

sf::Texture* ResourceManager::GetTileset(const std::string& FileName)
{
    if(Tilesets.find(FileName) == Tilesets.end())
    {
        sf::Texture* TilesetTexture = new sf::Texture();
        std::clog << "Loading texture: " << FileName << std::endl;
        TilesetTexture->loadFromFile("data/tileset/" + FileName);
        Tilesets[FileName] = TilesetTexture;
        return TilesetTexture;
    }

    return Tilesets[FileName];
}

void ResourceManager::RemoveTileset(const std::string& FileName)
{
    Tilesets.erase(FileName);
}