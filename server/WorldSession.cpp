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
#include "../shared/Log.hpp"

WorldSession::WorldSession(boost::asio::io_service& io) :
Socket                    (io),
Packet                    ((uint16)MSG_NULL),
Connected(true)
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
    boost::asio::async_read(Socket,
        boost::asio::buffer(Header, 4),
        boost::bind(&WorldSession::HandleHeader, this));
}

void WorldSession::HandleHeader()
{
    uint16 Size = Header[0];
    Packet.SetOpcode(Header[1]);
    Packet.Resize(Size);
    boost::asio::async_read(Socket,
        boost::asio::buffer(Packet.GetByteBuffer(), Size),
        boost::bind(&WorldSession::HandleReceive, this, boost::asio::placeholders::error));
}

void WorldSession::HandleReceive(const boost::system::error_code& Error)
{
    Packet.ResetReadPos();
    if(Error)
    {
        sLog.Write("Failed to receive packet");
        return; // This has effect of disconnecting - TODO: do proper cleanup
    }
    if(Packet.GetOpcode() >= MSG_COUNT)
    {
        sLog.Write("Received %u: Bad opcode!", Packet.GetOpcode());
        return; // This has effect of disconnecting - TODO: do proper cleanup
    }
    sLog.Write("Received Packet: %s, ", OpcodeTable[Packet.GetOpcode()].name);

    (this->*OpcodeTable[Packet.GetOpcode()].Handler)();
    if (Connected)
        Start();
}

void WorldSession::Send(WorldPacket& Packet)
{
    sLog.Write("Sending Packet: %s, ", OpcodeTable[Packet.GetOpcode()].name);

    MessageQueue.push(Packet.GetData());
    if(MessageQueue.size() == 1)
    {
        boost::asio::async_write(Socket,
            boost::asio::buffer(MessageQueue.front(), Packet.GetSize() + WorldPacket::HEADER_SIZE),
            boost::bind(&WorldSession::HandleSend, this, MessageQueue.front(), boost::asio::placeholders::error));
    }
}

void WorldSession::HandleSend(char* Data, const boost::system::error_code& Error)
{
    if(!Error)
    {
        sLog.Write("Successful!");
    }
    else
    {
        sLog.Write("Failed!");
    }

    delete Data;

    MessageQueue.pop();
    if(!MessageQueue.empty())
    {
        boost::asio::async_write(Socket,
            boost::asio::buffer(MessageQueue.front(), *(uint16*)MessageQueue.front() + WorldPacket::HEADER_SIZE),
            boost::bind(&WorldSession::HandleSend, this, MessageQueue.front(), boost::asio::placeholders::error));
    }
}

void WorldSession::SendLoginFailPacket(uint16 Reason)
{
    Packet.Clear();
    Packet.SetOpcode((uint16)MSG_LOGIN);
    Packet << Reason;
    Send(Packet);
    Socket.close();
}

void WorldSession::SendChatMessage(uint64 FromID, std::string const& Message)
{
    Packet.Clear();

    Packet.SetOpcode((uint16)MSG_CHAT_MESSAGE);
    Packet << FromID << Message;
    Send(Packet);
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
    sLog.Write("Packet is good!");
}

void WorldSession::HandleMoveObjectOpcode()
{
    uint8 Direction;
    Packet >> Direction;

    sLog.Write("Packet is good!");

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

    SpellPtr pSpell = sObjectMgr.GetSpell(SpellID);

    if(!pSpell)
    {
        sLog.Write("Invalid Spell ID!");
        return;
    }

    sLog.Write("Packet is good!");

    pPlayer->CastSpell(pSpell, Angle);
}

void WorldSession::HandleLogOutOpcode()
{
    pPlayer->LogOut();
    Connected = false;
}

void WorldSession::HandleChatMessageOpcode()
{
    std::string Message;
    Packet >> Message;

    if (!pPlayer->CanSpeak())
    {
        SendNotification("You can't speak !");
        return;
    }

    pPlayer->Say(Message.c_str());
}

void WorldSession::SendLogOutPacket()
{
    Packet.Clear();
    Packet.SetOpcode((uint16)MSG_LOG_OUT);
    Send(Packet);
}

void WorldSession::SendNotification(std::string const& NotificationMessage)
{
    Packet.Clear();
    Packet.SetOpcode((uint16)MSG_SYSTEM_MESSAGE);

    Packet << NotificationMessage;
    Send(Packet);
}
