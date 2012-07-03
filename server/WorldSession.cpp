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
#include "World.h"
#include "ObjectMgr.h"

WorldSession::WorldSession(sf::TcpSocket* pSocket, Player* pPlayer) :
pSocket(pSocket),
pPlayer(pPlayer)
{
}

void WorldSession::ReceivePackets()
{
    // Loop as long as there are packets to receive
    while(pSocket->receive(Packet) == sf::Socket::Status::Done)
    {
        // Extract the opcode and call the handler
        // function depending on the opcode recieved
        Packet >> Opcode;
        if(Opcode >= MSG_COUNT)
        {
            printf("Received %u: Bad opcode!\n", Opcode);
            continue;
        }
        printf("Received: %s, ", OpcodeTable[Opcode].name);
        (this->*OpcodeTable[Opcode].Handler)(Packet);
    }
}

void WorldSession::SendPacket(sf::Packet& Packet)
{
    Packet >> Opcode;
    printf("Sent: %s\n", OpcodeTable[Opcode].name);
    pSocket->send(Packet);
}

void WorldSession::HandleNULL(sf::Packet& Packet)
{
    // This is used as a placeholder for opcodes
    // which are handled in a special way, such as
    // MSG_LOGIN. (See: AuthSession, OpcodeHandler)
}

void WorldSession::HandleMoveObjectOpcode(sf::Packet& Packet)
{
    Uint8 Direction;
    Packet >> Direction;

    if(!Packet.endOfPacket())
    {
        printf("Packet is too big!\n");
        return;
    }
    Packet.clear();

    printf("Packet is good!\n");

    // Todo: Collision

    pPlayer->UpdateCoordinates(Direction);
    
    // Send movement update to all players in the map
    Packet << (Uint16)MSG_MOVE_OBJECT << pPlayer->GetObjectID() << Direction;
    pWorld->Maps[pPlayer->GetMapID()]->SendToPlayers(Packet);
}

void WorldSession::HandleCastSpellOpcode(sf::Packet& Packet)
{
    Uint16 SpellID;
    Uint8 Direction;
    Packet >> SpellID >> Direction;

    if(!Packet.endOfPacket())
    {
        printf("Packet is too big!\n");
        return;
    }
    printf("Packet is good!\n");

    //if(pPlayer->CanCastSpell(SpellID))
    {
        Packet.clear();
        printf("switching");
        switch(sObjectMgr->GetSpell(SpellID)->Effect)
        {
        case SPELL_BOLT:
            printf("packing");
            Packet << (Uint16)MSG_CAST_SPELL << (Uint16)SPELL_BOLT << pPlayer->GetObjectID() << sObjectMgr->GetSpell(SpellID)->DisplayID << Direction;
            printf("sending");
            pWorld->Maps[pPlayer->GetMapID()]->SendToPlayers(Packet);
            break;
        }
    }
}