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
#ifndef MAP_H
#define MAP_H

#include "SpellBox.hpp"
#include "Creature.hpp"

class Player;
struct PathfinderNode;

struct Map
{
    Map(const uint16 MapID);
    ~Map();

    void LoadCreatures();

    // TODO: maybe struct containing iswater/not walkable/walkable stuff along with WorldObjectPtr
    std::vector<std::vector<WorldObjectPtr> > TileGrid;

    void RemovePlayer(PlayerPtr pPlayer);
    void AddPlayer(PlayerPtr pPlayer);
    void AddSpell(UnitPtr pCaster, Spell* pSpell, float Angle);
    virtual void Update(int32 diff);
    void UnitUpdate(UnitPtr pVictim);

    void SendToPlayers(WorldPacket& Packet);

    // Entities
    std::vector<CreaturePtr> Creatures;
    std::vector<PlayerPtr> Players;
    std::vector<SpellBox> Spells;

    const uint16 MapID;
    uint32 NewSpellBoxID;
    int32 diff;
};

#endif