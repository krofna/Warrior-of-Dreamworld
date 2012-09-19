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

class Map
{
    public:
    Map(uint16 TMapID);
    ~Map();

    void LoadCreatures();

    void RemovePlayer(Player* pPlayer);
    void AddPlayer(Player* pPlayer);
    void AddSpell(Unit* pCaster, SpellPtr& pSpell, float Angle);
    virtual void Update(int64 diff);
    void UnitUpdate(Unit* pVictim, int64 diff);

    void SendToPlayers(WorldPacket& Packet);

    bool CheckOutside(int PosY, int PosX, uint8 Direction) const;

    // Entities
    std::vector<Creature*> Creatures;
    std::vector<Player*> Players;
    std::vector<SpellBoxPtr> Spells;

    uint16 MapID;
    uint32 NewSpellBoxID;

    // TODO: maybe struct containing iswater/not walkable/walkable stuff along with WorldObject*
    std::vector<std::vector<WorldObject*> > TileGrid;
};

#endif
