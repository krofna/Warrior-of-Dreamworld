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
#include "AuthSession.h"
#include "World.h"
#include "ObjectMgr.h"
#include <fstream>

AuthSession::AuthSession()
{
    Listener.listen(0xBEEF);
    Listener.setBlocking(false);
    NewSocket = new sf::TcpSocket();
}

AuthSession::~AuthSession()
{
    delete NewSocket;
}

void AuthSession::HandleAll()
{
    // As long as there are clients who want to connect
    // add new socket and set the blocking to false to
    // not pause server untill new packet is received
    while(Listener.accept(*NewSocket) == sf::Socket::Status::Done)
    {
        Sockets.push_back(NewSocket);
        NewSocket->setBlocking(false);
        NewSocket = new sf::TcpSocket();
    }

    sf::TcpSocket* pSocket;

    // We iterate through sockets who are not yet logged in, but
    // connected to the server to receive authentication requests
    for(auto SocketIterator = Sockets.begin(); SocketIterator != Sockets.end();)
    {
        pSocket = (*SocketIterator);

        if(pSocket->receive(AuthPacket) == sf::Socket::Status::Done)
        {
            AuthPacket >> Opcode;
            if(Opcode != (Uint16)MSG_LOGIN)
                continue;

            // Check if username exists
            AuthPacket >> Username;
            Player* pPlayer = sObjectMgr->GetPlayer(Username);

            if(pPlayer == nullptr)
            {
                // Invalid username, send response
                AuthPacket.clear();
                AuthPacket << (Uint16)MSG_LOGIN << (Uint16)LOGIN_FAIL_BAD_USERNAME;
                pSocket->send(AuthPacket);
                pSocket->disconnect();
                delete pSocket;
                SocketIterator = Sockets.erase(SocketIterator);
                continue;
            }

            // Check if passwords match
            AuthPacket >> Password;
            if(pPlayer->Password != Password)
            {
                // Invalid password, send response
                AuthPacket.clear();
                AuthPacket << (Uint16)MSG_LOGIN << (Uint16)LOGIN_FAIL_BAD_PASSWORD;
                pSocket->send(AuthPacket);
                pSocket->disconnect();
                delete pSocket;
                SocketIterator = Sockets.erase(SocketIterator);
                continue;
            }

            // If player is online, kick him
            pPlayer->LogOut();

            // Tell the client that he logged in sucessfully
            AuthPacket.clear();
            AuthPacket << (Uint16)MSG_LOGIN << (Uint16)LOGIN_SUCCESS << pPlayer->GetMapID();
            pSocket->send(AuthPacket);

            // Create new WorldSession for the player
            sWorld->AddSession(pSocket, pPlayer);

            // Remove pointer to socket
            SocketIterator = Sockets.erase(SocketIterator);
        }
        else
        {
            ++SocketIterator;
        }
    }
}