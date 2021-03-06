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
#ifndef CREATURE_H
#define CREATURE_H

#include "Unit.hpp"

struct CreatureTemplate;
class Pathfinder;
class CreatureAI;

class WOD_DLL_DECL Creature : public Unit
{
public:
    Creature(uint64 ObjID, Map* pMap, uint16 x, uint16 y, CreatureTemplate* pTemplate);
    ~Creature();

    void Update(int64 diff);

    WorldPacket PackData();

    void SpellHit(SpellBox* pSpellBox);
    void TakeDamage(int32 Damage, Unit* pAttacker);
    void DealDamage(int32 Damage, Unit* pTarget);
    int32 GetMeleeDamage() const { return 5; }

    void OnInteract(Player* pWho) ;
    
    CreatureAI* GetAI();
    
protected:
    void OnChat(const char* Text) { }
    
private:
    CreatureAI* pAI;
    Pathfinder* MovementGenerator;
    CreatureTemplate const* pTemplate;
};

#endif
