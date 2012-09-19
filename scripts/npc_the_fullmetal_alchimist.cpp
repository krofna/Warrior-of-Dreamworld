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
#include "CreatureAI.hpp"
#include "Creature.hpp"

enum
{
    SPELL_QUADRUPLE_ACCEL_NO_INTERVAL = 2
};

#define TEXT_SPECIAL_ATTACK "Burst Link !"

#define TEXT_ACCEL "First, ACCEL !"
#define TEXT_DOUBLE_ACCEL "Second, DOUBLE ACCEL !"
#define TEXT_TRIPLE_ACCEL "I NEED MORE POWER, THIRD ! TRIPLE ACCEL !"
#define TEXT_QUADRUPLE_ACCEL "YAAAAAAAAA, FINAL QUADRUPLE ACCEL, NO INTERVAL !"

#define TEXT_ULTIMATE_ATTACK "Physical Full Burst !"

class WOD_DLL_DECL npc_the_fullmetal_alchimistAI : public CreatureAI
{
public:
    npc_the_fullmetal_alchimistAI(Creature* pCreature) : CreatureAI(pCreature)
    {
        Reset();
    }

    int64 AttackSpecialTimer;
    int64 AttackUltimateTimer;
    int64 PhysicalFullBurstTimer;
    int64 SpellAccelState;
    int64 NanoIntervalDiff;

    bool UsingSpecialAttack;
    bool IsInPhysicalFullBurst;

    void Reset()
    {
        AttackSpecialTimer = 15000;
        AttackUltimateTimer = 50000;
        PhysicalFullBurstTimer = 30000;
        NanoIntervalDiff = 300;

        SpellAccelState = 1;

        UsingSpecialAttack = false;
        IsInPhysicalFullBurst = false;
    }

    void UpdateAI(int64 diff)
    {
        if(!pCreature->GetVictim())
            return;

        if (IsInPhysicalFullBurst)
        {
            pCreature->CastSpell(SPELL_QUADRUPLE_ACCEL_NO_INTERVAL, pCreature->GetVictim());

            if (PhysicalFullBurstTimer <= diff)
            {
                PhysicalFullBurstTimer = 30000;
                IsInPhysicalFullBurst = false;
            }
            else
                PhysicalFullBurstTimer -= diff;
        }

        if(AttackSpecialTimer <= diff)
        {
            pCreature->Say(TEXT_SPECIAL_ATTACK);
            ActivateAcceleration();
        }
        else
            AttackSpecialTimer -= diff;

        if (UsingSpecialAttack)
            UpdateQuadrupleAccel(diff);
    
        if (AttackUltimateTimer <= diff)
        {
            pCreature->Say(TEXT_ULTIMATE_ATTACK);
            IsInPhysicalFullBurst = true;
            AttackUltimateTimer = 40000;
        }
        else
            AttackUltimateTimer -= diff;

        pCreature->DoMeleeAttackIfReady(diff);
    }

    void UpdateQuadrupleAccel(int64 diff)
    {
        if (NanoIntervalDiff <= diff)
        {
            switch(SpellAccelState)
            {
                case 1:
                    pCreature->Say(TEXT_ACCEL);
                    break;
                case 2:
                    pCreature->Say(TEXT_DOUBLE_ACCEL);
                    break;
                case 3:
                    pCreature->Say(TEXT_TRIPLE_ACCEL);
                    break;
                case 4:
                    pCreature->Say(TEXT_QUADRUPLE_ACCEL);
                    break;
            }

            pCreature->CastSpell(SPELL_QUADRUPLE_ACCEL_NO_INTERVAL, pCreature->GetVictim());
            SpellAccelState++;
            NoInterval();
        }
        else
            NanoIntervalDiff -= diff;

        if (SpellAccelState > 4)
            ResetAcceleration();
    }

    void NoInterval()
    {
        NanoIntervalDiff = 300 * SpellAccelState;
    }

    void ActivateAcceleration()
    {
        NanoIntervalDiff = 300;
        SpellAccelState = 1;
        UsingSpecialAttack = true;
        AttackSpecialTimer = 7000;
    }

    void ResetAcceleration()
    {
        UsingSpecialAttack = false;
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
