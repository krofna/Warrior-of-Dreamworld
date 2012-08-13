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
#include "boost/thread.hpp"

class GameState;
class WorldObject;

class World : public GameState
{
    friend class WorldSession;

public:
    World(uint32 MeID);
    ~World();

    void LoadTileMap(uint16 MapID);
    void Draw();
    void HandleEvent(sf::Event Event);

    void RemoveObject(uint32 ObjectID);

private:
    void HandleTyping(sf::Event Event);

    // TODO: Map?
    std::string TilesetFileName;
    sf::RenderStates MapStates;
    sf::VertexArray TileMap;
    sf::View WorldView;
    uint8 MoveWorldView;
    
    int MapWidth;
    int MapHeight;

    int CameraLeft;
    int CameraRight;
    int CameraBottom;
    int CameraTop;
    // END TODO

    std::map<uint32, WorldObject*> WorldObjectMap;
    std::vector<Animation> Animations; // TODO; Only for spells SpellAnimations w/e
    const uint32 MeID;

    boost::mutex WorldObjectMutex;
    boost::mutex AnimationsMutex;
};

#endif