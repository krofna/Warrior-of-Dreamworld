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
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>

class Player;
struct PathfinderNode;

class Map : public boost::enable_shared_from_this<Map>
{
    public:
    Map(uint16 TMapID);
    ~Map();

    void LoadCreatures();

    void RemovePlayer(PlayerPtr pPlayer);
    void AddPlayer(PlayerPtr pPlayer);
    void AddSpell(UnitPtr pCaster, Spell* pSpell, float Angle);
    virtual void Update(int32 diff);
    void UnitUpdate(UnitPtr pVictim);

    void SendToPlayers(WorldPacket& Packet);

    bool CheckOutside(int PosY, int PosX, uint8 Direction) const;

    // Entities
    std::vector<CreaturePtr> Creatures;
    std::vector<PlayerPtr> Players;
    std::vector<SpellBox> Spells;

    uint16 MapID;
    uint32 NewSpellBoxID;
    int32 diff;

    // TODO: maybe struct containing iswater/not walkable/walkable stuff along with WorldObjectPtr
    std::vector<std::vector<WorldObjectPtr> > TileGrid;
};

#endif
