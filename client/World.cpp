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
#include "World.h"
#include "Utilities.h"
#include "Globals.h"
#include "ResourceManager.h"
#include <cassert>

World::World() :
TileMap     (sf::PrimitiveType::Quads),
WorldView   (sf::FloatRect(0, 0, WindowWidth, WindowHeight)),
MoveWorldView(MOVE_STOP)
{
    Window.setView(WorldView);
}

World::~World()
{
}

void World::LoadTileMap(Uint16 MapID)
{
    ResourceManager::RemoveTileset(TilesetFileName);

    std::string Path = "data/maps/map" + IntToString(MapID) + ".txt";
    std::ifstream File(Path);
    assert(File.good());

    float x, y, tx, ty;
    int TileCount, index = 0;

    File >> TilesetFileName >> TileCount;
    TileMap.resize(TileCount * 4);
    MapStates.texture = ResourceManager::GetTileset(TilesetFileName);

    while(File >> x >> y >> tx >> ty)
    {
        TileMap[index + 0].position = sf::Vector2f(x * TILE_SIZE, y * TILE_SIZE);
        TileMap[index + 1].position = sf::Vector2f(x * TILE_SIZE, (y + 1) * TILE_SIZE);
        TileMap[index + 2].position = sf::Vector2f((x + 1) * TILE_SIZE, (y + 1) * TILE_SIZE);
        TileMap[index + 3].position = sf::Vector2f((x + 1) * TILE_SIZE, y * TILE_SIZE);

        TileMap[index + 0].texCoords = sf::Vector2f(tx * TILE_SIZE, ty * TILE_SIZE);
        TileMap[index + 1].texCoords = sf::Vector2f(tx * TILE_SIZE, (ty + 1) * TILE_SIZE);
        TileMap[index + 2].texCoords = sf::Vector2f((tx + 1) * TILE_SIZE, (ty + 1) * TILE_SIZE);
        TileMap[index + 3].texCoords = sf::Vector2f((tx + 1) * TILE_SIZE, ty * TILE_SIZE);

        index += 4;
    }
}

void World::Draw()
{
    if(MoveWorldView != MOVE_STOP)
    {
        if((MoveWorldView & MOVE_UP) == MOVE_UP)
            WorldView.move(0, -WORLD_VIEW_OFFSET);

        else if((MoveWorldView & MOVE_DOWN) == MOVE_DOWN)
            WorldView.move(0, WORLD_VIEW_OFFSET);

        if((MoveWorldView & MOVE_RIGHT) == MOVE_RIGHT)
            WorldView.move(WORLD_VIEW_OFFSET, 0);

        else if((MoveWorldView & MOVE_LEFT) == MOVE_LEFT)
            WorldView.move(-WORLD_VIEW_OFFSET, 0);

        Window.setView(WorldView);
    }
    
    Window.draw(TileMap, MapStates);

    for(auto i = WorldObjectMap.begin(); i != WorldObjectMap.end(); ++i)
        (*i)->Draw();
}

void World::HandleEvent(sf::Event Event)
{
    switch(Event.type)
    {
    case sf::Event::KeyPressed:
        switch(Event.key.code) // Player move [ph]
        {
        case sf::Keyboard::Right:
            break;

        case sf::Keyboard::Left:
            break;

        case sf::Keyboard::Up:
            break;

        case sf::Keyboard::Down:
            break;
        default:
            break;
        }
        break;
        

    case sf::Event::MouseMoved:
        MoveWorldView = MOVE_STOP;

        if(sf::Mouse::getPosition(Window).x >= WindowWidth - (TILE_SIZE / 2))
            MoveWorldView |= MOVE_RIGHT;

        else if(sf::Mouse::getPosition(Window).x < TILE_SIZE / 2)
            MoveWorldView |= MOVE_LEFT;

        if(sf::Mouse::getPosition(Window).y > WindowHeight - (TILE_SIZE / 2))
            MoveWorldView |= MOVE_DOWN;

        else if(sf::Mouse::getPosition(Window).y < TILE_SIZE / 2)
            MoveWorldView |= MOVE_UP;

        break;
        
    default:
        break;
    }
    
 
}
