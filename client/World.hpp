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

class GameState;
class WorldObject;

class World : public GameState
{
    friend class WorldSession;

public:
    World(Uint32 MeID);
    ~World();

    void LoadTileMap(Uint16 MapID);
    void Draw();
    void HandleEvent(sf::Event Event);

    void RemoveObject(Uint32 ObjectID);

private:
    void HandleTyping(sf::Event Event);

    // TODO: Map?
    std::string TilesetFileName;
    sf::RenderStates MapStates;
    sf::VertexArray TileMap;
    sf::View WorldView;
    Uint8 MoveWorldView;
    
    int MapWidth;
    int MapHeight;

    int CameraLeft;
    int CameraRight;
    int CameraBottom;
    int CameraTop;
    // END TODO

    // Chat Members
    std::string Message;
    float ChatOffsetX;
    float ChatOffsetY;
    bool Typing;

    std::map<Uint32, WorldObject*> WorldObjectMap;
    std::vector<Animation> Animations; // TODO; Only for spells SpellAnimations w/e
    const Uint32 MeID;
};

#endif