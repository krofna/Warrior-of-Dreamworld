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
#include "WorldSession.hpp"
#include "../shared/Log.hpp"
#include "ObjectMgr.hpp"
#include "Database.hpp"
#include "../scripts/ScriptLoader.hpp"
#include "CreatureAIFactory.hpp"
#include "Pathfinder.hpp"
#include "boost/date_time/posix_time/posix_time.hpp"
#include "boost/bind.hpp"
#include "../shared/Log.hpp"
#include "CommandHandler.hpp"

#define SERVER_HEARTBEAT 50

World* sWorld;

World::World(boost::asio::io_service& io, tcp::endpoint& Endpoint) :
IsRunning   (true),
io          (io),
Timer       (io)

{
    pWorldAcceptor = new WorldAcceptor(io, Endpoint);
}

World::~World()
{
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
        sLog.Write("Connect to database...");
        sDatabase.Connect();
        sLog.Write("Connected to database.");

        sLog.Write("Load creature templates...");
        sObjectMgr.LoadCreatureTemplates();
        sLog.Write("Creature templates loaded.");

        sLog.Write("Load spells...");
        sObjectMgr.LoadSpells();
        sLog.Write("Spells loaded.");

        sLog.Write("Load players login informations...");
        sObjectMgr.LoadPlayersLoginInfo();
        sLog.Write("Players login informations loaded.");

        AIFactory = new CreatureAIFactory;

        sLog.Write("Load scripts...");
        LoadScripts();
        sLog.Write("Scripts loaded.");

        sLog.Write("Init Pathfinding...");
        Pathfinder::Init();
        sLog.Write("Pathfinding initialized.");

        sLog.Write("Loading maps...");
        for(uint16 i = 0; i < MAP_COUNT; ++i)
        {
            MapPtr pMap(new Map(i));
            sLog.Write("Loading creatures' map (%d)", i);
            pMap->LoadCreatures();
            sLog.Write("Creatures' map loaded (%d)", i);
            Maps.push_back(pMap);
        }
        sLog.Write("Maps loaded.");
    }
    catch(sql::SQLException &e)
    {
        sLog << "SQL Exception: " << e.what() << '\n';
        throw;
    }
    catch(std::exception &e)
    {
        sLog << "Exception: " << e.what() << '\n';
        throw;
    }
}

int World::Run()
{
    pWorldAcceptor->Accept();
    Timer.expires_from_now(boost::posix_time::milliseconds(50));
    OldTime = boost::chrono::high_resolution_clock::now();
    Timer.async_wait(boost::bind(&World::Update, this));
    io.run();
    return 0;
}

// [PH], needs extra work
void World::ConsoleInput()
{
    std::string Input;

    while (true)
    {
        sLog.Write("Console> ");
        std::getline(std::cin, Input);

        if (Input == "exit")
            break;
        else if (!HandleCommand(Input))
            sLog.Write("Unknown command!");
        else
            sLog.Write("WARN()");
    }
    IsRunning = false;
}

void World::Update()
{
    if(!IsRunning)
    {
        io.stop();
        return;
    }

    boost::chrono::milliseconds NewTime = boost::chrono::duration_cast<boost::chrono::milliseconds>(boost::chrono::high_resolution_clock::now() - OldTime);
    boost::chrono::high_resolution_clock::time_point newOldTime(NewTime);
    OldTime = newOldTime;

    for(auto MapIterator = Maps.begin(); MapIterator != Maps.end(); ++MapIterator)
    {
        (*MapIterator)->Update(NewTime.count());
    }
    Timer.expires_at(Timer.expires_at() + boost::posix_time::milliseconds(50));
    Timer.async_wait(boost::bind(&World::Update, this));
}

void World::AddSession(WorldSession* pWorldSession)
{
    PlayerPtr pPlayer = pWorldSession->GetPlayer();
    Maps[pPlayer->GetMapID()]->AddPlayer(pPlayer);
}

MapPtr World::GetMap(uint16 MapID)
{
    if(MapID < Maps.size())
        return Maps[MapID];

    return MapPtr();
}
bool World::HandleCommand(std::string& Command)
{
    CommandHandler Handler(Command);

    try
    {
        Handler.ExecuteCommand();
    }
    catch(CommandHandler::BadCommand& e)
    {
        sLog.Write("something went wrong with command");
    }

    return true;
}
