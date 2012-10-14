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
#include "shared/WorldPacket.hpp"
#include <list>

class Player;
class Creature;
class GameObject;
class WorldObject;
class MapScript;
struct MapTemplate;

typedef std::vector<std::vector<WorldObject*> > WorldObjectGrid;

class Map
{
public:
    Map(MapTemplate* pTemplate, uint32 MapID);
    ~Map();

    uint16 GetID() const;
    uint32 GetNewSpellBoxID() const;

    void Load();

    // THIS FUNCTION IS NOT TO BE FUCKED WITH
    void RemoveFromTileGrid(uint16 x, uint16 y);
    void AddToTileGrid(WorldObject* pWho);
    void RemovePlayer(Player* pPlayer);
    void AddPlayer(Player* pPlayer);
    void AddSpell(Unit* pCaster, SpellTemplate* pSpell, float Angle);

    virtual void Update(int64 diff);
    void UnitUpdate(Unit* pVictim, int64 diff);

    void SendToPlayers(WorldPacket& Packet);

    bool TryInteract(Player* pWho, uint16 x, uint16 y);
    WorldObject* GetObjectAt(uint16 x, uint16 y);
    WorldObjectGrid* GetWorldObjectGrid();

    bool CheckOutside(uint16 PosY, uint16 PosX, uint8 Direction) const;

private:
    // Entities
    std::list<GameObject*> GameObjects;
    std::list<Creature*> Creatures;
    std::list<Player*> Players;
    std::list<SpellBox*> Spells;
    WorldObjectGrid TileGrid;

    MapTemplate const* pTemplate;
    MapScript* pMapScript;
    uint32 NewSpellBoxID;
    uint32 MapID;
};

#endif
