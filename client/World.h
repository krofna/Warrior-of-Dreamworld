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

#include "GameState.h"
#include "Defines.h"
#include "WorldObject.h"

class GameState;
class WorldObject;

class World : public GameState
{
    friend class WorldSession;

public:
    World();
    ~World();
    void LoadTileMap(Uint16 MapID);
    void Draw();
    void HandleEvent(sf::Event Event);

    void CreateSpellEffect(Uint32 Caster, Uint8 Direction, Uint8 DisplayID, Uint8 Effect);

private:
    std::string TilesetFileName;
    sf::RenderStates MapStates;
    sf::VertexArray TileMap;
    sf::View WorldView;
    Uint8 MoveWorldView;

    // Chat Members
    std::string Message;
    float ChatOffsetX;
    float ChatOffsetY;
    bool typing;

    std::map<Uint32, WorldObject*> WorldObjectMap;
};

#endif