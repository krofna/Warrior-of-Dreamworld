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
#include "WorldSession.h"
#include "Opcodes.h"
#include "Defines.h"
#include "Game.h"
#include <cassert>

WorldSession::WorldSession(Game* pGame) :
pGame(pGame),
pWorld(NULL)
{
}

bool WorldSession::ConnectToServer()
{
    sf::Socket::Status Status = Socket.connect(sf::IpAddress::getLocalAddress(), 0xBEEF);
    Socket.setBlocking(false);
    return Status == sf::Socket::Status::Done;
}

void WorldSession::RecievePackets()
{
    while(Socket.receive(Packet) == sf::Socket::Status::Done)
    {
        Packet >> Opcode;
        if(Opcode > MSG_COUNT)
        {
            printf("Recieved %u: Bad opcode!\n", Opcode);
            continue;
        }
        printf("Recieved %u: ", Opcode);
        (this->*OpcodeTable[Opcode].Handler)(Packet);
    }
}

void WorldSession::SendPacket(sf::Packet& Packet)
{
    Socket.send(Packet);
}

void WorldSession::HandleLoginOpcode(sf::Packet& Packet)
{
    Uint16 Status;
    Packet >> Status;

    if(Status != (Uint16)LOGIN_SUCCESS)
    {
        printf("Login fail!\n");
        Socket.disconnect();
        return;
    }

    Uint16 MapID;
    
    Packet >> MapID;

    if(!Packet.endOfPacket())
    {
        printf("Packet is too big!\n");
        Socket.disconnect();
        return;
    }

    printf("Packet is good!\n");

    World* pWorld = new World();
    this->pWorld = pWorld;
    pWorld->LoadTileMap(MapID);
    pGame->ChangeState(pWorld);
}

void WorldSession::HandleAddObjectOpcode(sf::Packet& Packet)
{
    Uint16 x, y, tx, ty;
    std::string Tileset;
    Packet >> Tileset >> x >> y >> tx >> ty;

    if(!Packet.endOfPacket())
    {
        printf("Packet is too big!\n");
        return;
    }

    WorldObject* pNewObject = new WorldObject(Tileset, x, y, tx, ty);
    pWorld->WorldObjectMap.push_back(pNewObject);
    printf("Packet is good!\n");
}