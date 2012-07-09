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
#include "Map.h"
#include "Opcodes.h"

Map::Map(const Uint16 MapID) : MapID(MapID)
{
}

Map::~Map()
{
    for(auto ObjectIterator = MapObjects.begin(); ObjectIterator != MapObjects.end(); ++ObjectIterator)
    {
        delete (*ObjectIterator);
    }
}

void Map::Update(Int32 diff)
{
    for(auto ObjectIterator = MapObjects.begin(); ObjectIterator != MapObjects.end(); ++ObjectIterator)
    {
        (*ObjectIterator)->Update(diff);
    }

    for(auto PlayerIterator = Players.begin(); PlayerIterator != Players.end(); ++PlayerIterator)
    {
        (*PlayerIterator)->Update(diff);
    }
}

void Map::AddPlayer(Player* pPlayer)
{
    // Pack & send all data about world objects to new player
    for(auto ObjectIterator = MapObjects.begin(); ObjectIterator != MapObjects.end(); ++ObjectIterator)
    {
        pPlayer->SendPacket((*ObjectIterator)->PackData());
    }

    // Pack & send all data about players in map to new player
    // and send data about the new player to all other players
    for(auto PlayerIterator = Players.begin(); PlayerIterator != Players.end(); ++PlayerIterator)
    {
        pPlayer->SendPacket((*PlayerIterator)->PackData());
        ((*PlayerIterator)->SendPacket(pPlayer->PackData()));
    }

    // Send the new player his data
    pPlayer->SendPacket(pPlayer->PackData());

    // Finally, we add the new player to the map
    Players.push_back(pPlayer);
}

void Map::SendToPlayers(sf::Packet& Packet)
{
    for(auto PlayerIterator = Players.begin(); PlayerIterator != Players.end(); ++PlayerIterator)
    {
        (*PlayerIterator)->SendPacket(Packet);
    }
}

void Map::RemovePlayer(Player* pPlayer)
{
    sf::Packet Packet;
    Packet << (Uint16)MSG_REMOVE_OBJECT << pPlayer->GetObjectID();
    printf("Size: %u\n", Players.size());
    for(auto PlayerIterator = Players.begin(); PlayerIterator != Players.end();)
    {
        if((*PlayerIterator) == pPlayer)
        {
            printf("erasing");
            PlayerIterator = Players.erase(PlayerIterator);
        }
        else
        {
            printf("sending\n");
            (*PlayerIterator)->SendPacket(Packet);
            ++PlayerIterator;
        }
    }
    printf("done");
}