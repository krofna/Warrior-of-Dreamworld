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
#include <iostream>

WorldSession::WorldSession(boost::asio::io_service& io) :
Socket                    (io),
Packet                    ((uint16)MSG_NULL)
{
}

WorldSession::~WorldSession()
{
}

PlayerPtr WorldSession::GetPlayer()
{
    return pPlayer;
}

void WorldSession::Start()
{
    // TODO: make it non blocking, store header into vector<char> ByteBuffer
    // and make this nicer
    uint16 Header[2];
    boost::asio::async_read(Socket, boost::asio::buffer(&Header, 4), boost::bind(&WorldSession::HandleHeader, this, Header));
}

void WorldSession::HandleHeader(uint16* Header)
{
    uint16 Size = Header[0];
    uint32 Opcode = Header[1];
    boost::asio::async_read(Socket, boost::asio::buffer(Packet.GetData(), Size), boost::bind(&WorldSession::HandleReceive, this));
}

void WorldSession::HandleReceive()
{
    if(Packet.GetOpcode() >= MSG_COUNT)
    {
        printf("Received %u: Bad opcode!\n", Packet.GetOpcode());
    }
    printf("Received: %s, ", OpcodeTable[Packet.GetOpcode()].name);

    (this->*OpcodeTable[Packet.GetOpcode()].Handler)();

    Start();
}

void WorldSession::Send(WorldPacket& Packet)
{
    size_t PacketSize = Packet.GetSize();
    uint16 Opcode = Packet.GetOpcode();

    std::vector<char> buffer(PacketSize + WorldPacket::HEADER_SIZE);

    std::memcpy(&buffer[0], &PacketSize, 2);
    std::memcpy(&buffer[2], &Opcode, 2);
    std::memcpy(&buffer[4], Packet.GetData(), Packet.GetSize());
    
    boost::asio::async_write(Socket, boost::asio::buffer(buffer, buffer.size()), boost::bind(&WorldSession::HandleSend, this, Opcode));
}

void WorldSession::HandleSend(uint16 Opcode)
{
    std::cout << "SENT: " << OpcodeTable[Opcode].name << "\n";
}

void WorldSession::SendLoginFailPacket(uint16 Reason)
{
    Packet.Clear();
    Packet.SetOpcode((uint16)MSG_LOGIN);
    Packet << Reason;
    Send(Packet);
    Socket.close();
}

void WorldSession::HandleNULL()
{
}

void WorldSession::HandleLoginOpcode()
{
    // Check if username exists
    std::string Username;
    Packet >> Username;
    pPlayer = sObjectMgr.GetPlayer(Username);
    if(!pPlayer)
    {
        // Invalid username, send response
        SendLoginFailPacket((uint16)LOGIN_FAIL_BAD_USERNAME);
        return;
    }

    // Check if passwords match
    std::string Password;
    Packet >> Password;
    if(pPlayer->Password != Password)
    {
        // Invalid password, send response
        SendLoginFailPacket((uint16)LOGIN_FAIL_BAD_PASSWORD);
        return;
    }

    // If player is online, kick him
    if(pPlayer->IsInWorld())
        pPlayer->Kick();
    else if(!pPlayer->IsLoaded())
        pPlayer->LoadFromDB();

    // Tell the client that he logged in sucessfully
    Packet.Clear();
    Packet.SetOpcode((uint16)MSG_LOGIN);
    Packet << (uint16)LOGIN_SUCCESS << pPlayer->GetMapID() << pPlayer->GetObjectID();
    Send(Packet);

    // Add player to the world
    pPlayer->BindSession(this);
    sWorld->AddSession(this);
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
