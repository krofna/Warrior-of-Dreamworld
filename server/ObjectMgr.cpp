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
#include "Player.hpp"
#include <fstream>
#include <stdexcept>

ObjectMgr sObjectMgr;

ObjectMgr::~ObjectMgr()
{
    for(auto iter = CreatureTemplates.begin(); iter != CreatureTemplates.end(); ++iter)
    {
        delete iter->second;
    }

    for(auto iter = Players.begin(); iter != Players.end(); ++iter)
    {
        delete *iter;
    }
}

CreatureTemplate* ObjectMgr::GetCreatureTemplate(uint32 Entry)
{
    auto CTemplate = CreatureTemplates.find(Entry);
    if(CTemplate != CreatureTemplates.end())
        return CTemplate->second;

    throw std::runtime_error("Bad creature entry. Could not find template.");
}

SpellPtr ObjectMgr::GetSpell(uint16 ID)
{
    for(auto SpellIter = Spells.begin(); SpellIter != Spells.end(); ++SpellIter)
    {
        if((*SpellIter)->ID == ID)
            return (*SpellIter);
    }

    return SpellPtr();
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
    QueryResult Result(sDatabase.Query("SELECT * FROM `spells`"));

    while (Result->next())
    {
        Spells.push_back(SpellPtr(new Spell(Result->getUInt64(1), Result->getUInt(2), Result->getUInt(3), Result->getUInt(4), Result->getUInt(5), Result->getString(6))));
    }
}

void ObjectMgr::LoadPlayersLoginInfo()
{
    QueryResult Result(sDatabase.Query("SELECT username, password, guid FROM `players`"));

    while (Result->next())
    {
        Players.push_back(new Player(Result->getString(1), Result->getString(2), Result->getUInt64(3)));
    }
}

Player* ObjectMgr::GetPlayer(std::string& Username)
{
    for(auto iter = Players.begin(); iter != Players.end(); ++iter)
    {
        if((*iter)->GetUsername() == Username)
        {
            return *iter;
        }
    }

    return nullptr;
}
