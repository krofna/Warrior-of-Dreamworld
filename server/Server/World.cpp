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
#include <boost/bind.hpp>

#include "World.hpp"
#include "WorldSession.hpp"
#include "shared/Log.hpp"
#include "ObjectMgr.hpp"
#include "Database.hpp"
#include "scripts/ScriptLoader.hpp"
#include "AIFactory.hpp"
#include "Pathfinder.hpp"
#include "shared/Log.hpp"
#include "CommandHandler.hpp"

#define SERVER_HEARTBEAT 50

World* sWorld;

World::World(boost::asio::io_service& io) :
IsRunning   (true),
io          (io),
Timer       (io)
{
    pWorldAcceptor = new WorldAcceptor(io);
    pAIFactory = new AIFactory;
}

World::~World()
{
    std::for_each(Maps.begin(), Maps.end(), boost::bind(&operator delete, _1));
    delete pAIFactory;
    delete pWorldAcceptor;
}

AIFactory* World::GetAIFactory()
{
    return pAIFactory;
}

void World::Load()
{
    try
    {
        sLog.Write("Connect to database...");
        sDatabase.Connect();
        sLog.Write("Connected to database.");

        sLog.Write("Loading creature templates...");
        sObjectMgr.LoadCreatureTemplates();
        sLog.Write("Creature templates loaded.");
        
        sLog.Write("Loading creature quests...");
        sObjectMgr.LoadCreatureQuests();
        sLog.Write("Creature quests loaded.");

        sLog.Write("Loading spell templates...");
        sObjectMgr.LoadSpellTemplates();
        sLog.Write("Spells loaded.");
        
        sLog.Write("Loading gameobject templates...");
        sObjectMgr.LoadGameObjectTemplates();
        sLog.Write("Gameobject templates loaded.");
        
        sLog.Write("Loading map templates...");
        sObjectMgr.LoadMapTemplates();
        sLog.Write("Map templates loaded.");

        sLog.Write("Loading players login informations...");
        sObjectMgr.LoadPlayersLoginInfo();
        sLog.Write("Players login informations loaded.");

        sLog.Write("Loading scripts...");
        LoadScripts();
        sLog.Write("Scripts loaded.");

        sLog.Write("Init Pathfinding...");
        Pathfinder::Init();
        sLog.Write("Pathfinding initialized.");

        sLog.Write("Loading maps...");
        
        QueryResult Result(sDatabase.Query("SELECT * FROM `map`"));
        Map* pMap;
        
        while(Result->next())
        {
            pMap = new Map(sObjectMgr.GetMapTemplate(Result->getUInt64(2)), Result->getUInt(1));
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
    OldTime = boost::chrono::high_resolution_clock::now();
    Timer.expires_from_now(boost::posix_time::milliseconds(50));
    Timer.async_wait(boost::bind(&World::Update, this));
    io.run();
    return 0;
}

void World::ConsoleInput()
{
    std::string Input;

    while (true)
    {
        std::cout << "Console> " << std::flush;
        std::getline(std::cin, Input);

        if (Input == "exit")
            break;
        HandleCommand(Input);
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
    auto now = boost::chrono::high_resolution_clock::now();
    ms NewTime = boost::chrono::duration_cast<ms>(boost::chrono::high_resolution_clock::now() - OldTime);

    #ifdef DEBUG_UPDATE_TIME
    sLog.Write("World::Update(now: %u, old: %u, new: %u)", now, OldTime, NewTime);
    #endif

    for(auto MapIterator = Maps.begin(); MapIterator != Maps.end(); ++MapIterator)
    {
        (*MapIterator)->Update(NewTime.count());
    }
    OldTime += NewTime;

    Timer.expires_at(Timer.expires_at() + boost::posix_time::milliseconds(50));
    Timer.async_wait(boost::bind(&World::Update, this));
}

Map* World::GetMap(uint16 MapID)
{
    if(MapID < Maps.size())
        return Maps[MapID];

    return nullptr;
}

void World::HandleCommand(std::string& Command)
{
    CommandHandler Handler(Command);

    try
    {
        Handler.ExecuteCommand();
    }
    catch(CommandHandler::BadCommand&)
    {
        sLog.Write("Bad command !");
    }
    catch(sql::SQLException& e)
    {
        sLog.Write(e.what());
        sLog.Flush();
    }
}
