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

#include "Player.hpp"
#include "../shared/WorldPacket.hpp"
#include "ObjectMgr.hpp"
#include "boost/asio.hpp"

class Player;
class WorldPacket;

typedef boost::asio::ip::tcp::socket Socket;
typedef boost::shared_ptr<boost::asio::ip::tcp::socket> SocketPtr;

class WorldSession
{
    friend class WorldAcceptor;
public:
    WorldSession(boost::asio::io_service& io);
    ~WorldSession();

    void Start();
    void Send(WorldPacket& Packet);

    void SendLogOutPacket();

    // Opcode handlers
    void HandleNULL();
    void HandleMoveObjectOpcode();
    void HandleCastSpellOpcode();
    void HandleLogOutOpcode();
    void HandleLoginOpcode();

    PlayerPtr GetPlayer();

private:
    void HandleSend(char* Data, const boost::system::error_code& Error);
    void HandleReceive(const boost::system::error_code& Error);
    void HandleHeader();
    
    void SendLoginFailPacket(uint16 Reason);

    Socket Socket;

    uint16 Header[2];

    WorldPacket Packet;
    PlayerPtr pPlayer;
};

#endif
