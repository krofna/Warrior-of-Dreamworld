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
#ifndef CREATURE_AI_H
#define CREATURE_AI_H

#include "../shared/Defines.hpp"
#include "../shared/Config.hpp"

class Unit;
class Creature;
class SpellBox;
class Spell;

class WOD_DLL_DECL CreatureAI
{
public:
    explicit CreatureAI(CreaturePtr pCreature) : pCreature(pCreature) {}
    virtual ~CreatureAI() {};

    // Called on World::Update
    virtual void UpdateAI(int32 diff);

    // Called when creature enters combat
    virtual void EnterCombat(UnitPtr pEnemy){};

    // Called when creature starts running home
    virtual void RunHome(){}; // -- NYI

    // Called when creature reaches home
    virtual void ReachedHome(){}; // -- NYI

    //Called when creature dies
    virtual void JustDied(UnitPtr pKiller){}; // -- NYI

    // Called when creature kills unit
    virtual void KilledUnit(UnitPtr pVictim){}; // -- NYI

    // Called when creature is hit by melee attack
    virtual void Hit(UnitPtr pAttacker, uint32& Damage){}; // -- NYI

    // Called when creature is hit by spell
    virtual void SpellHit(SpellBoxPtr pSpellBox){};

    // Creature controlled by this AI
    CreaturePtr pCreature;
};

CreatureAI* CreateAI(const std::string& AIName, CreaturePtr pCreature);
WOD_DLL_DECL void RegisterAI(const std::string &AIName, CreatureAI*(*Creator)(CreaturePtr pCreature));

#endif
