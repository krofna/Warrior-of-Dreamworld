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
#ifndef WORLD_H
#define WORLD_H

#include "Map.hpp"
#include "AuthSession.hpp"

class CreatureAIFactory;

class World
{
    friend class WorldSession;
public:
    World();
    ~World();
    void Load();

    int Run();
    void ConsoleInput();

    void AddSession(sf::TcpSocket* pSocket, Player* pPlayer);
    Map* GetMap(uint8 MapID);

    CreatureAIFactory* GetAIFactory();

private:
    void Update(int32 diff);

    AuthSession* pAuthSession;
    std::vector<Map*> Maps;
    std::vector<WorldSession*> Sessions;

    CreatureAIFactory* AIFactory;

    volatile bool IsRunning;
};

extern World* sWorld;

#endif
