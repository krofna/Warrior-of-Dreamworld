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
#include "ObjectMgr.hpp"
#include "Spell.hpp"
#include <fstream>

ObjectMgr* sObjectMgr;

ObjectMgr::~ObjectMgr()
{
    for(auto SpellIter = Spells.begin(); SpellIter != Spells.end(); ++SpellIter)
    {
        delete (*SpellIter);
    }
}

Spell* ObjectMgr::GetSpell(Uint16 ID)
{
    for(auto SpellIter = Spells.begin(); SpellIter != Spells.end(); ++SpellIter)
    {
        if((*SpellIter)->ID == ID)
            return (*SpellIter);
    }

    return nullptr;
}

void ObjectMgr::LoadSpells()
{
    std::ifstream File("SpellDB.txt");

    Uint16 ID;
    Uint16 DisplayID;
    Uint16 Effect;
    Uint16 Value;
    Uint16 Cost;
    std::string Name;

    while(File >> ID >> DisplayID >> Effect >> Value >> Cost >> Name)
    {
        Spells.push_back(new Spell(ID, DisplayID, Effect, Value, Cost, Name));
    }
}