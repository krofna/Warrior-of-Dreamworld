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

Unit::Unit (Uint32 ObjID) :
WorldObject(ObjID)
{
}

void Unit::SpellHit(SpellBox* pSpellBox)
{
    // PH - reduce health and do shit
    sf::Packet Packet;
    Packet << (Uint16)MSG_SPELL_HIT << pSpellBox->SpellBoxID() << ObjID;
    pMap->SendToPlayers(Packet);
}

void Unit::CastSpell(Spell* pSpell, float Angle)
{
    // TODO: Reduce mana etc etc
    sf::Packet Packet;
    Packet << (Uint16)MSG_CAST_SPELL << pSpell->Effect << ObjID << pSpell->DisplayID << Angle << pMap->NewSpellBoxID;
    GetMap()->SendToPlayers(Packet);
    GetMap()->AddSpell(this, pSpell, Angle);
}