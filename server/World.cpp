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
#include "boost/date_time/posix_time/posix_time.hpp"
#include "boost/bind.hpp"

#define SERVER_HEARTBEAT 50

World* sWorld;

World::World(boost::asio::io_service& io, tcp::endpoint& Endpoint) :
IsRunning   (true),
Timer       (io),
io          (io)
{
    pWorldAcceptor = new WorldAcceptor(io, Endpoint);
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
        sObjectMgr.LoadPlayersLoginInfo();

        AIFactory = new CreatureAIFactory;
        LoadScripts();

        Pathfinder::Init();

        for(int i=0; i < MAP_COUNT; ++i)
        {
            Map* pMap = new Map(i);
            pMap->LoadCreatures();
            Maps.push_back(pMap);
        }
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
    pWorldAcceptor->Accept();
    Timer.expires_from_now(boost::posix_time::milliseconds(50));
    Timer.async_wait(boost::bind(&World::Update, this));
    io.run();
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

void World::Update(/*int32 diff*/)
{
    if(!IsRunning)
    {
        io.stop();
        return;
    }

    for(auto MapIterator = Maps.begin(); MapIterator != Maps.end(); ++MapIterator)
    {
        (*MapIterator)->Update(50);
    }
    Timer.expires_at(Timer.expires_at() + boost::posix_time::milliseconds(50));
    Timer.async_wait(boost::bind(&World::Update, this));
}

void World::AddSession(WorldSession* pWorldSession)
{
    Sessions.push_back(pWorldSession);
    Maps[pWorldSession->GetPlayer()->GetMapID()]->AddPlayer(pWorldSession->GetPlayer());
}

Map* World::GetMap(uint8 MapID)
{
    if(MapID < Maps.size())
        return Maps[MapID];

    return nullptr;
}
