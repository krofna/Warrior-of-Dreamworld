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
#ifndef MAP_H
#define MAP_H

#include "GameState.hpp"
#include "../shared/Defines.hpp"
#include "WorldObject.hpp"
#include "Animation.hpp"
#include <boost/thread.hpp>
#include <boost/shared_ptr.hpp>

class GameState;
class WorldObject;
class MessageChatArea;

typedef boost::shared_ptr<MessageChatArea> MessageChatAreaPtr;

class World : public GameState
{
    friend class WorldSession;
public:
    World(uint64 MeID);
    ~World();

    virtual void Load(WorldPacket Argv);
    void Update() {}
    void Draw();
    void HandleEvent(sf::Event Event);

    void AddObject(WorldObject* pWorldObject, uint64 ObjectID);
    void RemoveObject(uint64 ObjectID);

    void AddAnimation(Animation* pAnimation);

protected:
    void ReceiveNewMessage(uint64 ObjID, std::string const& Message);

private:
    MessageChatAreaPtr m_MessageArea;

    // TODO: Map?
    std::string TilesetFileName;
    sf::RenderStates MapStates;
    sf::VertexArray TileMap;
    sf::View WorldView;
    uint8 MoveWorldView;

    uint16 MapWidth;
    uint16 MapHeight;

    int CameraLeft;
    int CameraTop;
    int CameraRight;
    int CameraBottom;
    // END TODO

    std::map<uint64, WorldObject*> WorldObjectMap;
    std::vector<Animation*> Animations; // TODO; Only for spells SpellAnimations w/e
    const uint64 MeID;

    boost::mutex DrawingMutex;
};

#endif
