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
#include "Database.hpp"
#include "Player.hpp"
#include <boost/bind.hpp>

ObjectMgr sObjectMgr;

ObjectMgr::~ObjectMgr()
{
    std::for_each(CreatureTemplates.begin(), CreatureTemplates.end(), MapDeleter());
    std::for_each(GameObjectTemplates.begin(), GameObjectTemplates.end(), MapDeleter());
    std::for_each(ItemTemplates.begin(), ItemTemplates.end(), MapDeleter());
    std::for_each(QuestTemplates.begin(), QuestTemplates.end(), MapDeleter());
    std::for_each(SpellTemplates.begin(), SpellTemplates.end(), MapDeleter());
    std::for_each(MapTemplates.begin(), MapTemplates.end(), MapDeleter());
    std::for_each(Players.begin(), Players.end(), boost::bind(&operator delete, _1));
}

CreatureTemplate* ObjectMgr::GetCreatureTemplate(uint64 Entry) const
{
    auto CTemplate = CreatureTemplates.find(Entry);
    if(CTemplate != CreatureTemplates.end())
        return CTemplate->second;

    return nullptr;
}

GameObjectTemplate* ObjectMgr::GetGameObjectTemplate (uint64 Entry) const
{
    auto CTemplate = GameObjectTemplates.find(Entry);
    if(CTemplate != GameObjectTemplates.end())
        return CTemplate->second;

    return nullptr;
}

ItemTemplate* ObjectMgr::GetItemTemplate(uint64 Entry) const
{
    auto ITemplate = ItemTemplates.find(Entry);
    if (ITemplate != ItemTemplates.end())
        return ITemplate->second;

    return nullptr;
}

SpellTemplate* ObjectMgr::GetSpellTemplate(uint64 ID) const
{
    auto STemplate = SpellTemplates.find(ID);
    if (STemplate != SpellTemplates.end())
        return STemplate->second;

    return nullptr;
}

MapTemplate* ObjectMgr::GetMapTemplate(uint32 ID) const
{
    auto MTemplate = MapTemplates.find(ID);
    if (MTemplate != MapTemplates.end())
        return MTemplate->second;

    return nullptr;
}

WorldObjectQuests ObjectMgr::GetCreatureQuests(uint64 Entry) const
{
    return CreatureQuests.equal_range(Entry);
}

void ObjectMgr::LoadCreatureTemplates()
{
    QueryResult Result(sDatabase.Query("SELECT * FROM `creature_template`"));

    CreatureTemplate* pTemplate;

    while(Result->next())
    {
        pTemplate = new CreatureTemplate;

        pTemplate->Entry      = Result->getUInt64 (1);
        pTemplate->Name       = Result->getString (2);
        pTemplate->Tileset    = Result->getString (3);
        pTemplate->tx         = Result->getUInt   (4);
        pTemplate->ty         = Result->getUInt   (5);
        pTemplate->npcflag    = Result->getUInt   (6);
        pTemplate->MaxHealth  = Result->getUInt   (7);
        pTemplate->MaxPower   = Result->getUInt   (8);
        pTemplate->ScriptName = Result->getString (9);

        CreatureTemplates[pTemplate->Entry] = pTemplate;
    }
}

void ObjectMgr::LoadGameObjectTemplates()
{
    QueryResult Result(sDatabase.Query("SELECT * FROM `gameobject_template`"));

    GameObjectTemplate* pTemplate;

    while(Result->next())
    {
        pTemplate = new GameObjectTemplate;

        pTemplate->Entry      = Result->getUInt64 (1);
        pTemplate->Name       = Result->getString (2);
        pTemplate->Tileset    = Result->getString (3);
        pTemplate->tx         = Result->getUInt   (4);
        pTemplate->ty         = Result->getUInt   (5);
        pTemplate->ScriptName = Result->getString (6);

        GameObjectTemplates[pTemplate->Entry] = pTemplate;
    }
}

void ObjectMgr::LoadItemTemplates()
{
    QueryResult Result(sDatabase.Query("SELECT * FROM `item_template`"));

    ItemTemplate* pTemplate;

    while (Result->next())
    {
        pTemplate = new ItemTemplate;

        pTemplate->ItemID         = Result->getUInt64 (1);
        pTemplate->Class          = Result->getUInt   (2);
        pTemplate->SubClass       = Result->getUInt   (3);
        pTemplate->Name           = Result->getString (4);
        pTemplate->DisplayInfoID  = Result->getUInt   (5);
        pTemplate->InventoryType  = Result->getUInt   (6);
        pTemplate->ContainerSlots = Result->getUInt   (7);
        pTemplate->Description    = Result->getString (8);

        ItemTemplates[pTemplate->ItemID] = pTemplate;
    }
}

void ObjectMgr::LoadQuestTemplates()
{
    QueryResult Result(sDatabase.Query("SELECT * FROM `quest_template`"));
    
    QuestTemplate* pTemplate;
    
    while (Result->next())
    {
        pTemplate = new QuestTemplate;
        
        pTemplate->QuestID   = Result->getUInt64 (1);
        pTemplate->Title     = Result->getString (2);
        pTemplate->Details   = Result->getString (3);
        pTemplate->Objective = Result->getString (4);
        
        QuestTemplates[pTemplate->QuestID] = pTemplate;
    }
}

void ObjectMgr::LoadSpellTemplates()
{
    QueryResult Result(sDatabase.Query("SELECT * FROM `spell_template`"));

    SpellTemplate* pTemplate;

    while (Result->next())
    {
        pTemplate = new SpellTemplate;

        pTemplate->Entry     = Result->getUInt64 (1);
        pTemplate->DisplayID = Result->getUInt   (2); // Unused
        pTemplate->Effect    = Result->getUInt   (3);
        pTemplate->Value     = Result->getUInt   (4);
        pTemplate->Cost      = Result->getUInt   (5);
        pTemplate->Name      = Result->getString (6);

        SpellTemplates[pTemplate->Entry] = pTemplate;
    }
}

void ObjectMgr::LoadMapTemplates()
{
    QueryResult Result(sDatabase.Query("SELECT * FROM `map_template`"));

    MapTemplate* pTemplate;

    while (Result->next())
    {
        pTemplate = new MapTemplate;

        pTemplate->Entry      = Result->getUInt   (1);
        pTemplate->ScriptName = Result->getString (2);
        
        MapTemplates[pTemplate->Entry] = pTemplate;
    }
}

void ObjectMgr::LoadPlayersLoginInfo()
{
    QueryResult Result(sDatabase.Query("SELECT username, password, guid FROM `players`"));

    while (Result->next())
    {
        Players.push_front(new Player(Result->getString(1), Result->getString(2), Result->getUInt64(3)));
    }
}

void ObjectMgr::LoadCreatureQuests()
{
    QueryResult Result(sDatabase.Query("SELECT * FROM `creature_quest`"));
    
    while (Result->next())
    {
        CreatureQuests.insert(std::pair<uint64, uint64>(Result->getUInt64(1), Result->getUInt64(2)));
    }
}

Player* ObjectMgr::GetPlayer(std::string& Username)
{
    for(auto iter = Players.begin(); iter != Players.end(); ++iter)
        if((*iter)->GetUsername() == Username)
            return *iter;

    return nullptr;
}

Player* ObjectMgr::GetPlayer(uint64 ObjectID)
{
    for(auto iter = Players.begin(); iter != Players.end(); ++iter)
        if((*iter)->GetObjectID() == ObjectID)
            return *iter;

	return nullptr;
}
