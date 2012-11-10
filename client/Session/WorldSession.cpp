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

#include "shared/Opcodes.hpp"
#include "shared/Log.hpp"

#include "Game.hpp"
#include "Login.hpp"
#include "World.hpp"

#include <boost/bind.hpp>

WorldSession::WorldSession() :
Socket                    (*Game::GetInstance()->GetIO()),
Resolver                  (*Game::GetInstance()->GetIO()),
pWorld                    (nullptr)
{
}

WorldSession::~WorldSession()
{
}

void WorldSession::Connect(std::string Ip, std::string Port)
{
    TCPResolver::query Query(Ip.c_str(), Port.c_str());
    TCPResolver::iterator Iterator = Resolver.resolve(Query);
    boost::asio::async_connect(Socket, Iterator, boost::bind(&WorldSession::Start, this));
}

void WorldSession::Start()
{
    Packet = WorldPacket((uint16)MSG_NULL);

    boost::asio::async_read(Socket,
        boost::asio::buffer(Packet.GetDataWithHeader(), WorldPacket::HEADER_SIZE),
        boost::bind(&WorldSession::HandleHeader, this, boost::asio::placeholders::error));
}

void WorldSession::HandleHeader(const boost::system::error_code& Error)
{
    Packet.ReadHeader();

    // Header only packet
    if(Packet.GetSizeWithoutHeader() < 1 || Error)
    {
        HandleReceive(Error);
        return;
    }

    boost::asio::async_read(Socket,
        boost::asio::buffer(Packet.GetDataWithoutHeader(), Packet.GetSizeWithoutHeader()),
        boost::bind(&WorldSession::HandleReceive, this, boost::asio::placeholders::error));
}

void WorldSession::HandleReceive(const boost::system::error_code& Error)
{
    Packet.ResetReadPos();
    if(Error)
    {
        sLog.Write("Failed to receive packet");
        return;
    }
    if(Packet.GetOpcode() >= MSG_COUNT)
    {
        sLog.Write("Received %u: Bad opcode!", Packet.GetOpcode());
        return;
    }
    sLog.Write("Received Packet: %s, ", OpcodeTable[Packet.GetOpcode()].name);

    (this->*OpcodeTable[Packet.GetOpcode()].Handler)();

    Start();
}

void WorldSession::Send(WorldPacket& Packet)
{
    sLog.Write("Sending Packet: %s, ", OpcodeTable[Packet.GetOpcode()].name);

    Packet.UpdateSizeData();

    MessageQueue.push(Packet);
    if(MessageQueue.size() == 1)
    {
        boost::asio::async_write(Socket,
            boost::asio::buffer(MessageQueue.front().GetDataWithHeader(), MessageQueue.front().GetSizeWithHeader()),
            boost::bind(&WorldSession::HandleSend, this, boost::asio::placeholders::error));
    }
}

void WorldSession::HandleSend(const boost::system::error_code& Error)
{
    if(!Error)
    {
        sLog.Write("Successful!");
    }
    else
    {
        sLog.Write("Failed!");
    }

    MessageQueue.pop();

    if(!MessageQueue.empty())
    {
        boost::asio::async_write(Socket,
            boost::asio::buffer(MessageQueue.front().GetDataWithHeader(), MessageQueue.front().GetSizeWithHeader()),
            boost::bind(&WorldSession::HandleSend, this, boost::asio::placeholders::error));
    }
}

void WorldSession::HandleNULL()
{
}

void WorldSession::HandleLoginOpcode()
{
    uint16 Status;
    Packet >> Status;

    if(Status != (uint16)LOGIN_SUCCESS)
    {
        sLog.Write("Login fail!");
        Socket.close();
        return;
    }

    uint32 MapID;
    uint64 MeID;
    Packet >> MapID >> MeID;
    
    World* pWorld = new World(MeID);
    this->pWorld = pWorld;
    WorldPacket Argv;
    Argv << MapID;
    pWorld->Load(std::move(Argv));
    sLog.Write("Packet is good!");
}

void WorldSession::HandleAddObjectOpcode()
{
    uint64 ObjID;
    Packet >> ObjID;
}

void WorldSession::HandleRemoveObjectOpcode()
{
    uint64 ObjID;
    Packet >> ObjID;
}

void WorldSession::HandleMoveObjectOpcode()
{
    uint64 ObjID;
    uint16 x, y;
    Packet >> ObjID >> x >> y;
}

void WorldSession::HandleCastSpellOpcode()
{
}

void WorldSession::HandleRemoveSpellOpcode()
{
    uint32 SpellBoxID;
    Packet >> SpellBoxID;
}

void WorldSession::HandleLogOutOpcode()
{
    GoToLoginScreen();
}

void WorldSession::HandleSystemMessageOpcode()
{
    std::string Message;
    Packet >> Message;

    sLog.Write("System Msg: %s", Message);
    sLog.Write("Packet is good!");
}

void WorldSession::HandleChatMessageOpcode()
{
    uint64 ObjID;
    std::string Message;

    Packet >> ObjID >> Message;
}

void WorldSession::HandleCommandReponseOpcode()
{
    std::string Message;
    Packet >> Message;
}

void WorldSession::HandleNotificationMessageOpcode()
{
    std::string Message;
    Packet >> Message;
}

void WorldSession::HandleTextEmoteOpcode()
{
    std::string Message;
    uint64 ObjectID;

    Packet >> ObjectID;
    Packet >> Message;
}

void WorldSession::HandleSwapItemOpcode()
{
    uint8 srcbag, dstbag, srcslot, dstslot;
    Packet >> srcbag >> dstbag >> srcslot >> dstslot;
}

void WorldSession::HandleDeleteItemOpcode()
{
    uint8 srcslot, count;
    Packet >> srcslot >> count;
}

void WorldSession::HandleCreateItemOpcode()
{
    uint8 dstslot, dstbag;
    uint64 entry;

    Packet >> dstslot >> dstbag >> entry;
}

void WorldSession::HandleUseItemOpcode()
{

}

void WorldSession::HandleEquipItemOpcode()
{

}

void WorldSession::HandleInventoryDataOpcode()
{
    // Trololol?
    return;
    
    for (uint8 i = 0 ; i < 4 ; ++i)
    {
        uint8 bagStatus;
        Packet >> bagStatus;
        //if (bagStatus != BAG_USED)
            //continue;

        uint64 bagEntry;
        uint8 bagSlots;
        Packet >> bagEntry >> bagSlots;

        //pWorld->GetInventory()->Create(i, bagEntry);
        for (uint8 k = 0 ; k < bagSlots ; ++k)
        {
            uint8 itemStatus;
            //if (itemStatus != ITEM_USED)
                //continue;

            uint64 itemEntry;
            Packet >> itemEntry;

            //pWorld->GetInventory()->Create(i, k, itemEntry);
        }
    }
}

void WorldSession::HandleNpcInteractOpcode()
{
    uint64 Entry;
    Packet >> Entry;
}

void WorldSession::SendMovementRequest(uint8 Direction)
{
    WorldPacket Packet((uint16)MSG_MOVE_OBJECT);
    Packet << Direction;
    Send(Packet);
}

void WorldSession::SendAuthRequest(std::string Username, std::string Password)
{
    WorldPacket Packet((uint16)MSG_LOGIN);
    Packet << Username << Password;
    Send(Packet);
}

void WorldSession::SendCastSpellRequest(uint16 SpellID, float Angle)
{
    WorldPacket Packet((uint16)MSG_CAST_SPELL);
    Packet << SpellID << Angle;
    Send(Packet);
}

void WorldSession::SendAttackRequest()
{
    WorldPacket Packet((uint16)MSG_MELEE_ATTACK);
    Send(Packet);
}

void WorldSession::SendLogOutRequest()
{
    WorldPacket Packet((uint16)MSG_LOG_OUT);
    Send(Packet);
    GoToLoginScreen();
}

void WorldSession::SendChatMessage(std::string const& Message)
{
    WorldPacket Packet((uint16)MSG_CHAT_MESSAGE);
    Packet << Message;
    Send(Packet);
}

void WorldSession::GoToLoginScreen()
{
    Game::GetInstance()->PopAllStates();
    Game::GetInstance()->PushState(new Login());
    Game::GetInstance()->GetRenderWindow()->setView(Game::GetInstance()->GetRenderWindow()->getDefaultView());
}
