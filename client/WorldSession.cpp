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
#include <cassert>
#include <cstring>

WorldSession::WorldSession(boost::asio::io_service& io, tcp::resolver::iterator Iterator, Game* sGame) :
pWorld                    (nullptr),
Socket                    (io),
Packet                    ((uint16)MSG_NULL),
sGame                     (sGame)
{
    boost::asio::async_connect(Socket, Iterator, boost::bind(&WorldSession::Start, this));
}

void WorldSession::Start()
{
    boost::asio::async_read(Socket, boost::asio::buffer(Header, 4), boost::bind(&WorldSession::HandleHeader, this));
}

void WorldSession::HandleHeader()
{
    Packet.Clear();
    Packet.Resize(Header[0]);
    boost::asio::async_read(Socket, boost::asio::buffer(Packet.GetData(), Header[0]), boost::bind(&WorldSession::HandlePacket, this));
}

void WorldSession::HandlePacket()
{
    if(Header[1] >= MSG_COUNT)
    {
        printf("Recieved %u: Bad opcode!\n", Header[1]);
        return;
    }
    printf("Recieved: %s, ", OpcodeTable[Header[1]].name);

    (this->*OpcodeTable[Header[1]].Handler)();
    Packet.Clear();

    Start();
}

void WorldSession::Send(WorldPacket& Packet)
{
    size_t PacketSize = Packet.GetSize();
    uint16 Opcode = Packet.GetOpcode();

    buffer.resize(PacketSize + WorldPacket::HEADER_SIZE);

    std::memcpy(&buffer[0], &PacketSize, 2);
    std::memcpy(&buffer[2], &Opcode, 2);
    std::memcpy(&buffer[4], Packet.GetData(), Packet.GetSize());
    
    boost::asio::async_write(Socket, boost::asio::buffer(buffer, buffer.size()), boost::bind(&WorldSession::HandleSend, this, Opcode));
}

void WorldSession::HandleSend(uint16 Opcode)
{
    printf("Sent: %s\n", OpcodeTable[Opcode].name);
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
        printf("Login fail!\n");
        Socket.close();
        return;
    }

    uint16 MapID;
    uint32 MeID;
    Packet >> MapID >> MeID;

    World* pWorld = new World(MeID);
    this->pWorld = pWorld;
    WorldPacket Argv(0);
    Argv << MapID;
    sGame->AddToLoadQueue(pWorld, Argv);
    printf("Packet is good!\n");
}

void WorldSession::HandleAddObjectOpcode()
{
    uint32 ObjID;
    Packet >> ObjID;

    WorldObject* pNewObject = new WorldObject;
    sGame->AddToLoadQueue(pNewObject, Packet);
    pWorld->AddObject(pNewObject, ObjID);
    printf("Packet is good!\n");
}

void WorldSession::HandleRemoveObjectOpcode()
{
    uint32 ObjID;
    Packet >> ObjID;

    pWorld->RemoveObject(ObjID);
}

void WorldSession::HandleMoveObjectOpcode()
{
    uint32 ObjID;
    uint16 x, y;
    Packet >> ObjID >> x >> y;

    pWorld->DrawingMutex.lock();
    pWorld->WorldObjectMap[ObjID]->UpdateCoordinates(x, y);
    pWorld->DrawingMutex.unlock();
    printf("Packet is good!\n");
}

void WorldSession::HandleCastSpellOpcode()
{
    uint32 CasterID;
    Packet >> CasterID;
    Packet.UpdateWritePos();
    WorldObject* pCaster = pWorld->WorldObjectMap[CasterID];
    Packet << pCaster->GetPosition().x << pCaster->GetPosition().y;
    Animation* pAnim = new Animation;
    sGame->AddToLoadQueue(pAnim, Packet);
    pWorld->AddAnimation(pAnim);
    printf("Packet is good!\n");
}

void WorldSession::HandleLogOutOpcode()
{
    sGame->AddToLoadQueue(new Login(), WorldPacket(0));
}

void WorldSession::SendMovementRequest(uint8 Direction)
{
    Packet.Clear();
    Packet.SetOpcode((uint16)MSG_MOVE_OBJECT);
    Packet << Direction;
    Send(Packet);
}

void WorldSession::SendAuthRequest(std::string Username, std::string Password)
{
    Packet.Clear();
    Packet.SetOpcode((uint16)MSG_LOGIN);
    Packet << Username << Password;
    Send(Packet);
}

void WorldSession::SendCastSpellRequest(uint16 SpellID, float Angle)
{
    Packet.Clear();
    Packet.SetOpcode((uint16)MSG_CAST_SPELL);
    Packet << SpellID << Angle;
    Send(Packet);
}

void WorldSession::SendLogOutRequest()
{
    Packet.Clear();
    Packet.SetOpcode((uint16)MSG_LOG_OUT);
    Send(Packet);

    // Back to login screen?
    //sGame->ChangeState(new Login());
    //Window->setView(Window->getDefaultView());
}