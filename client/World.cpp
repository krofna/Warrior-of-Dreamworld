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
    Typing = false;
    ChatOffsetX = -(WindowWidth / 2 - 5);
    ChatOffsetY = 20;
}

World::~World()
{
    for(auto ObjIter = WorldObjectMap.begin(); ObjIter != WorldObjectMap.end(); ++ObjIter)
    {
        delete ObjIter->second;
    }
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
        if((MoveWorldView & MOVE_UP) == MOVE_UP && WorldView.getCenter().y >= (WindowHeight/2))
            WorldView.move(0, -WORLD_VIEW_OFFSET);

        else if((MoveWorldView & MOVE_DOWN) == MOVE_DOWN)
            WorldView.move(0, WORLD_VIEW_OFFSET);

        if((MoveWorldView & MOVE_RIGHT) == MOVE_RIGHT)
            WorldView.move(WORLD_VIEW_OFFSET, 0);

        else if((MoveWorldView & MOVE_LEFT) == MOVE_LEFT)
            WorldView.move(-WORLD_VIEW_OFFSET, 0);

        Window.setView(WorldView);
    }
    
    // Draw tile map
    Window.draw(TileMap, MapStates);

    // Draw static objects
    for(auto i = WorldObjectMap.begin(); i != WorldObjectMap.end(); ++i)
        (*i).second->Draw();

    // Update animations
    for(auto i = Animations.begin(); i != Animations.end(); ++i)
    {
        i->Update();
    }

    // Draw Text Messages
    for(auto i = Session->TextMessages.begin(); i != Session->TextMessages.end(); i++)
    {
        int currentmessage = std::distance(Session->TextMessages.begin(), i);

        (*i).setPosition(WorldView.getCenter().x + ChatOffsetX, WorldView.getCenter().y + ChatOffsetY + currentmessage * 20);
        Window.draw((*i));
    }
}

void World::HandleEvent(sf::Event Event)
{
    if(Typing)
    {
        HandleTyping(Event);
        return;
    }

    switch(Event.type)
    {
    case sf::Event::KeyPressed:
        switch(Event.key.code)
        {
        case sf::Keyboard::Return:
            Typing = true;
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

        case sf::Keyboard::T:
            Session->SendCastSpellRequest(0, MOVE_UP);
            break;

        case sf::Keyboard::Escape:
            Session->SendLogOutRequest();
            Window.close();
            return;

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

void World::HandleTyping(sf::Event Event)
{
    switch(Event.type)
    {
    case sf::Event::KeyPressed:
        switch(Event.key.code)
        {
        case sf::Keyboard::Return:
            Typing = false;
            if(!Message.empty())
                Session->SendTextMessage(Message);
            Message.clear();
            break;

        case sf::Keyboard::Space:
            //     Message += " ";
            break;

        default:
            break;
        }
        break;

    case sf::Event::TextEntered:
        // TODO: Make a function if key is valid
        // In chat handler (later)
        if(Typing && Event.text.unicode < 128)
            Message += static_cast<char>(Event.key.code);
        break;

    default:
        break;
    }
}
// TODO [PH]
void World::CreateSpellEffect(Uint32 Caster, Uint8 Direction, Uint16 DisplayID, Uint16 Effect)
{
    WorldObject* pCaster = WorldObjectMap[Caster];
    Animations.push_back(Animation(pCaster->GetPosition(), Direction));
}

void World::RemoveObject(Uint32 ObjectID)
{
    if(WorldObjectMap.find(ObjectID) == WorldObjectMap.end())
        return;

    delete WorldObjectMap[ObjectID];
    WorldObjectMap.erase(ObjectID);
}