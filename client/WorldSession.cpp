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

WorldSession::WorldSession(boost::asio::io_service& io, tcp::resolver::iterator Iterator) :
pWorld                    (nullptr),
Socket                    (io),
Packet                    ((uint16)MSG_NULL)
{
    boost::asio::async_connect(Socket, Iterator, boost::bind(&WorldSession::HandleConnect, this));
}

void WorldSession::HandleConnect()
{
    boost::asio::async_read(Socket, boost::asio::buffer(Header, 4), boost::bind(&WorldSession::HandleHeader, this));
}

void WorldSession::HandleHeader()
{
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
}

void WorldSession::SendPacket(WorldPacket& Packet)
{
    printf("Sent: %s\n", OpcodeTable[Packet.GetOpcode()].name);
    Socket.send(Packet);
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
        Socket.disconnect();
        return;
    }

    uint16 MapID;
    uint32 MeID;
    Packet >> MapID >> MeID;

    if(!Packet.endOfPacket())
    {
        printf("Packet is too big!\n");
        Socket.disconnect();
        return;
    }

    World* pWorld = new World(MeID);
    this->pWorld = pWorld;
    pWorld->LoadTileMap(MapID);
    sGame->ChangeState(pWorld);
    printf("Packet is good!\n");
}

void WorldSession::HandleAddObjectOpcode()
{
    uint16 x, y, tx, ty;
    uint32 ObjID;
    std::string Tileset, Username;
    Packet >> Tileset >> ObjID >> Username >> x >> y >> tx >> ty;

    WorldObject* pNewObject = new WorldObject(Tileset, Username, x, y, tx, ty);
    pWorld->WorldObjectMap[ObjID] = pNewObject;
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

    pWorld->WorldObjectMap[ObjID]->UpdateCoordinates(x, y);
    printf("Packet is good!\n");
}

void WorldSession::HandleCastSpellOpcode()
{
    float Angle;
    uint16 Effect, DisplayID;
    uint32 CasterID;
    uint32 SpellBoxID;
    Packet >> Effect >> CasterID >> DisplayID >> Angle >> SpellBoxID;

    WorldObject* pCaster = pWorld->WorldObjectMap[CasterID];
    pWorld->Animations.push_back(Animation(DisplayID, pCaster->GetPosition(), Angle, SpellBoxID));

    printf("Packet is good!\n");
}

void WorldSession::HandleLogOutOpcode()
{
    // [PH] TODO: Back to login screen, this is pretty nasty
    Window.close();
}

void WorldSession::HandleSpellHitOpcode()
{/*
    uint32 SpellBoxID, VictimID;
    Packet >> SpellBoxID >> VictimID;

    for(auto SpellBoxIter = pWorld->Animations.begin(); SpellBoxIter != pWorld->Animations.end(); ++SpellBoxIter)
    {
        if(SpellBoxIter->GetID() == SpellBoxID)
        {
            pWorld->Animations.erase(SpellBoxIter);
            break;
        }
    }

    //PH, dont delete, instead, die
    delete pWorld->WorldObjectMap[VictimID];
    pWorld->WorldObjectMap.erase(VictimID);
    printf("Packet is good!\n");*/
}

void WorldSession::SendMovementRequest(uint8 Direction)
{
    Packet << (uint16)MSG_MOVE_OBJECT << Direction;
    SendPacket(Packet);
}

void WorldSession::SendAuthRequest(std::string Username, std::string Password)
{
    Packet << (uint16)MSG_LOGIN << Username << Password;
    SendPacket(Packet);
}

void WorldSession::SendCastSpellRequest(uint16 SpellID, float Angle)
{
    Packet << (uint16)MSG_CAST_SPELL << SpellID << Angle;
    SendPacket(Packet);
}

void WorldSession::SendLogOutRequest()
{
    Packet << (uint16)MSG_LOG_OUT;
    SendPacket(Packet);

    // Back to login screen?
    //sGame->ChangeState(new Login());
    //Window.setView(Window.getDefaultView());
}