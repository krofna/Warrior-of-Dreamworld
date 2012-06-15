#include "World.h"
#include "Utilities.h"
#include "Defines.h"
#include "Globals.h"
#include "ResourceManager.h"
#include <fstream>
#include <cassert>
#include <iostream>

World::World() : 
TileMap     (sf::PrimitiveType::Quads)
{
}

void World::LoadTileMap(uint8_t MapID)
{
    std::string Path = "data/maps/map" + IntToString(MapID) + ".txt";
    std::ifstream File(Path);
    assert(File.good());

    float x, y, tx, ty;
    uint16_t TileCount;
    uint32_t index = 0;
    std::string FileName;

    File >> FileName >> TileCount;
    TileMap.resize(TileCount * 4);
    MapStates.texture = ResourceManager::GetTileset(FileName);

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
    Window.draw(TileMap, MapStates);
}

void World::HandleEvent(sf::Event Event)
{
    switch(Event.type)
    {
    case sf::Event::KeyPressed:
        switch(Event.key.code)
        {
        case sf::Keyboard::Right:
            MapView.move(1, 0);
            Window.setView(MapView);
            break;

        case sf::Keyboard::Left:
            MapView.move(-1, 0);
            Window.setView(MapView);
            break;

        case sf::Keyboard::Up:
            MapView.move(0, -1);
            Window.setView(MapView);
            break;

        case sf::Keyboard::Down:
            MapView.move(0, 1);
            Window.setView(MapView);
            break;
        }
        break;

    case sf::Event::MouseButtonPressed:
        break;
    }
}