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
#include "Map.hpp"
#include "../shared/Opcodes.hpp"
#include "../client/ResourceManager.hpp"
#include <algorithm>

Map::Map     (const Uint16 MapID) : 
MapID        (MapID),
NewSpellBoxID(0),
// [PH] This only works for map0, cause its size is 50x50 tiles
TileGrid     (50, std::vector<Tile>(50))
{
}

Map::~Map()
{
    for(auto CreatureIterator = Creatures.begin(); CreatureIterator != Creatures.end(); ++CreatureIterator)
    {
        delete (*CreatureIterator);
    }

    // Kick all players if they are online
    for(auto PlayerIter = Players.begin(); PlayerIter != Players.end(); ++PlayerIter)
    {
        if((*PlayerIter)->IsInWorld())
            (*PlayerIter)->Kick();
        delete (*PlayerIter);
    }
}

void Map::Update(Int32 diff)
{
    this->diff = diff;

    // Update spell box positions
    for(auto SpellBoxIter = Spells.begin(); SpellBoxIter != Spells.end(); ++SpellBoxIter)
    {
        SpellBoxIter->Update(diff);
    }

    std::for_each(Players.begin(), Players.end(), std::bind1st(std::mem_fun(&Map::UnitUpdate), this));
    std::for_each(Creatures.begin(), Creatures.end(), std::bind1st(std::mem_fun(&Map::UnitUpdate), this));
}

void Map::UnitUpdate(Unit* pUnit)
{
    // Check if unit got hit by spell
    for(auto SpellBoxIter = Spells.begin(); SpellBoxIter != Spells.end();)
    {
        if(SpellBoxIter->pCaster() != pUnit/*Is friendly? Healing?*/ && SpellBoxIter->CollidesWith(pUnit))
        {
            pUnit->SpellHit(&(*SpellBoxIter));
            SpellBoxIter = Spells.erase(SpellBoxIter);
        }
        else
        {
            ++SpellBoxIter;
        }
    }
    pUnit->Update(diff);
}

void Map::AddPlayer(Player* pPlayer)
{
    // Pack & send all data about world objects to new player
    for(auto CreatureIterator = Creatures.begin(); CreatureIterator != Creatures.end(); ++CreatureIterator)
    {
        pPlayer->SendPacket((*CreatureIterator)->PackData());
    }

    // Pack & send all data about players in map to new player
    // and send data about the new player to all other players
    for(auto PlayerIterator = Players.begin(); PlayerIterator != Players.end(); ++PlayerIterator)
    {
        pPlayer->SendPacket((*PlayerIterator)->PackData());
        ((*PlayerIterator)->SendPacket(pPlayer->PackData()));
    }

    // Send the new player his data
    pPlayer->SendPacket(pPlayer->PackData());

    // Finally, we add the new player to the map
    Players.push_back(pPlayer);
}

void Map::AddSpell(Unit* pCaster, Spell* pSpell, float Angle) // Unit* pCaster
{
    // PLACEHOLDER
    Spells.push_back(SpellBox(pSpell, pCaster, sf::FloatRect((float)pCaster->GetX()+(5/32), (float)pCaster->GetY()+(3/32), 1.0f-float(9/32), 1.f-float(8/32)), Angle, NewSpellBoxID));
    ++NewSpellBoxID;
}

void Map::SendToPlayers(sf::Packet& Packet)
{
    for(auto PlayerIterator = Players.begin(); PlayerIterator != Players.end(); ++PlayerIterator)
    {
        (*PlayerIterator)->SendPacket(Packet);
    }
}

void Map::RemovePlayer(Player* pPlayer)
{
    sf::Packet Packet;
    Packet << (Uint16)MSG_REMOVE_OBJECT << pPlayer->GetObjectID();
    for(auto PlayerIterator = Players.begin(); PlayerIterator != Players.end();)
    {
        if((*PlayerIterator) == pPlayer)
        {
            PlayerIterator = Players.erase(PlayerIterator);
        }
        else
        {
            (*PlayerIterator)->SendPacket(Packet);
            ++PlayerIterator;
        }
    }
}