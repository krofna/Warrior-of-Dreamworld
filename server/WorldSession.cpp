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
#include "WorldSession.hpp"
#include "../shared/Opcodes.hpp"
#include "World.hpp"
#include "boost/bind.hpp"
#include "ObjectMgr.hpp"
#include <iostream>

WorldSession::WorldSession(boost::asio::io_service& io, Player* pPlayer) :
pPlayer                   (pPlayer),
Socket                    (io),
Packet                    (1)
{
}

WorldSession::~WorldSession()
{
}

void WorldSession::Receive()
{
    // TODO: make it non blocking, store header into vector<char> ByteBuffer
    // and make this nicer
    if(!Socket.available())
        return;

    uint16 Size, Opcode;
    boost::asio::read(Socket, boost::asio::buffer(&Size, 2));
    boost::asio::read(Socket, boost::asio::buffer(&Opcode, 2));
    
    Packet = WorldPacket(Size);
    boost::asio::async_read(Socket, Packet.GetData(), boost::bind(&WorldSession::HandleReceive, this));
}

void WorldSession::HandleReceive()
{
    if(Packet.GetOpcode() >= MSG_COUNT)
    {
        printf("Received %u: Bad opcode!\n", Packet.GetOpcode());
    }
    printf("Received: %s, ", OpcodeTable[Packet.GetOpcode()].name);

    (this->*OpcodeTable[Packet.GetOpcode()].Handler)();
}

void WorldSession::Send(WorldPacket& Packet)
{
    size_t PacketSize = Packet.GetSize();
    uint16 Opcode = Packet.GetOpcode();

    std::vector<char> buffer(PacketSize + WorldPacket::HEADER_SIZE);

    std::memcpy(&buffer[0], &PacketSize, 2);
    std::memcpy(&buffer[2], &Opcode, 2);
    std::memcpy(&buffer[4], Packet.GetData(), Packet.GetSize());

    boost::asio::write(Socket, boost::asio::buffer(buffer), boost::bind(&WorldSession::HandleSend, this, Opcode));
}

void WorldSession::HandleSend(uint16 Opcode)
{
    std::cout << "SENT: " << OpcodeTable[Opcode].name << "\n";
}

void WorldSession::HandleNULL()
{
    // This is used as a placeholder for opcodes
    // which are handled in a special way, such as
    // MSG_LOGIN. (See: AuthSession, OpcodeHandler)
}

void WorldSession::HandleMoveObjectOpcode()
{
    uint8 Direction;
    Packet >> Direction;

    printf("Packet is good!\n");

    // If player colided, return
    if(!pPlayer->UpdateCoordinates(Direction))
        return;
    
    // Send movement update to all players in the map
    Packet.Clear();
    Packet.SetOpcode((uint16)MSG_MOVE_OBJECT);
    Packet << pPlayer->GetObjectID() << pPlayer->GetX() << pPlayer->GetY();
    sWorld->Maps[pPlayer->GetMapID()]->SendToPlayers(Packet);
}

void WorldSession::HandleCastSpellOpcode()
{
    uint16 SpellID;
    float Angle;
    Packet >> SpellID >> Angle;

    Spell* pSpell = sObjectMgr.GetSpell(SpellID);

    if(pSpell == nullptr)
    {
        printf("Invalid Spell ID!\n");
        return;
    }

    printf("Packet is good!\n");

    pPlayer->CastSpell(pSpell, Angle);
}

void WorldSession::HandleTextMessageOpcode()
{
    std::string Message;
    Packet >> Message;

    Packet.Clear();
    Packet.SetOpcode((uint16)MSG_SEND_TEXT);
    Packet << pPlayer->GetObjectID() << Message;
    pPlayer->GetMap()->SendToPlayers(Packet);
}

void WorldSession::HandleLogOutOpcode()
{
    pPlayer->LogOut();
}

void WorldSession::SendLogOutPacket()
{
    Packet.Clear();
    Packet.SetOpcode((uint16)MSG_LOG_OUT);
    Send(Packet);
}
