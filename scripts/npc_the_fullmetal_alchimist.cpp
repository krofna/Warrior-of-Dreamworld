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
    SPELL_QUADRUPLE_ACCEL_NO_INTERVAL = 2
};

#define TEXT_SPECIAL_ATTACK "Dark Repulser, Quadruple Accel, No Interval !"
#define TEXT_ENTER_COMBAT "FullMetal Alchimist: Enabling combat mode."
#define TEXT_HIT "FullMetal Alchimist: Missed."

class WOD_DLL_DECL npc_the_fullmetal_alchimistAI : public CreatureAI
{
public:
    npc_the_fullmetal_alchimistAI(Creature* pCreature) : CreatureAI(pCreature)
    {
        Reset();
    }

    int32 AttackSpecialTimer;
    int32 HitTimer;
    float Multiplier;

    void Reset()
    {
        AttackSpecialTimer = 10000;
        HitTimer = 30000;
        Multiplier = 1.5f;
    }

    void EnterCombat(Unit* pEnemy)
    {
        pCreature->Say(TEXT_ENTER_COMBAT);
    }

    void SpellHit(SpellBoxPtr& pSpellBox)
    {
        if (HitTimer - 1000 <= 0)
        {
            pCreature->Say(TEXT_HIT);
            // TODO: Make Spell no damage
            HitTimer = 30000 * Multiplier;
            Multiplier++;
        }
        else
            HitTimer -= 1000;
    }

    void UpdateAI(int32 diff)
    {
        if(!pCreature->GetVictim())
            return;

        if(AttackSpecialTimer <= diff)
        {
            pCreature->CastSpell(SPELL_QUADRUPLE_ACCEL_NO_INTERVAL, pCreature->GetVictim());
            pCreature->Say(TEXT_SPECIAL_ATTACK);
            AttackSpecialTimer = 3000;
        }
        else
            AttackSpecialTimer -= diff;

        pCreature->DoMeleeAttackIfReady(diff);
    }
};

CreatureAI* GetAI_npc_the_fullmetal_alchimist(Creature* pCreature)
{
    return new npc_the_fullmetal_alchimistAI(pCreature);
}

void LoadScript_npc_the_fullmetal_alchimist()
{
    RegisterAI("npc_the_fullmetal_alchimist", &GetAI_npc_the_fullmetal_alchimist);
}
