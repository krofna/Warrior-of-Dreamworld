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
#include "Database.hpp"
#include <fstream>

ObjectMgr sObjectMgr;

ObjectMgr::~ObjectMgr()
{
    for(auto SpellIter = Spells.begin(); SpellIter != Spells.end(); ++SpellIter)
    {
        delete (*SpellIter);
    }

    for(auto iter = CreatureTemplates.begin(); iter != CreatureTemplates.end(); ++iter)
    {
        delete iter->second;
    }
}

CreatureTemplate* ObjectMgr::GetCreatureTemplate(uint32 Entry)
{
    auto CTemplate = CreatureTemplates.find(Entry);
    if(CTemplate != CreatureTemplates.end())
        return CTemplate->second;

    throw std::exception("Bad creature entry. Could not find template.");
}

Spell* ObjectMgr::GetSpell(uint16 ID)
{
    for(auto SpellIter = Spells.begin(); SpellIter != Spells.end(); ++SpellIter)
    {
        if((*SpellIter)->ID == ID)
            return (*SpellIter);
    }

    return nullptr;
}

void ObjectMgr::LoadCreatureTemplates()
{
    QueryResult Result(sDatabase.Query("SELECT * FROM `creature_template`"));

    CreatureTemplate* pTemplate;

    while(Result->next())
    {
        pTemplate = new CreatureTemplate;
        pTemplate->Entry = Result->getUInt(1);
        pTemplate->Name = Result->getString(2);
        pTemplate->Tileset = Result->getString(3);
        pTemplate->tx = Result->getUInt(4);
        pTemplate->ty = Result->getUInt(5);
        pTemplate->ScriptName = Result->getString(6);

        CreatureTemplates[Result->getUInt(1)] = pTemplate;
    }
}

void ObjectMgr::LoadSpells()
{
    std::ifstream File("SpellDB.txt");

    uint16 ID;
    uint16 DisplayID;
    uint16 Effect;
    uint16 Value;
    uint16 Cost;
    std::string Name;

    while(File >> ID >> DisplayID >> Effect >> Value >> Cost >> Name)
    {
        Spells.push_back(new Spell(ID, DisplayID, Effect, Value, Cost, Name));
    }
}
