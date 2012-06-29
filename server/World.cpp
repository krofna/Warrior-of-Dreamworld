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
#include "World.h"

#define SERVER_HEARTBEAT 50

World* pWorld;

World::World()
{
    for(int i=0; i < MAP_COUNT; ++i)
    {
        Maps.push_back(new Map());
    }
}

int World::Run()
{    
    pAuthSession = new AuthSession();

    sf::Clock Clock;

    //TODO: Load all

    // Server main loop
    while(true)
    {
        pAuthSession->HandleAll();

        // Receive packets from all clients
        for(auto SessionIterator = Sessions.begin(); SessionIterator != Sessions.end(); ++SessionIterator)
            (*SessionIterator)->ReceivePackets();

        // If time between updates was less than 50ms, sleep
        if(Clock.getElapsedTime().asMilliseconds() < SERVER_HEARTBEAT)
            sf::sleep(sf::milliseconds(SERVER_HEARTBEAT - Clock.getElapsedTime().asMilliseconds()));

        // Call the world update
        Update(Clock.getElapsedTime().asMilliseconds());
        Clock.restart();
    }

    //TODO: Save all
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