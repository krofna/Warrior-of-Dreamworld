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
#include "Player.h"

Player::Player(sf::TcpSocket* Socket, const std::string& Username, const std::string& Password, Uint16 MapID, Uint16 x, Uint16 y) :
Socket(Socket),
Username(Username),
Password(Password),
MapID(MapID),
WorldObject(x, y)
{
}

Player::~Player()
{
    delete Socket;
}

void Player::Update(Int32 diff)
{
}