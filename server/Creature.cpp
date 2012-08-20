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
#include "Templates.hpp"
#include "CreatureAIFactory.hpp"
#include "../shared/Opcodes.hpp"
#include "../shared/Math.hpp"

Creature::Creature(uint32 ObjID, MapPtr pMap, uint16 x, uint16 y, CreatureTemplate* pTemplate) :
Unit              (ObjID),
pTemplate         (pTemplate)
{
    Position = Vector2i(x, y);
    this->pMap = pMap;
    pAI = CreateAI(pTemplate->ScriptName, CreaturePtr(this));
    MovementGenerator = new Pathfinder(WorldObjectPtr(this));
}

Creature::~Creature()
{
    delete pAI;
}

void Creature::Update(int32 diff)
{
    MovementGenerator->Update(diff);
    pAI->UpdateAI(diff);
}

WorldPacket Creature::PackData()
{
    WorldPacket Packet((uint16)MSG_ADD_OBJECT);
    Packet << ObjID << pTemplate->Tileset << pTemplate->Name << GetX() << GetY() << pTemplate->tx << pTemplate->ty;
    return Packet;
}

void Creature::SpellHit(SpellBox* pSpellBox)
{
    Unit::SpellHit(pSpellBox);

    if(!pVictim)
    {
        pVictim = pSpellBox->pCaster;
        MovementGenerator->UpdateTarget(pVictim);
        pAI->EnterCombat(pVictim);
    }
    pAI->SpellHit(pSpellBox);
}

CreatureAI* Creature::GetAI()
{
    return pAI;
}
