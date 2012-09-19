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
#include "shared/Opcodes.hpp"
#include "shared/Config.hpp"
#include "shared/Log.hpp"
#include <boost/bind.hpp>
#include "Player.hpp"
#include "Database.hpp"
#include "Pathfinder.hpp"
#include "ObjectMgr.hpp"

Map::Map     (uint16 TMapID) : 
NewSpellBoxID(0),
// [PH] This only works for map0, cause its size is 50x50 tiles
TileGrid     (50, std::vector<WorldObject*>(50, nullptr))
{
    MapID = TMapID;
    sLog.Write("Map %u loaded.", MapID);
}

Map::~Map()
{
    // Kick all players if they are online
    for(auto PlayerIter = Players.begin(); PlayerIter != Players.end(); ++PlayerIter)
    {
        if((*PlayerIter)->IsInWorld())
            (*PlayerIter)->Kick();
    }

    for(auto Iter = Creatures.begin(); Iter != Creatures.end(); ++Iter)
    {
        delete *Iter;
    }
    
    for(auto Iter = Spells.begin(); Iter != Spells.end(); ++Iter)
    {
        delete *Iter;
    }
    sLog.Write("Map %u destroyed.", MapID);
}

void Map::LoadCreatures()
{
    QueryResult Result(sDatabase.PQuery("SELECT * FROM `creature` WHERE map='%u'", MapID));
    Creature* pCreature;

    while(Result->next())
    {
        pCreature = new Creature(Result->getUInt(1), this, Result->getUInt(4), Result->getUInt(5), sObjectMgr.GetCreatureTemplate(Result->getUInt(2)));
        pCreature->InitializeAI();
        Creatures.push_back(pCreature);
    }
}

void Map::Update(int64 diff)
{
    // Update spell box positions
    for(auto SpellBoxIter = Spells.begin(); SpellBoxIter != Spells.end(); ++SpellBoxIter)
    {
        (*SpellBoxIter)->Update(diff);
    }

    std::for_each(Players.begin(), Players.end(), boost::bind(&Map::UnitUpdate, this, _1, diff));
    std::for_each(Creatures.begin(), Creatures.end(), boost::bind(&Map::UnitUpdate, this, _1, diff));
}

void Map::UnitUpdate(Unit* pUnit, int64 diff)
{
    // Check if unit got hit by spell
    for(auto SpellBoxIter = Spells.begin(); SpellBoxIter != Spells.end();)
    {
        if((*SpellBoxIter)->pCaster != pUnit/*Is friendly? Healing?*/ && (*SpellBoxIter)->CollidesWith(pUnit))
        {
            pUnit->SpellHit(*SpellBoxIter);
            delete *SpellBoxIter;
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
        WorldPacket CreatureData = (*CreatureIterator)->PackData();
        pPlayer->SendPacket(CreatureData);
    }

    // Pack & send all data about players in map to new player
    // and send data about the new player to all other players
    
    WorldPacket NewPlayerData = pPlayer->PackData();
    
    for(auto PlayerIterator = Players.begin(); PlayerIterator != Players.end(); ++PlayerIterator)
    {
        WorldPacket PlayerData = (*PlayerIterator)->PackData();
        pPlayer->SendPacket(PlayerData);
        (*PlayerIterator)->SendPacket(NewPlayerData);
    }

    // Send the new player his data
    pPlayer->SendPacket(NewPlayerData);

    // Finally, we add the new player to the map
    Players.push_back(pPlayer);
}

void Map::AddSpell(Unit* pCaster, Spell* pSpell, float Angle)
{
    // PLACEHOLDER
    Spells.push_back(new SpellBox(pSpell, pCaster, FloatRect((float)pCaster->GetX()+(5/32), (float)pCaster->GetY()+(3/32), 1.0f-float(9/32), 1.f-float(8/32)), Angle, NewSpellBoxID));
    ++NewSpellBoxID;
}

void Map::SendToPlayers(WorldPacket& Packet)
{
    for(auto PlayerIterator = Players.begin(); PlayerIterator != Players.end(); ++PlayerIterator)
    {
        (*PlayerIterator)->SendPacket(Packet);
    }
}

void Map::RemovePlayer(Player* pPlayer)
{
    WorldPacket Packet((uint16)MSG_REMOVE_OBJECT);
    Packet << pPlayer->GetObjectID();
    
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

bool Map::CheckOutside(int PosX, int PosY, uint8 Direction) const
{
    // FIXME: This works only for map0 (50x50)
    switch(Direction)
    {
        case MOVE_UP:
            return ((PosY - 1) < 0);
            break;
        case MOVE_DOWN:
            return ((PosY + 1) >= 50);
            break;
        case MOVE_LEFT:
            return ((PosX - 1) < 0);
            break;
        case MOVE_RIGHT:
            return ((PosX + 1) >= 50);
            break;
        default:
            return false;
            break;
    }
}
