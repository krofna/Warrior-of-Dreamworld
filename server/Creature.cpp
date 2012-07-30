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
#include "Creature.hpp"
#include "CreatureAI.hpp"
#include "SpellBox.hpp"
#include "Pathfinder.hpp"
#include "../shared/Opcodes.hpp"

Creature::Creature(Uint32 ObjID, Map* pMap, Uint16 x, Uint16 y) :
Unit              (ObjID),
pAI               (nullptr)
{
    Position = sf::Vector2i(x, y);
    this->pMap = pMap;
    this->pAI = pAI;
    MovementGenerator = new Pathfinder(this);
}

void Creature::Update(Int32 diff)
{
    MovementGenerator->Update(diff);
    pAI->UpdateAI(diff);
}

void Creature::BindAI(CreatureAI* pAI)
{
    delete this->pAI;
    this->pAI = pAI;
}

sf::Packet Creature::PackData()
{
    sf::Packet Packet;
    Packet << (Uint16)MSG_ADD_OBJECT << Tileset << ObjID << Name << GetX() << GetY() << tx << ty;
    return Packet;
}

void Creature::SpellHit(SpellBox* pSpellBox)
{
    Unit::SpellHit(pSpellBox);
    
    if(!pVictim)
    {
        MovementGenerator->UpdateTarget(pVictim);
        pAI->EnterCombat(pVictim);
    }
    pAI->SpellHit(pSpellBox->pCaster, pSpellBox->pSpell);
}

CreatureAI* Creature::GetAI()
{
    return pAI;
}
