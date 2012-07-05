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

#include "Player.h"

class Player;

class WorldSession
{
    friend class AuthSession;
public:
    WorldSession(sf::TcpSocket* pSocket, Player* pPlayer);
    ~WorldSession();

    void ReceivePackets();
    void SendPacket(sf::Packet& Packet);

    // Opcode handlers
    void HandleNULL();
    void HandleMoveObjectOpcode();
    void HandleCastSpellOpcode();
    void HandleTextMessageOpcode();
    void HandleLogOutOpcode();
    

private:
    Player* pPlayer;
    sf::TcpSocket* pSocket;
    sf::Packet Packet;
    Uint16 Opcode;
};

#endif