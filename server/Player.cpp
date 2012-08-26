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
#include "Player.hpp"
#include "../shared/Opcodes.hpp"
#include "Database.hpp"
#include "World.hpp"

Player::Player(std::string Username, std::string Password, uint64 ObjID) :
Unit         (ObjID),
pWorldSession(nullptr),
Username     (Username),
Password     (Password),
LoadedFromDB (false),
m_IsMute     (false)
{
}

Player::~Player()
{
}

// This is bugged cause there may someone already be on the tile player wants to spawn on
void Player::AddToWorld()
{
    //pMap->TileGrid[Position.y][Position.x] = this;
}

// PH: needs more work
void Player::RemoveFromWorld()
{
    pMap->TileGrid[Position.y][Position.x].reset();
    PlayerPtr me = static_pointer_cast<Player>(shared_from_this());
    pMap->RemovePlayer(me);
}

void Player::LoadFromDB()
{
    QueryResult Result(sDatabase.PQuery("SELECT `seclevel`, `tileset`, `tx`, `ty`, `mapid`, `x`, `y` FROM `players` WHERE guid='%llu'", ObjID));
    Result->next();

    SecLevel    = Result->getUInt(1);
    Tileset     = Result->getString(2);
    tx          = Result->getUInt(3);
    ty          = Result->getUInt(4);
    pMap        = sWorld->GetMap(Result->getUInt(5));
    Position.x  = Result->getUInt(6);
    Position.y  = Result->getUInt(7);

    sDatabase.PExecute("UPDATE `players` SET `online` = 1 WHERE `guid` = %llu", ObjID);

    LoadedFromDB = true;
}

bool Player::IsLoaded()
{
    return LoadedFromDB;
}

void Player::SpellHit(SpellBoxPtr pSpellBox)
{
    // PH, TODO: do player specific stuff
    Unit::SpellHit(pSpellBox);
}

void Player::CastSpell(SpellPtr pSpell, float Angle)
{
    // TODO: Uncomment this when player actually learns spells xD
    //if(!LearnedSpell(pSpell->ID))
        //return;

    Unit::CastSpell(pSpell, Angle);
}

bool Player::LearnedSpell(uint8 ID)
{
    for(auto SpellIter = Spells.begin(); SpellIter != Spells.end(); ++SpellIter)
    {
        if(*SpellIter == ID)
        {
            return true;
        }
    }
    return false;
}

void Player::Update(int32 diff)
{
}

void Player::SaveToDB()
{
    sDatabase.PExecute("UPDATE `players` SET x=%u, y=%u, mapid=%u WHERE guid=%u", Position.x, Position.y, pMap->MapID, GetObjectID());
}

WorldPacket Player::PackData()
{
    WorldPacket Packet((uint16)MSG_ADD_OBJECT);
    Packet  << GetObjectID() << Tileset << Username << GetX() << GetY() << tx << ty;
    return Packet;
}

void Player::SendPacket(WorldPacket Packet)
{
    pWorldSession->Send(Packet);
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
    LogOut();
}

void Player::LogOut()
{
    delete pWorldSession;
    pWorldSession = nullptr;
    this->RemoveFromWorld();
    this->SaveToDB();
}

bool Player::CanSpeak() const
{
    return !m_IsMute;
}
