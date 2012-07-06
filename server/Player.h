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
#ifndef PLAYER_H
#define PLAYER_H

#include <string>
#include "../client/Defines.h"
#include "WorldObject.h"
#include "WorldSession.h"
#include <SFML/Network.hpp>
#include "Spell.h"

class WorldSession;

class Player : public WorldObject
{
    friend class AuthSession;

public:
    Player
        (
        const std::string& Username, 
        const std::string& Password, 
        const std::string& Tileset,
        Map* pMap, Uint32 ObjID,
        Uint16 x, Uint16 y,
        Uint16 tx, Uint16 ty
        );

    ~Player();

    bool CanCastSpell(Uint8 ID);

    void Update(Int32 diff);
    void SaveToDB();

    void SendPacket(sf::Packet Packet);
    sf::Packet PackData();
    void BindSession(WorldSession* pWorldSession);

    void LogOut();
    void Kick();

private:
    WorldSession* pWorldSession;

    std::string Username;
    std::string Password;

    std::vector<Uint16> Spells;
};

#endif