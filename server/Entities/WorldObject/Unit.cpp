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
#include "Unit.hpp"
#include "Spell.hpp"
#include "Map.hpp"
#include "shared/Opcodes.hpp"
#include "shared/Math.hpp"
#include "ObjectMgr.hpp"

Unit::Unit      (uint64 ObjID) :
WorldObject     (ObjID),
MeeleAttackCooldown(0),
pVictim         (nullptr),
Health          (100) // Placeholder to fix pathfinder
{
}

void Unit::SpellHit(SpellBoxPtr pSpellBox)
{
    TakeDamage(pSpellBox->pSpell->Value, pSpellBox->pCaster);
}

void Unit::CastSpell(SpellPtr pSpell, float Angle)
{
    // TODO: Reduce mana etc etc
    WorldPacket* Packet = new WorldPacket((uint16)MSG_CAST_SPELL);
    *Packet << ObjID << pSpell->Effect << pSpell->DisplayID << Angle << pMap->NewSpellBoxID;
    pMap->SendToPlayers(Packet);
    pMap->AddSpell(this, pSpell, Angle);
}

void Unit::Say(const char* Text)
{
    WorldPacket* SayPacket = new WorldPacket((uint16)MSG_CHAT_MESSAGE);
    *SayPacket << GetObjectID() << std::string(Text);

    pMap->SendToPlayers(SayPacket);
}

void Unit::DoMeleeAttackIfReady(int64 diff)
{
    // If there is no victim, or victim is too far away, return
    if(!pVictim || math::GetManhattanDistance(Position, pVictim->GetPosition()) > 1)
        return;

    if(MeeleAttackCooldown <= diff)
    {
        DealDamage(GetMeleeDamage(), pVictim);
        MeeleAttackCooldown = 3000; // random blah
    }
    else
    {
        MeeleAttackCooldown -= diff;
    }
}

void Unit::CastSpell(uint16 Entry, Unit* pVictim)
{
    // TODO: Angle is bugged because client side GetAngle works differently
    CastSpell(sObjectMgr.GetSpell(Entry), math::GetAngle(Position * TILE_SIZE, pVictim->GetPosition() * TILE_SIZE));
}

void Unit::CastSpell(uint16 Entry, float Angle)
{
    CastSpell(sObjectMgr.GetSpell(Entry), Angle);
}

void Unit::Kill()
{
    Health = 0; // :)
}

void Unit::DealDamage(int32 Damage, Unit* pTarget)
{
    pTarget->TakeDamage(Damage, this);
}

void Unit::TakeDamage(int32 Damage, Unit* pAttacker)
{
    if (Health - Damage <= 0)
        Health = 0;
    else
        Health -= Damage;
}

int32 Unit::GetMeleeDamage() const
{
    return 1; // TODO: Use stats
}

Unit* Unit::GetVictim()
{
    return pVictim;
}
