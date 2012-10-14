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

struct WorldObjectTemplate
{
    uint64 Entry;
    std::string Name;
    std::string Tileset;
    uint16 tx;
    uint16 ty;
    std::string ScriptName;
};

enum //npcflag
{
    NPC_QUEST_GIVER = 1,
    NPC_VENDOR      = 2,
    NPC_REPAIR      = 4,
    NPC_CHAT        = 8
};

struct CreatureTemplate : WorldObjectTemplate
{
    uint16 npcflag;
    uint32 MaxHealth;
    uint32 MaxPower;
};

enum //goflag
{
};

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
    uint64 QuestID;
    std::string Title;
    std::string Details;
    std::string Objective;
};

enum //Effect
{
    SPELL_BOLT = 0
};

struct SpellTemplate
{
    uint64 Entry;
    std::string Name;
    uint16 DisplayID;
    uint16 Effect;
    uint16 Value;
    uint16 Cost;
};

enum //mapflag
{
    MAP_INSTANCED = 1
};

struct MapTemplate
{
    uint32 Entry;
    uint16 SizeX;
    uint16 SizeY;
    uint8 mapflag;
    std::string ScriptName;
};

#endif
