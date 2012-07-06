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
    ConnectToServer();
}

bool WorldSession::ConnectToServer()
{
    sf::Socket::Status Status = Socket.connect(sf::IpAddress::getLocalAddress(), 0xBEEF);
    Socket.setBlocking(false);
    return Status == sf::Socket::Status::Done;
}

void WorldSession::RecievePackets()
{
    // Loop as long as there are packets to receive from server
    while(Socket.receive(Packet) == sf::Socket::Status::Done)
    {
        Packet >> Opcode;
        if(Opcode >= MSG_COUNT)
        {
            printf("Recieved %u: Bad opcode!\n", Opcode);
            continue;
        }
        printf("Recieved: %s, ", OpcodeTable[Opcode].name);
        // Handle the packet
        (this->*OpcodeTable[Opcode].Handler)();
    }
}

void WorldSession::SendPacket(sf::Packet& Packet)
{
    Packet >> Opcode;
    printf("Sent: %s\n", OpcodeTable[Opcode].name);
    Socket.send(Packet);
}

void WorldSession::HandleLoginOpcode()
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

    World* pWorld = new World();
    this->pWorld = pWorld;
    pWorld->LoadTileMap(MapID);
    pGame->ChangeState(pWorld);
    printf("Packet is good!\n");
}

void WorldSession::HandleAddObjectOpcode()
{
    Uint16 x, y, tx, ty;
    Uint32 ObjID;
    std::string Tileset, Username;
    Packet >> Tileset >> ObjID >> Username >> x >> y >> tx >> ty;

    if(!Packet.endOfPacket())
    {
        printf("Packet is too big!\n");
        return;
    }

    WorldObject* pNewObject = new WorldObject(Tileset, Username, x, y, tx, ty);
    pWorld->WorldObjectMap[ObjID] = pNewObject;
    printf("Packet is good!\n");
}

void WorldSession::HandleNULL()
{
}

void WorldSession::HandleMoveObjectOpcode()
{
    Uint32 ObjID;
    Uint8 Direction;
    Packet >> ObjID >> Direction;

    if(!Packet.endOfPacket())
    {
        printf("Packet is too big!\n");
        return;
    }

    pWorld->WorldObjectMap[ObjID]->UpdateCoordinates(Direction);
    printf("Packet is good!\n");
}

void WorldSession::HandleCastSpellOpcode()
{
    Uint8 Direction;
    Uint16 Effect, DisplayID;
    Uint32 ObjectID; // Caster
    Packet >> Effect >> ObjectID >> DisplayID >> Direction;

    if(!Packet.endOfPacket())
    {
        printf("Packet is too big!\n");
        return;
    }

    pWorld->CreateSpellEffect(ObjectID, Direction, DisplayID, Effect);
    printf("Packet is good!\n");
}

void WorldSession::HandleTextMessageOpcode()
{
    Uint32 ObjID;
    sf::Text textMessage;
    std::string Message, Username;
    Packet >> ObjID >> Message;

    textMessage.setString(Username + ": " + Message);
    textMessage.setCharacterSize(18);
    textMessage.setColor(sf::Color::Magenta);
    TextMessages.push_back(textMessage);
}

void WorldSession::HandleLogOutOpcode()
{
    // [PH] TODO: Back to login screen, this is pretty nasty
    Window.close();
}

void WorldSession::SendMovementRequest(Uint8 Direction)
{
    Packet << (Uint16)MSG_MOVE_OBJECT << Direction;
    SendPacket(Packet);
}

void WorldSession::SendAuthRequest(std::string Username, std::string Password)
{
    Packet << (Uint16)MSG_LOGIN << Username << Password;
    SendPacket(Packet);
}

// TODO: [PH]
void WorldSession::SendCastSpellRequest(Uint16 SpellID, Uint8 Direction)
{
    Packet << (Uint16)MSG_CAST_SPELL << SpellID << Direction;
    SendPacket(Packet);
}

void WorldSession::SendTextMessage(std::string Message)
{
    Packet << (Uint16)MSG_SEND_TEXT << Message;
    SendPacket(Packet);
}

void WorldSession::SendLogOutRequest()
{
    Packet << (Uint16)MSG_LOG_OUT;
    SendPacket(Packet);
}