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
#include "Player.hpp"
#include "CreatureAI.hpp"
#include "Pathfinder.hpp"
#include "shared/Templates.hpp"
#include "Map.hpp"
#include "AIFactory.hpp"
#include "shared/Opcodes.hpp"

/*
TODO:
Check if TileGrid[y][x] is actually avaliable,
else dont spawn the creature
*/
Creature::Creature(uint64 ObjID, Map* pMap, uint16 x, uint16 y, CreatureTemplate* pTemplate) :
Unit              (ObjID),
pTemplate         (pTemplate)
{
    Position = Vector2i(x, y);
    this->pMap = pMap;
    pMap->AddToTileGrid(this);
    this->Health = pTemplate->MaxHealth;
    this->Power = pTemplate->MaxPower;
    pAI = CreateAI(pTemplate->ScriptName, this);
    MovementGenerator = new Pathfinder(this);
}

Creature::~Creature()
{
    delete pAI;
    delete MovementGenerator;
}

void Creature::Update(int64 diff)
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

void Creature::DealDamage(int32 Damage, Unit* pTarget)
{
    Unit::DealDamage(Damage, pTarget);
    if (pTarget->IsDead())
        pAI->KilledUnit(pTarget);
}

void Creature::TakeDamage(int32 Damage, Unit* pAttacker)
{
    Unit::TakeDamage(Damage, pAttacker);
    pAI->Hit(pAttacker, Damage);
    if (IsDead())
        pAI->JustDied(pAttacker);
}

void Creature::OnInteract(Player* pWho)
{
    WorldPacket Packet((uint16)MSG_NPC_INTERACT);
    Packet << pTemplate->Entry;
    
    if(pTemplate->npcflag & NPC_CHAT)
    {
        // TODO: Placeholder
        Packet << std::string("Hello!");
    }
    if(pTemplate->npcflag & NPC_QUEST_GIVER)
    {
        // Append all quests offered by this creature
        // No need to say how many, its end of packet anyway
        WorldObjectQuests QuestRange = sObjectMgr.GetCreatureQuests(pTemplate->Entry);
        for(auto iter = QuestRange.first; iter != QuestRange.second; ++iter)
        {
            // TODO: Is player eligible for quest?
            Packet << iter->second;
        }
    }
    
    pWho->SendPacket(Packet);
}

CreatureAI* Creature::GetAI()
{
    return pAI;
}
