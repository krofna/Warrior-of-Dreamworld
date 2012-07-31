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
#include "../shared/Opcodes.hpp"
#include "../shared/Math.hpp"
#include "ObjectMgr.hpp"

Unit::Unit      (Uint32 ObjID) :
WorldObject     (ObjID),
pVictim         (nullptr)
{
}

void Unit::SpellHit(SpellBox* pSpellBox)
{
    // PH - reduce health and do shit
    // Threat checks etc?
    sf::Packet Packet;
    Packet << (Uint16)MSG_SPELL_HIT << pSpellBox->SpellBoxID << ObjID;
    pMap->SendToPlayers(Packet);
}

void Unit::CastSpell(Spell* pSpell, float Angle)
{
    // TODO: Reduce mana etc etc
    sf::Packet Packet;
    Packet << (Uint16)MSG_CAST_SPELL << pSpell->Effect << ObjID << pSpell->DisplayID << Angle << pMap->NewSpellBoxID;
    pMap->SendToPlayers(Packet);
    pMap->AddSpell(this, pSpell, Angle);
}

void Unit::CastSpell(Uint16 Entry, Unit* pVictim)
{
    // TODO: Angle is bugged because client side GetAngle works differently
    CastSpell(sObjectMgr.GetSpell(Entry), math::GetAngle(Position * TILE_SIZE, pVictim->Position * TILE_SIZE));
}

void Unit::CastSpell(Uint16 Entry, float Angle)
{
    CastSpell(sObjectMgr.GetSpell(Entry), Angle);
}

Unit* Unit::GetVictim()
{
    return pVictim;
}
