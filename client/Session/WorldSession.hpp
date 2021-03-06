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

#include "shared/WorldPacket.hpp"
#include "World.hpp"
#include <boost/asio.hpp>
#include <queue>

class World;

typedef boost::asio::ip::tcp::socket TCPSocket;
typedef boost::asio::ip::tcp::resolver TCPResolver;

class WorldSession
{
public:
    static WorldSession* GetInstance();
    static void Create(boost::asio::io_service& io);
    static void Destroy();
    ~WorldSession();

    void Connect(std::string Ip, std::string Port);
    void Send(WorldPacket& Packet);

    // Opcode handlers
    void HandleNULL();
    
    void HandleLoginOpcode();

    void HandleAddObjectOpcode();
    void HandleRemoveObjectOpcode();
    void HandleMoveObjectOpcode();

    void HandleCastSpellOpcode();
    void HandleRemoveSpellOpcode();

    void HandleLogOutOpcode();

    void HandleSystemMessageOpcode();
    void HandleChatMessageOpcode();
    void HandleCommandReponseOpcode();
    void HandleNotificationMessageOpcode();
    void HandleTextEmoteOpcode();

    void HandleSwapItemOpcode();
    void HandleDeleteItemOpcode();
    void HandleCreateItemOpcode();
    void HandleUseItemOpcode();
    void HandleEquipItemOpcode();
    void HandleInventoryDataOpcode();

    void HandleNpcInteractOpcode();

    // Requests
    void SendAuthRequest(std::string Username, std::string Password);
    void SendMovementRequest(uint8 Direction);
    void SendCastSpellRequest(uint16 SpellID, float Angle);
    void SendAttackRequest();
    void SendLogOutRequest();
    void SendChatMessage(std::string const& Message);
    
    // Misc
    void GoToLoginScreen();

private:
    WorldSession(boost::asio::io_service& io);
    static WorldSession* pInstance;

    void Start();
    void HandleReceive(const boost::system::error_code& Error);
    void HandleHeader(const boost::system::error_code& Error);
    void HandleSend(const boost::system::error_code& Error);

    WorldPacket Packet;
    TCPSocket Socket;
    TCPResolver Resolver;

    World* pWorld;

    std::queue<WorldPacket> MessageQueue;
};

#endif
