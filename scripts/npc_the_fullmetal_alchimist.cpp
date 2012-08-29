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
#define TEXT_ENTER_COMBAT "Enabling combat mode."
#define TEXT_HIT "Missed."
#define TEXT_ACCEL "First, ACCEL !"
#define TEXT_DOUBLE_ACCEL "Second, DOUBLE ACCEL !"
#define TEXT_TRIPLE_ACCEL "I NEED MORE POWER, THIRD ! TRIPLE ACCEL !"
#define TEXT_QUADRUPLE_ACCEL "YAAAAAAAAA, FINAL QUADRUPLE ACCEL, NO INTERVAL !"

class WOD_DLL_DECL npc_the_fullmetal_alchimistAI : public CreatureAI
{
public:
    npc_the_fullmetal_alchimistAI(CreaturePtr& pCreature) : CreatureAI(pCreature)
    {
        Reset();
    }

    int32 AttackSpecialTimer;
    int32 HitTimer;
    float Multiplier;
    int32 Casted;
    int32 NanoIntervalDiff;
    bool UsingSpecialAttack;

    void Reset()
    {
        AttackSpecialTimer = 15000;
        HitTimer = 40000;
        Multiplier = 1.5f;
        Casted = 0;
        UsingSpecialAttack = false;
        NanoIntervalDiff = 300;
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
            // TODO: Make Spell no damage
            HitTimer = 40000 * Multiplier;
            Multiplier += 0.5f;
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
            pCreature->Say(TEXT_SPECIAL_ATTACK);
            pCreature->CastSpell(SPELL_QUADRUPLE_ACCEL_NO_INTERVAL, pCreature->GetVictim());
            Casted++;
            AttackSpecialTimer = 17000;
            UsingSpecialAttack = true;
        }
        else
            AttackSpecialTimer -= diff;

        if (UsingSpecialAttack && Casted < 4 && NanoIntervalDiff <= diff)
        {
            pCreature->CastSpell(SPELL_QUADRUPLE_ACCEL_NO_INTERVAL, pCreature->GetVictim());
            if (Casted == 1)
                pCreature->Say(TEXT_ACCEL);
            else if (Casted == 2)
                pCreature->Say(TEXT_DOUBLE_ACCEL);
            else if (Casted == 3)
                pCreature->Say(TEXT_TRIPLE_ACCEL);
            else if (Casted == 4)
                pCreature->Say(TEXT_QUADRUPLE_ACCEL);
            
            Casted++;

            NanoIntervalDiff = 750;
        }
        else
            NanoIntervalDiff -= diff;

        if (Casted == 4)
        {
            Casted = 1;
            UsingSpecialAttack = false;
        }

        pCreature->DoMeleeAttackIfReady(diff);
    }
};

CreatureAI* GetAI_npc_the_fullmetal_alchimist(CreaturePtr pCreature)
{
    return new npc_the_fullmetal_alchimistAI(pCreature);
}

void LoadScript_npc_the_fullmetal_alchimist()
{
    RegisterAI("npc_the_fullmetal_alchimist", &GetAI_npc_the_fullmetal_alchimist);
}
