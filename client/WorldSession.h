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

#include <SFML/Network.hpp>
#include "World.h"

class Game;
class World;

class WorldSession
{
public:
    WorldSession(Game* pGame);

    bool ConnectToServer();
    void RecievePackets();
    void SendPacket(sf::Packet& Packet);

    // Opcode handlers
    void HandleLoginOpcode(sf::Packet& Packet);
    void HandleAddObjectOpcode(sf::Packet& Packet);
    void HandleMoveObjectOpcode(sf::Packet& Packet);
    void HandleCastSpellOpcode(sf::Packet& Packet);
    void HandleTextMessageOpcode(sf::Packet& Packet);

    // Requests
    void SendMovementRequest(Uint8 Direction);
    void SendAuthRequest(std::string Username, std::string Password);
    void SendTextMessage(std::string Message);

private:
    sf::TcpSocket Socket;
    sf::Uint16 Opcode;
    sf::Packet Packet;

    Game* pGame;
    World* pWorld;
};

#endif