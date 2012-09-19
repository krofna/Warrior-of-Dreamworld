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
#include "WorldAcceptor.hpp"
#include <boost/chrono.hpp>

class CreatureAIFactory;

class World
{
    friend class WorldSession;
public:
    World(boost::asio::io_service& io, tcp::endpoint& Endpoint);
    ~World();
    void Load();

    int Run();
    void ConsoleInput();

    void AddPlayer(Player* pPlayer);
    Map* GetMap(uint16 MapID);

    CreatureAIFactory* GetAIFactory();

private:
    void Update();
    void HandleCommand(std::string& Command);

    WorldAcceptor* pWorldAcceptor;
    std::vector<Map*> Maps;

    CreatureAIFactory* AIFactory;

    volatile bool IsRunning;

    boost::asio::io_service& io;
    boost::asio::deadline_timer Timer;
    boost::chrono::high_resolution_clock::time_point OldTime;
};

typedef boost::chrono::milliseconds ms;
extern World* sWorld;

#endif
