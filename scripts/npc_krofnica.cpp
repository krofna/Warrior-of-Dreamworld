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
#include "../server/CreatureAI.hpp"
#include "../server/Creature.hpp"

enum
{
    SPELL_FIREBALL = 0
};

class WOD_DLL_DECL npc_krofnicaAI : public CreatureAI
{
public:
    npc_krofnicaAI(CreaturePtr& pCreature) : CreatureAI(pCreature)
    {
        Reset();
    }

    int32 FireballTimer;

    void Reset()
    {
        FireballTimer = 5000;
    }

    void UpdateAI(int32 diff)
    {
        if(!pCreature->GetVictim())
            return;

        if(FireballTimer <= diff)
        {
            pCreature->CastSpell(SPELL_FIREBALL, pCreature->GetVictim());
            FireballTimer = 5000;
        }
        else
        {
            FireballTimer -= diff;
        }

        pCreature->DoMeleeAttackIfReady(diff);
    }
};

CreatureAI* GetAI_npc_krofnica(CreaturePtr pCreature)
{
    return new npc_krofnicaAI(pCreature);
}

void LoadScript_npc_krofnica()
{
    RegisterAI("npc_krofnica", &GetAI_npc_krofnica);
}