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
#ifndef OBJECT_MGR_H
#define OBJECT_MGR_H

#include "shared/Defines.hpp"
#include "shared/Templates.hpp"
#include <map>
#include <boost/unordered_map.hpp>

class Player;

typedef std::pair<std::multimap<uint64, uint64>::const_iterator, std::multimap<uint64, uint64>::const_iterator> WorldObjectQuests;

class ObjectMgr
{
public:
    ~ObjectMgr();

    CreatureTemplate*   GetCreatureTemplate   (uint64 Entry) const;
    GameObjectTemplate* GetGameObjectTemplate (uint64 Entry) const;
    ItemTemplate*       GetItemTemplate       (uint64 Entry) const;
    QuestTemplate*      GetQuestTemplate      (uint64 Entry) const;
    SpellTemplate*      GetSpellTemplate      (uint64 Entry) const;
    MapTemplate*        GetMapTemplate        (uint32 Entry) const;

    WorldObjectQuests   GetCreatureQuests     (uint64 Entry) const;

    void LoadSpellTemplates();
    void LoadCreatureTemplates();
    void LoadGameObjectTemplates();
    void LoadItemTemplates();
    void LoadQuestTemplates();
    void LoadMapTemplates();
    void LoadCreatureQuests();
    void LoadPlayersLoginInfo();
    
    // TODO: this overload is evil
    Player* GetPlayer(std::string& Username);
	Player* GetPlayer(uint64 ObjectID);

private:
    boost::unordered_map<uint64, Player*>             Players;
    
    boost::unordered_map<uint64, SpellTemplate*>      SpellTemplates;
    boost::unordered_map<uint64, CreatureTemplate*>   CreatureTemplates;
    boost::unordered_map<uint64, GameObjectTemplate*> GameObjectTemplates;
    boost::unordered_map<uint64, ItemTemplate*>       ItemTemplates;
    boost::unordered_map<uint32, MapTemplate*>        MapTemplates;

    std::map<uint64, QuestTemplate*>                  QuestTemplates;
    
    // Key = Creature ObjID
    // T   = Quest ID
    std::multimap<uint64, uint64>         CreatureQuests;
};

extern ObjectMgr sObjectMgr;

#endif
