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

class Spell;
class SpellBox;

class WOD_DLL_DECL Unit : public WorldObject
{
public:
    Unit(uint64 ObjID);
    virtual ~Unit() { } 

    virtual void Update(int32 diff) = 0;

    virtual WorldPacket* PackData() = 0;

    virtual void Say(const char* Text);

    virtual void SpellHit(SpellBoxPtr pSpellBox);
    virtual void CastSpell(SpellPtr pSpell, float Angle);
    void CastSpell(uint16 Entry, Unit* pVictim);
    void CastSpell(uint16 Entry, float Angle);

    virtual int32 GetMeleeDamage() const;
    virtual void DealDamage(int32 Damage, Unit* pTarget);
    virtual void TakeDamage(int32 Damage, Unit* pAttacker);

    void DoMeleeAttackIfReady(int32 diff);
    void Kill();

    bool IsDead() const { return Health >= 1; }
    bool IsAlive() const { return !IsDead(); }

    Unit* GetVictim();

protected:
    uint32 Health;
    uint32 Power;
    int32 MeeleAttackCooldown;

    Unit* pVictim;
};

#endif
