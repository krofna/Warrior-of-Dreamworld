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
#include "Globals.hpp"
#include "Game.hpp"
#include "WorldSession.hpp"
#include "MessageChatArea.hpp"
#include "Inventory.hpp"
#include "Bag.hpp"

#include "shared/Math.hpp"
#include "shared/Utils.hpp"
#include <cassert>

World::World  (uint64 MeID) :
m_MessageArea (new MessageChatArea),
TileMap       (sf::PrimitiveType::Quads),
WorldView     (sf::FloatRect(0, 0, WindowWidth, WindowHeight)),
MoveWorldView (MOVE_STOP),
CameraLeft    (WorldView.getCenter().x - WindowWidth / 2),
CameraTop     (WorldView.getCenter().y - WindowHeight / 2),
CameraRight   (WindowWidth),
CameraBottom  (WindowHeight),
MeID          (MeID),
m_PointMode   (false)
{
}

World::~World()
{
    std::for_each(WorldObjectMap.begin(), WorldObjectMap.end(), MapDeleter());
    std::for_each(Animations.begin(), Animations.end(), Deleter());
    delete pInventory;
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
    MapStates.texture = sObjectMgr->GetTileset(TilesetFileName);

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


    Bag::InitializePositionsBag();
    pInventory = new Inventory("data/icons/bag.png");

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

    // Draw static objects
    for(auto i = WorldObjectMap.begin(); i != WorldObjectMap.end(); ++i)
        (*i).second->Draw();

    // Update animations
    for(auto i = Animations.begin(); i != Animations.end(); ++i)
    {
        (*i)->Update();
    }

    // Draw Inventory
    pInventory->Draw();

    // Draw chat GUI
    m_MessageArea->Draw(m_UpdateClock.restart().asMilliseconds());
}

void World::HandleEvent(sf::Event Event)
{
    if (m_MessageArea->HandleTyping(Event) || pInventory->HandleEvent(Event))
        return;

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
            Session->GoToLoginScreen();
            return;
        case sf::Keyboard::P:
            m_PointMode = !m_PointMode;
            return;

        case sf::Keyboard::LControl:
        case sf::Keyboard::RControl:
            Session->SendAttackRequest();
            break;

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
        if (m_PointMode)
            std::cout << Event.mouseButton.x << ";" << Event.mouseButton.y << std::endl;
        else
            Session->SendCastSpellRequest(1, math::GetAngle(WorldObjectMap[MeID]->GetPosition(), Window->convertCoords(sf::Mouse::getPosition())));
        break;
        
    default:
        break;
    }
}

void World::AddObject(WorldObject* pWorldObject, uint64 ObjectID)
{
    this->WorldObjectMap[ObjectID] = pWorldObject;
}

void World::RemoveObject(uint64 ObjectID)
{
    auto Iter = WorldObjectMap.find(ObjectID);
    if(Iter == WorldObjectMap.end())
        return;

    delete Iter->second;
    WorldObjectMap.erase(Iter);
}

void World::ReceiveNewMessage(uint64 ObjectID, std::string const& Text)
{
    auto Iter = WorldObjectMap.find(ObjectID);
    if (Iter == WorldObjectMap.end())
        return;

    m_MessageArea->AddMessage(Iter->second->GetObjectName(), Text);
}

void World::ReceiveCommandReponse(std::string const& Text)
{
    m_MessageArea->AddServerMessage(Text, sf::Color::Blue);
}

void World::ReceiveNotification(std::string const& Text)
{
    m_MessageArea->AddServerMessage(Text, sf::Color::Red);
}

void World::ReceiveEmote(TypeEmote type, uint64 ObjectID, std::string const& Text)
{
    if (type == VOICE_EMOTE)
    {
//        PlayVoice(Text); // Text = Path
        return;
    }

    sf::Color color;

    if (type == TEXT_EMOTE)
        color = sf::Color::Magenta;

    auto Iter = WorldObjectMap.find(ObjectID);
    if (Iter == WorldObjectMap.end())
        return;

    std::string Msg = Iter->second->GetObjectName() + Text;

    m_MessageArea->AddRawMessage(Msg, color);
}

void World::AddAnimation(Animation* pAnimation)
{
    Animations.push_back(pAnimation);
}

void World::RemoveAnimation(uint32 ID)
{
    for(auto iter = Animations.begin(); iter != Animations.end(); ++iter)
    {
        if((*iter)->GetID() == ID)
        {
            delete *iter;
            Animations.erase(iter);
            return;
        }
    }
}

Inventory* World::GetInventory() 
{
    return pInventory;
}
