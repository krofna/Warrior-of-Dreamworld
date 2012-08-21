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
#include "Unit.hpp"
#include "WorldSession.hpp"
#include "Spell.hpp"

class WorldSession;

class WOD_DLL_DECL Player : public Unit
{
    friend class WorldSession;

public:
    Player(std::string Username, std::string Password, uint32 ObjID);
    ~Player();

    // TODO
    void AddToWorld();
    void RemoveFromWorld();

    void LoadFromDB();
    void SaveToDB();
    bool IsLoaded();

    void SpellHit(SpellBoxPtr pSpellBox);

    void CastSpell(SpellPtr pSpell, float Angle);

    void Update(int32 diff);

    void SendPacket(WorldPacket Packet);
    WorldPacket PackData();
    void BindSession(WorldSession* pWorldSession);

    void LogOut();
    void Kick();
    bool IsInWorld();

    std::string GetUsername() { return Username; }

private:
    WorldSession* pWorldSession;

    std::string Username;
    std::string Password;

    std::string Tileset;
    uint16 tx;
    uint16 ty;

    std::vector<uint16> Spells;

    bool LearnedSpell(uint8 ID);

    bool LoadedFromDB;
};

#endif
