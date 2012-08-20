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
    OldTime = boost::posix_time::microsec_clock::local_time();
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

    boost::posix_time::ptime NewTime = boost::posix_time::microsec_clock::local_time();
    Diff = NewTime - OldTime;
    OldTime = NewTime;

    for(auto MapIterator = Maps.begin(); MapIterator != Maps.end(); ++MapIterator)
    {
        (*MapIterator)->Update(Diff.total_milliseconds());
    }
    Timer.expires_at(Timer.expires_at() + boost::posix_time::milliseconds(50));
    Timer.async_wait(boost::bind(&World::Update, this));
}

void World::AddSession(WorldSession* pWorldSession)
{
    Maps[pWorldSession->GetPlayer()->GetMapID()]->AddPlayer(pWorldSession->GetPlayer());
}

Map* World::GetMap(uint8 MapID)
{
    if(MapID < Maps.size())
        return Maps[MapID];

    return nullptr;
}
bool World::HandleCommand(std::string const& CommandName)
{
    std::istringstream in(CommandName);

    std::string Command;
    in >> Command;

    if (Command == "account")
    {
        std::string Subcommand;
        if (Subcommand == "create")
        {
            std::string UserName, Password;
            in >> UserName >> Password;
            // TODO: Create account.
            sLog.Write("Account successfully created.");
            return true;
        }
    }

    return false;
}
