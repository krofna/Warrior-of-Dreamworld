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
#include "../shared/Defines.hpp"
#include "../shared/WorldPacket.hpp"
#include "boost/asio.hpp"

class Player;
class WorldPacket;

class WorldSession
{
    friend class AuthSession;
public:
    WorldSession(boost::asio::io_service& io, Player* pPlayer);
    ~WorldSession();

    void Receive();
    void Send(WorldPacket& Packet);

    void SendLogOutPacket();

    // Opcode handlers
    void HandleNULL();
    void HandleMoveObjectOpcode();
    void HandleCastSpellOpcode();
    void HandleTextMessageOpcode();
    void HandleLogOutOpcode();

private:
    void HandleSend(uint16 Opcode);
    void HandleReceive();

    boost::asio::ip::tcp::socket Socket;

    WorldPacket Packet;
    Player* pPlayer;
};

#endif
