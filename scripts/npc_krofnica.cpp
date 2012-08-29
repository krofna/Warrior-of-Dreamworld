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
    SPELL_FIREBALL = 1
};

// It's not FullMetal-Alchimist who have write this :P
#define TEXT_ATTACK "I'm Krofnica, the boss."
#define TEXT_ENRAGE "I'M THE CREATOR OF THIS GAME, I CAN'T DIE !"
#define TEXT_ENTER_COMBAT "You dare to challenge Krofnica?!"
#define TEXT_HIT "You think that can hurt mighty Krofnica? Fool."

class WOD_DLL_DECL npc_krofnicaAI : public CreatureAI
{
public:
    npc_krofnicaAI(CreaturePtr& pCreature) : CreatureAI(pCreature)
    {
        Reset();
    }

    int32 FireballTimer;
    int32 AttackTimer;
    int32 EnrageTimer;
    int32 HitTimer;

    void Reset()
    {
        FireballTimer = 8000;
        AttackTimer = 9000;
        EnrageTimer = 30000;
        HitTimer = 38000;
    }

    void EnterCombat(UnitPtr& pEnemy)
    {
        pCreature->Say(TEXT_ENTER_COMBAT);
    }

    void SpellHit(SpellBoxPtr& pSpellBox)
    {
        if (HitTimer - 1000 <= 0)
        {
            pCreature->Say(TEXT_HIT);
            HitTimer = 38000;
        }
        else
            HitTimer -= 1000;
    }

    void UpdateAI(int32 diff)
    {
        if(!pCreature->GetVictim())
            return;

        if(FireballTimer <= diff)
        {
            pCreature->CastSpell(SPELL_FIREBALL, pCreature->GetVictim());
            FireballTimer = 6000;
        }
        else
            FireballTimer -= diff;

        if (AttackTimer <= diff)
        {
            pCreature->Say(TEXT_ATTACK);
            AttackTimer = 8000;
        }
        else
            AttackTimer -= diff;

        if (EnrageTimer <= diff)
        {
            pCreature->Say(TEXT_ENRAGE);
            EnrageTimer = 40000;
        }
        else
            EnrageTimer -= diff;

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
