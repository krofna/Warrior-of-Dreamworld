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
#include "Opcodes.h"
#include "World.h"
#include <fstream>

AuthSession::AuthSession()
{
    Listener.listen(0xBEEF);
    Listener.setBlocking(false);
    NewSocket = new sf::TcpSocket();
    LoadOfflinePlayers();
}

AuthSession::~AuthSession()
{
    delete NewSocket;
}

void AuthSession::HandleAll()
{
    while(Listener.accept(*NewSocket) == sf::Socket::Status::Done)
    {
        Sockets.push_back(NewSocket);
        NewSocket->setBlocking(false);
        NewSocket = new sf::TcpSocket();
    }

    sf::TcpSocket* pSocket;

    for(auto SocketIterator = Sockets.begin(); SocketIterator != Sockets.end();)
    {
        pSocket = (*SocketIterator);

        if(pSocket->receive(AuthPacket) == sf::Socket::Status::Done)
        {
            AuthPacket >> Opcode;
            if(Opcode != (Uint16)MSG_LOGIN)
                continue;

            AuthPacket >> Username;
            auto Iterator = OfflinePlayers.find(Username);
            if(Iterator == OfflinePlayers.end())
            {
                AuthPacket.clear();
                AuthPacket << (Uint16)MSG_LOGIN << (Uint16)LOGIN_FAIL_BAD_USERNAME;
                pSocket->send(AuthPacket);
                pSocket->disconnect();
                delete pSocket;
                SocketIterator = Sockets.erase(SocketIterator);
                continue;
            }

            AuthPacket >> Password;
            Player* pPlayer = Iterator->second;
            if(pPlayer->Password != Password)
            {
                AuthPacket.clear();
                AuthPacket << (Uint16)MSG_LOGIN << (Uint16)LOGIN_FAIL_BAD_PASSWORD;
                pSocket->send(AuthPacket);
                pSocket->disconnect();
                delete pSocket;
                SocketIterator = Sockets.erase(SocketIterator);
                continue;
            }

            AuthPacket.clear();
            AuthPacket << (Uint16)MSG_LOGIN << (Uint16)LOGIN_SUCCESS << pPlayer->MapID << pPlayer->x << pPlayer->y;
            pSocket->send(AuthPacket);
            pPlayer->Socket = pSocket;
            pWorld->Maps[pPlayer->MapID]->MapObjects.push_back(pPlayer);
            pWorld->pWorldSession->Sockets.push_back(pSocket);

            SocketIterator = Sockets.erase(SocketIterator);
        }
        else
        {
            ++SocketIterator;
        }
    }
}

void AuthSession::LoadOfflinePlayers()
{
    std::ifstream File("PlayerDB.txt");
    Uint16 MapID, x, y;

    File >> Username >> Password >> MapID >> x >> y;
    OfflinePlayers[Username] = new Player(NULL, Username, Password, MapID, x, y);
}