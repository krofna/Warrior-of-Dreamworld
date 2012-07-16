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
#include "Opcodes.h"
#include "Database.h"
#include "World.h"

Player::Player(std::string Username, std::string Password, Uint32 ObjID) :
Username     (Username),
Password     (Password),
WorldObject  (ObjID),
pWorldSession(nullptr),
LoadedFromDB (false)
{
}

Player::~Player()
{
}

// This is bugged cause there may someone already be on the tile player wants to spawn on
void Player::AddToWorld()
{
    pMap->TileGrid[y][x].pWorldObject = this;
}

// PH: needs more work
void Player::RemoveFromWorld()
{
    pMap->TileGrid[y][x].pWorldObject = nullptr;
    pMap->RemovePlayer(this);
}

void Player::LoadFromDB()
{
    // TODO: Not all columns.
    QueryResult Result(sDatabase.PQuery("SELECT * FROM `players` WHERE id='%u'", ObjID));
    Result->next();

    //ObjID       = Result->getInt(1);
    //Username    = Result->getString(2);
    //Password    = Result->getString(3);
    Tileset     = Result->getString(4);
    tx          = Result->getInt(5);
    ty          = Result->getInt(6);
    pMap        = sWorld->GetMap(Result->getInt(7));
    x           = Result->getInt(8);
    y           = Result->getInt(9);

    LoadedFromDB = true;
}

bool Player::IsLoaded()
{
    return LoadedFromDB;
}

bool Player::CanCastSpell(Uint8 ID)
{
    // TODO: Power checks etc etc
    for(auto SpellIter = Spells.begin(); SpellIter != Spells.end(); ++SpellIter)
    {
        if(*SpellIter == ID)
        {
            return true;
        }
    }
    return false;
}

void Player::Update(Int32 diff)
{
}

void Player::SaveToDB()
{
    sDatabase.PExecute("UPDATE `players` SET x=%u, y=%u, map=%u WHERE id=%u", x, y, pMap->MapID, GetObjectID());
}

sf::Packet Player::PackData()
{
    sf::Packet Packet;
    Packet << (Uint16)MSG_ADD_OBJECT << Tileset << GetObjectID() << Username << x << y << tx << ty;
    return Packet;
}

void Player::SendPacket(sf::Packet Packet)
{
    pWorldSession->SendPacket(Packet);
}

void Player::BindSession(WorldSession* pWorldSession)
{
    this->pWorldSession = pWorldSession;
}

bool Player::IsInWorld()
{
    return pWorldSession != nullptr;
}

void Player::Kick()
{
    pWorldSession->SendLogOutPacket();
    LogOut();
}

void Player::LogOut()
{
    delete pWorldSession;
    pWorldSession = nullptr;
    this->RemoveFromWorld();
    this->SaveToDB();
}