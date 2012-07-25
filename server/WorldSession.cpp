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
#include <iostream>

WorldSession::WorldSession(sf::TcpSocket* pSocket, Player* pPlayer) :
pSocket(pSocket),
pPlayer(pPlayer)
{
}

WorldSession::~WorldSession()
{
    delete pSocket;
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
        (this->*OpcodeTable[Opcode].Handler)();
    }
}

void WorldSession::SendPacket(sf::Packet& Packet)
{
    Packet >> Opcode;
    printf("Sent: %s\n", OpcodeTable[Opcode].name);
    pSocket->send(Packet);
}

void WorldSession::HandleNULL()
{
    // This is used as a placeholder for opcodes
    // which are handled in a special way, such as
    // MSG_LOGIN. (See: AuthSession, OpcodeHandler)
}

void WorldSession::HandleMoveObjectOpcode()
{
    Uint8 Direction;
    Packet >> Direction;

    RETURN_IF_PACKET_TOO_BIG

    printf("Packet is good!\n");

    // If player colided, return
    if(!pPlayer->UpdateCoordinates(Direction))
        return;
    
    // Send movement update to all players in the map
    Packet.clear();
    Packet << (Uint16)MSG_MOVE_OBJECT << pPlayer->GetObjectID() << Direction;
    sWorld->Maps[pPlayer->GetMapID()]->SendToPlayers(Packet);
}

void WorldSession::HandleCastSpellOpcode()
{
    Uint16 SpellID;
    float Angle;
    Packet >> SpellID >> Angle;

    RETURN_IF_PACKET_TOO_BIG

    Spell* pSpell = sObjectMgr->GetSpell(SpellID);

    if(pSpell == nullptr)
    {
        printf("Invalid Spell ID!\n");
        return;
    }

    printf("Packet is good!\n");

    //if(pPlayer->CanCastSpell(SpellID))
    {
        Packet.clear();
        switch(pSpell->Effect)
        {
        case SPELL_BOLT:
            Packet << (Uint16)MSG_CAST_SPELL << (Uint16)SPELL_BOLT << pPlayer->GetObjectID() << pSpell->DisplayID << Angle << pPlayer->GetMap()->NewSpellBoxID;
            pPlayer->GetMap()->SendToPlayers(Packet);
            pPlayer->GetMap()->AddSpell(pPlayer, pSpell, Angle);
            break;
        }
    }
}

void WorldSession::HandleTextMessageOpcode()
{
    std::string Message;
    Packet >> Message;

    RETURN_IF_PACKET_TOO_BIG

    Packet.clear();
    Packet << (Uint16)MSG_SEND_TEXT << pPlayer->GetObjectID() << Message;
    pPlayer->GetMap()->SendToPlayers(Packet);
}

void WorldSession::HandleLogOutOpcode()
{
    pPlayer->LogOut();
}

void WorldSession::SendLogOutPacket()
{
    Packet.clear();
    Packet << (Uint16)MSG_LOG_OUT;
    SendPacket(Packet);
}