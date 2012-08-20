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
#ifndef WORLD_SESSION_H
#define WORLD_SESSION_H

#include "../shared/Hash/SHA512.hpp"
#include "../shared/WorldPacket.hpp"
#include "World.hpp"
#include "boost/asio.hpp"
#include "boost/thread.hpp"
using boost::asio::ip::tcp;

class Game;
class World;

class WorldSession
{
public:
    WorldSession(boost::asio::io_service& io, Game* sGame);

    void Connect(std::string Ip, std::string Port);

    void Send(WorldPacket& Packet);

    // Opcode handlers
    void HandleNULL();
    void HandleLoginOpcode();
    void HandleAddObjectOpcode();
    void HandleRemoveObjectOpcode();
    void HandleMoveObjectOpcode();
    void HandleCastSpellOpcode();
    void HandleTextMessageOpcode();
    void HandleLogOutOpcode();

    // Requests
    void SendAuthRequest(std::string Username, std::string Password);
    void SendMovementRequest(uint8 Direction);
    void SendCastSpellRequest(uint16 SpellID, float Angle);
    void SendLogOutRequest();

private:
    void Start();
    void HandlePacket(const boost::system::error_code& Error);
    void HandleHeader();
    void HandleSend(char* Data, const boost::system::error_code& Error);

    tcp::socket Socket;
    uint16 Header[2];
    WorldPacket Packet;

    boost::shared_ptr<boost::asio::io_service::work> Work;
    tcp::resolver Resolver;

    Game* sGame;
    World* pWorld;
};

#endif
