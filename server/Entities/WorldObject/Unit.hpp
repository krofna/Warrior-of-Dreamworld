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
#ifndef UNIT_H
#define UNIT_H

#include "WorldObject.hpp"
#include <map>

class SpellBox;
struct SpellTemplate;

class WOD_DLL_DECL Unit : public WorldObject
{
public:
    Unit(uint64 ObjID);
    virtual ~Unit() { sUnits.erase(GetObjectID()); } 

    // **************************
    // Static functions
    // **************************
    
    static Unit* GetUnitByObjectID(uint64 ObjID);


    virtual void Update(int64 diff) = 0;

    virtual WorldPacket PackData() = 0;

    virtual void Say(const char* Text);

    virtual void SpellHit(SpellBox* pSpellBox);
    virtual void CastSpell(SpellTemplate* pSpell, float Angle);
    void CastSpell(uint16 Entry, Unit* pVictim);
    void CastSpell(uint16 Entry, float Angle);

    void Teleport(int x, int y);
    virtual void Teleport(Vector2i const& Pos);

    virtual int32 GetMeleeDamage() const;
    virtual void DealDamage(int32 Damage, Unit* pTarget);
    virtual void TakeDamage(int32 Damage, Unit* pAttacker);
    
    //NOTE: THIS FUNCTION IS NOT TO BE FUCKED WITH
    void ClearVictim() { this->pVictim = nullptr; }

    void DoMeleeAttackIfReady(int64 diff);
    void Kill();

    bool IsAlive() const { return Health > 0; }
    bool IsDead() const { return !IsAlive(); }
    
    // This one is currently called by Player
    virtual void UpdatePosition(Vector2i const& Position) { WorldObject::UpdatePosition(Position); }
    virtual void OnInteract(Player* pWho) {}

    virtual bool IsInWorld() const { return true; /*??*/ }

    Unit* GetVictim();

protected:
    virtual void OnChat(const char* Text) = 0;

protected:
    static std::map<uint64, Unit* > sUnits;

protected:
    uint32 Health;
    uint32 Power;
    int64 MeeleAttackCooldown;

    Unit* pVictim;
};

#endif
