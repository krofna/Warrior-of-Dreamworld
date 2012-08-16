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
#include "Utilities.hpp"
#include "Globals.hpp"
#include "Game.hpp"
#include "WorldSession.hpp"
#include "ResourceManager.hpp"
#include "../shared/Math.hpp"
#include <cassert>

World::World (uint32 MeID) :
TileMap      (sf::PrimitiveType::Quads),
WorldView    (sf::FloatRect(0, 0, WindowWidth, WindowHeight)),
MoveWorldView(MOVE_STOP),
CameraLeft   (WorldView.getCenter().x - WindowWidth / 2),
CameraTop    (WorldView.getCenter().y - WindowHeight / 2),
CameraRight  (WindowWidth),
CameraBottom (WindowHeight),
MeID         (MeID)
{
}

World::~World()
{
    for(auto ObjIter = WorldObjectMap.begin(); ObjIter != WorldObjectMap.end(); ++ObjIter)
    {
        delete ObjIter->second;
    }

    for(auto AnimIter = Animations.begin(); AnimIter != Animations.end(); ++AnimIter)
    {
        delete *AnimIter;
    }
}

void World::Load(WorldPacket Argv)
{
    Window->setView(WorldView);

    uint16 MapID;
    Argv >> MapID;
    ResourceManager::RemoveTileset(TilesetFileName);
    std::string Path = "data/maps/map" + IntToString(MapID) + ".txt";

    std::ifstream File(Path);
    assert(File.good());

    float x, y, tx, ty;
    int index = 0;

    File >> TilesetFileName >> MapWidth >> MapHeight;
    TileMap.resize(MapWidth * MapHeight * 4);
    MapWidth *= TILE_SIZE;
    MapHeight *= TILE_SIZE;
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

    sGame->PopState();
    sGame->PushState(this);
}

void World::Draw()
{
    // Network thread shouldn't add new stuff while drawing
    boost::mutex::scoped_lock lock(DrawingMutex);


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

    // Draw static objects
    for(auto i = WorldObjectMap.begin(); i != WorldObjectMap.end(); ++i)
        (*i).second->Draw();

    // Update animations
    for(auto i = Animations.begin(); i != Animations.end(); ++i)
    {
        (*i)->Update();
    }
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
            Session->SendMovementRequest(MOVE_RIGHT);
            break;

        case sf::Keyboard::A:
            Session->SendMovementRequest(MOVE_LEFT);
            break;

        case sf::Keyboard::W:
            Session->SendMovementRequest(MOVE_UP);
            break;

        case sf::Keyboard::S:
            Session->SendMovementRequest(MOVE_DOWN);
            break;

        case sf::Keyboard::Escape:
            Session->SendLogOutRequest();
            Window->close();
            return;

        default:
            break;
        }
        break;

    case sf::Event::MouseMoved:
        MoveWorldView = MOVE_STOP;

        if(sf::Mouse::getPosition(*Window).x >= WindowWidth - (TILE_SIZE / 2))
            MoveWorldView |= MOVE_RIGHT;

        else if(sf::Mouse::getPosition(*Window).x < TILE_SIZE / 2)
            MoveWorldView |= MOVE_LEFT;

        if(sf::Mouse::getPosition(*Window).y > WindowHeight - (TILE_SIZE / 2))
            MoveWorldView |= MOVE_DOWN;

        else if(sf::Mouse::getPosition(*Window).y < TILE_SIZE / 2)
            MoveWorldView |= MOVE_UP;

        break;

    case sf::Event::MouseButtonPressed:
        // PH
        Session->SendCastSpellRequest(0, math::GetAngle(WorldObjectMap[MeID]->GetPosition(), Window->convertCoords(sf::Mouse::getPosition())));
        break;
        
    default:
        break;
    }
}

void World::AddObject(WorldObject* pWorldObject, uint32 ObjectID)
{
    boost::mutex::scoped_lock lock(DrawingMutex);
    this->WorldObjectMap[ObjectID] = pWorldObject;
}

void World::RemoveObject(uint32 ObjectID)
{
    boost::mutex::scoped_lock lock(DrawingMutex);
    auto Iter = WorldObjectMap.find(ObjectID);
    if(Iter == WorldObjectMap.end())
        return;

    delete Iter->second;
    WorldObjectMap.erase(Iter);
}

void World::AddAnimation(Animation* pAnimation)
{
    boost::mutex::scoped_lock lock(DrawingMutex);
    Animations.push_back(pAnimation);
}