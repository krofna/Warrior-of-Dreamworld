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
#include "../shared/Defines.hpp"
#include "Game.hpp"
#include "Login.hpp"
#include "boost/bind.hpp"
#include <cstring>

WorldSession::WorldSession(boost::asio::io_service& io, Game* sGame) :
Socket                    (io),
Packet                    ((uint16)MSG_NULL),
sGame                     (sGame),
pWorld                    (nullptr),
Resolver                  (io),
Work                      (new boost::asio::io_service::work(io))
{
}

void WorldSession::Connect(std::string Ip, std::string Port)
{
    tcp::resolver::query Query(Ip.c_str(), Port.c_str());
    tcp::resolver::iterator Iterator = Resolver.resolve(Query);
    boost::asio::async_connect(Socket, Iterator, boost::bind(&WorldSession::Start, this));
}

void WorldSession::Start()
{
    boost::asio::async_read(Socket,
        boost::asio::buffer(Packet.GetDataWithHeader(), WorldPacket::HEADER_SIZE),
        boost::bind(&WorldSession::HandleHeader, this));
}

void WorldSession::HandleHeader()
{
    Packet.ReadHeader();
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

    boost::mutex::scoped_lock lock(MessageQueueMutex);

    MessageQueue.push(Packet.GetDataWithHeader());
    if(MessageQueue.size() == 1)
    {
        boost::asio::async_write(Socket,
            boost::asio::buffer(MessageQueue.front(), Packet.GetSizeWithHeader()),
            boost::bind(&WorldSession::HandleSend, this, MessageQueue.front(), boost::asio::placeholders::error));
    }
}

void WorldSession::HandleSend(void* Data, const boost::system::error_code& Error)
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

    boost::mutex::scoped_lock lock(MessageQueueMutex);
    MessageQueue.pop();

    if(!MessageQueue.empty())
    {
        boost::asio::async_write(Socket,
            boost::asio::buffer(MessageQueue.front(), *(uint16*)MessageQueue.front() + WorldPacket::HEADER_SIZE),
            boost::bind(&WorldSession::HandleSend, this, MessageQueue.front(), boost::asio::placeholders::error));
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

    uint16 MapID;
    uint64 MeID;
    Packet >> MapID >> MeID;

    World* pWorld = new World(MeID);
    this->pWorld = pWorld;
    WorldPacket Argv(0);
    Argv << MapID;
    sGame->AddToLoadQueue(pWorld, Argv);
    sLog.Write("Packet is good!");
}

void WorldSession::HandleAddObjectOpcode()
{
    uint64 ObjID;
    Packet >> ObjID;
    WorldObject* pNewObject = new WorldObject;
    sGame->AddToLoadQueue(pNewObject, Packet);
    pWorld->AddObject(pNewObject, ObjID);
    sLog.Write("Packet is good!");
}

void WorldSession::HandleRemoveObjectOpcode()
{
    uint64 ObjID;
    Packet >> ObjID;

    pWorld->RemoveObject(ObjID);
}

void WorldSession::HandleMoveObjectOpcode()
{
    uint64 ObjID;
    uint16 x, y;
    Packet >> ObjID >> x >> y;

    pWorld->DrawingMutex.lock();
    pWorld->WorldObjectMap[ObjID]->UpdateCoordinates(x, y);
    pWorld->DrawingMutex.unlock();
    sLog.Write("Packet is good!");
}

void WorldSession::HandleCastSpellOpcode()
{
    uint64 CasterID;
    Packet >> CasterID;
    Packet.UpdateWritePos();
    pWorld->DrawingMutex.lock();
    WorldObject* pCaster = pWorld->WorldObjectMap[CasterID];
    pWorld->DrawingMutex.unlock();
    Packet << pCaster->GetPosition().x << pCaster->GetPosition().y;
    Animation* pAnim = new Animation;
    sGame->AddToLoadQueue(pAnim, Packet);
    pWorld->AddAnimation(pAnim);
    sLog.Write("Packet is good!");
}

void WorldSession::HandleLogOutOpcode()
{
    sGame->AddToLoadQueue(new Login(), WorldPacket(0));
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

    pWorld->ReceiveNewMessage(ObjID, Message);
    sLog.Write("Packet is good!");
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

    //EncryptSHA512(Password);

    Packet << Username << Password;
    Send(Packet);
}

void WorldSession::SendCastSpellRequest(uint16 SpellID, float Angle)
{
    WorldPacket Packet((uint16)MSG_CAST_SPELL);
    Packet << SpellID << Angle;
    Send(Packet);
}

void WorldSession::SendLogOutRequest()
{
    WorldPacket Packet((uint16)MSG_LOG_OUT);
    Send(Packet);

    // Back to login screen?
    //sGame->ChangeState(new Login());
    //Window->setView(Window->getDefaultView());
}
