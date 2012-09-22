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
#ifndef TEMPLATES_H
#define TEMPLATES_H

#include <string>

typedef uint8_t uint8;
typedef uint16_t uint16;
typedef uint32_t uint32;
typedef uint64_t uint64;

typedef int8_t int8;
typedef int16_t int16;
typedef int32_t int32;
typedef int64_t int64;


enum //npcflag
{
    NPC_QUEST_GIVER = 1,
    NPC_VENDOR      = 2,
    NPC_REPAIR      = 4,
                    //8
                    //...
};

struct WorldObjectTemplate
{
    WorldObjectTemplate(uint64 Entry, std::string const& Name, std::string const& Tileset, uint16 tx, uint16 ty)
    {
        this->Entry = Entry;
        this->Name = Name;
        this->Tileset = Tileset;
        this->tx = tx;
        this->ty = ty;
    }

    uint64 Entry;
    std::string Name;
    std::string Tileset;
    uint16 tx;
    uint16 ty;
};

struct CreatureTemplate : WorldObjectTemplate
{
    CreatureTemplate(uint64 Entry, std::string const& Name, std::string const& Tileset, uint16 tx, uint16 ty, uint16 npcflag, uint32 maxhealth, uint32 maxpower, std::string const& ScriptName) : WorldObjectTemplate(Entry, Name, Tileset, tx, ty)
    {
        this->npcflag = npcflag;
        MaxHealth = maxhealth;
        MaxPower = maxpower;
        this->ScriptName = ScriptName;
    }

    // Bitmask, see: enum npcflag
    uint16 npcflag;
    uint32 MaxHealth;
    uint32 MaxPower;
    std::string ScriptName;
};

// NOT YET IN DATABASE
struct GameObjectTemplate : WorldObjectTemplate
{
};

enum InventoryType
{
    INVTYPE_NON_EQUIP,
    INVTYPE_BAG
};

struct ItemTemplate
{
    ItemTemplate(uint64 itemID, uint32 Class, uint32 subClass, std::string const& name, uint32 displayInfoID, uint32 inventoryType, uint8 containerSlot, std::string const& description)
    {
        ItemID = itemID;
        this->Class = Class;
        SubClass = subClass;
        Name = name;
        DisplayInfoID = displayInfoID;
        InventoryType = inventoryType;
        ContainerSlots = containerSlot;
        Description = description;
    }

    uint64 ItemID;
    uint32 Class;
    uint32 SubClass;
    std::string Name;
    uint32 DisplayInfoID;
    uint32 InventoryType;
    uint8 ContainerSlots;
    std::string Description;
};

struct QuestTemplate
{
    QuestTemplate(uint64 questID, std::string const& title, std::string const& details, std::string const& objective)
    {
        QuestID = questID;
        Title = title;
        Details = details;
        Objective = objective;
    }

    uint64 QuestID;
    std::string Title;
    std::string Details;
    std::string Objective;
};

#endif
