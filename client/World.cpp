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
#include "World.hpp"
#include "Game.hpp"
#include "WorldSession.hpp"
#include "shared/Math.hpp"
#include "shared/Utils.hpp"
#include "ObjectMgr.hpp"
#include <fstream>
#include <cassert>

World::World  (uint64 MeID) :
TileMap       (sf::PrimitiveType::Quads),
WorldView     (sf::FloatRect(0, 0, Window->getSize().x, Window->getSize().y)),
MoveWorldView (MOVE_STOP),
CameraLeft    (WorldView.getCenter().x - Window->getSize().x / 2),
CameraTop     (WorldView.getCenter().y - Window->getSize().y / 2),
CameraRight   (Window->getSize().x),
CameraBottom  (Window->getSize().y),
MeID          (MeID),
m_PointMode   (false)
{
}

World::~World()
{
}

void World::Load(WorldPacket Argv)
{
    Window->setView(WorldView);

    uint32 MapID;
    Argv >> MapID;

    std::string Path = "data/maps/map" + ToString(MapID) + ".map";

    std::ifstream File(Path);
    assert(File.good());
    
    int index = 0;

    File >> MapWidth >> MapHeight;
    File >> TilesetFileName;

    float x, y, tx, ty;

    TileMap.resize(MapWidth * MapHeight * 4);
    MapWidth *= TILE_SIZE;
    MapHeight *= TILE_SIZE;
    MapStates.texture = ObjectMgr::GetInstance()->GetTileset(TilesetFileName);

    while (File >> x >> y >> tx >> ty)
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

    Game::GetInstance().PopState();
    Game::GetInstance().PushState(this);
}

void World::Draw()
{
    if(MoveWorldView != MOVE_STOP)
    {
        if(MoveWorldView & MOVE_UP && CameraTop > 0)
        {
            CameraTop -= TILE_SIZE;
            CameraBottom -= TILE_SIZE;
            WorldView.move(0, -TILE_SIZE);
        }

        else if(MoveWorldView & MOVE_DOWN && CameraBottom < MapHeight)
        {
            CameraTop += TILE_SIZE;
            CameraBottom += TILE_SIZE;
            WorldView.move(0, TILE_SIZE);
        }

        if(MoveWorldView & MOVE_RIGHT && CameraRight < MapWidth)
        {
            CameraLeft += TILE_SIZE;
            CameraRight += TILE_SIZE;
            WorldView.move(TILE_SIZE, 0);
        }

        else if(MoveWorldView & MOVE_LEFT && CameraLeft > 0)
        {
            CameraLeft -= TILE_SIZE;
            CameraRight -= TILE_SIZE;
            WorldView.move(-TILE_SIZE, 0);
        }

        Window->setView(WorldView);
    }
    
    // Draw tile map
    Window->draw(TileMap, MapStates);
}

void World::HandleEvent(sf::Event Event)
{
    switch(Event.type)
    {
    case sf::Event::KeyPressed:
        switch(Event.key.code)
        {
        case sf::Keyboard::Return:
            break;

        case sf::Keyboard::D:
            WorldSession::GetInstance()->SendMovementRequest(MOVE_RIGHT);
            break;

        case sf::Keyboard::A:
            WorldSession::GetInstance()->SendMovementRequest(MOVE_LEFT);
            break;

        case sf::Keyboard::W:
            WorldSession::GetInstance()->SendMovementRequest(MOVE_UP);
            break;

        case sf::Keyboard::S:
            WorldSession::GetInstance()->SendMovementRequest(MOVE_DOWN);
            break;

        case sf::Keyboard::Escape:
            WorldSession::GetInstance()->SendLogOutRequest();
            WorldSession::GetInstance()->GoToLoginScreen();
            return;
        case sf::Keyboard::P:
            m_PointMode = !m_PointMode;
            return;

        case sf::Keyboard::LControl:
        case sf::Keyboard::RControl:
            WorldSession::GetInstance()->SendAttackRequest();
            break;

        default:
            break;
        }
        break;

    case sf::Event::MouseMoved:
        MoveWorldView = MOVE_STOP;

        if(sf::Mouse::getPosition(*Window).x >= Window->getSize().x - (TILE_SIZE / 2))
            MoveWorldView |= MOVE_RIGHT;

        else if(sf::Mouse::getPosition(*Window).x < TILE_SIZE / 2)
            MoveWorldView |= MOVE_LEFT;

        if(sf::Mouse::getPosition(*Window).y > Window->getSize().y - (TILE_SIZE / 2))
            MoveWorldView |= MOVE_DOWN;

        else if(sf::Mouse::getPosition(*Window).y < TILE_SIZE / 2)
            MoveWorldView |= MOVE_UP;

        break;

    case sf::Event::MouseButtonPressed:
        // ...
        
    default:
        break;
    }
}
