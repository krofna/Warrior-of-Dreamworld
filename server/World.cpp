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
#include "ObjectMgr.hpp"
#include "Database.hpp"
#include "../scripts/ScriptLoader.hpp"
#include "CreatureAIFactory.hpp"
#include "Pathfinder.hpp"

#define SERVER_HEARTBEAT 50

World* sWorld;

World::World() :
IsRunning   (true)
{
}

World::~World()
{
    Pathfinder::Destroy();
    delete AIFactory;
}

CreatureAIFactory* World::GetAIFactory()
{
    return AIFactory;
}

void World::Load()
{
    try
    {
        sDatabase.Connect();

        sObjectMgr.LoadCreatureTemplates();
        sObjectMgr.LoadSpells();

        AIFactory = new CreatureAIFactory;
        LoadScripts();

        Pathfinder::Init();

        for(int i=0; i < MAP_COUNT; ++i)
        {
            Map* pMap = new Map(i);
            pMap->LoadCreatures();
            Maps.push_back(pMap);
        }

        pAuthSession = new AuthSession();
        pAuthSession->LoadPlayersLoginInfo();
    }
    catch(sql::SQLException &e) 
    {
        std::cout << "SQL Exception: " << e.what();
        throw;
    }
    catch(std::exception &e)
    {
        std::cout << "Exception: " << e.what();
        throw;
    }
}

int World::Run()
{
    sf::Clock Clock;
    Int32 Diff;

    // Server main loop
    while(IsRunning)
    {
        // Handle Auth requests
        pAuthSession->HandleAll();

        // Receive packets from all clients
        for(auto SessionIterator = Sessions.begin(); SessionIterator != Sessions.end(); ++SessionIterator)
            (*SessionIterator)->ReceivePackets();

        // If time between updates was less than 50ms, sleep
        if(Clock.getElapsedTime().asMilliseconds() < SERVER_HEARTBEAT)
            sf::sleep(sf::milliseconds(SERVER_HEARTBEAT - Clock.getElapsedTime().asMilliseconds()));

        Diff = Clock.getElapsedTime().asMilliseconds();
        Clock.restart();

        // Call the world update
        Update(Diff);
    }

    return 0;
}

// [PH], needs extra work
void World::ConsoleInput()
{
    std::string Input;

    while(Input != "shutdown")
    {
        std::cin >> Input;
    }
    IsRunning = false;
}

void World::Update(Int32 diff)
{
    for(auto MapIterator = Maps.begin(); MapIterator != Maps.end(); ++MapIterator)
    {
        (*MapIterator)->Update(diff);
    }
}

void World::AddSession(sf::TcpSocket* pSocket, Player* pPlayer)
{
    WorldSession* pWorldSession = new WorldSession(pSocket, pPlayer);
    pPlayer->BindSession(pWorldSession);
    Sessions.push_back(pWorldSession);
    Maps[pPlayer->GetMapID()]->AddPlayer(pPlayer);
}

Map* World::GetMap(Uint8 MapID)
{
    if(MapID < Maps.size())
        return Maps[MapID];

    return nullptr;
}
